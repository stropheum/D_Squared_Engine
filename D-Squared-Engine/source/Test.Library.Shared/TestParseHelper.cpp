#include "pch.h"
#include "TestParseHelper.h"
#include <winnt.h>
#include "TestSharedData.h"


namespace Library
{
    RTTI_DEFINITIONS(TestParseHelper)

        TestParseHelper::TestParseHelper(Library::XmlParseMaster* xmlParseMaster) :
        IXmlParseHelper(xmlParseMaster)
    {
    }

    void TestParseHelper::Initialize(XmlParseMaster* const xmlParseMaster)
    {
        m_xmlParseMaster = xmlParseMaster;
    }

    bool TestParseHelper::StartElementHandler(XmlParseMaster::SharedData& sharedData,
        const std::string& element, const Library::HashMap<std::string, std::string> attributes)
    {
        TestSharedData* data = sharedData.As<TestSharedData>();

        if (data != nullptr && element == "Person")
        {
            data->m_name = (*attributes.Find("Name")).second;
            data->m_health = std::stoi((*attributes.Find("Health")).second);
            data->m_mana = std::stoi((*attributes.Find("Mana")).second);
            return true;
        }

        return false;
    }

    bool TestParseHelper::EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& element)
    {
        UNREFERENCED_PARAMETER(sharedData);
        UNREFERENCED_PARAMETER(element);
        return false;
    }

    void TestParseHelper::CharDataHandler(XmlParseMaster::SharedData& sharedData, const std::string& buffer, const std::uint32_t& bufferLength)
    {
        UNREFERENCED_PARAMETER(sharedData);
        UNREFERENCED_PARAMETER(buffer);
        UNREFERENCED_PARAMETER(bufferLength);
    }

    IXmlParseHelper* TestParseHelper::Clone()
    {
        return new TestParseHelper();
    }
}