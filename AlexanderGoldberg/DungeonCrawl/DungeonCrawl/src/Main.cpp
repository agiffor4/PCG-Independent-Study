#include "DeltaTime.h"
#include "Timer.h"
#include "Tile.h"
#include "Scene.h"
#include "InputManager.h"
#include "Quit.h"
#include "SDL.h"
#include "SDL_mixer.h"
#include "World.h"

#include "BSP.h"
#include "AStarSearch.h"

#include"Player.h"
#include "Camera.h"

#include <vector>
#include <stack>
#include<memory>
#include <iostream>
#define UseHigherGrid 0
void CleanUpSDL(SDL_Window* _window, SDL_Renderer* _renderer);


 

void mainLoop()
{
	const int seed = 1024;// time(NULL));
	printf("Using seed %d.\n", seed);
	srand(seed);
	//quit functionality
	bool play = true;
	Quit quit = Quit(&play);
	InputManager::GetInputManager()->SubscribeToInput(&quit, InputManager::KeyPressType::DOWN);
	Renderable::renderOrderChanged() = false;
	//Init SDL
	SDL_Window* window = SDL_CreateWindow("test", 
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, 
		640, 480, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	int width = SDL_GetWindowSurface(window)->w;
	int height = SDL_GetWindowSurface(window)->h;	

	DeltaTime deltaTime = DeltaTime();	
	

#if UseHigherGrid == 0
	int gridSizeX = 30;
	int gridSizeY = 30;
#else
	int gridSizeX = 50;
	int gridSizeY = 50;
#endif
	
	std::unique_ptr<Scene> scene(new Scene(renderer));//create scene
	World myWorld = World(gridSizeX, gridSizeY, scene.get());
	InputManager::GetInputManager()->SubscribeToInput(&myWorld, InputManager::KeyPressType::UP);
	Camera::SetDimensions(width, height);
	Camera::SetWorld(&myWorld);
	myWorld.SetWindowRef(window);
	myWorld.GenerateTiles(width, height);
	myWorld.Generate();
	

	Timer timer = Timer(3.0f);
	SDL_Event e;

	while (play)
	{
		float dt = deltaTime.GetCurrentDeltaTime();
		
		while (SDL_PollEvent(&e)) 
		{
			InputManager::GetInputManager()->CheckInput(e);			
			if (e.type == SDL_QUIT)
			{
				play = false;
			}

		}
		
		scene->Update(dt);
		scene->PlayAudio();		
		scene->Render();

	}
	CleanUpSDL(window, renderer);
	printf("Used seed %d.\n", seed);
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

