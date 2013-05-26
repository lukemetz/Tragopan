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
  for (auto func : functions) {
    func(x,y,z,voxel);
  }
}

void VoxelFunction::addFunction(std::function<void(int, int, int, Voxel&)> func)
{
  functions.push_back(func);
}

