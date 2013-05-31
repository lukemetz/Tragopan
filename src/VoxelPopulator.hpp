#pragma once

#include "VoxelFunction.hpp"
#include "TypeDefs.hpp"
#include <memory>
template <typename VolumeType>
class VoxelPopulator
{
  public:
    VoxelPopulator(std::shared_ptr<VolumeType> volume);
    
    void update(VoxelFunction & function, const Region & region);

  private:
    std::shared_ptr<VolumeType> voxel_data;
};

#include "VoxelPopulator.inl"
