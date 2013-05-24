#pragma once
#include <PolyVoxCore/Vector.h>

class Voxel
{
  public:
    Voxel(){};
    Voxel(float dens, PolyVox::Vector3DFloat colorToSet) : density(dens), color(colorToSet) {};
    bool operator==(const Voxel & rhs)
    {
      return rhs.getDensity() == density;
    }

    bool operator!=(const Voxel & rhs)
    {
      return !(*this == rhs);
    }
    
    float getDensity() const {return density;}

    PolyVox::Vector3DFloat & getColor() { return color; };
    
    void setDensity(float dens) {density = dens;}
    void setMaterial(const PolyVox::Vector3DFloat& mat) {color = mat;}
    PolyVox::Vector3DFloat & getMaterial() {return color;}

  private:
    float density;
    PolyVox::Vector3DFloat color;
};
