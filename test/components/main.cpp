#include "../../src/Parse.h"

int main(int argc, char ** argv)
{
	InitWindow(1280, 1080, "Context");
	SetTargetFPS(60);
	NoGUI::GUIManager manager = NoGUI::GUIManager();
	std::shared_ptr< NoMEM::MEMManager > assets = std::make_shared< NoMEM::MEMManager >();
	Vector2 center = {640, 540};
	int spacing = 100;
	Color INVISIBLE = (Color){0, 0, 0, 1};
	
	NoGUI::CText textStyle = NoGUI::CText();
	textStyle.align = NoGUI::TextAlign::CENTER;
	NoGUI::CText labelText = NoGUI::CText();
	labelText.align = NoGUI::TextAlign::TOP;
	labelText.wrap = NoGUI::TextWrap::DOWN;
	labelText.margin.y = 2;
	
	std::vector< NoGUI::Style > styles;
	
	NoGUI::Style elemStyle = {BLACK, RAYWHITE, (Vector2){center.x, 40}, (Vector2){75, 35}, 4, 4, 0};
	NoGUI::Style elemText = {INVISIBLE, BLACK, (Vector2){center.x, elemStyle.pos.y + elemStyle.radius.y * 2 + 25}, (Vector2){200, 50}, 4, 4, 0};
	NoGUI::Style imageStyle = {BLACK, RAYWHITE, (Vector2){center.x, elemText.pos.y + spacing}, (Vector2){75, 35}, 4, 4, 0};
	NoGUI::Style imageText = {INVISIBLE, BLACK, (Vector2){center.x, imageStyle.pos.y + imageStyle.radius.y * 2 + 25}, (Vector2){200, 50}, 4, 4, 0};
	NoGUI::Style inputStyle = {BLACK, RAYWHITE, (Vector2){center.x, imageText.pos.y + spacing}, (Vector2){75, 35}, 4, 4, 0};
	NoGUI::Style inputText = {INVISIBLE, BLACK, (Vector2){center.x, inputStyle.pos.y + imageStyle.radius.y * 2 + 25}, (Vector2){200, 50}, 4, 4, 0};
	NoGUI::Style boxStyle = {BLACK, RAYWHITE, (Vector2){center.x, inputText.pos.y + spacing}, (Vector2){75, 35}, 4, 4, 0};
	NoGUI::Style boxText = {INVISIBLE, BLACK, (Vector2){center.x, boxStyle.pos.y + inputStyle.radius.y * 2 + 25}, (Vector2){200, 50}, 4, 4, 0};
	NoGUI::Style boxTriStyle = {BLACK, RAYWHITE, (Vector2){-50, 0}, (Vector2){25, 25}, 3, 1, 0};
	styles.push_back(boxTriStyle);
	NoGUI::Style boxCircleStyle = {BLACK, RAYWHITE, (Vector2){0, 0}, (Vector2){25, 25}, 0, 1, 0};
	styles.push_back(boxCircleStyle);
	NoGUI::Style boxSquareStyle = {BLACK, RAYWHITE, (Vector2){50, 0}, (Vector2){25, 25}, 4, 1, 0};
	styles.push_back(boxSquareStyle);
	NoGUI::Style comboStyle = {BLACK, RAYWHITE, (Vector2){center.x, boxText.pos.y + spacing}, (Vector2){75, 35}, 4, 4, 0};
	NoGUI::Style comboText = {INVISIBLE, BLACK, (Vector2){center.x, comboStyle.pos.y + boxStyle.radius.y * 2 + 25}, (Vector2){200, 50}, 4, 4, 0};
	
	std::shared_ptr< NoGUI::Page > page = manager.getPage();
	page->addComponents("Label", std::make_shared< NoGUI::CContainer >());
	page->addComponents("Base", std::make_shared< NoGUI::CContainer >());
	page->addComponents("Image", std::make_shared< NoGUI::CContainer >());
	page->addComponents("Input", std::make_shared< NoGUI::CContainer >());
	page->addComponents("ZInput", std::make_shared< NoGUI::CContainer >());
	page->addComponents("Multi", std::make_shared< NoGUI::CContainer >());
	
	page->getComponents("Label")->addComponent< NoGUI::CText >(labelText);
	page->getComponents("Base")->addComponent< NoGUI::CText >(textStyle);
	page->getComponents("Input")->addComponent< NoGUI::CText >(textStyle);
	page->getComponents("Input")->addComponent< NoGUI::CInput >();
	page->getComponents("ZInput")->addComponent< NoGUI::CText >(textStyle);
	page->getComponents("Multi")->addComponent< NoGUI::CText >(textStyle);
	page->getComponents("Multi")->addComponent< NoGUI::CMultiStyle >(styles);
	std::shared_ptr< Texture > texture = std::make_shared< Texture >(LoadTexture("../imgs/ImageText.png"));
	page->getComponents("Image")->addComponent< NoGUI::CImage >(texture);
	
	std::shared_ptr< NoGUI::Element > element = manager.addElement< NoGUI::Element >(elemStyle, "Text", "Base");
	std::shared_ptr< NoGUI::Element > elemLabel = manager.addElement< NoGUI::Element >(elemText, "draws Element::inner relative to Element's position", "Label");
	std::shared_ptr< NoGUI::Element > image = manager.addElement< NoGUI::Element >(imageStyle, "Image", "Image");
	std::shared_ptr< NoGUI::Element > imageLabel = manager.addElement< NoGUI::Element >(imageText, "draws a texture relative to Element's position", "Label");
	std::shared_ptr< NoGUI::Element > input = manager.addElement< NoGUI::Input >(inputStyle, "Input", "Input");
	std::shared_ptr< NoGUI::Element > inputLabel = manager.addElement< NoGUI::Element >(inputText, "can enter/remove characters when hovering over", "Label");
	std::shared_ptr< NoGUI::Element > box = manager.addElement< NoGUI::Element >(boxStyle, "Multi Style", "Multi");
	std::shared_ptr< NoGUI::Element > boxLabel = manager.addElement< NoGUI::Element >(boxText, "Draws multiple shapes relative to Element's position", "Label");
	std::shared_ptr< NoGUI::Element > combo = manager.addElement< NoGUI::Input >(comboStyle, "Drop Down", "ZInput");
	std::shared_ptr< NoGUI::Element > comboLabel = manager.addElement< NoGUI::Element >(comboText, "Draws multiple elements relative to Element's position on focus", "Label");
	std::shared_ptr< NoGUI::DropDown > dropdown = manager.addDropDown(combo, NoGUI::TextWrap::DOWN);
	dropdown->addComponents("Option", std::make_shared< NoGUI::CContainer >());
	dropdown->getComponents("Option")->addComponent< NoGUI::CText >(textStyle);
	std::shared_ptr< NoGUI::Element > option1 = dropdown->addElement< NoGUI::Button >("Option 1");
	std::shared_ptr< NoGUI::Element > option2 = dropdown->addElement< NoGUI::Button >("Option 2");
	manager.update();
	NoGUI::savePage(manager.getPage(0), assets);
	std::cout << "loading copy!" << std::endl;
	std::shared_ptr< NoGUI::Page > testPg = NoGUI::loadPage("./page.json", assets);
	std::cout << "saving copy!" << std::endl;
	NoGUI::savePage(testPg, assets, "./page(copy).json");
	assets->clear();
	
	return 0;
}