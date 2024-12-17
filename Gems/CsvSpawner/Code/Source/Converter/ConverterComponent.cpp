//
// Created by pantosz on 17.12.24.
//

#include "ConverterComponent.h"

#include "../../Include/Converter/ConverterBus.h"

#include <AzCore/JSON/writer.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <rapidjson/prettywriter.h>

namespace Converter
{
    void ConverterComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<ConverterComponent, AZ::Component>()->Version(0)->Field("FileName", &ConverterComponent::m_fileName);

            if (AZ::EditContext* editContext = serializeContext->GetEditContext())
            {
                editContext->Class<ConverterComponent>("Converter", "A conversion component")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "Converter")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default, &ConverterComponent::m_fileName, "FileName", "The name of the file to save.");
            }
        }
    }

    void ConverterComponent::Activate()
    {
        ConverterRequestBus::Handler::BusConnect(GetEntityId());
        m_doc.SetObject();
        rapidjson::Document::AllocatorType& allocator = m_doc.GetAllocator();

        rapidjson::Value features(rapidjson::kArrayType);
        m_doc.AddMember("type", "FeatureCollection", allocator);
        m_doc.AddMember("features", features, allocator);
    }

    void ConverterComponent::Deactivate()
    {
        ConverterRequestBus::Handler::BusDisconnect();
    }

    void ConverterComponent::AddToGeoJSON(const ROS2::WGS::WGS84Coordinate& coords, const AZStd::string& spawnableName)
    {
        AZ_Printf("TMP", "AddToGeoJSON");
        rapidjson::Document::AllocatorType& allocator = m_doc.GetAllocator();

        rapidjson::Value feature(rapidjson::kObjectType);
        feature.AddMember("type", "Feature", allocator);

        rapidjson::Value geometry(rapidjson::kObjectType);
        geometry.AddMember("type", "Point", allocator);

        rapidjson::Value coordinates(rapidjson::kArrayType);
        coordinates.PushBack(coords.m_longitude, allocator);
        coordinates.PushBack(coords.m_latitude, allocator);

        geometry.AddMember("coordinates", coordinates, allocator);
        feature.AddMember("geometry", geometry, allocator);

        rapidjson::Value properties(rapidjson::kObjectType);
        properties.AddMember("id", m_currentId++, allocator);
        properties.AddMember("spawnable_name", rapidjson::Value(spawnableName.c_str(), allocator), allocator);

        feature.AddMember("properties", properties, allocator);

        m_doc["features"].PushBack(feature, allocator);
    }

    void ConverterComponent::SaveGeoJSON()
    {
        AZ::IO::SystemFile file;
        if (file.Open(m_fileName.c_str(), AZ::IO::SystemFile::SF_OPEN_CREATE | AZ::IO::SystemFile::SF_OPEN_WRITE_ONLY))
        {
            rapidjson::StringBuffer buffer;
            rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
            m_doc.Accept(writer);

            file.Write(buffer.GetString(), buffer.GetSize());
            file.Close();
        }
        else
        {
            AZ_Error("Converter", false, "Failed to save geojsone file.");
        }
    }


} // namespace Converter