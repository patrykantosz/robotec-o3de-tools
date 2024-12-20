#pragma once

#include "PointcloudComponentController.h"

#include "Clients/PointcloudComponent.h"

#include <AzCore/Component/TransformBus.h>
#include <AzFramework/Entity/EntityDebugDisplayBus.h>
#include <AzFramework/Visibility/BoundsBus.h>
#include <AzToolsFramework/API/ComponentEntitySelectionBus.h>
#include <AzToolsFramework/Entity/EditorEntityInfoBus.h>
#include <AzToolsFramework/ToolsComponents/EditorComponentAdapter.h>
#include <AzToolsFramework/ToolsComponents/EditorComponentBase.h>
#include <Pointcloud/PointcloudFeatureProcessorInterface.h>
#include <Pointcloud/PointcloudTypeIds.h>

namespace Pointcloud
{

    using PointcloudEditorComponentBase =
        AzToolsFramework::Components::EditorComponentAdapter<PointcloudComponentController, PointcloudComponent, PointcloudComponentConfig>;

    class PointcloudEditorComponent
        : public PointcloudEditorComponentBase
        , private AZ::TransformNotificationBus::Handler
        , private AzToolsFramework::EditorEntityInfoNotificationBus::Handler
        , private AzFramework::EntityDebugDisplayEventBus::Handler
        , public AzFramework::BoundsRequestBus::Handler
        , public AzToolsFramework::EditorComponentSelectionRequestsBus::Handler
    {
    public:
        AZ_EDITOR_COMPONENT(PointcloudEditorComponent, PointcloudEditorComponentTypeId, AzToolsFramework::Components::EditorComponentBase);
        PointcloudEditorComponent() = default;
        explicit PointcloudEditorComponent(const PointcloudComponentConfig& configuration);
        ~PointcloudEditorComponent() = default;

        static void Reflect(AZ::ReflectContext* context);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);

        // EditorComponentBase interface overrides ...
        void Activate() override;
        void Deactivate() override;

        // AzFramework::BoundsRequestBus overrides ...
        AZ::Aabb GetWorldBounds() const override;
        AZ::Aabb GetLocalBounds() const override;

        // AzToolsFramework::EditorComponentSelectionRequestsBus overrides ...
        AZ::Aabb GetEditorSelectionBoundsViewport(const AzFramework::ViewportInfo& viewportInfo) override;
        bool EditorSelectionIntersectRayViewport(
            const AzFramework::ViewportInfo& viewportInfo, const AZ::Vector3& src, const AZ::Vector3& dir, float& distance) override;

        bool SupportsEditorRayIntersect() override;
        bool SupportsEditorRayIntersectViewport(const AzFramework::ViewportInfo& viewportInfo) override;

        // AzFramework::EntityDebugDisplayEventBus overrides ...
        void DisplayEntityViewport(const AzFramework::ViewportInfo& viewportInfo, AzFramework::DebugDisplayRequests& debugDisplay) override;

    private:
        // AZ::TransformNotificationBus::Handler overrides ...
        void OnTransformChanged(const AZ::Transform& local, const AZ::Transform& world) override;

        // AzToolsFramework::EditorEntityInfoNotificationBus overrides ...
        void OnEntityInfoUpdatedVisibility(AZ::EntityId entityId, bool visible) override;
    };
} // namespace Pointcloud
