
#include "TestGemModuleInterface.h"
#include <AzCore/Memory/Memory.h>

#include <TestGem/TestGemTypeIds.h>

#include <Clients/TestGemSystemComponent.h>

namespace TestGem
{
    AZ_TYPE_INFO_WITH_NAME_IMPL(TestGemModuleInterface,
        "TestGemModuleInterface", TestGemModuleInterfaceTypeId);
    AZ_RTTI_NO_TYPE_INFO_IMPL(TestGemModuleInterface, AZ::Module);
    AZ_CLASS_ALLOCATOR_IMPL(TestGemModuleInterface, AZ::SystemAllocator);

    TestGemModuleInterface::TestGemModuleInterface()
    {
        // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
        // Add ALL components descriptors associated with this gem to m_descriptors.
        // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
        // This happens through the [MyComponent]::Reflect() function.
        m_descriptors.insert(m_descriptors.end(), {
            TestGemSystemComponent::CreateDescriptor(),
            });
    }

    AZ::ComponentTypeList TestGemModuleInterface::GetRequiredSystemComponents() const
    {
        return AZ::ComponentTypeList{
            azrtti_typeid<TestGemSystemComponent>(),
        };
    }
} // namespace TestGem