#include "Parse.h"

using namespace NoPARSE;

int NoGUI::savePage(std::shared_ptr< NoGUI::Page > pg, std::shared_ptr< NoMEM::MEMManager > assets, const std::string& path)
{
	rapidjson::StringBuffer sb;
	rapidjson::PrettyWriter< rapidjson::StringBuffer > writer(sb);
	// create path for Page DropDown Component
	char dirChar = '/';
	std::string dropPath;
	// replace Page filename with dropdown.json
	size_t dirPos = path.rfind(dirChar);
	if (  dirPos != std::string::npos )
	{
		dropPath = path.substr(0, dirPos + 1);
	}
	else
	{
		dirChar = '\\';
		dropPath = path.substr(0, path.rfind(dirChar) + 1) + "dropdown.json";
	}
	if ( dropPath.front() == '.' ) // convert to full path
	{
		dropPath = std::string(GetWorkingDirectory()) + dropPath.substr(dropPath.find(dirChar), dropPath.size());
	}
	writer.StartObject();
		// Loaded Files
		if ( assets )
		{
			serializeAssets(writer, assets);
		}
		// Page Components
		serializeComponents(writer, pg->getComponents(), assets, dropPath);
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
						std::string elemDropPath = dropPath.substr(0, dropPath.rfind(dirChar) + 1) + idStr + ".json";
						serializeElement(writer, elem, idStr, assets, elemDropPath);
					}
				writer.EndArray();
			}
		writer.EndObject();
	writer.EndObject();
	// Write out
	int res = writeFile(sb, path);
	
	return res;
}

int NoMEM::saveAssets(std::shared_ptr< NoMEM::MEMManager > assets, const std::string& path)
{
	rapidjson::StringBuffer sb;
	rapidjson::PrettyWriter< rapidjson::StringBuffer > writer(sb);
	writer.StartObject();
		serializeAssets(writer, assets);
	writer.EndObject();
	
	int res = writeFile(sb, path);
	
	return res;
}

std::shared_ptr< NoMEM::MEMManager > NoMEM::loadAssets(const std::string& filePath)
{
	rapidjson::Document d;
	if ( readFile(filePath, d) == 0 )
	{
		std::shared_ptr< NoMEM::MEMManager > assets = std::make_shared< NoMEM::MEMManager >();
		const rapidjson::Value& assetJSON = d["Assets"].GetObject();
		deserializeAssets(assets, assetJSON);
		
		return assets;
	}
	else
	{

		return nullptr;
	}
}

std::shared_ptr< NoMEM::MEMManager > NoMEM::loadAssets(NoMEM::Config& dir)
{
	std::shared_ptr< NoMEM::MEMManager > assets = std::make_shared< NoMEM::MEMManager >(dir);
	std::string texturePath = (dir.cwd() + dir.textureDir);
	std::string spritePath = (dir.cwd() + dir.spriteDir);
	std::string fontPath = (dir.cwd() + dir.fontDir);
	std::string soundPath = (dir.cwd() + dir.soundDir);
	std::string musicPath = (dir.cwd() + dir.musicDir);
	
	NoMEM::loadTextures(assets, texturePath);
	NoMEM::loadSprites(assets, spritePath);
	NoMEM::loadFonts(assets, fontPath);
	NoMEM::loadSounds(assets, soundPath);
	NoMEM::loadMusic(assets, musicPath);
	
	return assets;
}

void NoMEM::loadTextures(std::shared_ptr< NoMEM::MEMManager > assets, const std::string& dirPath)
{
	int count = 0;
	char **textures = GetDirectoryFiles(dirPath.c_str(), &count);
	for (int i=0; i < count; i++)
	{
		int fileChars = strlen(textures[i]);
		std::string filePath = dirPath + std::string(textures[i]);
		if ( fileChars > 4 ) // check to see if valid file
		{
			if ( textures[i][fileChars - 4] == '.' ) // check for file extension
			{
				std::string name(GetFileNameWithoutExt(filePath.c_str()));
				assets->addTexture(name);
			}
		}
		else
		{
			std::cout << "could not recognize: " << GetFileNameWithoutExt(filePath.c_str()) << std::endl;
		}
	}
	ClearDirectoryFiles();
}

void NoMEM::loadSprites(std::shared_ptr< NoMEM::MEMManager > assets, const std::string& dirPath)
{
	int count = 0;
	char **sprites = GetDirectoryFiles(dirPath.c_str(), &count);
	for (int i=0; i < count; i++)
	{
		int fileChars = strlen(sprites[i]);
		std::string filePath = dirPath + std::string(sprites[i]);
		if ( fileChars > 4 ) // check to see if valid file
		{
			if ( sprites[i][fileChars - 4] == '.' ) // check for file extension
			{
				std::string name(GetFileNameWithoutExt(filePath.c_str()));
				assets->addSprite(name);
			}
		}
		else
		{
			std::cout << "could not recognize: " << GetFileNameWithoutExt(filePath.c_str()) << std::endl;
		}
	}
	ClearDirectoryFiles();
}

void NoMEM::loadFonts(std::shared_ptr< NoMEM::MEMManager > assets, const std::string& dirPath)
{
	int count = 0;
	char **fonts = GetDirectoryFiles(dirPath.c_str(), &count);
	for (int i=0; i < count; i++)
	{
		int fileChars = strlen(fonts[i]);
		std::string filePath = dirPath + std::string(fonts[i]);
		if ( fileChars > 4 ) // check to see if valid file
		{
			if ( fonts[i][fileChars - 4] == '.' ) // check for file extension
			{
				std::string name(GetFileNameWithoutExt(filePath.c_str()));
				assets->addFont(name);
			}
		}
		else
		{
			std::cout << "could not recognize: " << GetFileNameWithoutExt(filePath.c_str()) << std::endl;
		}
	}
	ClearDirectoryFiles();
}

void NoMEM::loadSounds(std::shared_ptr< NoMEM::MEMManager > assets, const std::string& dirPath)
{
	int count = 0;
	char **sounds = GetDirectoryFiles(dirPath.c_str(), &count);
	for (int i=0; i < count; i++)
	{
		int fileChars = strlen(sounds[i]);
		std::string filePath = dirPath + std::string(sounds[i]);
		if ( fileChars > 4 ) // check to see if valid file
		{
			if ( sounds[i][fileChars - 4] == '.' ) // check for file extension
			{
				std::string name(GetFileNameWithoutExt(filePath.c_str()));
				assets->addSound(name);
			}
		}
		else
		{
			std::cout << "could not recognize: " << GetFileNameWithoutExt(filePath.c_str()) << std::endl;
		}
	}
	ClearDirectoryFiles();
}

void NoMEM::loadMusic(std::shared_ptr< NoMEM::MEMManager > assets, const std::string& dirPath)
{
	int count = 0;
	char **music = GetDirectoryFiles(dirPath.c_str(), &count);
	for (int i=0; i < count; i++)
	{
		int fileChars = strlen(music[i]);
		std::string filePath = dirPath + std::string(music[i]);
		if ( fileChars > 4 ) // check to see if valid file
		{
			if ( music[i][fileChars - 4] == '.' ) // check for file extension
			{
				std::string name(GetFileNameWithoutExt(filePath.c_str()));
				assets->addMusic(name);
			}
		}
		else
		{
			std::cout << "could not recognize: " << GetFileNameWithoutExt(filePath.c_str()) << std::endl;
		}
	}
	ClearDirectoryFiles();
}

std::shared_ptr< NoGUI::Page > NoGUI::loadPage(std::string path, std::shared_ptr< NoMEM::MEMManager > assets)
{
	rapidjson::Document d;
	if ( readFile(path, d) == 0 )
	{
		std::shared_ptr< NoGUI::Page > pg = std::make_shared< NoGUI::Page >();
		deserializePage(d, pg, assets);
		pg->update();
		
		return pg;
	}
	else
	{

		return nullptr;
	}
}

std::shared_ptr< NoGUI::GUIManager > NoGUI::loadManager(std::string path, std::shared_ptr< NoMEM::MEMManager > assets)
{
	rapidjson::Document d;
	if ( readFile(path, d) == 0 )
	{
		std::shared_ptr< NoGUI::GUIManager > model = std::make_shared< NoGUI::GUIManager >();
		std::shared_ptr< NoGUI::Page > pg = model->getPage();
		deserializePage(d, pg, assets);
		pg->update();
		for (std::shared_ptr< NoGUI::Element > elem : pg->getElements())
		{
			NoGUI::CDropDown dropdown = elem->getComponent< NoGUI::CDropDown >();
			if ( dropdown.owned )
			{
				std::shared_ptr< NoGUI::Page > dropPg = model->addPage(dropdown.options);
				dropPg->update();
			}
		}
		
		return model;
	}
	else
	{

		return nullptr;
	}
}

// TODO: handle case if asset is already loaded
// TODO: try to add assets without the added path argument where possible
void NoMEM::deserializeAssets(std::shared_ptr< NoMEM::MEMManager > assets, const rapidjson::Value& assetJSON)
{
	rapidjson::Value::ConstMemberIterator configIt = assetJSON.FindMember("Config");
	rapidjson::Value::ConstMemberIterator customIt = assetJSON.FindMember("Custom");
	const rapidjson::Value& fonts = assetJSON["Fonts"];
	const rapidjson::Value& textures = assetJSON["Textures"];
	const rapidjson::Value& sprites = assetJSON["Sprites"];
	const rapidjson::Value& sounds = assetJSON["Sounds"];
	const rapidjson::Value& music = assetJSON["Music"];
	
	
	if ( configIt != assetJSON.MemberEnd() )
	{
		assets->clear();
		const rapidjson::Value& config = configIt->value.GetObject();
		std::string basePath = config["Base"].GetString();
		std::string fontPath = config["Fonts"].GetString();
		std::string texturePath = config["Textures"].GetString();
		std::string spritePath = config["Sprites"].GetString();
		std::string soundPath = config["Sounds"].GetString();
		std::string musicPath = config["Music"].GetString();
		assets->conf = NoMEM::Config(basePath, texturePath, spritePath, fontPath, soundPath, musicPath);
	}
		
	for (auto& font : fonts.GetObject())
	{
		assets->addFont(font.name.GetString());
	}
		
	for (auto& texture : textures.GetObject())
	{
		assets->addTexture(texture.name.GetString());
	}
		
	for (auto& sprite : sprites.GetObject())
	{
		assets->addSprite(sprite.name.GetString());
	}
	
	for (auto& sound : sounds.GetObject())
	{
		assets->addSound(sound.name.GetString());
	}
	
	for (auto& song : music.GetObject())
	{
		assets->addMusic(song.name.GetString());
	}
	
	if ( customIt != assetJSON.MemberEnd() )
	{
		const rapidjson::Value& customPaths = customIt->value.GetObject();
		const rapidjson::Value& customFonts = customPaths["Fonts"];
		const rapidjson::Value& customTextures = customPaths["Textures"];
		const rapidjson::Value& customSprites = customPaths["Sprites"];
		const rapidjson::Value& customSounds = customPaths["Sounds"];
		const rapidjson::Value& customMusic = customPaths["Music"];
		
		for (auto& font : customFonts.GetObject())
		{
			assets->addFont(font.name.GetString(), font.value.GetString());
		}
		
		for (auto& texture : customTextures.GetObject())
		{
			assets->addTexture(texture.name.GetString(), texture.value.GetString());
		}
		
		for (auto& sprite : customSprites.GetObject())
		{
			assets->addSprite(sprite.name.GetString(), sprite.value.GetString());
		}
	
		for (auto& sound : customSounds.GetObject())
		{
			assets->addSound(sound.name.GetString(), sound.value.GetString());
		}
	
		for (auto& song : customMusic.GetObject())
		{
			assets->addMusic(song.name.GetString(), song.value.GetString());
		}
	}
}

void NoGUI::deserializePage(rapidjson::Document& d, std::shared_ptr< NoGUI::Page > pg, std::shared_ptr< NoMEM::MEMManager > assets)
{
	rapidjson::Value::ConstMemberIterator assetIt = d.FindMember("Assets");
	const rapidjson::Value& pgComps = d["Components"];
	const rapidjson::Value& pgElems = d["Elements"];
	if ( assetIt != d.MemberEnd() )
	{
		if ( assets == nullptr )
		{
			assets = std::make_shared< NoMEM::MEMManager >("../assets/");
		}
		deserializeAssets(assets, assetIt->value.GetObject());
	}
	pg->setComponents(loadComponents(pgComps, assets));
	for (auto& classGroup : pgElems.GetObject())
	{
		for (auto& elemData : classGroup.value.GetArray())
		{
			for (auto& elem : elemData.GetObject()) // should only be one iteration. We just don't know what the key is going to be
			{
				const rapidjson::Value& data = elem.value.GetObject();
				const rapidjson::Value& hoverArray = data["Hover Colour"].GetArray();
				const rapidjson::Value& elemComps = data["Components"];
				std::string elemType(elem.name.GetString());
				NoGUI::Style elemStyle = loadStyle(data);
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
				}
				newElem->setHoverCol(hovCol);
				for (auto& elemComp : elemComps.GetObject())
				{
					std::string compType = std::string(elemComp.name.GetString());
					if ( compType == "Text" )
					{
						newElem->addComponent< NoGUI::CText >(loadCText(elemComp.value.GetObject(), assets));
					}
					else if ( compType == "Image" )
					{
						newElem->addComponent< NoGUI::CImage >(loadCImage(elemComp.value.GetObject(), assets));
					}
					else if ( compType == "MultiStyle" )
					{
						newElem->addComponent< NoGUI::CMultiStyle >(loadCMultiStyle(elemComp.value.GetObject()));
					}
					else if ( compType == "Input" )
					{
						newElem->addComponent< NoGUI::CInput >(loadCInput(elemComp.value.GetObject()));
					}
					else if ( compType == "DropDown" )
					{
						newElem->addComponent< NoGUI::CDropDown >(loadCDropDown(elemComp.value.GetObject(), newElem, assets));
					}
				}
//				newElem->setComponents(loadComponents(elemComps, assets, newElem)); // this overwrites any components the Page added.
			}
		}
	}
}

void NoGUI::deserializeCText(NoGUI::CText& text, const rapidjson::Value& textJSON, std::shared_ptr< NoMEM::MEMManager > assets)
{
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
				
				break;
			}
		}
	}
	if ( angleIt != textJSON.MemberEnd() )
	{
		text.rotation = angleIt->value.GetDouble();
	}
}

void NoGUI::deserializeCImage(NoGUI::CImage& img, const rapidjson::Value& imgJSON, std::shared_ptr< NoMEM::MEMManager > assets)
{
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
}

void NoGUI::deserializeCInput(NoGUI::CInput& input, const rapidjson::Value& inputJSON)
{
	rapidjson::Value::ConstMemberIterator i = inputJSON.FindMember("Index");
	input.cap = inputJSON["Max"].GetInt();
	if ( i != inputJSON.MemberEnd() )
	{
		input.i = i->value.GetDouble();
	}
}

void NoGUI::deserializeCMultiStyle(NoGUI::CMultiStyle& styles, const rapidjson::Value& stylesJSON)
{
	for (auto& style : stylesJSON["Styles"].GetArray())
	{
		styles.styles.push_back(loadStyle(style));
	}
}

void NoGUI::deserializeStyle(NoGUI::Style& style, const rapidjson::Value& elemJSON)
{
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
		style.outlineThick = outlineArray->value[4].GetInt();
		style.outlineCol = outCol;
	}
}

void NoGUI::deserializeComponents(NoGUI::Components& components, const rapidjson::Value& compJSON, std::shared_ptr< NoMEM::MEMManager > assets, std::shared_ptr< NoGUI::Element > dropParent)
{
	NoGUI::CText& textComp = std::get< NoGUI::CText >(components);
	NoGUI::CImage& imageComp = std::get< NoGUI::CImage >(components);
	NoGUI::CInput& inputComp = std::get< NoGUI::CInput >(components);
	NoGUI::CMultiStyle& stylesComp = std::get< NoGUI::CMultiStyle >(components);
	NoGUI::CDropDown& optionsComp = std::get< NoGUI::CDropDown >(components);
	for (auto& component : compJSON.GetObject())
	{
		std::string key(component.name.GetString());
		if ( key == "Text" )
		{
			textComp = NoGUI::loadCText(component.value, assets);
		}
		else if ( key == "Image" )
		{
			imageComp = NoGUI::loadCImage(component.value, assets);
		}
		else if ( key == "Input" )
		{
			inputComp = NoGUI::loadCInput(component.value);
		}
		else if ( key == "MultiStyle" )
		{
			stylesComp = NoGUI::loadCMultiStyle(component.value);
		}
		else if ( key == "DropDown" )
		{
			optionsComp = NoGUI::loadCDropDown(component.value, dropParent, assets);
		}
	}
}

NoGUI::CText NoGUI::loadCText(const rapidjson::Value& textJSON, std::shared_ptr< NoMEM::MEMManager > assets)
{
	NoGUI::CText text;
	deserializeCText(text, textJSON, assets);
	text.owned = true;
	
	return text;
}

NoGUI::CImage NoGUI::loadCImage(const rapidjson::Value& imgJSON, std::shared_ptr< NoMEM::MEMManager > assets)
{
	NoGUI::CImage img;
	deserializeCImage(img, imgJSON, assets);
	img.owned = true;
	
	return img;
}

NoGUI::CInput NoGUI::loadCInput(const rapidjson::Value& inputJSON)
{
	NoGUI::CInput input;
	deserializeCInput(input, inputJSON);
	input.owned = true;
	
	return input;
}

NoGUI::CMultiStyle NoGUI::loadCMultiStyle(const rapidjson::Value& stylesJSON)
{
	NoGUI::CMultiStyle styles;
	deserializeCMultiStyle(styles, stylesJSON);
	styles.owned = true;
	
	return styles;
}

NoGUI::CDropDown NoGUI::loadCDropDown(const rapidjson::Value& dropJSON, std::shared_ptr< NoGUI::Element > parent, std::shared_ptr< NoMEM::MEMManager > assets)
{
	NoGUI::CDropDown dropdown;
	rapidjson::Document d;
	std::string path(dropJSON["File"].GetString());
	rapidjson::Value::ConstMemberIterator wrapIt = dropJSON.FindMember("Wrapping");
	NoGUI::TextWrap elemWrap = NoGUI::TextWrap::NONE;
	if ( wrapIt != dropJSON.MemberEnd() )
	{
		std::string val(wrapIt->value.GetString());
		for (auto entry : WrapMap)
		{
			if ( entry.second ==  val )
			{
				elemWrap = entry.first;
				
				break;
			}
		}
	}
	std::shared_ptr< NoGUI::DropDown > pg = std::make_shared< NoGUI::DropDown >(parent, elemWrap);
	if ( readFile(path, d) == 0 )
	{
		deserializePage(d, pg, assets);
		pg->update();
	}
	dropdown.options = pg;
	dropdown.owned = true;
	
	return dropdown;
}

NoGUI::Components NoGUI::loadComponents(const rapidjson::Value& compJSON, std::shared_ptr< NoMEM::MEMManager > assets, std::shared_ptr< NoGUI::Element > dropParent)
{
	NoGUI::Components components;
	deserializeComponents(components, compJSON, assets, dropParent);
	
	return components;
}

std::shared_ptr< NoGUI::Element > loadElement(const rapidjson::Value::ConstMemberIterator& elemJSON, const size_t& id, std::shared_ptr< NoMEM::MEMManager > assets)
{
	const rapidjson::Value& data = elemJSON->value.GetObject();
	const rapidjson::Value& hoverArray = data["Hover Colour"].GetArray();
	const rapidjson::Value& elemComps = data["Components"];
	std::string elemType(elemJSON->name.GetString());
	NoGUI::Style elemStyle = NoGUI::loadStyle(data);
	std::string elemInner(data["Inner"].GetString());
	std::string elemId(data["ID"].GetString());
	std::shared_ptr< NoGUI::Element > newElem;
					
	Color hovCol;
	hovCol.r = hoverArray[0].GetInt();
	hovCol.g = hoverArray[1].GetInt();
	hovCol.b = hoverArray[2].GetInt();
	hovCol.a = hoverArray[3].GetInt();
	if ( elemType == "Element" )
	{
		newElem = std::make_shared< NoGUI::Element >(id, elemStyle, hovCol, elemInner);
	}
	else if ( elemType == "CheckBox" )
	{
		newElem = std::make_shared< NoGUI::CheckBox >(id, elemStyle, hovCol, elemInner);
	}
	else if ( elemType == "Button" )
	{
		newElem = std::make_shared< NoGUI::Button >(id, elemStyle, hovCol, elemInner);
	}
	else if ( elemType == "Input" )
	{
		newElem = std::make_shared< NoGUI::Input >(id, elemStyle, hovCol, elemInner);
	}
	else if ( elemType == "InputButton" )
	{
		newElem = std::make_shared< NoGUI::InputButton >(id, elemStyle, hovCol, elemInner);
	}
	else if ( elemType == "InputToggle" )
	{
		newElem = std::make_shared< NoGUI::InputToggle >(id, elemStyle, hovCol, elemInner);
	}
	else if ( elemType == "InputTrigger" )
	{
		newElem = std::make_shared< NoGUI::InputTrigger >(id, elemStyle, hovCol, elemInner);
	}
	else if ( elemType == "Toggle" )
	{
		newElem = newElem = std::make_shared< NoGUI::Toggle >(id, elemStyle, hovCol, elemInner);
	}
	else if ( elemType == "Trigger" )
	{
		newElem = std::make_shared< NoGUI::Trigger >(id, elemStyle, hovCol, elemInner);
	}
	else
	{
		std::cerr << "Could not parse Element type " << elemType << std::endl;
						
		return nullptr;
	}
//	newElem->setHoverCol(hovCol);
	if ( assets )
	{
		newElem->setComponents(NoGUI::loadComponents(elemComps, assets));
	}
	
	return newElem;
}

NoGUI::Style NoGUI::loadStyle(const rapidjson::Value& elemJSON)
{
	NoGUI::Style style;
	deserializeStyle(style, elemJSON);
	
	return style;
}

void NoMEM::serializeAssets(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, std::shared_ptr< NoMEM::MEMManager > assets)
{
	std::unordered_map< std::string, std::string > customPaths = assets->conf.get();
	std::string basePath = assets->conf.cwd();
	if ( basePath.back() != '/' )
	{
		basePath += "/";
	}
	std::string fontPath = assets->conf.fontDir;
	std::string texturePath = assets->conf.textureDir;
	std::string spritePath = assets->conf.spriteDir;
	std::string soundPath = assets->conf.soundDir;
	std::string musicPath = assets->conf.musicDir;
	std::unordered_map< std::string, std::string > customFonts;
	std::unordered_map< std::string, std::string > customTextures;
	std::unordered_map< std::string, std::string > customSprites;
	std::unordered_map< std::string, std::string > customSounds;
	std::unordered_map< std::string, std::string > customMusic;
	writer.Key("Assets");
	writer.StartObject();
		writer.Key("Config");
		writer.StartObject();
			writer.Key("Base");
			writer.String(basePath.c_str());
			writer.Key("Fonts");
			writer.String(fontPath.c_str());
			writer.Key("Textures");
			writer.String(texturePath.c_str());
			writer.Key("Sprites");
			writer.String(spritePath.c_str());
			writer.Key("Sounds");
			writer.String(soundPath.c_str());
			writer.Key("Music");
			writer.String(musicPath.c_str());
		writer.EndObject();
		writer.Key("Fonts");
		writer.StartObject();
			for (auto font : assets->getAll< Font >())
			{
//				std::string filePath = (fontPath.front() == '.') ? basePath + "/" + fontPath + font.first : fontPath + font.first; // get full path
				std::string filePath = basePath + "/" + fontPath + font.first;
				std::string out = font.first; // relative path
				if ( FileExists(filePath.c_str()) )
				{
				
				}
				else if ( FileExists(((filePath + ".ttf").c_str())) )
				{
					out += ".ttf";
				}
				else if ( FileExists(((filePath	+ ".fnt").c_str())) )
				{
					out += ".fnt";
				}
				else if ( FileExists(((filePath + ".png").c_str())) )
				{
					out += ".png";
				}
				else
				{					
					auto found = customPaths.find(font.first);
					if ( found != customPaths.end() )
					{
						customFonts[font.first] = found->second;
						customPaths.erase(found);
						
						continue;
					}
					else
					{
						std::cerr << "could not find path for " << font.first << std::endl;
						
						continue;
					}
				}
				writer.Key(font.first.c_str());
				writer.String(out.c_str());
			}
		writer.EndObject();

		writer.Key("Textures");
		writer.StartObject();
			for (auto texture : assets->getAll< Texture2D >())
			{
//				std::string filePath = (texturePath.front() == '.') ? basePath + "/" + texturePath + texture.first : texturePath + texture.first; // get full path
				std::string filePath = basePath + "/" + texturePath + texture.first;
				std::string out = texture.first; // relative path
				if ( FileExists(filePath.c_str()) )
				{
			
				}
				else if ( FileExists(((filePath + ".png").c_str())) )
				{
					out += ".png";
				}
				else if ( FileExists(((filePath + ".gif").c_str())) )
				{
					out += ".gif";
				}
				else if ( FileExists(((filePath + ".hdr").c_str())) )
				{
					out += ".hdr";
				}
				else if ( FileExists(((filePath + ".dds").c_str())) )
				{
					out += ".dds";
				}
				else
				{					
					auto found = customPaths.find(texture.first);
					if ( found != customPaths.end() )
					{
						customTextures[texture.first] = found->second;
						customPaths.erase(found);
						
						continue;
					}
					else
					{
						std::cerr << "could not find path for " << texture.first << std::endl;
						
						continue;
					}
				}
				writer.Key(texture.first.c_str());
				writer.String(out.c_str());
			}
		writer.EndObject();
		
		writer.Key("Sprites");
		writer.StartObject();
			for (auto sprite : assets->getAll< NoMEM::Sprite >())
			{
//				std::string filePath = (spritePath.front() == '.') ? basePath + "/" + spritePath + sprite.first : spritePath + sprite.first; // get full path
				std::string filePath = basePath + "/" + spritePath + sprite.first;
				std::string out = sprite.first;
				if ( FileExists(filePath.c_str()) )
				{
			
				}
				else if ( FileExists(((filePath + ".png").c_str())) )
				{
					out += ".png";
				}
				else if ( FileExists(((filePath + ".gif").c_str())) )
				{
					out += ".gif";
				}
				else if ( FileExists(((filePath + ".hdr").c_str())) )
				{
					out += ".hdr";
				}
				else if ( FileExists(((filePath + ".dds").c_str())) )
				{
					out += ".dds";
				}
				else
				{
					auto found = customPaths.find(sprite.first);
					if ( found != customPaths.end() )
					{
						customSprites[sprite.first] = found->second;
						customPaths.erase(found);
						
						continue;
					}
					else
					{
						std::cerr << "could not find path for " << sprite.first << std::endl;
						
						continue;
					}
				}
				writer.Key(sprite.first.c_str());
				writer.String(out.c_str());
			}
		writer.EndObject();
		
		writer.Key("Sounds");
		writer.StartObject();
			for (auto sound : assets->getAll< Sound >())
			{
//				std::string filePath = (soundPath.front() == '.') ? basePath + "/" + soundPath + sound.first : soundPath + sound.first; // get full path
				std::string filePath = basePath + "/" + soundPath + sound.first;
				std::string out = sound.first; // relative path
				// TODO: figure out what files to support
				if ( FileExists(filePath.c_str()) )
				{
				
				}
				else if ( FileExists(((filePath + ".wav").c_str())) )
				{
					out += ".wav";
				}
				else if ( FileExists(((filePath + ".mp3").c_str())) )
				{
					out += ".mp3";
				}
				else if ( FileExists(((filePath + ".ogg").c_str())) )
				{
					out += ".ogg";
				}
				else
				{
					auto found = customPaths.find(sound.first);
					if ( found != customPaths.end() )
					{
						customSounds[sound.first] = found->second;
						customPaths.erase(found);
						
						continue;
					}
					else
					{
						std::cerr << "could not find path for " << sound.first << std::endl;
						
						continue;
					}
				}
				writer.Key(sound.first.c_str());
				writer.String(out.c_str());
			}
		writer.EndObject();
		
		writer.Key("Music");
		writer.StartObject();
			for (auto music : assets->getAll< Music >())
			{
//				std::string filePath = (musicPath.front() == '.') ? basePath + "/" + musicPath + music.first : musicPath + music.first; // get full path
				std::string filePath = basePath + "/" + musicPath + music.first;
				std::string out = music.first; // relative path
				// TODO: figure out what files to support
				if ( FileExists(filePath.c_str()) )
				{
				
				}
				else if ( FileExists(((filePath + ".wav").c_str())) )
				{
					out += ".wav";
				}
				else if ( FileExists(((filePath + ".mp3").c_str())) )
				{
					out += ".mp3";
				}
				else if ( FileExists(((filePath + ".ogg").c_str())) )
				{
					out += ".ogg";
				}
				else
				{
					auto found = customPaths.find(music.first);
					if ( found != customPaths.end() )
					{
						customSounds[music.first] = found->second;
						customPaths.erase(found);
						
						continue;
					}
					else
					{
						std::cerr << "could not find path for " << music.first << std::endl;
						
						continue;
					}
				}
				writer.Key(music.first.c_str());
				writer.String(out.c_str());
			}
		writer.EndObject();
		
		writer.Key("Custom");
		writer.StartObject();
			writer.Key("Fonts");
			writer.StartObject();
				for (auto customFont : customFonts)
				{
					writer.Key(customFont.first.c_str());
					writer.String(customFont.second.c_str());
				}
			writer.EndObject();
			
			writer.Key("Textures");
			writer.StartObject();
				for (auto customTexture : customTextures)
				{
					writer.Key(customTexture.first.c_str());
					writer.String(customTexture.second.c_str());
				}
			writer.EndObject();
		
			writer.Key("Sprites");
			writer.StartObject();
				for (auto customSprite : customSprites)
				{
					writer.Key(customSprite.first.c_str());
					writer.String(customSprite.second.c_str());
				}
			writer.EndObject();
			
			writer.Key("Sounds");
			writer.StartObject();
				for (auto customSound : customSounds)
				{
					writer.Key(customSound.first.c_str());
					writer.String(customSound.second.c_str());
				}
			writer.EndObject();
			
			writer.Key("Music");
			writer.StartObject();
				for (auto customSong : customMusic)
				{
					writer.Key(customSong.first.c_str());
					writer.String(customSong.second.c_str());
				}
			writer.EndObject();	
		writer.EndObject();
		
	writer.EndObject();
}

void NoGUI::serializeComponents(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, NoGUI::Components components, std::shared_ptr< NoMEM::MEMManager > assets, const std::string& dropPath)
{
	NoGUI::CText pgText = std::get< NoGUI::CText >(components);
	NoGUI::CInput pgInput = std::get< NoGUI::CInput >(components);
	NoGUI::CImage pgImage = std::get< NoGUI::CImage >(components);
	NoGUI::CDropDown pgDropDown = std::get< NoGUI::CDropDown >(components);
	NoGUI::CMultiStyle pgStyle = std::get< NoGUI::CMultiStyle >(components);
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
			writer.Key("DropDown");
			serializeCDropDown(writer, pgDropDown, assets, dropPath);
		}
	writer.EndObject();
}

void NoGUI::serializeElement(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, std::shared_ptr< NoGUI::Element > elem, const std::string& id, std::shared_ptr< NoMEM::MEMManager > assets, std::string dropPath)
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
			writer.String(id.c_str());
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
			if ( assets )
			{
				if ( dropPath.empty() )
				{
					dropPath = std::string(GetWorkingDirectory()) + "/" + id + ".json";
				}
				serializeComponents(writer, elem->getComponents(), assets, dropPath);
			}
		writer.EndObject();
	writer.EndObject();
}

void NoGUI::serializeStyle(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, const NoGUI::Style& style)
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
		writer.Uint(style.outlineCol.r);
		writer.Uint(style.outlineCol.g);
		writer.Uint(style.outlineCol.b);
		writer.Uint(style.outlineCol.a);
		writer.Uint(style.outlineThick);
	writer.EndArray();
}

void NoGUI::serializeCMultiStyle(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, const NoGUI::CMultiStyle& styles)
{
	writer.StartObject();
		writer.Key("Styles");
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

void NoGUI::serializeCImage(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, const NoGUI::CImage& imageFmt, std::shared_ptr< NoMEM::MEMManager > assets)
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
					
					break;
				}
			}
		}
		writer.String(imgStr.c_str());
		writer.Key("Scale");
		writer.StartArray();
			writer.Double(imageFmt.scale.x);
			writer.Double(imageFmt.scale.y);
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

void NoGUI::serializeCText(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, const NoGUI::CText& textFmt, std::shared_ptr< NoMEM::MEMManager > assets)
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

void NoGUI::serializeCDropDown(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, const NoGUI::CDropDown& dropFmt, std::shared_ptr< NoMEM::MEMManager > assets, std::string path)
{
	if ( path.empty() )
	{
		std::shared_ptr< NoGUI::Element > dropParent = dropFmt.options->getParent();
		if ( dropParent )
		{
			path = std::string(GetWorkingDirectory()) + "/" + std::to_string(dropParent->getId()) + ".json";
		}
		else
		{
			path = std::string(GetWorkingDirectory()) + "/dropdown.json";
		}
	}
	writer.StartObject();
		writer.Key("File");
		writer.String(path.c_str());
		writer.Key("Wrapping");
		writer.String(WrapMap.at(dropFmt.options->getWrap()).c_str());
	writer.EndObject();
	if ( dropFmt.options )
	{
		NoGUI::savePage(dropFmt.options, assets, path);
	}
}

void NoGUI::serializeCInput(rapidjson::PrettyWriter< rapidjson::StringBuffer >& writer, const NoGUI::CInput& inputFmt)
{
	writer.StartObject();
		writer.Key("Max");
		writer.Uint(inputFmt.cap);
		writer.Key("Index");
		writer.Uint(inputFmt.i);
	writer.EndObject();
}

int NoPARSE::writeFile(rapidjson::StringBuffer& sb, const std::string& path)
{
	std::ofstream out(path);
	if (!out)
	{
		std::cerr << "Could not write file: " << path << std::endl;
			
		return 1;
	}
	else
	{
		std::cout << "saving page to: " << path << std::endl;
		out << sb.GetString() << std::endl;
	}
	
	return 0;
}

int NoPARSE::readFile(const std::string& path, rapidjson::Document&  d)
{
	if ( FileExists(path.c_str()) )
	{
		FILE* fp = fopen(path.c_str(), "rb"); // non-Windows use "r"
 
		char readBuffer[65536];
		rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
 
		d.ParseStream(is);
 
		fclose(fp);
		
		return 0;
	}
	else
	{
		std::cerr << "could not open: " << path << std::endl;
		
		return 1;
	}
}