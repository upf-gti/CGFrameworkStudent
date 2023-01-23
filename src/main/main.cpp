#include "framework/application.h"
#include "framework/utils.h"

int main(int argc, char **argv)
{
	// Launch the app (app is a global variable)
	Application* app = new Application( "Computer Graphics", 1280, 720);
	app->Init();

	std::cout << "Starting loop..." << std::endl;
	launchLoop(app);

	delete app;

	return 0;
}
