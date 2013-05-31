#pragma once

#include "TypeDefs.hpp"

#include <PolyVoxCore/Vector.h>

class Voxel
{
  public:
    Voxel() : density(1), color(Vec3I(1,0,0)){};
    Voxel(float dens, Vec3F colorToSet) : density(dens), color(colorToSet) {};
    bool operator==(const Voxel & rhs)
    {
      return rhs.getDensity() == density;
    }

    bool operator!=(const Voxel & rhs)
    {
      return !(*this == rhs);
    }
    
    float getDensity() const {return density;}

    Vec3F & getColor() { return color; };
    
    void setDensity(float dens) {density = dens;}
    void setMaterial(const Vec3F & mat) {color = mat;}
    Vec3F & getMaterial() {return color;}

  private:
    float density;
    Vec3F color;
};
