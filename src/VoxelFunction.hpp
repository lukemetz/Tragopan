#pragma once
#include <functional>
#include <vector>
#include <map>

#include "Voxel.hpp"
#include "TypeDefs.hpp"

#include <PolyVoxCore/Vector.h>
#include <PolyVoxCore/Region.h>

class VoxelFunction
{
  public:
    VoxelFunction();
    ~VoxelFunction();
    
    Voxel execute(int x, int y, int z);
    void execute(int x, int y, int z, Voxel & voxel);
    
    void addFunction(const Region & region, voxel_function func);
    void addFunction(std::pair<const Region&, voxel_function> pair);

  private:
    std::multimap<Vec3I, voxel_function> functions;

};
