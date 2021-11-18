#include "Parse.h"

using namespace NoPARSE;

int NoGUI::savePage(std::shared_ptr< NoGUI::Page > pg, std::string path)
{
	NoGUI::CText pgText = pg->getComponent< NoGUI::CText >();
	NoGUI::CInput pgInput = pg->getComponent< NoGUI::CInput >();
	NoGUI::CImage pgImage = pg->getComponent< NoGUI::CImage >();
	NoGUI::CDropDown pgDropDown = pg->getComponent< NoGUI::CDropDown >();
	NoGUI::CMultiStyle pgStyle = pg->getComponent< NoGUI::CMultiStyle >();
	rapidjson::StringBuffer sb;
	rapidjson::PrettyWriter< rapidjson::StringBuffer > writer(sb);
	writer.StartObject();
		// Page Components
		writer.Key("Components");
		writer.StartObject();
			if ( pgInput.owned )
			{
				writer.Key("Input");
				seralizeCInput(writer, pgInput);
			}
			if ( pgText.owned )
			{
				writer.Key("Text");
				seralizeCText(writer, pgText);
			}
			if ( pgImage.owned )
			{
				writer.Key("Image");
				seralizeCImage(writer, pgImage);
			}
			if ( pgStyle.owned )
			{
				writer.Key("MultiStyle");
				seralizeCMultiStyle(writer, pgStyle);
			}
			if ( pgDropDown.owned )
			{
				writer.Key("Drop Down");
				seralizeCDropDown(writer, pgDropDown);
			}
		writer.EndObject();
		// Elements
		writer.Key("Elements");
		writer.StartObject();
			for (auto elem :  pg->getElements())
			{
				if ( dynamic_cast< NoGUI::CheckBox* >(elem.get()) )
				{
					writer.Key("CheckBox");
				}
				else if ( dynamic_cast< NoGUI::Button* >(elem.get()) )
				{
					writer.Key("Button");
				}
				else if ( dynamic_cast< NoGUI::Input* >(elem.get()) )
				{
					writer.Key("Input");
				}
				else if ( dynamic_cast< NoGUI::InputButton* >(elem.get()) )
				{
					writer.Key("InputButton");
				}
				else if ( dynamic_cast< NoGUI::InputToggle* >(elem.get()) )
				{
					writer.Key("InputToggle");
				}
				else if ( dynamic_cast< NoGUI::InputTrigger* >(elem.get()) )
				{
					writer.Key("InputTrigger");
				}
				else if ( dynamic_cast< NoGUI::Toggle* >(elem.get()) )
				{
					writer.Key("Toggle");
				}
				else if ( dynamic_cast< NoGUI::Trigger* >(elem.get()) )
				{
					writer.Key("Trigger");
				}
				else
				{
					writer.Key("Element");
				}
				writer.StartObject();
					writer.Key("Style");
					seralizeStyle(writer, elem->styling());
					writer.Key("Components");
					NoGUI::CText eText = elem->getComponent< NoGUI::CText >();
					NoGUI::CInput eInput = elem->getComponent< NoGUI::CInput >();
					NoGUI::CImage eImage = elem->getComponent< NoGUI::CImage >();
					NoGUI::CDropDown eDropDown = elem->getComponent< NoGUI::CDropDown >();
					NoGUI::CMultiStyle eStyles = elem->getComponent< NoGUI::CMultiStyle >();
					writer.StartObject();
						if ( eInput.owned )
						{
							writer.Key("Input");
							seralizeCInput(writer, eInput);
						}
						if ( eText.owned )
						{
							writer.Key("Text");
							seralizeCText(writer, eText);
						}
						if ( eImage.owned )
						{
							writer.Key("Image");
							seralizeCImage(writer, eImage);
						}
						if ( eStyles.owned )
						{
							writer.Key("MultiStyle");
							seralizeCMultiStyle(writer, eStyles);
						}
						if ( eDropDown.owned )
						{
							writer.Key("Drop Down");
							seralizeCDropDown(writer, eDropDown);
						}
					writer.EndObject();
				writer.EndObject();
			}
		writer.EndObject();
	writer.EndObject();
	// Write out
	std::ofstream out(path);
	if (!out)
	{
		std::cerr << "Could not open file for saving" << std::endl;
			
		return 1;
	}
	else
	{
		std::cout << sb.GetString() << std::endl;
		out << sb.GetString() << std::endl;
	}
	
	return 0;
}

void NoPARSE::seralizeStyle(rapidjson::Writer< rapidjson::StringBuffer >& writer, const NoGUI::Style& style)
{
	writer.StartObject();
		writer.Key("Colour");
		writer.StartArray();
			writer.Uint(style.backCol.r);
			writer.Uint(style.backCol.g);
			writer.Uint(style.backCol.b);
			writer.Uint(style.backCol.a);
		writer.EndArray();
		writer.Key("Outline");
		writer.StartArray();
			writer.Uint(style.backCol.r);
			writer.Uint(style.backCol.g);
			writer.Uint(style.backCol.b);
			writer.Uint(style.backCol.a);
			writer.Uint(style.outlineThick);
		writer.EndArray();
		writer.Key("Position");
		writer.StartArray();
			writer.Uint(style.pos.x);
			writer.Uint(style.pos.y);
		writer.EndArray();
		writer.Key("Size");
		writer.StartArray();
			writer.Uint(style.radius.x);
			writer.Uint(style.radius.y);
		writer.EndArray();
		writer.Key("Num Sides");
		writer.Uint(style.sides);
	writer.EndObject();
}

void NoPARSE::seralizeCMultiStyle(rapidjson::Writer< rapidjson::StringBuffer >& writer, const NoGUI::CMultiStyle& styles)
{
	writer.StartObject();
		writer.StartArray();
			for (auto style : styles.styles)
			{
				seralizeStyle(writer, style);
			}
		writer.EndArray();
	writer.EndObject();
}

void NoPARSE::seralizeCImage(rapidjson::Writer< rapidjson::StringBuffer >& writer, const NoGUI::CImage& imageFmt)
{
	writer.StartObject();
		writer.Key("File");
		writer.String(""); // TODO: implement
		writer.Key("Scale");
		writer.StartArray();
			writer.Double(imageFmt.margin.x);
			writer.Double(imageFmt.margin.y);
		writer.EndArray();
		writer.Key("Margin");
		writer.StartArray();
			writer.Double(imageFmt.margin.x);
			writer.Double(imageFmt.margin.y);
		writer.EndArray();
		writer.Key("Angle");
		writer.Double(imageFmt.rotation);
		writer.Key("Cropping");
		writer.String(CropMap.at(imageFmt.cropping).c_str());
		writer.Key("Back Colour");
		writer.StartArray();
			writer.Uint(imageFmt.col.r);
			writer.Uint(imageFmt.col.g);
			writer.Uint(imageFmt.col.b);
			writer.Uint(imageFmt.col.a);
		writer.EndArray();
	writer.EndObject();
}

void NoPARSE::seralizeCDropDown(rapidjson::Writer< rapidjson::StringBuffer >& writer, const NoGUI::CDropDown& dropFmt)
{
	writer.StartObject();
		writer.Key("File");
		writer.String(""); // TODO: implement
		writer.Key("Spacing");
		writer.Double(dropFmt.spacing);
		writer.EndArray();
		writer.Key("Wrapping");
		writer.String(WrapMap.at(dropFmt.wrap).c_str());
		writer.Key("Align");
		writer.String(AlignMap.at(dropFmt.align).c_str());
	writer.EndObject();
}

void NoPARSE::seralizeCInput(rapidjson::Writer< rapidjson::StringBuffer >& writer, const NoGUI::CInput& inputFmt)
{
	writer.StartObject();
		writer.Key("Max");
		writer.Uint(inputFmt.cap);
		writer.Key("Index");
		writer.Uint(inputFmt.i);
	writer.EndObject();
}

void NoPARSE::seralizeCText(rapidjson::Writer< rapidjson::StringBuffer >& writer, const NoGUI::CText& textFmt)
{
	writer.StartObject();
		writer.Key("Font");
		writer.String(""); // TODO: implement
		writer.Key("Size");
		writer.Double(textFmt.size);
		writer.Key("Colour");
		writer.StartArray();
			writer.Uint(textFmt.col.r);
			writer.Uint(textFmt.col.g);
			writer.Uint(textFmt.col.b);
			writer.Uint(textFmt.col.a);
		writer.EndArray();
		writer.Key("Margin");
		writer.StartArray();
			writer.Double(textFmt.margin.x);
			writer.Double(textFmt.margin.y);
		writer.EndArray();
		writer.Key("Whitespace");
		writer.StartArray();
			writer.Double(textFmt.spacing.x);
			writer.Double(textFmt.spacing.y);
		writer.EndArray();
		writer.Key("Angle");
		writer.Double(textFmt.rotation);
		writer.Key("Wrapping");
		writer.String(WrapMap.at(textFmt.wrap).c_str());
		writer.Key("Align");
		writer.String(AlignMap.at(textFmt.align).c_str());
		writer.Key("Cropping");
		writer.String(CropMap.at(textFmt.cropping).c_str());
		if ( textFmt.shadow.draw )
		{
			writer.Key("Shadow");
			writer.StartArray();
				writer.Double(textFmt.shadow.offset.x);
				writer.Double(textFmt.shadow.offset.y);
				writer.Uint(textFmt.shadow.col.r);
				writer.Uint(textFmt.shadow.col.g);
				writer.Uint(textFmt.shadow.col.b);
				writer.Uint(textFmt.shadow.col.a);
			writer.EndArray();
		}
	writer.EndObject();
}