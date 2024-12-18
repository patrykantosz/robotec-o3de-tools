
#include <TestGem/TestGemTypeIds.h>
#include <TestGemModuleInterface.h>
#include "TestGemEditorSystemComponent.h"
#include <TestGem/TextLabelsTestEditorComponent.h>

namespace TestGem
{
    class TestGemEditorModule
        : public TestGemModuleInterface
    {
    public:
        AZ_RTTI(TestGemEditorModule, TestGemEditorModuleTypeId, TestGemModuleInterface);
        AZ_CLASS_ALLOCATOR(TestGemEditorModule, AZ::SystemAllocator);

        TestGemEditorModule()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            // Add ALL components descriptors associated with this gem to m_descriptors.
            // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
            // This happens through the [MyComponent]::Reflect() function.
            m_descriptors.insert(m_descriptors.end(), {
                TestGemEditorSystemComponent::CreateDescriptor(),
                Test::TextLabelsTestEditorComponent::CreateDescriptor()
            });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         * Non-SystemComponents should not be added here
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList {
                azrtti_typeid<TestGemEditorSystemComponent>(),
            };
        }
    };
}// namespace TestGem

#if defined(O3DE_GEM_NAME)
AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME, _Editor), TestGem::TestGemEditorModule)
#else
AZ_DECLARE_MODULE_CLASS(Gem_TestGem_Editor, TestGem::TestGemEditorModule)
#endif
