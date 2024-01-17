#include "pch.h"

#include "AssetHandler.h"
#include "stb_image.h"

std::string texturePaths[] {
	"res/textures/dirt.png",
	"res/textures/grass_block_side.png",
	"res/textures/grass_block_top.png"
};

AssetHandler::AssetHandler(Instance* instance) {
	textures.resize(3);

	for (uint8 i = 0; i < 3; i++) {
		Int2 texSize;
		void* pixels = stbi_load(texturePaths[i].c_str(), &texSize.x, &texSize.y, nullptr, 4);
		if (pixels == nullptr)
			MW_ERROR("Failed to load image.");

		Image* image = Image::Create(instance, texSize);
		image->SetData(pixels);

		stbi_image_free(pixels);

		textures[i] = Texture::Create(instance, image, true);
	}

	ubo = Buffer::Create(instance, sizeof(Uniform), BUFFER_USAGE_UNIFORM);
	uniform = {};
}

AssetHandler::~AssetHandler() {
	delete ubo;

	for (uint8 i = 0; i < 3; i++) {
		Image* image = textures[i]->GetImage();
		delete textures[i];
		delete image;
	}
}