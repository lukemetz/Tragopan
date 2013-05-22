#include "Voxelizor.hpp"
#include <OgreSceneManager.h>
#include <OgreManualObject.h>
#include <OgreRoot.h>
#include <algorithm>
Voxelizor::Voxelizor()
{
  voxel_data = std::make_shared<PolyVox::SimpleVolume<PolyVox::Material8>>(PolyVox::Region(PolyVox::Vector3DInt32(0,0,0), PolyVox::Vector3DInt32(63,63,63)));
}

Voxelizor::~Voxelizor()
{
  
}

void Voxelizor::fill()
{
  PolyVox::Vector3DFloat center(32, 32, 32);
  for (int z = 0; z < voxel_data->getDepth(); z++) {
    for (int y = 0; y < voxel_data->getHeight(); y++) {
      for (int x = 0; x < voxel_data->getWidth(); x++) {
        PolyVox::Vector3DFloat cur(x,y,z);
        float dist = (cur - center).length();
        uint8_t val = std::min(255.f, std::max(0.f, dist - 30));
        if (dist < 30) {
          val = x+y*10;
        } else {
          val = 0;
        }
        voxel_data->setVoxelAt(x, y, z, val);
      }
    }
  }
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

  PolyVox::SurfaceMesh<PolyVox::PositionMaterialNormal> mesh;
  std::cout << "creating surface" << std::endl;
  PolyVox::CubicSurfaceExtractorWithNormals<PolyVox::SimpleVolume<PolyVox::Material8>>
    surf(voxel_data.get(), voxel_data->getEnclosingRegion(), &mesh);
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
    float mat = vertex.getMaterial();
    ogre_mesh->colour(mat/200, mat/200, mat/200);
  }
  ogre_mesh->end();
  std::cout << "MesH size" <<  vecVertices.size() <<std::endl;

}

