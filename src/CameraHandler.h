#pragma once

#include "pch.h"

namespace CameraHandler {
	void Move(mw::PerspectiveCamera* camera, mw::Window* window, float dt, float speed);
	void Look(mw::MouseRawMovedEvent* event, Float3* rot, mw::Window* window, float speed);

	#ifdef CAMERA_HANDLER_IMPL

	void Move(mw::PerspectiveCamera* camera, mw::Window* window, float dt, float speed) {
		Float3 movedir(0);

		if (window->GetKeyDown(KEY_W))
			movedir += Float3(0, 0, -1);
		if (window->GetKeyDown(KEY_S))
			movedir += Float3(0, 0, 1);
		if (window->GetKeyDown(KEY_A))
			movedir += Float3(-1, 0, 0);
		if (window->GetKeyDown(KEY_D))
			movedir += Float3(1, 0, 0);
		if (window->GetKeyDown(KEY_E))
			movedir += Float3(0, 1, 0);
		if (window->GetKeyDown(KEY_Q))
			movedir += Float3(0, -1, 0);

		if (movedir != Float3(0))
			movedir = movedir.Normalize();

		movedir = RotateXYZ(camera->rot) * Float4(movedir);
		movedir *= speed * dt;

		camera->pos += movedir;
	}

	void Look(mw::MouseRawMovedEvent* event, Float3& rot, mw::Window* window, float speed) {
		Int2 mouseMoved = event->GetMoved();
		Float3 toMove(-mouseMoved.y, -mouseMoved.x, 0);

		toMove *= speed;
		rot += toMove;
		rot.x = Clamp<float>(Radians<float>(-85), Radians<float>(85), rot.x);

		window->SetMousePos((Float2) window->GetClientSize() / 2.0f);
	}

	#endif
}