#pragma once
#include <functional>
#include <vector>
#include "Voxel.hpp"

class VoxelFunction
{
  public:
    VoxelFunction();
    ~VoxelFunction();
    
    Voxel execute(int x, int y, int z);
    void execute(int x, int y, int z, Voxel & voxel);
    
    void addFunction(std::function<void(int, int, int, Voxel&)> func);

  private:
    std::vector<std::function<void(int, int, int, Voxel&)>> functions;

};
