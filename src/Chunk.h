#pragma once

#include "pch.h"
#include "AssetHandler.h"
#include "Block.h"

const uint8 CHUNK_LENGTH = 16;
const uint8 CHUNK_WIDTH = 16;
const uint8 CHUNK_HEIGHT = 16;
const uint32 BLOCKS_PER_CHUNK = CHUNK_LENGTH * CHUNK_WIDTH * CHUNK_HEIGHT;

class Chunk {
public:
	Chunk(Framebuffer* framebuffer, AssetHandler* assetHandler);
	~Chunk();
	
	void Render(CommandBuffer* commandBuffer);
	void Build();
	bool IsBlockInSpace(Float3 pos);

	Block blocks[CHUNK_LENGTH][CHUNK_WIDTH][CHUNK_HEIGHT];

	AssetHandler* assetHandler;
	Buffer* buffer = nullptr;
	BufferRegion vertexRegion;
	BufferRegion indexRegion;
	BufferRegion instanceRegion;
	Shader* vertexShader;
	Shader* fragmentShader;
	GraphicsPipeline* graphicsPipeline;
	Framebuffer* framebuffer;
};