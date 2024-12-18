
#include <TestGem/TestGemTypeIds.h>
#include <TestGemModuleInterface.h>
#include "TestGemSystemComponent.h"

namespace TestGem
{
    class TestGemModule
        : public TestGemModuleInterface
    {
    public:
        AZ_RTTI(TestGemModule, TestGemModuleTypeId, TestGemModuleInterface);
        AZ_CLASS_ALLOCATOR(TestGemModule, AZ::SystemAllocator);
    };
}// namespace TestGem

#if defined(O3DE_GEM_NAME)
AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME), TestGem::TestGemModule)
#else
AZ_DECLARE_MODULE_CLASS(Gem_TestGem, TestGem::TestGemModule)
#endif
