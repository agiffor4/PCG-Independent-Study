#include "DeltaTime.h"
#include "Timer.h"
#include "Tile.h"
#include "Scene.h"
#include "InputManager.h"
#include "Quit.h"
#include "SDL.h"
#include <Vector>
#include<memory>
#include <iostream>
#include "SDL_mixer.h"
#include "World.h"
#include "BinaryTree.h"

void CleanUpSDL(SDL_Window* _window, SDL_Renderer* _renderer);


 

void mainLoop()
{
	//quit functionality
	bool play = true;
	Quit quit = Quit(&play);
	InputManager::GetInputManager()->SubscribeToInput(&quit, InputManager::KeyPressType::DOWN);
	
	//Init SDL
	SDL_Window* window = SDL_CreateWindow("test", 0, 0, 640, 480, 0);	
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	int width = SDL_GetWindowSurface(window)->w;
	int height = SDL_GetWindowSurface(window)->h;


	DeltaTime deltaTime = DeltaTime();

	std::unique_ptr<Scene> scene(new Scene(renderer));//create scene
	World myWorld = World(20, 15);
	myWorld.GenerateTiles(scene.get(), width, height);

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

