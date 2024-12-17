#pragma once

#include "../../Include/Converter/ConverterBus.h"

#include "ROS2/Georeference/GeoreferenceStructures.h"

#include <AzCore/Component/Component.h>

namespace Converter
{

    class ConverterComponent
        : public AZ::Component
        , public ConverterRequestBus::Handler
    {
    public:
        AZ_COMPONENT(ConverterComponent, "{3b2d72b5-f967-4b87-a24b-498f284a817a}", AZ::Component);

        ConverterComponent() = default;
        ~ConverterComponent() = default;

        static void Reflect(AZ::ReflectContext* context);

        void Activate() override;
        void Deactivate() override;

        void AddToGeoJSON(const ROS2::WGS::WGS84Coordinate& coords, const AZStd::string& spawnableName) override;
        void SaveGeoJSON() override;

    public:
        rapidjson::Document m_doc;
        int m_currentId = 0;
        AZStd::string m_fileName{""};
    };

} // namespace Converter
