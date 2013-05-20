#pragma once

#include <PolyVoxCore/CubicSurfaceExtractorWithNormals.h>
#include <PolyVoxCore/MarchingCubesSurfaceExtractor.h>
#include <PolyVoxCore/SurfaceMesh.h>
#include <PolyVoxCore/SimpleVolume.h>
#include <PolyVoxCore/Material.h>
#include <memory.h>

#include <OgreSceneManager.h>

class Voxelizor
{
public:
  Voxelizor();
  ~Voxelizor();
  
  void fill();
  
  void to_ogre_mesh(Ogre::SceneManager & mgr);

  std::shared_ptr<PolyVox::SimpleVolume<PolyVox::Material8>> voxel_data;

};
