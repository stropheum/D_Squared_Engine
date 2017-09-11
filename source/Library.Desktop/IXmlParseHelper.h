#pragma once
#include "HashMap.h"
#include "XmlParseMaster.h"
#include "RTTI.h"

namespace Library
{
	class IXmlParseHelper :
		public RTTI
	{
		RTTI_DECLARATIONS(IXmlParseHelper, RTTI)
	public:
		
		/**
		 * Constructor
		 * @Param xmlParseMaster: The Parse master associated with this helper
		 */
		explicit IXmlParseHelper(XmlParseMaster* const xmlParseMaster = nullptr);

		/**
		 * Destructor
		 */
		virtual ~IXmlParseHelper();

		/**
		 * Initializes the helper to be associated with a Parse master
		 * @Param xmlParseMaster: The Parse master that will be associated with this helper
		 */
		virtual void Initialize(XmlParseMaster* const xmlParseMaster) = 0;

		/**
		 * Handles elements from the Expat callback with the same Name
		 * @Param sharedData: The shared data object being written to
		 * @Param element: The Name of the entry being parsed
		 * @Param attributes: The collection of string,string pairs of attributes associated with the entry
		 * @Return: True if the helper can handle parsing the data
		 */
		virtual bool StartElementHandler(XmlParseMaster::SharedData& sharedData,  const std::string& element, const HashMap<std::string, std::string> attributes) = 0;

		/**
		 * Handles elements from the Expat callback with the same Name
		 * @Param sharedData: The shared data object being written to
		 * @Param element: The Name of the entry being parsed
		 * @Return: True if the helper can handle parsing the data
		 */
		virtual bool EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& element) = 0;

		/**
		 * Handles elements from the Expat callback with the same Name
		 * @Param sharedData: The shared data object being written to
		 * @Param buffer: The character data being parsed
		 * @Param bufferLength: The number of characters in the buffer
		 * @Return: True if the helper can handle parsing the data
		 */
		virtual void CharDataHandler(XmlParseMaster::SharedData& sharedData, const std::string& buffer, const std::uint32_t& bufferLength);

		/**
		 * Method used for copying instances of Parse helpers
		 * @Return: Pointer to the freshly allocated Parse helper
		 */
		virtual IXmlParseHelper* Clone() = 0;

		/**
		 * RTTI ToString representation of the Parse helper
		 * @Return: A string representation of the class Name
		 */
		std::string ToString() const override;

	protected:
		XmlParseMaster* mXmlParseMaster;
		const std::string mValidElementName;
	};
}
