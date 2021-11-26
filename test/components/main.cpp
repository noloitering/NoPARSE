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
	
	std::shared_ptr< Texture > texture = assets->addTexture("ImageText", "../imgs/ImageText.png");
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
	
	std::shared_ptr< NoGUI::Element > element = manager.addElement< NoGUI::Element >(elemStyle, "Text", "Base");
	std::shared_ptr< NoGUI::Element > elemLabel = manager.addElement< NoGUI::Element >(elemText, "draws Element::inner relative to Element's position", "Label");
	std::shared_ptr< NoGUI::Element > image = manager.addElement< NoGUI::Element >(imageStyle, "Image", "Image");
	std::shared_ptr< NoGUI::Element > imageLabel = manager.addElement< NoGUI::Element >(imageText, "draws a texture relative to Element's position", "Label");
	std::shared_ptr< NoGUI::Element > input = manager.addElement< NoGUI::Input >(inputStyle, "Input", "Input");
	std::shared_ptr< NoGUI::Element > inputLabel = manager.addElement< NoGUI::Element >(inputText, "can enter/remove characters when hovering over", "Label");
	std::shared_ptr< NoGUI::Element > box = manager.addElement< NoGUI::Element >(boxStyle, "Multi Style", "Input");
	std::shared_ptr< NoGUI::Element > boxLabel = manager.addElement< NoGUI::Element >(boxText, "Draws multiple shapes relative to Element's position", "Label");
	std::shared_ptr< NoGUI::Element > combo = manager.addElement< NoGUI::Input >(comboStyle, "Drop Down", "ZInput");
	std::shared_ptr< NoGUI::Element > comboLabel = manager.addElement< NoGUI::Element >(comboText, "Draws multiple elements relative to Element's position on focus", "Label");
	std::shared_ptr< NoGUI::DropDown > dropdown = manager.addDropDown(combo, NoGUI::TextWrap::DOWN);
	dropdown->addComponent< NoGUI::CText >(textStyle);
	std::shared_ptr< NoGUI::Element > option1 = dropdown->addElement< NoGUI::Button >("Option 1");
	std::shared_ptr< NoGUI::Element > option2 = dropdown->addElement< NoGUI::Button >("Option 2");
	
	elemLabel->addComponent< NoGUI::CText >(labelText);
	imageLabel->addComponent< NoGUI::CText >(labelText);
	inputLabel->addComponent< NoGUI::CText >(labelText);
	boxLabel->addComponent< NoGUI::CText >(labelText);
	comboLabel->addComponent< NoGUI::CText >(labelText);
	element->addComponent< NoGUI::CText >(textStyle);
	image->addComponent< NoGUI::CImage >(texture);
	input->addComponent< NoGUI::CText >(textStyle);
	input->addComponent< NoGUI::CInput >();
//	box->addComponent< NoGUI::CMultiStyle >(styles);
	box->addComponent< NoGUI::CText >(textStyle);
	combo->addComponent< NoGUI::CText >(textStyle);
	manager.update();
	NoGUI::savePage(manager.getPage(0), assets);
	std::shared_ptr< NoGUI::Page > testPg = NoGUI::loadPage("./page.json", assets);
	NoGUI::savePage(testPg, assets, "./page(copy).json");
	assets->clear();
	
	return 0;
}