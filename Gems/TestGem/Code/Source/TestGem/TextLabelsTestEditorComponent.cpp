
#include "TextLabelsTestEditorComponent.h"

#include <AzCore/Component/TransformBus.h>
#include <AzCore/Serialization/EditContext.h>
#include <Viewport/ViewportMessages.h>

namespace Test
{
    void TextLabelsTestEditorComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<TextLabelsTestEditorComponent, AzToolsFramework::Components::EditorComponentBase>()->Version(0);

            if (auto* editContext = serializeContext->GetEditContext())
            {
                editContext->Class<TextLabelsTestEditorComponent>("TextLabelsTestEditorComponent", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "TextLabelsTestEditorComponent")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
                    ->Attribute(AZ::Edit::Attributes::Category, "TestGems");
            }
        }
    }

    void TextLabelsTestEditorComponent::Activate()
    {
        AzFramework::ViewportDebugDisplayEventBus::Handler::BusConnect(AzToolsFramework::GetEntityContextId());
    }

    void TextLabelsTestEditorComponent::Deactivate()
    {
        AzFramework::ViewportDebugDisplayEventBus::Handler::BusDisconnect();
    }

    void TextLabelsTestEditorComponent::BuildGameEntity(AZ::Entity*)
    {
    }

    void TextLabelsTestEditorComponent::DisplayViewport(
        const AzFramework::ViewportInfo& viewportInfo, AzFramework::DebugDisplayRequests& debugDisplay)
    {
        AZ::Transform transform = GetEntity()->GetTransform()->GetWorldTM();
        AZStd::vector<AZ::Vector3> points{ { 0.0f, 0.0f, 0.0f },     { 15.0f, 15.0f, 0.0f },   { 30.0f, 30.0f, 0.0f },
                                           { 45.0f, 45.0f, 0.0f },   { 60.0f, 60.0f, 0.0f },   { 75.0f, 75.0f, 0.0f },
                                           { 90.0f, 90.0f, 0.0f },   { 105.0f, 105.0f, 0.0f }, { 120.0f, 120.0f, 0.0f },
                                           { 135.0f, 135.0f, 0.0f }, { 150.0f, 150.0f, 0.0f }, { 165.0f, 165.0f, 0.0f },
                                           { 180.0f, 180.0f, 0.0f }, { 195.0f, 195.0f, 0.0f }, { 210.0f, 210.0f, 0.0f },
                                           { 225.0f, 225.0f, 0.0f }, { 240.0f, 240.0f, 0.0f }, { 255.0f, 255.0f, 0.0f },
                                           { 270.0f, 270.0f, 0.0f }, { 285.0f, 285.0f, 0.0f }, { 300.0f, 300.0f, 0.0f },
                                           { 315.0f, 315.0f, 0.0f }, { 330.0f, 330.0f, 0.0f }, { 345.0f, 345.0f, 0.0f },
                                           { 360.0f, 360.0f, 0.0f }, { 375.0f, 375.0f, 0.0f }, { 390.0f, 390.0f, 0.0f },
                                           { 405.0f, 405.0f, 0.0f }, { 420.0f, 420.0f, 0.0f } };

        const AZ::u32 stateBefore = debugDisplay.GetState();
        debugDisplay.CullOff();
        debugDisplay.DepthTestOff();
        debugDisplay.SetLineWidth(4.0);

        debugDisplay.PushMatrix(transform);
        for (const auto& point : points)
        {
            const AZ::Vector3 elevatedPoint = point + AZ::Vector3(0.0f, 0.0f, 10.0f);
            debugDisplay.SetColor(AZ::Colors::White);
            debugDisplay.DrawLine(point, elevatedPoint);
            // AZStd::string text = AZStd::string::format("X: %f, Y: %f", point.GetX(), point.GetY());
            AZStd::string text{"X"};
            debugDisplay.DrawTextLabel(transform.TransformPoint(elevatedPoint), 1.0f, text.c_str());
        }
        debugDisplay.PopMatrix();
        debugDisplay.SetState(stateBefore);
    }

} // namespace Test