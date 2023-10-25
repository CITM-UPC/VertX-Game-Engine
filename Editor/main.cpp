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

int main(int argc, char ** argv)
{
	LOG("Starting game '%s'...", TITLE);

	int main_return = EXIT_FAILURE;
	main_states state = MAIN_CREATION;
	Application* App = NULL;

	int fps = 60;
	double deltaTime = 1000 / fps; // In ms

	std::chrono::high_resolution_clock::time_point lastFrameTime = std::chrono::high_resolution_clock::now();


	while (state != MAIN_EXIT)
	{

		switch (state)
		{
		case MAIN_CREATION:

			LOG("-------------- Application Creation --------------");
			App = new Application();
			state = MAIN_START;
			break;

		case MAIN_START:

			LOG("-------------- Application Init --------------");
			if (App->Init() == false)
			{
				LOG("Application Init exits with ERROR");
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_UPDATE;
				LOG("-------------- Application Update --------------");
			}

			break;

		case MAIN_UPDATE:
		{
			int update_return = App->Update();

			// Calculate the time taken to render the frame
			std::chrono::high_resolution_clock::time_point currentFrameTime = std::chrono::high_resolution_clock::now();
			double frameDuration = std::chrono::duration_cast<std::chrono::milliseconds>(currentFrameTime - lastFrameTime).count();
			lastFrameTime = currentFrameTime;

			// Implement a frame rate cap by sleeping if the frame took less time than the desired frame time
			if (frameDuration < deltaTime) {
				double sleepTime = deltaTime - frameDuration;
				std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long long>(sleepTime)));
			}

			if (update_return == UPDATE_ERROR)
			{
				LOG("Application Update exits with ERROR");
				state = MAIN_EXIT;
			}

			if (update_return == UPDATE_STOP)
				state = MAIN_FINISH;
		}
			break;

		case MAIN_FINISH:

			LOG("-------------- Application CleanUp --------------");
			if (App->CleanUp() == false)
			{
				LOG("Application CleanUp exits with ERROR");
			}
			else
				main_return = EXIT_SUCCESS;

			state = MAIN_EXIT;

			break;

		}
	}

	delete App;
	LOG("Exiting game '%s'...\n", TITLE);
	return main_return;
}