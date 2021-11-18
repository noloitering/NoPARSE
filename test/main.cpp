#include "../src/Parse.h"

int main(int argc, char ** argv)
{
	NoGUI::Style elemStyle = {BLACK, RAYWHITE, (Vector2){640, 40}, (Vector2){75, 35}, 4, 4, 0};
	NoGUI::GUIManager manager = NoGUI::GUIManager();
	Color INVISIBLE = (Color){0, 0, 0, 1};
	Color BACKGROUND = (Color){100, 100, 100, 255};
	Vector2 center = {640, 540};
	// text
	NoGUI::CText titleStyle = NoGUI::CText();
	titleStyle.col = PURPLE;
	titleStyle.align = NoGUI::TextAlign::CENTER;
	titleStyle.size = 50;
	NoGUI::CText labelStyle = NoGUI::CText();
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
	manager.getPage()->addComponent< NoGUI::CText >(labelStyle);
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
	// elements
	std::shared_ptr< NoGUI::Element > imgLabel = manager.getPage()->addElement< NoGUI::Element >(backStyle, "Background", "AImage", "BackgroundImg");
	std::shared_ptr< NoGUI::Element > connectButton = manager.getPage()->addElement< NoGUI::Button >(connectStyle, "Connect", "Button", "Launcher");
	std::shared_ptr< NoGUI::Element > sendButton = manager.getPage()->addElement< NoGUI::Button >(sendStyle, "SEND", "Button");
	std::shared_ptr< NoGUI::Element > IPLabel = manager.getPage()->addElement< NoGUI::Element >(IPStyle, "IP Address:", "Label");
	std::shared_ptr< NoGUI::Element > portLabel = manager.getPage()->addElement< NoGUI::Element >(portStyle, "Port:", "Label");
	std::shared_ptr< NoGUI::Element > nameLabel = manager.getPage()->addElement< NoGUI::Element >(nameStyle, "Username:", "Label");
	std::shared_ptr< NoGUI::Element > msgLabel = manager.getPage()->addElement< NoGUI::Element >(msgStyle, "Message:", "Label");
	std::shared_ptr< NoGUI::Element > IPInput = manager.getPage()->addElement< NoGUI::Input >(IPInStyle, "", "Input", "IP");
	std::shared_ptr< NoGUI::Element > portInput = manager.getPage()->addElement< NoGUI::Input >(portInStyle, "", "Input", "Port");
	std::shared_ptr< NoGUI::Element > nameInput = manager.getPage()->addElement< NoGUI::Input >(nameInStyle, "", "Input", "Username");
	std::shared_ptr< NoGUI::Element > msgInput = manager.getPage()->addElement< NoGUI::Input >(msgInStyle, "", "Input", "Msg");
	// components
IPLabel->addComponent< NoGUI::CText >(labelStyle);
	connectButton->addComponent< NoGUI::CText >(buttonStyle);
	sendButton->addComponent< NoGUI::CText >(buttonStyle);
//	imgLabel->addComponent< NoGUI::CImage >(joinBack);
	IPInput->addComponent< NoGUI::CText >(inputStyle);
	portInput->addComponent< NoGUI::CText >(inputStyle);
	nameInput->addComponent< NoGUI::CText >(inputStyle);
	msgInput->addComponent< NoGUI::CText >(inputStyle);
	IPInput->addComponent< NoGUI::CInput >(50);
	portInput->addComponent< NoGUI::CInput >(10);
	nameInput->addComponent< NoGUI::CInput >(50);
	msgInput->addComponent< NoGUI::CInput >(200);
	manager.update();
	NoGUI::savePage(manager.getPage(0));
}