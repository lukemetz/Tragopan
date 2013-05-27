#pragma once
#include <PolyVoxCore/Region.h>
#include <PolyVoxCore/Vector.h>
#include <functional>

class Voxel;

typedef std::function<void(int, int, int, Voxel&)> voxel_function;

typedef PolyVox::Region Region;
typedef PolyVox::Vector3DFloat Vec3F;
typedef PolyVox::Vector3DInt32 Vec3I;

