#include "TypeDefs.hpp"

#include "Voxel.hpp"
#include <PolyVoxCore/Region.h>
#include <functional>

namespace FunctionLibrary
{
  std::pair<Region, voxel_function> makeBox(
        const Vec3F & center,
        const Vec3F & size,
        const Vec3F & color = Vec3F(1,1,1),
        const float & steep = 10);

  
  std::pair<Region, voxel_function> clearAll(const float & density = -30);
 
  std::pair<Region, voxel_function> makeEllipsoid(
        const Vec3F & position,
        const Vec3F & radius,
        const Vec3F & color = Vec3F(1,1,1),
        const float & steep = 10);
};
