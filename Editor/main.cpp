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
	Application* App = NULL;

	while (state != MAIN_EXIT)
	{

		switch (state)
		{
		case MAIN_CREATION:

			App = new Application();
			state = MAIN_START;
			break;

		case MAIN_START:

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


	LOG("-------------EXITING VertX Game Engine-------------", NULL);
	delete App;
	return main_return;
}