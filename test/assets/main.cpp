#include "../../src/Parse.h"

int main(int argc, char ** argv)
{
	InitWindow(420, 720, "Assets");
	InitAudioDevice();
	NoMEM::Config config = NoMEM::Config("../assets/");
//	config.textureDir = "null";
	std::cout << "initialized config" << std::endl;
	std::shared_ptr< NoMEM::MEMManager > assets = NoMEM::loadAssets(config);
	std::cout << "succesfully loaded assets" << std::endl;
	NoMEM::saveAssets(assets);
	std::cout << "succesfully saved assets" << std::endl;
	std::shared_ptr< NoMEM::MEMManager > testMEM = NoMEM::loadAssets("./assets.json");
	NoMEM::saveAssets(testMEM, "./assets(copy).json");
	assets->clear();
}