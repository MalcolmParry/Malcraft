#pragma once

#include "pch.h"

#include "Chunk.h"
#include "AssetHandler.h"

class Application {
public:
	Application() = default;

	void Start();
	void End();
	void Update(float dt);
	void Render();
	void OnEvent(Event* event);

	void Move(float dt);
	void Log();

	Chunk* chunk;
	CommandBuffer* commandBuffer;
	PerspectiveCamera camera;
	AssetHandler* assetHandler;

	const float moveSpeed = 5.0f;
	const float lookSpeed = 0.002f;
	const float gravitationalVelocity = 9;

	// player controler
	struct {
		Float3 pos;
		Float3 dir;
		Float3 velocity;
	} player;

	Window* window;
	Instance* instance;
	Display* display;
	bool toClose = false;
	bool focused = false;
	float fps = 100.0f;
	Float3 sunDir = Float3(1, 1, 0);
};