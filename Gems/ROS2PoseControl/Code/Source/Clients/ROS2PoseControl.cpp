#include "ROS2PoseControl.h"
#include <AzCore/Component/ComponentApplicationBus.h>
#include <AzCore/Component/TransformBus.h>
#include <AzCore/Math/Transform.h>
#include <AzCore/Script/ScriptTimePoint.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>

#include <imgui/imgui.h>

#include <ROS2/ROS2Bus.h>
#include <ROS2/ROS2GemUtilities.h>
#include <ROS2/Utilities/ROS2Conversions.h>
#include <ROS2/Utilities/ROS2Names.h>
#include <ROS2/Frame/ROS2FrameComponent.h>
#include <tf2_ros/transform_listener.h>
#include <ROS2PoseControl/ROS2PoseControlConfiguration.h>

#include "RigidBodyComponent.h"
#include "AzCore/std/string/regex.h"
#include "AzCore/Time/ITime.h"
#include "AzFramework/Entity/GameEntityContextComponent.h"
#include "AzFramework/Physics/SimulatedBodies/RigidBody.h"

namespace ROS2PoseControl {
    ROS2PoseControl::ROS2PoseControl() {
        m_configuration.m_poseTopicConfiguration.m_topic = "goal_pose";
        m_configuration.m_poseTopicConfiguration.m_type = "geometry_msgs::msg::PoseStamped";
        m_configuration.m_targetFrame = "base_link";
        m_configuration.m_referenceFrame = "map";
    }

    void ROS2PoseControl::GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType &required) {
        required.push_back(AZ_CRC("ROS2Frame"));
    }

    void ROS2PoseControl::Activate() {
        auto ros2Node = ROS2::ROS2Interface::Get()->GetNode();
        if (m_configuration.m_tracking_mode == ROS2PoseControlConfiguration::TrackingMode::TF2) {
            m_poseSubscription.reset();
            m_tf_buffer = std::make_unique<tf2_ros::Buffer>(ros2Node->get_clock());
            m_tf_listener = std::make_shared<tf2_ros::TransformListener>(*m_tf_buffer);
            AZ::TickBus::Handler::BusConnect();
        } else if (m_configuration.m_tracking_mode == ROS2PoseControlConfiguration::TrackingMode::PoseMessages) {
            m_tf_buffer.reset();
            m_tf_listener.reset();
            AZ::TickBus::Handler::BusDisconnect();
            OnTopicConfigurationChanged();
        }
        ImGui::ImGuiUpdateListenerBus::Handler::BusConnect();
    }

    void ROS2PoseControl::Deactivate() {
        m_poseSubscription.reset();
        m_tf_buffer.reset();
        m_tf_listener.reset();
        AZ::TickBus::Handler::BusDisconnect();
        ImGui::ImGuiUpdateListenerBus::Handler::BusDisconnect();
    }

    AZ::Outcome<AZ::Transform, const char *> ROS2PoseControl::GetCurrentTransformViaTF2() const {
        geometry_msgs::msg::TransformStamped transformStamped;
        if (m_tf_buffer->canTransform(m_configuration.m_referenceFrame.c_str(), m_configuration.m_targetFrame.c_str(),
                                      tf2::TimePointZero)) {
            transformStamped = m_tf_buffer->lookupTransform(m_configuration.m_referenceFrame.c_str(),
                                                            m_configuration.m_targetFrame.c_str(),
                                                            tf2::TimePointZero);
        } else {
            AZ_Warning("ROS2PositionControl", false, "Could not transform %s to %s",
                       m_configuration.m_targetFrame.c_str(), m_configuration.m_referenceFrame.c_str());
            return AZ::Failure("Could not transform");
        }
        const AZ::Quaternion rotation = ROS2::ROS2Conversions::FromROS2Quaternion(transformStamped.transform.rotation);
        const AZ::Vector3 translation = ROS2::ROS2Conversions::FromROS2Vector3(transformStamped.transform.translation);
        return AZ::Success(AZ::Transform::CreateFromQuaternionAndTranslation(rotation, translation));
    }

    void ROS2PoseControl::OnTick(float deltaTime, AZ::ScriptTimePoint time) {
        if (m_configuration.m_tracking_mode == ROS2PoseControlConfiguration::TrackingMode::TF2) {
            const AZ::Outcome<AZ::Transform, const char *> transform_outcome = GetCurrentTransformViaTF2();
            if (!transform_outcome.IsSuccess()) {
                return;
            } else {
                AZ::TransformBus::Event(GetEntityId(), &AZ::TransformBus::Events::SetLocalTM,
                                        transform_outcome.GetValue());
            }
        }
    }


    void ROS2PoseControl::OnTopicConfigurationChanged() {
        if (m_configuration.m_tracking_mode != ROS2PoseControlConfiguration::TrackingMode::PoseMessages) {
            return;
        }
        auto ros2Node = ROS2::ROS2Interface::Get()->GetNode();
        const auto *ros2_frame_component = m_entity->FindComponent<ROS2::ROS2FrameComponent>();
        auto namespaced_topic_name = ROS2::ROS2Names::GetNamespacedName(ros2_frame_component->GetNamespace(),
                                                                        m_configuration.m_poseTopicConfiguration.
                                                                        m_topic);
        m_poseSubscription = ros2Node->create_subscription<geometry_msgs::msg::PoseStamped>(
            namespaced_topic_name.data(),
            m_configuration.m_poseTopicConfiguration.GetQoS(),
            [this](const geometry_msgs::msg::PoseStamped::SharedPtr msg) {
                if (m_configuration.m_tracking_mode != ROS2PoseControlConfiguration::TrackingMode::PoseMessages || !
                    m_isTracking) {
                    return;
                }
                const AZ::Transform transform = ROS2::ROS2Conversions::FromROS2Pose(msg->pose);
                AZ::TransformBus::Event(GetEntityId(), &AZ::TransformBus::Events::SetWorldTM, transform);
            });
    }

    void ROS2PoseControl::OnIsTrackingChanged() {
        if (m_configuration.m_tracking_mode == ROS2PoseControlConfiguration::TrackingMode::TF2) {
            if (m_isTracking) {
                AZ::TickBus::Handler::BusConnect();
            } else {
                AZ::TickBus::Handler::BusDisconnect();
            }
        }
    }

    void ROS2PoseControl::SetIsTracking(const bool isTracking) {
        m_isTracking = isTracking;
        OnIsTrackingChanged();
    }



    void ROS2PoseControl::Reflect(AZ::ReflectContext *context) {
        if (auto *serialize = azrtti_cast<AZ::SerializeContext *>(context)) {
            serialize->Class<ROS2PoseControl, AZ::Component>()
                    ->Version(1)
                    ->Field("m_isTracking", &ROS2PoseControl::m_isTracking)
                    ->Field("m_configuration", &ROS2PoseControl::m_configuration);

            if (AZ::EditContext *ec = serialize->GetEditContext()) {
                ec->Class<ROS2PoseControl>("ROS2PoseControl",
                                           "A component that controls the pose of the entity based on ROS 2 data.")
                        ->ClassElement(AZ::Edit::ClassElements::EditorData, "ROS2PoseControl")
                        ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("Game"))
                        ->Attribute(AZ::Edit::Attributes::Category, "Perception Simulation")
                        ->DataElement(AZ::Edit::UIHandlers::Default, &ROS2PoseControl::m_isTracking, "Is Tracking",
                                      "Turn the tracking on or off")
                        ->DataElement(AZ::Edit::UIHandlers::Default, &ROS2PoseControl::m_configuration, "Configuration",
                                      "Configuration for ROS2PoseControl component");
            }
        }
    }

    void ROS2PoseControl::OnImGuiUpdate() {
        std::stringstream ss;
        ss << "ROS2PoseControl Entity: " << GetEntity()->GetName().c_str() << " , Id: " << GetEntityId().ToString().
                c_str();
        ImGui::Begin(ss.str().c_str());
        if (ImGui::Checkbox("Is Tracking", &m_isTracking)) {
            OnIsTrackingChanged();
        }
        ImGui::Text("Tracking Mode: %s", m_configuration.m_tracking_mode == ROS2PoseControlConfiguration::TrackingMode::TF2
                                           ? "TF2" : "Pose Messages");

        ImGui::Text("Position %f %f %f", GetEntity()->GetTransform()->GetWorldTranslation().GetX(),
                    GetEntity()->GetTransform()->GetWorldTranslation().GetY(),
                    GetEntity()->GetTransform()->GetWorldTranslation().GetZ());
        ImGui::Text("Rotation %f %f %f", GetEntity()->GetTransform()->GetWorldRotation().GetX(),
                    GetEntity()->GetTransform()->GetWorldRotation().GetY(),
                    GetEntity()->GetTransform()->GetWorldRotation().GetZ());
        ImGui::End();
    }
} // namespace PerceptionSimulation
