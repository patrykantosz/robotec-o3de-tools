
#include "TestGemSystemComponent.h"

#include <TestGem/TestGemTypeIds.h>

#include <AzCore/Serialization/SerializeContext.h>

namespace TestGem
{
    AZ_COMPONENT_IMPL(TestGemSystemComponent, "TestGemSystemComponent",
        TestGemSystemComponentTypeId);

    void TestGemSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<TestGemSystemComponent, AZ::Component>()
                ->Version(0)
                ;
        }
    }

    void TestGemSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("TestGemService"));
    }

    void TestGemSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("TestGemService"));
    }

    void TestGemSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
    }

    void TestGemSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }

    TestGemSystemComponent::TestGemSystemComponent()
    {
        if (TestGemInterface::Get() == nullptr)
        {
            TestGemInterface::Register(this);
        }
    }

    TestGemSystemComponent::~TestGemSystemComponent()
    {
        if (TestGemInterface::Get() == this)
        {
            TestGemInterface::Unregister(this);
        }
    }

    void TestGemSystemComponent::Init()
    {
    }

    void TestGemSystemComponent::Activate()
    {
        TestGemRequestBus::Handler::BusConnect();
        AZ::TickBus::Handler::BusConnect();
    }

    void TestGemSystemComponent::Deactivate()
    {
        AZ::TickBus::Handler::BusDisconnect();
        TestGemRequestBus::Handler::BusDisconnect();
    }

    void TestGemSystemComponent::OnTick([[maybe_unused]] float deltaTime, [[maybe_unused]] AZ::ScriptTimePoint time)
    {
    }

} // namespace TestGem
