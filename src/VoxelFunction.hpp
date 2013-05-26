#pragma once
#include <functional>
#include <vector>
#include <map>

#include "Voxel.hpp"

#include <PolyVoxCore/Vector.h>
#include <PolyVoxCore/Region.h>

class VoxelFunction
{
  public:
    VoxelFunction();
    ~VoxelFunction();
    
    Voxel execute(int x, int y, int z);
    void execute(int x, int y, int z, Voxel & voxel);
    
    void addFunction(const PolyVox::Region & region, std::function<void(int, int, int, Voxel&)> func);
    void addFunction(std::pair<const PolyVox::Region&,
        std::function<void(int, int, int, Voxel&)>> pair);

  private:
    std::multimap<PolyVox::Vector3DInt32, std::function<void(int, int, int, Voxel&)>> functions;

};
