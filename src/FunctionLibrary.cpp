#include "FunctionLibrary.hpp"

namespace FunctionLibrary
{
  std::pair<PolyVox::Region,
    std::function<void(int, int, int, Voxel&)>> makeEllipsoid(
        const PolyVox::Vector3DFloat & position,
        const PolyVox::Vector3DFloat & radius,
        const PolyVox::Vector3DFloat & color,
        const float & steep)
  {
    auto lambda = [&position, &radius, &color, &steep](int x, int y, int z, Voxel& v) {
      float oldDensity = v.getDensity();
      float newDensity = (1 - (
                         ((x - position.getX()) * (x - position.getX()) / (radius.getX() * radius.getX())) + 
                         ((y - position.getY()) * (y - position.getY()) / (radius.getY() * radius.getY())) +
                         ((z - position.getZ()) * (z - position.getZ()) / (radius.getZ() * radius.getZ()))
                         )) * steep;
      v.setDensity(-100);
      if (oldDensity <= newDensity) {
        std::cout << x << "," << y << "," << z << ", " << oldDensity << "New:: " << newDensity << std::endl;
        v.setDensity(newDensity);
        v.setMaterial(color);
      }
    };
    
    //4 is arbitrary just to overshoot to ensure correctness in the isofield
    auto lower_f = position-radius-PolyVox::Vector3DFloat(4, 4, 4);
    auto upper_f = position+radius+PolyVox::Vector3DFloat(4, 4, 4);
    auto lower = PolyVox::Vector3DInt32(lower_f.getX(), lower_f.getY(), lower_f.getZ());
    auto upper = PolyVox::Vector3DInt32(upper_f.getX(), upper_f.getY(), upper_f.getZ());

    auto region = PolyVox::Region(lower, upper);
    return std::make_pair<PolyVox::Region&, std::function<void(int, int, int, Voxel&)>>(region, lambda);
  }

  std::pair<PolyVox::Region,
    std::function<void(int, int, int, Voxel&)>> clearAll(const float & density)
  {
    auto lambda = [&density](int x, int y, int z, Voxel& v) {
      v.setDensity(density);
      v.setMaterial(PolyVox::Vector3DFloat(0,0,0));
    };
    //TODO FIX THIS hardcoding of max size. Should really pass 0,0,0 to 0,0,0 and it should do automatically
    auto region = PolyVox::Region(PolyVox::Vector3DInt32(0,0,0), PolyVox::Vector3DInt32(1024, 1024, 1024));
    return std::make_pair<PolyVox::Region&, std::function<void(int, int, int, Voxel&)>>(region, lambda);
  }

}
