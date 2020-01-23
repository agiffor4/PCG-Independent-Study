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

void CleanUpSDL(SDL_Window* _window, SDL_Renderer* _renderer);


World generateTiles(Scene* _scene, int _tileCountH, int _tileCountV, int _screenWidth, int _screenHeight) {
	Vector2 targetSize = Vector2(_screenWidth / _tileCountH, _screenHeight / _tileCountV);
	World myWorld = World(_tileCountH, _tileCountV);
	for (size_t i = 0; i < _tileCountV; i++)
	{
		for (size_t j = 0; j < _tileCountH; j++)
		{
			
			std::string name = "Tile (" + std::to_string(j) + ", " + std::to_string(i) + ")";
			Tile* t = new Tile();
			if (j % 3 == 0)
			{
				t->Init("img/block_tile.bmp", name, myWorld.GetTileCount(), j, i, Vector2(j * targetSize.X, i * targetSize.Y), _scene->GetRenderer());
				t->SetPassable(false);
			}
			else
			{
				t->Init("img/blank_tile.bmp", name, myWorld.GetTileCount(), j, i, Vector2(j * targetSize.X, i * targetSize.Y), _scene->GetRenderer());
			}
				
			t->SetSize(targetSize);
			myWorld.AddTile(t);
			
			_scene->AddRenderable(t);
		}
	}
	return myWorld;
	
}

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
	float height = SDL_GetWindowSurface(window)->h;


	DeltaTime deltaTime = DeltaTime();

	std::unique_ptr<Scene> scene(new Scene(renderer));//create scene
	World myWorld = generateTiles(scene.get(), 20, 15, width, height);

	Timer timer = Timer(3.0f);
	for (int i = 0; i < 20; i++)
	{
		printf("Check from starting point %d in direction is %s\n", i, (myWorld.IsViableDirectionToMoveIn(i, World::TileDirection::LEFT) ? "true" : "false"));
	}
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

