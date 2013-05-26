#include "Voxelizor.hpp"
#include <OgreSceneManager.h>
#include <OgreManualObject.h>
#include <OgreRoot.h>
#include <algorithm>

#include "ExtractionController.hpp"
#include "ModifiedMarchingCubesSurfaceExtractor.h"
#include "PositionNormalData.hpp"

#include "VoxelFunction.hpp"

Voxelizor::Voxelizor()
{
  voxel_data = std::make_shared<PolyVox::LargeVolume<Voxel>>
    (PolyVox::Region(PolyVox::Vector3DInt32(0,0,0), PolyVox::Vector3DInt32(127,127,127)));
  //Kill for now due to compression bugs
  voxel_data->setCompressionEnabled(false);
}

Voxelizor::~Voxelizor()
{
  
}

void Voxelizor::fill()
{
  int size = 128;
  PolyVox::Vector3DFloat center(size/2, size/2, size/2);

  auto cube = [&size](int x, int y, int z, Voxel& v) {
    v.setDensity(size/2-20 - std::max( std::max(std::fabs(z-size/2), std::fabs(x-size/2)), std::fabs(y-size/2)));
    v.setMaterial(PolyVox::Vector3DFloat(x/static_cast<float>(size), y/static_cast<float>(size), z/static_cast<float>(size)));
  };

  auto sphere = [&center, &size](int x, int y, int z,Voxel & v) {
        PolyVox::Vector3DFloat cur(x,y,z);
        float dist = (cur - center).length();
        float circle_density = size/2 - 5 - dist;
        if (v.getDensity() < circle_density) {
          v.setDensity(circle_density);
          v.setMaterial(PolyVox::Vector3DFloat(1, 0, 0));
        }
  };
  
  VoxelFunction combine;
  combine.addFunction(PolyVox::Region(PolyVox::Vector3DInt32(0,0,0), PolyVox::Vector3DInt32(size, size, size)),
      cube);
  
  //combine.addFunction(PolyVox::Region(PolyVox::Vector3DInt32(0,0,0), PolyVox::Vector3DInt32(size, size, size)),
  //    sphere);

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

void Voxelizor::to_ogre_mesh(Ogre::SceneManager &mgr)
{
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

  PolyVox::SurfaceMesh<PositionNormalData<PolyVox::Vector3DFloat>> mesh;
  std::cout << "creating surface" << std::endl;
  //PolyVox::CubicSurfaceExtractorWithNormals<PolyVox::SimpleVolume<Voxel>>
  ExtractionController controller;
  PolyVox::ModifiedMarchingCubesSurfaceExtractor<PolyVox::LargeVolume<Voxel>, PolyVox::Vector3DFloat, ExtractionController>
    surf(voxel_data.get(), voxel_data->getEnclosingRegion(), &mesh, controller);
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

    auto finalPos = pos + static_cast<PolyVox::Vector3DFloat>(mesh.m_Region.getLowerCorner());
    
    ogre_mesh->position(finalPos.getX(), finalPos.getY(), finalPos.getZ());
    ogre_mesh->normal(normal.getX(), normal.getY(), normal.getZ());
    PolyVox::Vector3DFloat mat = vertex.getMaterial();
    //ogre_mesh->colour(mat/200, mat/200, mat/200);
    ogre_mesh->colour(mat.getX(), mat.getY(), mat.getZ());
  }
  ogre_mesh->end();
  std::cout << "MesH size" <<  vecVertices.size() <<std::endl;
}

