#pragma once
#include <limits>
#include <PolyVoxCore/Vector.h>

class ExtractionController
{
  public:
    ExtractionController()
    {
      threshold = 0;
    }

    //Required implementations
    
		typedef float DensityType;
		typedef PolyVox::Vector3DFloat MaterialType;
    
    float convertToDensity(const Voxel & voxel) { return voxel.getDensity();}
    //TODO fix me
    PolyVox::Vector3DFloat& convertToMaterial(Voxel voxel) { return voxel.getMaterial();}
    float getThreshold() {return threshold;}

  private:
    float threshold;
};
