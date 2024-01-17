#include "pch.h"

#include "Chunk.h"
#include "AssetHandler.h"

struct Vertex {
	Float2 position;
	Float2 texCoords;
};

struct PerInstance {
	uint32 texture;
	Float3 normal;
	Float4x4 model;
};

const std::vector<Vertex> vertices = {
	{ {  0.5f, -0.5f }, { 1.0f, 1.0f } },
	{ {  0.5f,  0.5f }, { 1.0f, 0.0f } },
	{ { -0.5f,  0.5f }, { 0.0f, 0.0f } },
	{ { -0.5f, -0.5f }, { 0.0f, 1.0f } }
};

const std::vector<uint16> indices = {
	0, 1, 2, 2, 3, 0
};

Chunk::Chunk(Framebuffer* framebuffer, AssetHandler* assetHandler) {
	this->assetHandler = assetHandler;

	for (uint32 x = 0; x < CHUNK_LENGTH; x++) {
		for (uint32 y = 0; y < CHUNK_HEIGHT; y++) {
			for (uint32 z = 0; z < CHUNK_WIDTH; z++) {
				blocks[x][y][z] = BLOCK_AIR;
				if (y <= 8) {
					blocks[x][y][z] = BLOCK_DIRT;
				} else if (y == 9) {
					//if ((x - (z % 2 == 0)) % 2 == 0) {
						blocks[x][y][z] = BLOCK_GRASS;
					//}
				}
			}
		}
	}

	buffer = nullptr;
	vertexShader = Shader::Create(framebuffer->GetInstance(), "res/shaders/vert.spv");
	fragmentShader = Shader::Create(framebuffer->GetInstance(), "res/shaders/frag.spv");
	graphicsPipeline = GraphicsPipeline::Create(framebuffer->GetInstance());
	graphicsPipeline->framebuffer = framebuffer;
	this->framebuffer = framebuffer;
}

Chunk::~Chunk() {
	delete graphicsPipeline;
	delete fragmentShader;
	delete vertexShader;
	delete buffer;
}

void Chunk::Render(CommandBuffer* commandBuffer) {
	commandBuffer->QueueDraw(graphicsPipeline, vertexRegion, indexRegion, instanceRegion, assetHandler->ubo, assetHandler->textures);
}

void Chunk::Build() {
	uint32 visibleBlocks = 0;
	for (uint32 x = 0; x < CHUNK_LENGTH; x++) {
		for (uint32 y = 0; y < CHUNK_HEIGHT; y++) {
			for (uint32 z = 0; z < CHUNK_WIDTH; z++) {
				if (!IsTransparent(blocks[x][y][z]))
					visibleBlocks++;
			}
		}
	}

	uint32 facesToDraw = 6 * visibleBlocks;
	std::vector<PerInstance> instances(facesToDraw);

	Float3 faces[] {
		{ 0, 0, -1 },
		{ 0, 0, 1 },
		{ 1, 0, 0 },
		{ -1, 0, 0 },
		{ 0, 1, 0 },
		{ 0, -1, 0 }
	};

	Float3 faceRotations[] {
		Radians(Float3(0, -180, 0)),
		Radians(Float3(0, 0, 0)),
		Radians(Float3(0, 90, 0)),
		Radians(Float3(0, -90, 0)),
		Radians(Float3(-90, 0, 0)),
		Radians(Float3(90, 0, 0))
	};

	uint32 i = 0;
	for (uint32 x = 0; x < CHUNK_LENGTH; x++) {
		for (uint32 y = 0; y < CHUNK_HEIGHT; y++) {
			for (uint32 z = 0; z < CHUNK_WIDTH; z++) {
				if (IsTransparent(blocks[x][y][z]))
					continue;

				for (uint8 face = 0; face < 6; face++) {
					PerInstance& instance = instances[i];

					if (IsBlockInSpace(Int3(x, y, z) + (Int3) faces[face])) {
						facesToDraw--;
						continue;
					}

					Transform transform(
						(faces[face] / 2.0f) + Float3(0.5, 0.5, 0.5) + Float3(x, y, z),
						Float3(1),
						faceRotations[face]
					);

					instance.texture = GetBlockInfo(blocks[x][y][z]).texture[face];
					instance.normal = faces[face];
					instance.model = transform.GetMatrix();

					i++;
				}
			}
		}
	}
	
	if (buffer != nullptr)
		delete buffer;

	buffer = Buffer::Create(
		framebuffer->GetInstance(),
		sizeof(Vertex) * vertices.size() + sizeof(uint16) * indices.size() + sizeof(PerInstance) * facesToDraw,
		BUFFER_USAGE_ALL
	);

	vertexRegion = { buffer, sizeof(Vertex) * vertices.size(), 0 };
	indexRegion = { buffer, sizeof(uint16) * indices.size(), vertexRegion.offset + vertexRegion.size };
	instanceRegion = { buffer, sizeof(PerInstance) * facesToDraw, indexRegion.offset + indexRegion.size };

	vertexRegion.SetData((void*) vertices.data());
	indexRegion.SetData((void*) indices.data());
	instanceRegion.SetData((void*) instances.data());

	graphicsPipeline->vertexSpecification = { SHADER_DATA_TYPE_FLOAT_VEC2, SHADER_DATA_TYPE_FLOAT_VEC2 };
	graphicsPipeline->indexCount = indices.size();
	graphicsPipeline->instanceSpecification = { SHADER_DATA_TYPE_UINT32, SHADER_DATA_TYPE_FLOAT_VEC3, SHADER_DATA_TYPE_FLOAT_MAT4 };
	graphicsPipeline->instanceCount = facesToDraw;
	graphicsPipeline->hasUniformBuffer = true;
	graphicsPipeline->textureCount = 3;
	graphicsPipeline->cullingMode = CULLING_MODE_BACK_BIT;
	graphicsPipeline->depthTesting = true;
	graphicsPipeline->vertexShader = { vertexShader, "main" };
	graphicsPipeline->fragmentShader = { fragmentShader, "main" };
	graphicsPipeline->Rebuild();
}

bool Chunk::IsBlockInSpace(Float3 pos) {
	if (pos.x < 0 || pos.x >= CHUNK_LENGTH)
		return false;
	if (pos.y < 0 || pos.y >= CHUNK_HEIGHT)
		return false;
	if (pos.z < 0 || pos.z >= CHUNK_WIDTH)
		return false;

	UInt3 upos = pos;
	return blocks[upos.x][upos.y][upos.z] != 0;
}
