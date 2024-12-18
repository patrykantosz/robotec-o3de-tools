
#pragma once

#include <AzToolsFramework/API/ToolsApplicationAPI.h>

#include <Clients/TestGemSystemComponent.h>

namespace TestGem
{
    /// System component for TestGem editor
    class TestGemEditorSystemComponent
        : public TestGemSystemComponent
        , protected AzToolsFramework::EditorEvents::Bus::Handler
    {
        using BaseSystemComponent = TestGemSystemComponent;
    public:
        AZ_COMPONENT_DECL(TestGemEditorSystemComponent);

        static void Reflect(AZ::ReflectContext* context);

        TestGemEditorSystemComponent();
        ~TestGemEditorSystemComponent();

    private:
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        // AZ::Component
        void Activate() override;
        void Deactivate() override;
    };
} // namespace TestGem
