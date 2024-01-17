#pragma once

#include "pch.h"

enum BlockFace : uint8 {
	FACE_FRONT,
	FACE_BACK,
	FACE_LEFT,
	FACE_RIGHT,
	FACE_UP,
	FACE_DOWN,
	FACE_COUNT
};

Float3 GetFaceDirection(BlockFace face);
Float3 GetFaceEulerAngles(BlockFace face);

enum Block : uint8 {
	BLOCK_AIR,
	BLOCK_DIRT,
	BLOCK_GRASS,
	BLOCK_COUNT
};

struct BlockInfo {
	bool transparent;
	uint8 texture[FACE_COUNT];
};

BlockInfo GetBlockInfo(Block block);
bool IsTransparent(Block block);