#include "VoxelFunction.hpp"

VoxelFunction::VoxelFunction()
{
}

VoxelFunction::~VoxelFunction()
{
}

Voxel VoxelFunction::execute(int x, int y, int z)
{
  Voxel v;
  execute(x,y,z,v);
  return v;
}

void VoxelFunction::execute(int x, int y, int z, Voxel & voxel)
{
  auto key = Vec3I((x/32)*32, (y/32)*32, (z/32)*32);
  auto range = functions.equal_range(key);
  for (auto it = range.first; it != range.second; ++it) {
    (it->second)(x,y,z,voxel);
  }
}

void VoxelFunction::addFunction(std::pair<const Region&, voxel_function> pair)
{
  return addFunction(pair.first, pair.second);
}

void VoxelFunction::addFunction(const Region & region,
    std::function<void(int, int, int, Voxel&)> func)
{
  //TODO don't hard code in 32 for cell size
  auto lower = region.getLowerCorner();
  int lx = (lower.getX()/32)*32;
  int ly = (lower.getY()/32)*32;
  int lz = (lower.getZ()/32)*32;

  auto upper = region.getUpperCorner();
  int ux = (upper.getX()/32+1)*32;
  int uy = (upper.getY()/32+1)*32;
  int uz = (upper.getZ()/32+1)*32;

  for (int x = lx; x <= ux; x+=32) {
    for (int y = ly; y <= uy; y+=32) {
      for (int z = lz; z <= uz; z+=32) {
        Vec3I loc(x,y,z);
        auto pair = std::pair<Vec3I, voxel_function>(loc, func);
        functions.insert(pair);
      }
    }
  }
}

