
#pragma once

#include <TestGem/TestGemTypeIds.h>

#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>

namespace TestGem
{
    class TestGemRequests
    {
    public:
        AZ_RTTI(TestGemRequests, TestGemRequestsTypeId);
        virtual ~TestGemRequests() = default;
        // Put your public methods here
    };

    class TestGemBusTraits
        : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //////////////////////////////////////////////////////////////////////////
    };

    using TestGemRequestBus = AZ::EBus<TestGemRequests, TestGemBusTraits>;
    using TestGemInterface = AZ::Interface<TestGemRequests>;

} // namespace TestGem
