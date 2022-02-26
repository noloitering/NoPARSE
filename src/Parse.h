#ifndef PARSE_H
#define PARSE_H

#include <fstream>


#include "../../NoGUI/src/GUI.h"
#include "../../NoMEM/src/NoMEM.h"
#include "../../NoECS/src/Entity.h"
#include "../include/rapidjson/document.h"
#include "../include/rapidjson/filereadstream.h"
#include "../include/rapidjson/prettywriter.h"
#include "../include/rapidjson/reader.h"
#include "../include/rapidjson/stringbuffer.h"
#include "../include/rapidjson/writer.h"

namespace NoGUI
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
		{NoGUI::TextAlign::BOTTOMR, "BOTTOM RIGHT"}
	};
	const std::map< NoGUI::TextWrap, std::string > WrapMap = {
		{NoGUI::TextWrap::NONE, "NONE"},
		{NoGUI::TextWrap::DOWN, "DOWN"},
		{NoGUI::TextWrap::UP, "UP"},
		{NoGUI::TextWrap::AROUND, "AROUND"}
	};
	
	void serializeCDropDown(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, const NoGUI::CDropDown& dropFmt, std::shared_ptr< NoMEM::MEMManager > assets, std::string path="");
	void serializeCImage(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, const NoGUI::CImage& imageFmt, std::shared_ptr< NoMEM::MEMManager > assets);
	void serializeCInput(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, const NoGUI::CInput& inputFmt);
	void serializeCMultiStyle(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, const NoGUI::CMultiStyle& styles);
	void serializeCText(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, const NoGUI::CText& textFmt, std::shared_ptr< NoMEM::MEMManager > assets);
	void serializeStyle(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, const NoGUI::Style& style);
	void serializeComponents(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, NoGUI::Components components, std::shared_ptr< NoMEM::MEMManager > assets, const std::string& dropPath="");
	void serializeCContainer(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, std::shared_ptr< NoGUI::CContainer > components, std::shared_ptr< NoMEM::MEMManager > assets, const std::string& key="Components");
	void serializeElement(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, std::shared_ptr< NoGUI::Element > elem, const std::string& id, std::shared_ptr< NoMEM::MEMManager > assets=nullptr, std::string dropPath="");
	
	void deserializeCText(NoGUI::CText& text, const rapidjson::Value& textJSON, std::shared_ptr< NoMEM::MEMManager > assets);
	void deserializeCImage(NoGUI::CImage& img, const rapidjson::Value& imgJSON, std::shared_ptr< NoMEM::MEMManager > assets);
	void deserializeCInput(NoGUI::CInput& input, const rapidjson::Value& inputJSON);
	void deserializeCMultiStyle(NoGUI::CMultiStyle& styles, const rapidjson::Value& stylesJSON);
	void deserializeStyle(NoGUI::Style& style, const rapidjson::Value& elemJSON);
	void deserializeComponents(NoGUI::Components& components, const rapidjson::Value& compJSON, std::shared_ptr< NoMEM::MEMManager > assets, std::shared_ptr< NoGUI::Element > dropParent=nullptr);
	void deserializeCContainer(std::shared_ptr< NoGUI::CContainer > components, const rapidjson::Value& compJSON, std::shared_ptr< NoMEM::MEMManager > assets, std::shared_ptr< NoGUI::Element > dropParent=nullptr);
	void deserializePage(rapidjson::Document& d, std::shared_ptr< NoGUI::Page > pg, std::shared_ptr< NoMEM::MEMManager > assets=nullptr);
	
	NoGUI::Components loadComponents(const rapidjson::Value& compJSON, std::shared_ptr< NoMEM::MEMManager > assets, std::shared_ptr< NoGUI::Element > dropParent=nullptr);
	std::shared_ptr< NoGUI::CContainer > loadCContainer(const rapidjson::Value& compJSON, std::shared_ptr< NoMEM::MEMManager > assets, std::shared_ptr< NoGUI::Element > dropParent=nullptr);
	std::shared_ptr< NoGUI::Element > loadElement(const rapidjson::Value::ConstMemberIterator& elemJSON, const size_t id, std::shared_ptr< NoMEM::MEMManager > assets=nullptr);
	NoGUI::CText loadCText(const rapidjson::Value& textJSON, std::shared_ptr< NoMEM::MEMManager > assets);
	NoGUI::CImage loadCImage(const rapidjson::Value& imgJSON, std::shared_ptr< NoMEM::MEMManager > assets);
	NoGUI::CInput loadCInput(const rapidjson::Value& inputJSON);
	NoGUI::CMultiStyle loadCMultiStyle(const rapidjson::Value& stylesJSON);
	NoGUI::CDropDown loadCDropDown(const rapidjson::Value& dropJSON, std::shared_ptr< NoGUI::Element > parent, std::shared_ptr< NoMEM::MEMManager > assets=nullptr);
	NoGUI::Style loadStyle(const rapidjson::Value& elemJSON);
	std::shared_ptr< Page > loadPage(std::string path, std::shared_ptr< NoMEM::MEMManager > assets=nullptr);
	std::shared_ptr< NoGUI::GUIManager > loadManager(std::string path, std::shared_ptr< NoMEM::MEMManager > assets=nullptr);
	
	int savePage(std::shared_ptr< Page > pg, std::shared_ptr< NoMEM::MEMManager > assets=nullptr, const std::string& path="./page.json");
}

namespace NoECS
{
	void serializeCTransform(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, const NoECS::CTransform& transform);
	void serializeCBBox(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, const NoECS::CBBox& bbox);
	void serializeCSprite(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, const NoECS::CSprite& sprite, std::shared_ptr< NoMEM::MEMManager > assets);
	void serializeCPoly(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, const NoECS::CPoly& polygon);
	void serializeComponents(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, NoECS::Components components, std::shared_ptr< NoMEM::MEMManager > assets);
	void serializeEntity(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, std::shared_ptr< NoECS::Entity > entity, std::shared_ptr< NoMEM::MEMManager > assets);
	
	void deserializeCTransform(NoECS::CTransform& transform, const rapidjson::Value& transformJSON);
	void deserializeCBBox(NoECS::CBBox& bbox, const rapidjson::Value& bboxJSON);
	void deserializeCSprite(NoECS::CSprite& sprite, const rapidjson::Value& spriteJSON, std::shared_ptr< NoMEM::MEMManager > assets);
	void deserializeCPoly(NoECS::CPoly& polygon, const rapidjson::Value& polyJSON);
	void deserializeManager(rapidjson::Value& managerJSON, std::shared_ptr< NoECS::EntityManager > manager, std::shared_ptr< NoMEM::MEMManager > assets=nullptr);
	void deserializeComponents(NoECS::Components& components, const rapidjson::Value& compJSON, std::shared_ptr< NoMEM::MEMManager > assets);

	NoECS::CTransform loadCTransform(const rapidjson::Value& transformJSON);
	NoECS::CBBox loadCBBox(const rapidjson::Value& bboxJSON);
	NoECS::CSprite loadCSprite(const rapidjson::Value& spriteJSON, std::shared_ptr< NoMEM::MEMManager > assets);
	NoECS::CPoly loadCPoly(const rapidjson::Value& polyJSON);
	NoECS::Components loadComponents(const rapidjson::Value& compJSON, std::shared_ptr< NoMEM::MEMManager > assets);
	std::shared_ptr< NoECS::EntityManager > loadManager(std::string path, std::shared_ptr< NoMEM::MEMManager > assets=nullptr);
	
	int saveManager(std::shared_ptr< NoECS::EntityManager > manager, std::shared_ptr< NoMEM::MEMManager > assets=nullptr, const std::string& path="./page.json");
}

namespace NoMEM
{
	void serializeAssets(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, std::shared_ptr< NoMEM::MEMManager > assets);
	void deserializeAssets(std::shared_ptr< NoMEM::MEMManager > assets, const rapidjson::Value& assetJSON);
	int saveAssets(std::shared_ptr< NoMEM::MEMManager > assets, const std::string& path="./assets.json");
	std::shared_ptr< NoMEM::MEMManager > loadAssets(const std::string& filePath="./assets.json");
	std::shared_ptr< NoMEM::MEMManager > loadAssets(NoMEM::Config& dir);
	void loadTextures(std::shared_ptr< NoMEM::MEMManager > assets, const std::string& dirPath);
	void loadSprites(std::shared_ptr< NoMEM::MEMManager > assets, const std::string& dirPath);
	void loadFonts(std::shared_ptr< NoMEM::MEMManager > assets, const std::string& dirPath);
	void loadSounds(std::shared_ptr< NoMEM::MEMManager > assets, const std::string& dirPath);
	void loadMusic(std::shared_ptr< NoMEM::MEMManager > assets, const std::string& dirPath);
}

namespace NoPARSE
{	
	int readFile(const std::string& path, rapidjson::Document&  d);
	int writeFile(rapidjson::StringBuffer& sb, const std::string& path);
}

#endif