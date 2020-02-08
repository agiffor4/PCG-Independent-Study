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
	std::vector<std::vector<int>> rooms = bsp.GetRooms(&myWorld);
	for (size_t i = 0; i < rooms.size(); i++)
	{
		for (size_t j = 0; j < rooms[i].size(); j++)
		{
			myWorld.GetTileAtIndex(rooms[i][j])->SetPassable(true);
		}
	}

	/*AStarSearch AStar = AStarSearch();
	AStar.CastTilesToAStarNodes(myWorld);
	AStar.Initialize(myWorld.GetMapDimentions(), myWorld.GetTileCount(), false);

	int timesToDig = rooms.size() / 2;
	int worldWidth = myWorld.GetMapDimentions().X;
	for (size_t i = 0; i < timesToDig; i++)
	{
		int lastRoom = rooms.size() - (i + 1);
		int firstRoom = i;
		int index1 = rooms[firstRoom][rooms[firstRoom].size() / 2];
		int index2 = rooms[lastRoom][rooms[lastRoom].size() / 2];
		int x1 = index1 % worldWidth;
		int y1 = worldWidth - ((index1 - x1) / worldWidth);
		int x2 = index2 % worldWidth;
		int y2 = width - ((index2 - x2) / worldWidth);
		std::stack<int> path = AStar.BeginSearch(index1, index2);
		int timesToPop = path.size();
		for (size_t j = 0; j < timesToPop; j++)
		{
			myWorld.GetTileAtIndex(path.top())->SetPassable(true);
			path.pop();
		}
	}*/

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

