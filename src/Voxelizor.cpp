#include "Voxelizor.hpp"
#include <OgreSceneManager.h>
#include <OgreManualObject.h>
#include <OgreRoot.h>
#include <algorithm>
#include <thread>
#include <future>
#include <mutex>
#include <chrono>

#include "ExtractionController.hpp"
#include "ModifiedMarchingCubesSurfaceExtractor.h"
#include "ModifiedCubicSurfaceExtractor.h"
#include "ModifiedCubicSurfaceExtractorWithNormals.h"
#include "PositionNormalData.hpp"

#include "VoxelPopulator.hpp"
#include "VoxelFunction.hpp"
#include "FunctionLibrary.hpp"

Voxelizor::Voxelizor()
{
  voxel_data = std::make_shared<PolyVox::LargeVolume<Voxel>>
    (PolyVox::Region(Vec3I(0,0,0), Vec3I(127,127,127)));
  //Kill for now due to compression bugs
  voxel_data->setCompressionEnabled(false);
  voxel_data->setBorderValue(Voxel(-100, Vec3F(0,0,0)));
}

Voxelizor::~Voxelizor()
{
  
}

void Voxelizor::fill()
{
  VoxelFunction combine;
  combine.addFunction(FunctionLibrary::clearAll());
  
  combine.addFunction(FunctionLibrary::makeEllipsoid(
        Vec3F(64, 64, 64),
        Vec3F(60, 5, 30)));

  combine.addFunction(FunctionLibrary::makeBox(
        Vec3F(64, 64, 64),
        Vec3F(110, 5, 50)));

  
  for (int z = 0; z < voxel_data->getDepth(); z++) {
    for (int y = 0; y < voxel_data->getHeight(); y++) {
      for (int x = 0; x < voxel_data->getWidth(); x++) {
        voxel_data->setVoxelAt(x, y, z, combine.execute(x,y,z));
      }
    }
  }

  std::cout << "Size is (mb): " << voxel_data->calculateSizeInBytes()/1000000.0
    << " Compresed: " << voxel_data->calculateCompressionRatio() << std::endl;
}

void Voxelizor::make_wall()
{
  std::cout << "Adding Functions" << std::endl;
  VoxelFunction combine;
  combine.addFunction(FunctionLibrary::clearAll());
  
  for (int x = 0; x < 9; x++) {
    for (int y = 0; y < 10; y++) {
      combine.addFunction(FunctionLibrary::makeBox(
            Vec3F(10+x*11+y%2*5, 2+y*6, 64), //center
            Vec3F(8, 4, 10), //size
            Vec3F(128./256, 110./256, 70./256) //color
            ));
    }
  }
  combine.addFunction(FunctionLibrary::makeBox(
        Vec3F(58, 30, 64), //center
        Vec3F(97, 60, 4), //size
        Vec3F(61./256, 54./256, 37./256) //Color
        ));
  
  combine.addFunction(FunctionLibrary::makeBox(
        Vec3F(50, 1, 64), //center
        Vec3F(150, 2, 150), //size
        Vec3F(0./256, 143./256, 2./256), //Color
        30.0f // steep
        ));
  
  combine.addFunction(FunctionLibrary::addPerlinDensity(
        13, // amplitude
        Vec3F(.05, .05, .05))); // frequency
  
  combine.addFunction(FunctionLibrary::addPerlinDensity(
        4.5, // amplitude
        Vec3F(.5, .5, .5))); //frequency
  
  combine.addFunction(FunctionLibrary::addPerlinColor(
        -.2, // amplitude
        Vec3F(.5, .5, .5))); //frequency

  combine.addFunction(FunctionLibrary::clampColor());

  VoxelPopulator<PolyVox::LargeVolume<Voxel>> populator(voxel_data);
  populator.update(combine, voxel_data->getEnclosingRegion());

  std::cout << "Size is (mb): " << voxel_data->calculateSizeInBytes()/1000000.0
    << " Compresed: " << voxel_data->calculateCompressionRatio() << std::endl;
}

void Voxelizor::to_ogre_mesh(Ogre::SceneManager &mgr)
{
  std::cout << "Making mesh" << std::endl;
  Ogre::ManualObject *ogre_mesh;
  ogre_mesh = mgr.createManualObject("name");
  ogre_mesh->setDynamic(true);
  ogre_mesh->begin("Terrain", Ogre::RenderOperation::OT_TRIANGLE_LIST);
  {
    ogre_mesh->position(0,0,0);
    ogre_mesh->normal(0,0,0);
    ogre_mesh->colour(1,0,0);
  }
  ogre_mesh->end();

  Ogre::SceneNode *scene_node = mgr.getRootSceneNode()->createChildSceneNode("vox", Ogre::Vector3(0,0,0));
  scene_node->attachObject(ogre_mesh);

  PolyVox::SurfaceMesh<PositionNormalData<Vec3F>> mesh;
  //PolyVox::SurfaceMesh<PositionData<Vec3F>> mesh;
  std::cout << "creating surface" << std::endl;
  
  ExtractionController controller;
  PolyVox::ModifiedCubicSurfaceExtractorWithNormals<PolyVox::LargeVolume<Voxel>, Vec3F> 
    surf(voxel_data.get(), voxel_data->getEnclosingRegion(), &mesh/*, false*/);
  //PolyVox::ModifiedMarchingCubesSurfaceExtractor<PolyVox::LargeVolume<Voxel>, Vec3F, ExtractionController>
  //  surf(voxel_data.get(), voxel_data->getEnclosingRegion(), &mesh, controller);
  
  surf.execute();

  std::cout << "updating ogre" << std::endl;
  ogre_mesh->beginUpdate(0);
  
  auto vecVertices = mesh.getVertices();
  auto vecIndices = mesh.getIndices();

  unsigned int LodLevel = 0;

  int beginIndex = mesh.m_vecLodRecords[LodLevel].beginIndex;
  int endIndex = mesh.m_vecLodRecords[LodLevel].endIndex;

  for (int index = beginIndex; index < endIndex; ++index) {
    auto vertex = vecVertices[vecIndices[index]];
    auto pos = vertex.getPosition();
    auto normal = vertex.getNormal();

    auto finalPos = pos + static_cast<Vec3F>(mesh.m_Region.getLowerCorner());
    
    ogre_mesh->position(finalPos.getX(), finalPos.getY(), finalPos.getZ());
    //Not for the cubic
    ogre_mesh->normal(normal.getX(), normal.getY(), normal.getZ());
    Vec3F mat = vertex.getMaterial();
    ogre_mesh->colour(mat.getX(), mat.getY(), mat.getZ());
  }
  ogre_mesh->end();
  std::cout << "MesH size" <<  vecVertices.size() <<std::endl;
}

