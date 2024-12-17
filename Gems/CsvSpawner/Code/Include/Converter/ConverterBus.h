#pragma once

#include <ROS2/Georeference/GeoreferenceStructures.h>
#include <AzCore/Component/ComponentBus.h>

namespace Converter
{
    class ConverterRequests : public AZ::ComponentBus
    {
    public:
        using BusIdType = AZ::EntityId;

        virtual void AddToGeoJSON(const ROS2::WGS::WGS84Coordinate& coords, const AZStd::string& spawnableName) = 0;
        virtual void SaveGeoJSON() = 0;
    };

    using ConverterRequestBus = AZ::EBus<ConverterRequests>;
} // namespace Converter