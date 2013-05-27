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
		typedef Vec3F MaterialType;
    
    float convertToDensity(const Voxel & voxel) { return voxel.getDensity();}
    //TODO fix me
    Vec3F& convertToMaterial(Voxel voxel) { return voxel.getMaterial();}
    float getThreshold() {return threshold;}

  private:
    float threshold;
};
