#include "../../src/Parse.h"

int main(int argc, char ** argv)
{
	std::shared_ptr< NoMEM::MEMManager > assets = std::make_shared< NoMEM::MEMManager >();
	NoGUI::GUIManager manager = NoGUI::GUIManager();
	
	Color INVISIBLE = (Color){0, 0, 0, 1};
	Color BACKGROUND = (Color){100, 100, 100, 255};
	Vector2 center = {235, 360};
	InitWindow(1280, 1080, "Context");
	std::shared_ptr< Font > font = assets->addFont("jupiter_crash", "../fonts/jupiter_crash.png");
	std::shared_ptr< Texture2D > joinImg = assets->addTexture("joinBack", "../imgs/background.png");
	// text
	NoGUI::CText labelStyle = NoGUI::CText(font);
	labelStyle.col = MAROON;
	labelStyle.align = NoGUI::TextAlign::RIGHT;
	labelStyle.wrap = NoGUI::TextWrap::DOWN;
	labelStyle.margin = (Vector2){0, 0};
	labelStyle.spacing = (Vector2){2, 0};
	labelStyle.size = 25;
	labelStyle.shadow.draw = true;
	labelStyle.shadow.offset = {2, -2};
	NoGUI::CText inputStyle = labelStyle;
	inputStyle.shadow.draw = false;
	NoGUI::CText buttonStyle = inputStyle; // copy
	buttonStyle.align = NoGUI::TextAlign::CENTER;
	NoGUI::CText joinText = inputStyle; // copy
	joinText.align = NoGUI::TextAlign::TOP;
	joinText.margin = (Vector2){0, 0};
	NoGUI::CText hostText = joinText; // copy
	hostText.margin = (Vector2){3, 0};
	NoGUI::CText hostLabelStyle = labelStyle; // copy
	hostLabelStyle.align = NoGUI::TextAlign::LEFT;
	std::shared_ptr< NoGUI::Page > pg = manager.getPage();
	// Styles
	NoGUI::Style backStyle = {BACKGROUND, BLACK, (Vector2){center.x, center.y + 25}, (Vector2){center.x, center.y - 25}, 4, 0, 0};
	NoGUI::Style IPStyle = {INVISIBLE, BLACK, (Vector2){center.x, center.y - 250}, (Vector2){112, 25}, 4, 0, 0};
	NoGUI::Style portStyle = {INVISIBLE, BLACK, (Vector2){center.x, center.y - 150}, (Vector2){112, 25}, 4, 0, 0};
	NoGUI::Style nameStyle = {INVISIBLE, BLACK, (Vector2){center.x, center.y - 50}, (Vector2){112, 25}, 4, 0, 0};
	NoGUI::Style msgStyle = {INVISIBLE, BLACK, (Vector2){center.x, center.y + 50}, (Vector2){112, 25}, 4, 0, 0};
	NoGUI::Style connectStyle = {GRAY, BLACK, (Vector2){center.x, center.y + 225}, (Vector2){115, 50}, 4, 1, 0};
	NoGUI::Style sendStyle = {GREEN, BLACK, (Vector2){center.x - msgStyle.radius.x - 25, msgStyle.pos.y + 30}, (Vector2){25, 25}, 4, 1, 0};
	NoGUI::Style IPInStyle = {LIGHTGRAY, BLACK, (Vector2){IPStyle.pos.x, IPStyle.pos.y + 30}, (Vector2){112, 25}, 4, 2, 0};
	NoGUI::Style portInStyle = {LIGHTGRAY, BLACK, (Vector2){portStyle.pos.x, portStyle.pos.y + 30}, (Vector2){112, 25}, 4, 2, 0};
	NoGUI::Style nameInStyle = {LIGHTGRAY, BLACK, (Vector2){nameStyle.pos.x, nameStyle.pos.y + 30}, (Vector2){112, 25}, 4, 2, 0};
	NoGUI::Style msgInStyle = {LIGHTGRAY, BLACK, (Vector2){msgStyle.pos.x, sendStyle.pos.y}, (Vector2){112, 25}, 4, 2, 0};
	// Components
	pg->addComponents("AImage", std::make_shared< NoGUI::CContainer >());
	pg->addComponents("Button", std::make_shared< NoGUI::CContainer >());
	pg->addComponents("Label", std::make_shared< NoGUI::CContainer >());
	pg->addComponents("Input", std::make_shared< NoGUI::CContainer >());
	
	pg->getComponents("Label")->addComponent< NoGUI::CText >(labelStyle);
	pg->getComponents("Button")->addComponent< NoGUI::CText >(buttonStyle);
	pg->getComponents("Input")->addComponent< NoGUI::CText >(inputStyle);
	pg->getComponents("Input")->addComponent< NoGUI::CInput >(200);
	pg->getComponents("AImage")->addComponent< NoGUI::CImage >(joinImg);
	// Elements
	std::shared_ptr< NoGUI::Element > imgLabel = pg->addElement< NoGUI::Element >(backStyle, "AImage", "Background");
	std::shared_ptr< NoGUI::Element > connectButton = pg->addElement< NoGUI::Button >(connectStyle, "Button", "Connect");
	std::shared_ptr< NoGUI::Element > sendButton = pg->addElement< NoGUI::Button >(sendStyle, "Button", "SEND");
	std::shared_ptr< NoGUI::Element > IPLabel = pg->addElement< NoGUI::Element >(IPStyle, "Label", "IP Address:");
	std::shared_ptr< NoGUI::Element > portLabel = pg->addElement< NoGUI::Element >(portStyle, "Label", "Port:");
	std::shared_ptr< NoGUI::Element > nameLabel = pg->addElement< NoGUI::Element >(nameStyle, "Label", "Username:");
	std::shared_ptr< NoGUI::Element > msgLabel = pg->addElement< NoGUI::Element >(msgStyle, "Label", "Message:");
	std::shared_ptr< NoGUI::Element > IPInput = pg->addElement< NoGUI::Input >(IPInStyle, "Input", "");
	std::shared_ptr< NoGUI::Element > portInput = pg->addElement< NoGUI::Input >(portInStyle, "Input", "");
	std::shared_ptr< NoGUI::Element > nameInput = pg->addElement< NoGUI::Input >(nameInStyle, "Input", "");
	std::shared_ptr< NoGUI::Element > msgInput = pg->addElement< NoGUI::Input >(msgInStyle, "Input", "");
	manager.update();
	
	NoMEM::saveAssets(assets);
	NoGUI::savePage(manager.getPage(0), assets);
	std::shared_ptr< NoGUI::Page > testPg = NoGUI::loadPage("./page.json", assets);
	std::shared_ptr< NoMEM::MEMManager > testMEM = NoMEM::loadAssets("./assets.json");
	NoMEM::saveAssets(testMEM, "./assets(copy).json");
	NoGUI::savePage(testPg, assets, "./page(copy).json");
	assets->clear();
	testMEM->clear();
}