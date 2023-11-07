#include "Application.h"
#include "Globals.h"
#include <chrono>
#include <thread>

enum main_states
{
	MAIN_CREATION,
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};

Application* App = NULL;

int main(int argc, char ** argv)
{
	LOG("-------------STARTING VertX Game Engine-------------", NULL);

	int main_return = EXIT_FAILURE;
	main_states state = MAIN_CREATION;
	//Application* App = NULL;

	// Start computing the game loop tick time
	std::chrono::high_resolution_clock::time_point lastFrameTime = std::chrono::high_resolution_clock::now();

	while (state != MAIN_EXIT)
	{

		switch (state)
		{
		case MAIN_CREATION:

			LOG("-------------- Application Creation --------------", NULL);
			App = new Application();
			state = MAIN_START;
			break;

		case MAIN_START:

			LOG("-------------- Application Init --------------", NULL);
			if (App->Init() == false)
			{
				LOG("Application Init exits with ERROR: %s", SDL_GetError());

				state = MAIN_EXIT;
			}
			else
			{
				LOG("-------------- Application Update --------------", NULL);

				state = MAIN_UPDATE;
			}

			break;

		case MAIN_UPDATE:
		{
			int update_return = App->Update();
			
			if (!App->game_engine->renderer3D_engine->vsync)
			{
				// Calculate the time taken to render the frame
				std::chrono::high_resolution_clock::time_point currentFrameTime = std::chrono::high_resolution_clock::now();
				App->deltaTime = 1000 / App->fps; // Delta time in ms
				App->frameDuration = std::chrono::duration_cast<std::chrono::milliseconds>(currentFrameTime - lastFrameTime).count();
				lastFrameTime = currentFrameTime;

				// Implement a frame rate cap by sleeping if the frame took less time than the desired frame time
				if (App->frameDuration < App->deltaTime) {
					double sleepTime = App->deltaTime - App->frameDuration;
					std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long long>(sleepTime)));
				}
			}

			if (update_return == UPDATE_ERROR)
			{
				LOG("Application Update exits with ERROR: %s", SDL_GetError());
				
				state = MAIN_EXIT;
			}

			if (update_return == UPDATE_STOP)
				state = MAIN_FINISH;
		}
			break;

		case MAIN_FINISH:

			LOG("-------------- Application CleanUp --------------", NULL);
			if (App->CleanUp() == false)
			{
				LOG("Application CleanUp exits with ERROR: %s", SDL_GetError());
			}
			else
				main_return = EXIT_SUCCESS;

			state = MAIN_EXIT;

			break;

		}
	}

	delete App;
	LOG("EXITING VertX Game Engine----------------------", NULL);
	return main_return;
}