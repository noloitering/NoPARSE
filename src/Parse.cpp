#include "Parse.h"

using namespace NoPARSE;

int NoGUI::savePage(std::shared_ptr< NoGUI::Page > pg, std::shared_ptr< NoMEM::MEMManager > assets, const std::string& path)
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
				serializeCInput(writer, pgInput);
			}
			if ( pgText.owned )
			{
				writer.Key("Text");
				serializeCText(writer, pgText, assets);
			}
			if ( pgImage.owned )
			{
				writer.Key("Image");
				serializeCImage(writer, pgImage, assets);
			}
			if ( pgStyle.owned )
			{
				writer.Key("MultiStyle");
				serializeCMultiStyle(writer, pgStyle);
			}
			if ( pgDropDown.owned )
			{
				writer.Key("Drop Down");
				serializeCDropDown(writer, pgDropDown);
			}
		writer.EndObject();
		// Elements
		writer.Key("Elements");
		writer.StartObject();
			std::map< std::string, size_t > pgIds = pg->getIds();
			for (auto entry :  pg->getBody())
			{
				writer.Key(entry.first.c_str()); // classname
				writer.StartArray();
					for (auto elem : entry.second) // type
					{
						writer.StartObject();
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
								writer.Key("ID");
								std::string idStr = "";
								for (auto id : pgIds)
								{
									if ( id.second == elem->getId() )
									{
										idStr = id.first;
									
										break;
									}
								}
								if ( !idStr.empty() )
								{
									pgIds.erase(idStr);
								}
								else
								{
									idStr = std::to_string(elem->getId());
								}
								writer.String(idStr.c_str());
								serializeStyle(writer, elem->styling());
								writer.Key("Hover Colour");
								writer.StartArray();
									Color hoverCol = elem->getHoverCol();
									writer.Uint(hoverCol.r);
									writer.Uint(hoverCol.g);
									writer.Uint(hoverCol.b);
									writer.Uint(hoverCol.a);
								writer.EndArray();
								writer.Key("Inner");
								writer.String(elem->getInner().c_str());
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
										serializeCInput(writer, eInput);
									}
									if ( eText.owned )
									{
										writer.Key("Text");
										serializeCText(writer, eText, assets);
									}
									if ( eImage.owned )
									{
										writer.Key("Image");
										serializeCImage(writer, eImage, assets);
									}
									if ( eStyles.owned )
									{
										writer.Key("MultiStyle");
										serializeCMultiStyle(writer, eStyles);
									}
									if ( eDropDown.owned )
									{
										writer.Key("Dropdown");
										serializeCDropDown(writer, eDropDown);
									}
								writer.EndObject();
							writer.EndObject();
						writer.EndObject();
					}
				writer.EndArray();
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
		std::cout << "saving page to: " << path << std::endl;
		out << sb.GetString() << std::endl;
	}
	
	return 0;
}

std::shared_ptr< NoGUI::Page > NoGUI::loadPage(std::string path, std::shared_ptr< NoMEM::MEMManager > assets)
{
	if ( FileExists(path.c_str()) )
	{
		FILE* fp = fopen(path.c_str(), "rb"); // non-Windows use "r"
 
		char readBuffer[65536];
		rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
 
		rapidjson::Document d;
		d.ParseStream(is);
 
		fclose(fp);
		std::shared_ptr< NoGUI::Page > pg = std::make_shared< NoGUI::Page >();
		const rapidjson::Value& pgComps = d["Components"];
		const rapidjson::Value& pgElems = d["Elements"];
		for (auto& v : pgComps.GetObject())
		{
			std::string key(v.name.GetString());
			if ( key == "Text" )
			{
				pg->addComponent< NoGUI::CText >(NoPARSE::deserializeCText(v.value, assets));
			}
			else if ( key == "Image" )
			{
				pg->addComponent< NoGUI::CImage >(NoPARSE::deserializeCImage(v.value, assets));
			}
			else if ( key == "Input" )
			{
				pg->addComponent< NoGUI::CInput >(NoPARSE::deserializeCInput(v.value));
			}
			else if ( key == "MultiStyle" )
			{
				pg->addComponent< NoGUI::CMultiStyle >(NoPARSE::deserializeCMultiStyle(v.value));
			}
			else if ( key == "Dropdown" )
			{
				pg->addComponent< NoGUI::CDropDown >(NoPARSE::deserializeCDropDown(v.value));
			}
		}
		for (auto& classGroup : pgElems.GetObject())
		{
			for (auto& elemData : classGroup.value.GetArray()  )
			{
				for (auto& elem : elemData.GetObject()) // should only be one iteration. We just don't know what the key is going to be
				{
					const rapidjson::Value& data = elem.value.GetObject();
					const rapidjson::Value& hoverArray = data["Hover Colour"].GetArray();
					const rapidjson::Value& elemComps = data["Components"];
					std::string elemType(elem.name.GetString());
					NoGUI::Style elemStyle = deserializeStyle(data);
					std::string elemInner(data["Inner"].GetString());
					std::string elemId(data["ID"].GetString());
					std::string elemTag(classGroup.name.GetString());
					std::shared_ptr< NoGUI::Element > newElem;
					
					Color hovCol;
					hovCol.r = hoverArray[0].GetInt();
					hovCol.g = hoverArray[1].GetInt();
					hovCol.b = hoverArray[2].GetInt();
					hovCol.a = hoverArray[3].GetInt();
					if ( elemType == "Element" )
					{
						newElem = pg->addElement< NoGUI::Element >(elemStyle, elemInner, elemTag, elemId);
					}
					else if ( elemType == "CheckBox" )
					{
						newElem = pg->addElement< NoGUI::CheckBox >(elemStyle, elemInner, elemTag, elemId);
					}
					else if ( elemType == "Button" )
					{
						newElem = pg->addElement< NoGUI::Button >(elemStyle, elemInner, elemTag, elemId);
					}
					else if ( elemType == "Input" )
					{
						newElem = pg->addElement< NoGUI::Input >(elemStyle, elemInner, elemTag, elemId);
					}
					else if ( elemType == "InputButton" )
					{
						newElem = pg->addElement< NoGUI::InputButton >(elemStyle, elemInner, elemTag, elemId);
					}
					else if ( elemType == "InputToggle" )
					{
						newElem = pg->addElement< NoGUI::InputToggle >(elemStyle, elemInner, elemTag, elemId);
					}
					else if ( elemType == "InputTrigger" )
					{
						newElem = pg->addElement< NoGUI::InputTrigger >(elemStyle, elemInner, elemTag, elemId);
					}
					else if ( elemType == "Toggle" )
					{
						newElem = pg->addElement< NoGUI::Toggle >(elemStyle, elemInner, elemTag, elemId);
					}
					else if ( elemType == "Trigger" )
					{
						newElem = pg->addElement< NoGUI::Trigger >(elemStyle, elemInner, elemTag, elemId);
					}
					else
					{
						std::cerr << "Could not parse Element type " << elemType << std::endl;
						
						return nullptr;
					}
					newElem->setHoverCol(hovCol);
					for (auto& v : elemComps.GetObject())
					{
						std::string key(v.name.GetString());
						if ( key == "Text" )
						{
							newElem->addComponent< NoGUI::CText >(NoPARSE::deserializeCText(v.value, assets));
						}
						else if ( key == "Image" )
						{
							newElem->addComponent< NoGUI::CImage >(NoPARSE::deserializeCImage(v.value, assets));
						}
						else if ( key == "Input" )
						{
							newElem->addComponent< NoGUI::CInput >(NoPARSE::deserializeCInput(v.value));
						}
						else if ( key == "MultiStyle" )
						{
							newElem->addComponent< NoGUI::CMultiStyle >(NoPARSE::deserializeCMultiStyle(v.value));
						}
						else if ( key == "Dropdown" )
						{
							newElem->addComponent< NoGUI::CDropDown >(NoPARSE::deserializeCDropDown(v.value));
						}
						else
						{
							std::cerr << "Could not parse Component type " << key << std::endl;
						}
					}
				}
			}
		}
		pg->update();
		
		return pg;
	}
	else
	{
		std::cerr << "could not open JSON file" << std::endl;
		
		return nullptr;
	}
}

NoGUI::CText NoPARSE::deserializeCText(const rapidjson::Value& textJSON, std::shared_ptr< NoMEM::MEMManager > assets)
{
	NoGUI::CText text;
	Color col;
	std::string val(textJSON["Align"].GetString());
	std::string fontKey(textJSON["Font"].GetString());
	const rapidjson::Value& colArray = textJSON["Colour"];
	rapidjson::Value::ConstMemberIterator shadowArray = textJSON.FindMember("Shadow");
	rapidjson::Value::ConstMemberIterator marginArray = textJSON.FindMember("Margin");
	rapidjson::Value::ConstMemberIterator spaceArray = textJSON.FindMember("Whitespace");
	rapidjson::Value::ConstMemberIterator wrapIt = textJSON.FindMember("Wrapping");
	rapidjson::Value::ConstMemberIterator cropIt = textJSON.FindMember("Cropping");
	rapidjson::Value::ConstMemberIterator angleIt = textJSON.FindMember("Angle");
	
	col.r = colArray[0].GetInt();
	col.g = colArray[1].GetInt();
	col.b = colArray[2].GetInt();
	col.a = colArray[3].GetInt();
	text.col = col;
	text.size = textJSON["Size"].GetDouble();
	if ( assets )
	{
		// check to see if already loaded
		std::shared_ptr< Font > font;
		NoMEM::FontMap fonts = assets->getAll< Font >();
		for (auto fontEntry : fonts)
		{
			if ( fontEntry.first == fontKey )
			{
				font = fontEntry.second;
			}
		}
		if ( font )
		{
			text.font = font;
		}
		else
		{
			// check to see if in custom paths
			auto found = assets->conf.find(fontKey);
			if ( found != assets->conf.customEnd() )
			{
				font = assets->addFont(fontKey, found->second);
			}
			if ( font )
			{
				text.font = font;
			}
			else
			{
				// try loading from MEMManager's path
				font = assets->addFont(fontKey);
				if ( font )
				{
					text.font = font;
				}
				else
				{
					// try loading as path
					font = assets->addFont(fontKey, fontKey);
					if ( font )
					{
						text.font == font;
					}
					else
					{
						std::cerr << "Could not load font: " << fontKey << std::endl;
					}
				}
			}
		}
	}
	if (marginArray != textJSON.MemberEnd())
	{
		text.margin.x = marginArray->value[0].GetDouble();
		text.margin.y = marginArray->value[1].GetDouble();
	}
	if (spaceArray != textJSON.MemberEnd())
	{
		text.spacing.x = spaceArray->value[0].GetDouble();
		text.spacing.y = spaceArray->value[1].GetDouble();
	}
	if (shadowArray != textJSON.MemberEnd())
	{
		text.shadow.offset.x = shadowArray->value[0].GetDouble();
		text.shadow.offset.y = shadowArray->value[1].GetDouble();
		text.shadow.col.r = shadowArray->value[2].GetInt();
		text.shadow.col.g = shadowArray->value[3].GetInt();
		text.shadow.col.b = shadowArray->value[4].GetInt();
		text.shadow.col.a = shadowArray->value[5].GetInt();
		text.shadow.draw = true;
	}
	for (auto entry : AlignMap)
	{
		if ( entry.second ==  val)
		{
			text.align = entry.first;
		}
	}
	if ( cropIt != textJSON.MemberEnd() )
	{
		val = std::string(cropIt->value.GetString());
		for (auto entry : CropMap)
		{
			if ( entry.second ==  val )
			{
				text.cropping = entry.first;
			}
		}
	}
	if ( wrapIt != textJSON.MemberEnd() )
	{
		val = std::string(wrapIt->value.GetString());
		for (auto entry : WrapMap)
		{
			if ( entry.second == val )
			{
				text.wrap = entry.first;
			}
		}
	}
	if ( angleIt != textJSON.MemberEnd() )
	{
		text.rotation = angleIt->value.GetDouble();
	}
	text.owned = true;
	
	return text;
}

NoGUI::CImage NoPARSE::deserializeCImage(const rapidjson::Value& imgJSON, std::shared_ptr< NoMEM::MEMManager > assets)
{
	NoGUI::CImage img;
	Color col;
	std::string textureKey(imgJSON["File"].GetString());
	rapidjson::Value::ConstMemberIterator colArray = imgJSON.FindMember("Back Colour");
	rapidjson::Value::ConstMemberIterator marginArray = imgJSON.FindMember("Margin");
	rapidjson::Value::ConstMemberIterator scaleArray = imgJSON.FindMember("Scale");
	rapidjson::Value::ConstMemberIterator angleIt = imgJSON.FindMember("Angle");
	rapidjson::Value::ConstMemberIterator cropIt = imgJSON.FindMember("Cropping");
	
	if ( assets )
	{
		// check to see if already loaded
		std::shared_ptr< Texture2D > texture;
		NoMEM::TextureMap textures = assets->getAll< Texture2D >();
		for (auto textureEntry : textures)
		{
			if ( textureEntry.first == textureKey )
			{
				texture = textureEntry.second;
			}
		}
		if ( texture )
		{
			img.texture = texture;
		}
		else
		{
			// check to see if in custom paths
			auto found = assets->conf.find(textureKey);
			if ( found != assets->conf.customEnd() )
			{
				texture = assets->addTexture(textureKey, found->second);
			}
			if ( texture )
			{
				img.texture = texture;
			}
			else
			{
				// try loading from MEMManager's path
				texture = assets->addTexture(textureKey);
				if ( texture )
				{
					img.texture = texture;
				}
				else
				{
					// try loading as path
					texture = assets->addTexture(textureKey, textureKey);
					if ( texture )
					{
						img.texture == texture;
					}
					else
					{
						std::cerr << "Could not load texture: " << textureKey << std::endl;
					}
				}
			}
		}
	}
	if ( colArray != imgJSON.MemberEnd() )
	{
		col.r = colArray->value[0].GetInt();
		col.g = colArray->value[1].GetInt();
		col.b = colArray->value[2].GetInt();
		col.a = colArray->value[3].GetInt();
		img.col = col;
	}
	if ( marginArray != imgJSON.MemberEnd() )
	{
		img.margin.x = marginArray->value[0].GetDouble();
		img.margin.y = marginArray->value[1].GetDouble();
	}
	if ( scaleArray != imgJSON.MemberEnd() )
	{
		img.scale.x = scaleArray->value[0].GetDouble();
		img.scale.y = scaleArray->value[1].GetDouble();
	}
	if ( cropIt != imgJSON.MemberEnd() )
	{
		std::string val(cropIt->value.GetString());
		for (auto entry : CropMap)
		{
			if ( entry.second ==  val )
			{
				img.cropping = entry.first;
			}
		}
	}
	if ( angleIt != imgJSON.MemberEnd() )
	{
		img.rotation = angleIt->value.GetDouble();
	}
	img.owned = true;
	
	return img;
}

NoGUI::CInput NoPARSE::deserializeCInput(const rapidjson::Value& inputJSON)
{
	NoGUI::CInput input;
	Color col;
	rapidjson::Value::ConstMemberIterator i = inputJSON.FindMember("Index");
	
	input.cap = inputJSON["Max"].GetInt();
	if ( i != inputJSON.MemberEnd() )
	{
		input.cap = i->value.GetDouble();
	}
	input.owned = true;
	
	return input;
}

NoGUI::CMultiStyle NoPARSE::deserializeCMultiStyle(const rapidjson::Value& stylesJSON)
{
	NoGUI::CMultiStyle styles;
	for (auto& v : stylesJSON.GetArray())
	{
		styles.styles.push_back(deserializeStyle(v));
	}
	styles.owned = true;
	
	return styles;
}

NoGUI::CDropDown NoPARSE::deserializeCDropDown(const rapidjson::Value& dropJSON)
{
	NoGUI::CDropDown dropdown;
	rapidjson::Value::ConstMemberIterator spaceIt = dropJSON.FindMember("Spacing");
	rapidjson::Value::ConstMemberIterator wrapIt = dropJSON.FindMember("Wrapping");
	rapidjson::Value::ConstMemberIterator alignIt = dropJSON.FindMember("Align");
	
	if ( alignIt != dropJSON.MemberEnd() )
	{
		std::string val(alignIt->value.GetString());
		for (auto entry : AlignMap)
		{
			if ( entry.second ==  val )
			{
				dropdown.align = entry.first;
			}
		}
	}
	if ( wrapIt != dropJSON.MemberEnd() )
	{
		std::string val(wrapIt->value.GetString());
		for (auto entry : WrapMap)
		{
			if ( entry.second ==  val )
			{
				dropdown.wrap = entry.first;
			}
		}
	}
	if ( spaceIt != dropJSON.MemberEnd() )
	{
		dropdown.spacing = spaceIt->value.GetDouble();
	}
	dropdown.owned = true;
	
	return dropdown;
}

NoGUI::Style NoPARSE::deserializeStyle(const rapidjson::Value& elemJSON)
{
	NoGUI::Style style;
	Color col;
	Color outCol;
	const rapidjson::Value& colArray = elemJSON["Colour"].GetArray();
	const rapidjson::Value& sizeArray = elemJSON["Size"].GetArray();
	const rapidjson::Value& posArray = elemJSON["Position"].GetArray();
	rapidjson::Value::ConstMemberIterator outlineArray = elemJSON.FindMember("Outline");
	
	col.r = colArray[0].GetInt();
	col.g = colArray[1].GetInt();
	col.b = colArray[2].GetInt();
	col.a = colArray[3].GetInt();
	style.backCol = col;
	style.sides = elemJSON["Num Sides"].GetInt();
	style.pos.x = posArray[0].GetDouble();
	style.pos.y = posArray[1].GetDouble();
	style.radius.x = sizeArray[0].GetDouble();
	style.radius.y = sizeArray[1].GetDouble();
	if ( outlineArray != elemJSON.MemberEnd() )
	{
		outCol.r = outlineArray->value[0].GetInt();
		outCol.g = outlineArray->value[1].GetInt();
		outCol.b = outlineArray->value[2].GetInt();
		outCol.a = outlineArray->value[3].GetInt();
		style.outlineThick = outlineArray->value[4].GetDouble();
		style.outlineCol = outCol;
	}
	
	return style;
}



void NoPARSE::serializeStyle(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, const NoGUI::Style& style)
{
	writer.Key("Num Sides");
	writer.Uint(style.sides);
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
}

void NoPARSE::serializeCMultiStyle(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, const NoGUI::CMultiStyle& styles)
{
	writer.StartObject();
		writer.StartArray();
			for (auto style : styles.styles)
			{
				writer.StartObject();
					serializeStyle(writer, style);
				writer.EndObject();
			}
		writer.EndArray();
	writer.EndObject();
}

void NoPARSE::serializeCImage(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, const NoGUI::CImage& imageFmt, std::shared_ptr< NoMEM::MEMManager > assets)
{
	writer.StartObject();
		writer.Key("File");
		std::string imgStr = "";
		if ( imageFmt.texture && assets )
		{
			NoMEM::TextureMap textures = assets->getAll< Texture2D >();
			for (auto texture : textures)
			{
				if ( texture.second == imageFmt.texture )
				{
					imgStr = texture.first;
				}
			}
		}
		writer.String(imgStr.c_str());
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

void NoPARSE::serializeCText(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, const NoGUI::CText& textFmt, std::shared_ptr< NoMEM::MEMManager > assets)
{
	writer.StartObject();
		writer.Key("Font");
		std::string fontStr = "";
		if ( textFmt.font && assets )
		{
			NoMEM::FontMap fonts = assets->getAll< Font >();
			for (auto font : fonts)
			{
				if ( font.second == textFmt.font )
				{
					fontStr = font.first;
				}
			}
		}
		writer.String(fontStr.c_str());
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

void NoPARSE::serializeCDropDown(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, const NoGUI::CDropDown& dropFmt)
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

void NoPARSE::serializeCInput(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, const NoGUI::CInput& inputFmt)
{
	writer.StartObject();
		writer.Key("Max");
		writer.Uint(inputFmt.cap);
		writer.Key("Index");
		writer.Uint(inputFmt.i);
	writer.EndObject();
}