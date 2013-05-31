#include "Voxelizor.hpp"
#include <OgreSceneManager.h>
#include <OgreManualObject.h>
#include <OgreRoot.h>
#include <algorithm>
#include <thread>
#include <future>
#include <mutex>

#include "ExtractionController.hpp"
#include "ModifiedMarchingCubesSurfaceExtractor.h"
#include "ModifiedCubicSurfaceExtractor.h"
#include "ModifiedCubicSurfaceExtractorWithNormals.h"
#include "PositionNormalData.hpp"

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
  std::cout << "starting fill" << std::endl;
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
  int workSize = 32*32*32;
  std::vector<Vec3I> remaining_offsets;
  for (int z = 0; z < voxel_data->getDepth(); z+=32) {
    for (int y = 0; y < voxel_data->getHeight(); y+=32) {
      for (int x = 0; x < voxel_data->getWidth(); x+=32) {
        remaining_offsets.push_back(Vec3I(x,y,z));
        
        /*for (int xx = 0; xx < 32; ++xx) {
          for (int yy = 0; yy < 32; ++yy) {
            for (int zz = 0; zz < 32; ++zz) {

              voxel_data->setVoxelAt(x+xx, y+yy, z+zz, combine.execute(x+xx,y+yy,z+zz));
            }
          }
        }*/
      }
    }
  }

  int num_tasks = 6;
  std::vector<std::vector<Voxel>> temps;
  std::mutex done_mutex;
  enum codes {
    k_finished_task = 0,
    k_running = 1,
    k_stopped = 2
  };
  std::vector<int> done; //Store status codes
  std::vector<std::thread> threads;
  std::vector<Vec3I> on_offset;

  auto function = [&temps, &combine, &done_mutex, &done](int thread_index, Vec3I offset) {
    std::cout << "Thread "<< thread_index << "Running" << std::endl;
    std::cout << "On offset (" << offset.getX() <<"," << offset.getY() <<","<<offset.getZ()<< ")" <<std::endl;
    
    for (auto i=temps[thread_index].begin(); i != temps[thread_index].end(); ++i) {
      int loc = i - temps[thread_index].begin();
      int x = loc%32;
      int y = (loc%(32*32)) / 32;
      int z = loc/32/32;
      combine.execute(offset.getX()+x, offset.getY()+y, offset.getZ()+z, *i);
    }
    std::cout << "Thread " << thread_index << "Done" << std::endl;
    
    done_mutex.lock();
    done[thread_index] = k_finished_task;
    done_mutex.unlock();
  };


  std::cout << "b1" << std::endl; 
  for (int i=0; i < num_tasks; ++i) {
    done_mutex.lock();
    done.push_back(k_running);
    done_mutex.unlock();
    
    temps.push_back(std::vector<Voxel>());// workSize, Voxel(1., Vec3F(1,1,1))));
    for (int j =0; j < workSize; j++) {
      temps[i].push_back(Voxel());
    }

    Vec3I offset = remaining_offsets.back();
    remaining_offsets.pop_back();
    on_offset.push_back(offset);

    auto bind_func = std::bind(function, i, offset);
    std::cout << "Thread " << i << " Started" << std::endl;
    
    threads.push_back(std::thread(bind_func));
  }

  while (!std::all_of(done.begin(), done.end(), [](int f) {return f == k_stopped;})) {

    auto found = std::find(done.begin(), done.end(), k_finished_task);
    if (found == done.end()) {
      //Still running
      continue;
    }
    int i = found - done.begin();
    threads[i].join();
    Vec3I cur_offset = on_offset[i]; 
    for (int loc=0; loc < workSize; ++loc) {
      int x = loc%32;
      int y = (loc%(32*32)) / 32;
      int z = loc/32/32;
      Voxel v = temps[i][loc];
      voxel_data->setVoxelAt(cur_offset.getX() + x, cur_offset.getY() + y, cur_offset.getZ() + z, v);
    }
    

    if (!remaining_offsets.empty()) {
      Vec3I offset = remaining_offsets.back();
      remaining_offsets.pop_back();
      on_offset[i] = offset;
      
      auto bind_func = std::bind(function, i, offset);
      done_mutex.lock();
      done[i] = k_running;
      done_mutex.unlock();
      
      std::cout << "Restarted Thread " << i << " Started" << std::endl;
      threads[i] = std::thread(bind_func);
    } else {
      std::cout << "Stopped thread" << i << std::endl;
      done_mutex.lock();
      done[i] = k_stopped;
      done_mutex.unlock();
    }
  }
  
  for (auto temp : temps) {
    std::cout << temp[0].getDensity() << temp[199].getDensity() << std::endl;
  }

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

