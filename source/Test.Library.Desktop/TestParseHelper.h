#pragma once
#include "IXmlParseHelper.h"


namespace Library
{
	class TestParseHelper :
		public IXmlParseHelper
	{
		RTTI_DECLARATIONS(TestParseHelper, IXmlParseHelper)
	public:
		explicit TestParseHelper(XmlParseMaster* xmlParseMaster = nullptr);

		void initialize(XmlParseMaster* const xmlParseMaster) override;
		bool startElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& element, const Library::HashMap<std::string, std::string> attributes) override;
		bool endElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& element) override;
		void charDataHandler(XmlParseMaster::SharedData& sharedData, const std::string& buffer, const std::uint32_t bufferLength) override;
		IXmlParseHelper* clone() override;
	};
}
