#ifndef PARSE_H
#define PARSE_H

#include <fstream>

#include "../NoGUI/src/GUI.h"
#include "../NoMEM/src/NoMEM.h"
#include "../include/rapidjson/document.h"
#include "../include/rapidjson/filereadstream.h"
#include "../include/rapidjson/prettywriter.h"
#include "../include/rapidjson/reader.h"
#include "../include/rapidjson/stringbuffer.h"
#include "../include/rapidjson/writer.h"

namespace NoGUI
{
	int savePage(std::shared_ptr< Page > pg, std::shared_ptr< NoMEM::MEMManager > assets=nullptr, const std::string& path="./page.json");
	std::shared_ptr< Page > loadPage(std::string path, std::shared_ptr< NoMEM::MEMManager > assets=nullptr);
}

namespace NoPARSE
{
	const std::map< NoGUI::Crop, std::string > CropMap = {
		{NoGUI::Crop::NONE, "NONE"},
		{NoGUI::Crop::FIT, "FIT"},
		{NoGUI::Crop::SCROLL, "SCROLL"}
	};
	const std::map< NoGUI::TextAlign, std::string > AlignMap = {
		{NoGUI::TextAlign::LEFT, "LEFT"},
		{NoGUI::TextAlign::CENTER, "CENTER"},
		{NoGUI::TextAlign::RIGHT, "RIGHT"},
		{NoGUI::TextAlign::TOP, "TOP"},
		{NoGUI::TextAlign::BOTTOM, "BOTTOM"},
		{NoGUI::TextAlign::BOTTOML, "BOTTOM LEFT"},
		{NoGUI::TextAlign::BOTTOMR, "BOTTOM RIGHT"},
	};
	const std::map< NoGUI::TextWrap, std::string > WrapMap = {
		{NoGUI::TextWrap::NONE, "NONE"},
		{NoGUI::TextWrap::DOWN, "DOWN"},
		{NoGUI::TextWrap::UP, "UP"},
		{NoGUI::TextWrap::AROUND, "AROUND"}
	};
	class Parser
	{
	private:
		std::unique_ptr< NoMEM::MEMManager > assets;
	public:
		Parser(std::unique_ptr< NoMEM::MEMManager > a)
			: assets(move(a)) {}
		
	};
	
	NoGUI::CText deserializeCText(const rapidjson::Value& textJSON, std::shared_ptr< NoMEM::MEMManager > assets);
	NoGUI::CImage deserializeCImage(const rapidjson::Value& imgJSON, std::shared_ptr< NoMEM::MEMManager > assets);
	NoGUI::CInput deserializeCInput(const rapidjson::Value& inputJSON);
	NoGUI::CMultiStyle deserializeCMultiStyle(const rapidjson::Value& stylesJSON);
	NoGUI::CDropDown deserializeCDropDown(const rapidjson::Value& dropJSON);
	NoGUI::Style deserializeStyle(const rapidjson::Value& elemJSON);
	void serializeCDropDown(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, const NoGUI::CDropDown& dropFmt);
	void serializeCImage(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, const NoGUI::CImage& imageFmt, std::shared_ptr< NoMEM::MEMManager > assets);
	void serializeCInput(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, const NoGUI::CInput& inputFmt);
	void serializeCMultiStyle(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, const NoGUI::CMultiStyle& styles);
	void serializeCText(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, const NoGUI::CText& textFmt, std::shared_ptr< NoMEM::MEMManager > assets);
	void serializeStyle(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, const NoGUI::Style& style);
	void serializeComponents(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, NoGUI::Components components, std::shared_ptr< NoMEM::MEMManager > assets);
	void seralizeElement(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, std::shared_ptr< NoGUI::Element > elem, const std::string& id, std::shared_ptr< NoMEM::MEMManager > assets);
	int writeFile(rapidjson::StringBuffer& sb, const std::string& path);
}

#endif