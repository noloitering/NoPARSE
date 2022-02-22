#include "../../src/Parse.h"

Vector2 window = {1280, 720};
Vector2 center = {window.x / 2, window.y / 2};
Vector2 scale = {1.0, 1.0};
float size = 35;
int fps = 60;

int main(int argc, char ** argv)
{
	InitWindow(window.x, window.y, "Entities");
	SetTargetFPS(fps);
	std::shared_ptr< NoECS::EntityManager > manager = std::make_shared< NoECS::EntityManager >();
	std::shared_ptr< NoMEM::MEMManager > assets = std::make_shared< NoMEM::MEMManager >();
	std::shared_ptr< NoECS::Entity > player = manager->addEntity("player");
	std::shared_ptr< NoECS::Entity > ground1 = manager->addEntity("tile");
	std::shared_ptr< NoECS::Entity > ground2 = manager->addEntity("tile");
	std::shared_ptr< NoECS::Entity > ground3 = manager->addEntity("tile");
	std::shared_ptr< NoECS::Entity > ground4 = manager->addEntity("tile");
	std::shared_ptr< NoECS::Entity > ground5 = manager->addEntity("tile");
	
	std::shared_ptr< NoMEM::Sprite > sprite = assets->addSprite("player", "../resources/player3.png", 8);
	NoECS::CTransform& transform = player->addComponent< NoECS::CTransform >((Vector2){64, 192}, scale, 0);
	player->addComponent< NoECS::CSprite >(std::make_shared< Texture2D >(sprite->texture), sprite->frames);
	
	// ground
	ground1->addComponent< NoECS::CTransform >((Vector2){0, window.y - 64}, scale, 0);
	ground1->addComponent< NoECS::CPoly >(GRAY, BLACK, 32, 4, 2);
	ground2->addComponent< NoECS::CTransform >((Vector2){64, window.y - 64}, scale, 0);
	ground2->addComponent< NoECS::CPoly >(GRAY, BLACK, 32, 4, 2);
	ground3->addComponent< NoECS::CTransform >((Vector2){128, window.y - 64}, scale, 0);
	ground3->addComponent< NoECS::CPoly >(GRAY, BLACK, 32, 4, 2);
	ground4->addComponent< NoECS::CTransform >((Vector2){192, window.y - 64}, scale, 0);
	ground4->addComponent< NoECS::CPoly >(GRAY, BLACK, 32, 4, 2);
	ground5->addComponent< NoECS::CTransform >((Vector2){192, window.y - 192}, scale, 0);
	ground5->addComponent< NoECS::CPoly >(GRAY, BLACK, 32, 4, 2);
	
	manager->update();
	NoECS::saveManager(manager, assets, "./scene.json");
	std::cout << "saving complete" << std::endl;
	std::shared_ptr< NoECS::EntityManager > testManager = NoECS::loadManager("./scene.json", assets);
	std::cout << "loading complete" << std::endl;
	NoECS::saveManager(testManager, assets, "./scene(copy).json");
	std::cout << "saved copy" << std::endl;
	assets->clear();
}