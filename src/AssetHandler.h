#pragma once

#include "pch.h"

#pragma pack 16
struct Uniform {
	Float4x4 view;
	Float4x4 proj;
	Float3 sunDir;
};

struct AssetHandler {
	AssetHandler(Instance* instance);
	~AssetHandler();

	std::vector<Texture*> textures;
	Buffer* ubo;
	Uniform uniform;
};