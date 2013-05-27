#pragma once

#include <PolyVoxCore/CubicSurfaceExtractorWithNormals.h>
#include <PolyVoxCore/MarchingCubesSurfaceExtractor.h>
#include <PolyVoxCore/SurfaceMesh.h>
#include <PolyVoxCore/LargeVolume.h>
#include <PolyVoxCore/SimpleVolume.h>
#include <PolyVoxCore/Material.h>
#include <PolyVoxCore/MaterialDensityPair.h>
#include <memory.h>

#include <OgreSceneManager.h>

#include "Voxel.hpp"
class Voxelizor
{
public:
  Voxelizor();
  ~Voxelizor();
  
  void fill();
  void make_wall();
  
  void to_ogre_mesh(Ogre::SceneManager & mgr);

  std::shared_ptr<PolyVox::LargeVolume<Voxel>> voxel_data;

};
