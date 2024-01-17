#include "pch.h"

#include "Application.h"

#define CAMERA_HANDLER_IMPL
#include "CameraHandler.h"

void Application::Start() {
	assetHandler = new AssetHandler(instance);

	window->SetWindowState(WINDOW_STATE_MAXIMIZED);
	window->cursorType = CURSOR_TYPE_HIDDEN;

	Float2 size = window->GetClientSize();
	camera = PerspectiveCamera(size.x / size.y);
	camera.pos = Float3(8, 16, 8);
	camera.rot = Radians(Float3(-45, 45, 0));

	commandBuffer = CommandBuffer::Create(instance);
	chunk = new Chunk(display->GetFramebuffer(), assetHandler);
	chunk->Build();

	player.pos = Float3(8, 16, 8);
	player.dir = Float3(0, 0, 0);
	player.velocity = Float3(0);
}

void Application::End() {
	instance->WaitUntilIdle();

	delete chunk;
	delete commandBuffer;
	delete assetHandler;
}

void Application::Update(float dt) {
	MW_LOG("Delta Time: {0}", dt);
	MW_LOG("FPS: {0}", 1 / dt);
	MW_LOG("");
	Float2 size = window->GetClientSize();
	camera.aspectRatio = size.x / size.y;

	//CameraHandler::Move(&camera, window, dt, moveSpeed);
	Move(dt);

	camera.pos = player.pos + Float3(0, 1.75, 0);
	camera.rot = player.dir;

	player.pos += player.velocity * dt;

	if (chunk->IsBlockInSpace(player.pos - Float3(0, 0.1, 0))) {
		//player.pos = (Int3) player.pos;
		player.velocity.y = 0;
	} else {
		player.velocity.y -= gravitationalVelocity * dt;
	}

	//MW_LOG("FPS: {0}", 1 / dt);
}

void Application::Render() {
	Uniform uniform {
		camera.GetViewMatrix(),
		camera.GetProjectionMatrix(),
		sunDir
	};
	assetHandler->ubo->SetData(&uniform, sizeof(Uniform));

	chunk->Build();

	commandBuffer->StartFrame(display->GetFramebuffer());
	chunk->Render(commandBuffer);
	commandBuffer->EndFrame();

	instance->WaitUntilIdle();
}

void Application::OnEvent(Event* event) {
	switch (event->GetType()) {
	case EVENT_WINDOW_CLOSED:
		toClose = true;
		return;
	case EVENT_KEY_DOWN:
	{
		KeyDownEvent* e = (KeyDownEvent*) event;

		switch (e->GetKeycode()) {
		case KEY_ESCAPE:
			toClose = true;
			break;
		case KEY_F3:
			Log();
			break;
		case KEY_L:
			sunDir = RotateXYZ(player.dir) * Float4(-MW_VECTOR_FORWARD);
			break;
		}
	}

		return;
	case EVENT_RAW_MOUSE_MOVED:
		if (focused)
			CameraHandler::Look((MouseRawMovedEvent*) event, player.dir, window, lookSpeed);

		return;
	case EVENT_WINDOW_FOCUS:
		focused = true;
		return;
	case EVENT_WINDOW_LOST_FOCUS:
		focused = false;
		return;
	}
}

void Application::Move(float dt) {
	Float3 movedir(0);

	if (window->GetKeyDown(KEY_W))
		movedir += Float3(0, 0, -1);
	if (window->GetKeyDown(KEY_S))
		movedir += Float3(0, 0, 1);
	if (window->GetKeyDown(KEY_A))
		movedir += Float3(-1, 0, 0);
	if (window->GetKeyDown(KEY_D))
		movedir += Float3(1, 0, 0);
	if (window->GetKeyDown(KEY_SPACE))
		movedir += Float3(0, 1, 0);
	if (window->GetKeyDown(KEY_LCONTROL))
		movedir += Float3(0, -1, 0);

	if (movedir == Float3(0))
		return;

	movedir = movedir.Normalize();

	movedir = RotateY<float>(player.dir.y) * Float4(movedir);
	movedir *= moveSpeed * dt;

	player.pos += movedir;
}

void Application::Log() {
	Float3 dir = Degrees(player.dir);
	dir.x = fmod(dir.x, 360);
	dir.y = fmod(dir.y, 360);
	dir.z = fmod(dir.z, 360);

	MW_LOG("Position: {0}", player.pos);
	MW_LOG("Orientation: {0}", dir);
	MW_LOG("Velocity: {0}\n", player.velocity);
}
