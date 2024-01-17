#include "pch.h"

#include "Block.h"

Float3 faceDirections[] {
	{ 0, 0, -1 },
	{ 0, 0, 1 },
	{ 1, 0, 0 },
	{ -1, 0, 0 },
	{ 0, 1, 0 },
	{ 0, -1, 0 }
};

Float3 faceEulerAngles[] {
	Radians(Float3(0, -180, 0)),
	Radians(Float3(0, 0, 0)),
	Radians(Float3(0, 90, 0)),
	Radians(Float3(0, -90, 0)),
	Radians(Float3(-90, 0, 0)),
	Radians(Float3(90, 0, 0))
};

Float3 GetFaceDirection(BlockFace face) {
	return faceDirections[face];
}

Float3 GetFaceEulerAngles(BlockFace face) {
	return faceEulerAngles[face];
}

BlockInfo blockInfos[BLOCK_COUNT] {
	{ // Air
		true,
		{ 0, 0, 0, 0, 0, 0 }
	},
	{ // Dirt
		false,
		{ 0, 0, 0, 0, 0, 0 }
	},
	{ // grass
		false,
		{ 1, 1, 1, 1, 2, 0 }
	}
};

BlockInfo GetBlockInfo(Block block) {
	return blockInfos[block];
}

bool IsTransparent(Block block) {
	return blockInfos[block].transparent;
}
