#include "DeltaTime.h"
#include "Timer.h"
#include "Tile.h"
#include "Scene.h"
#include "InputManager.h"
#include "Quit.h"
#include "SDL.h"
#include <vector>
#include <stack>
#include<memory>
#include <iostream>
#include "SDL_mixer.h"
#include "World.h"
#include "BSP.h"
#include "AStarSearch.h"

void CleanUpSDL(SDL_Window* _window, SDL_Renderer* _renderer);


 

void mainLoop()
{
	srand(1024);// time(NULL));
	//quit functionality
	bool play = true;
	Quit quit = Quit(&play);
	InputManager::GetInputManager()->SubscribeToInput(&quit, InputManager::KeyPressType::DOWN);
	
	//Init SDL
	SDL_Window* window = SDL_CreateWindow("test", 
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, 
		640, 480, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	int width = SDL_GetWindowSurface(window)->w;
	int height = SDL_GetWindowSurface(window)->h;	

	DeltaTime deltaTime = DeltaTime();
	int gridSizeX = 25;
	int gridSizeY = 25;
	std::unique_ptr<Scene> scene(new Scene(renderer));//create scene
	World myWorld = World(gridSizeX, gridSizeY);
	myWorld.GenerateTiles(scene.get(), width, height);
	//BSP bsp = BSP(40, 30);
	BSP bsp = BSP(gridSizeX, gridSizeY);
	bsp.BeginSplit(4);
	
	AStarSearch AStar = AStarSearch();
	AStar.CastTilesToAStarNodes(myWorld);
	AStar.Initialize(myWorld.GetMapDimentions(), myWorld.GetTileCount(), false);
	std::vector<std::vector<int>> rooms;
	std::vector<int> paths;
	bsp.GenerateRoomsAndPaths(AStar, rooms, paths);
	myWorld.AddRooms(rooms);
	myWorld.AddPaths(paths);


	Timer timer = Timer(3.0f);
	while (play)
	{
		float dt = deltaTime.GetCurrentDeltaTime();
		InputManager::GetInputManager()->CheckInput();
		scene->PlayAudio();
		scene->Render();

	}
	CleanUpSDL(window, renderer);
}


int main(int argc, char* argv[]) 
{
	
	SDL_Init(SDL_INIT_EVERYTHING);
	mainLoop();

	char name[256];
	std::cin.getline(name, 256);
	return 0;
}



void CleanUpSDL(SDL_Window* _window, SDL_Renderer* _renderer) {
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

