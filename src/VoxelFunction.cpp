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
  auto key = PolyVox::Vector3DInt32((x/32)*32, (y/32)*32, (z/32)*32);
  auto range = functions.equal_range(key);

  for (auto it = range.first; it != range.second; ++it) {
    (it->second)(x,y,z,voxel);
  }
}

void VoxelFunction::addFunction(const PolyVox::Region & region,
    std::function<void(int, int, int, Voxel&)> func)
{
  //TODO don't hard code in 32 for cell size
  auto lower = region.getLowerCorner();
  int lx = (lower.getX()/32)*32;
  int ly = (lower.getY()/32)*32;
  int lz = (lower.getZ()/32)*32;

  auto upper = region.getUpperCorner();
  int ux = (upper.getX()/32)*32;
  int uy = (upper.getY()/32)*32;
  int uz = (upper.getZ()/32)*32;

  for (int x = lx; x <= ux; x+=32) {
    for (int y = ly; y <= uy; y+=32) {
      for (int z = lz; z <= uz; z+=32) {
        PolyVox::Vector3DInt32 loc(x,y,z);

        auto pair = std::pair<PolyVox::Vector3DInt32, 
           std::function<void(int, int, int, Voxel&)>>(loc, func);
        
        functions.insert(pair);
      }
    }
  }
}

