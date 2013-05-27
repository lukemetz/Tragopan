#include "Voxel.hpp"

#include <PolyVoxCore/Region.h>

#include <functional>

namespace FunctionLibrary
{
  std::pair<PolyVox::Region,
    std::function<void(int, int, int, Voxel&)>> makeBox(
        const PolyVox::Vector3DFloat & center,
        const PolyVox::Vector3DFloat & size,
        const PolyVox::Vector3DFloat & color = PolyVox::Vector3DFloat(1,1,1),
        const float & steep = 10);

  
  std::pair<PolyVox::Region,
    std::function<void(int, int, int, Voxel&)>> clearAll(const float & density = -30);
 
  std::pair<PolyVox::Region,
    std::function<void(int, int, int, Voxel&)>> makeEllipsoid(
        const PolyVox::Vector3DFloat & position,
        const PolyVox::Vector3DFloat & radius,
        const PolyVox::Vector3DFloat & color = PolyVox::Vector3DFloat(1,1,1),
        const float & steep = 10);
};
