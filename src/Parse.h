#ifndef PARSE_H
#define PARSE_H

#include <fstream>

#include "../NoGUI/src/GUI.h"
#include "../NoMEM/src/NoMEM.h"
#include "../include/rapidjson/reader.h"
#include "../include/rapidjson/stringbuffer.h"
#include "../include/rapidjson/writer.h"
#include "../include/rapidjson/prettywriter.h"

namespace NoGUI
{
	int savePage(std::shared_ptr< Page > pg, std::string path="./page.json");
	std::shared_ptr< Page > loadPage(std::string path);
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
	
	void seralizeCDropDown(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, const NoGUI::CDropDown& dropFmt);
	void seralizeCImage(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, const NoGUI::CImage& imageFmt);
	void seralizeCInput(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, const NoGUI::CInput& inputFmt);
	void seralizeCMultiStyle(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, const NoGUI::CMultiStyle& styles);
	void seralizeCText(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, const NoGUI::CText& textFmt);
	void seralizeStyle(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, const NoGUI::Style& style);
}

#endif