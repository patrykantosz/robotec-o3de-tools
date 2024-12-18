#pragma once

#include <AzFramework/Entity/EntityDebugDisplayBus.h>
#include <AzToolsFramework/ToolsComponents/EditorComponentBase.h>

namespace Test
{
    class TextLabelsTestEditorComponent
        : public AzToolsFramework::Components::EditorComponentBase
        , protected AzFramework::ViewportDebugDisplayEventBus::Handler
    {
    public:
        AZ_EDITOR_COMPONENT(TextLabelsTestEditorComponent, "{f76b272e-abec-479a-97b8-b435b38a9a5a}");
        TextLabelsTestEditorComponent() = default;
        ~TextLabelsTestEditorComponent() override = default;

        static void Reflect(AZ::ReflectContext* context);

        void Activate() override;
        void Deactivate() override;
        void BuildGameEntity(AZ::Entity*) override;

    private:
        void DisplayViewport(const AzFramework::ViewportInfo& viewportInfo, AzFramework::DebugDisplayRequests& debugDisplay) override;
    };

} // namespace Test