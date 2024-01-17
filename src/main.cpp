#include "pch.h"

#include "Application.h"

void Callback(void* userptr, Event& event) {
	Application* app = (Application*) userptr;
	app->OnEvent(&event);
}

int main() {
	Application* app = new Application();
	app->window = new Window({ 500, 500 }, "Malcraft", "Malcraft", app);
	app->window->SetCallback(Callback);
	app->instance = Instance::Create(API_VULKAN, app->window, "Malcraft", MW_MAKE_VERSION(1, 0, 0), true);
	app->display = Display::Create(app->instance, app->window);

	app->Start();

	std::chrono::time_point start = std::chrono::high_resolution_clock::now();
	float timeSinceFrame = 0.0f;
	while (!app->toClose) {
		uint64 nsTime = (std::chrono::high_resolution_clock::now() - start).count();
		float sTime = nsTime / 1'000'000'000.0f;
		start = std::chrono::high_resolution_clock::now();
		timeSinceFrame += sTime;

		float targetTime = 1 / app->fps;
		if (timeSinceFrame >= targetTime) {
			app->Update(timeSinceFrame);
			app->Render();
			app->window->Update();

			timeSinceFrame = 0.0f;
		}
	}

	app->End();

	delete app->display;
	delete app->instance;
	delete app->window;
	delete app;
}