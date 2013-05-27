#include "FunctionLibrary.hpp"

namespace FunctionLibrary
{
  std::pair<Region, voxel_function> makeEllipsoid(
        const Vec3F & position,
        const Vec3F & radius,
        const Vec3F & color,
        const float & steep)
  {
    auto lambda = [&position, &radius, &color, &steep](int x, int y, int z, Voxel& v) {
      float oldDensity = v.getDensity();
      float newDensity = (1 - (
                         ((x - position.getX()) * (x - position.getX()) / (radius.getX() * radius.getX())) + 
                         ((y - position.getY()) * (y - position.getY()) / (radius.getY() * radius.getY())) +
                         ((z - position.getZ()) * (z - position.getZ()) / (radius.getZ() * radius.getZ()))
                         )) * steep;
      if (oldDensity <= newDensity) {
        v.setDensity(newDensity);
        v.setMaterial(color);
      }
    };
    //4 is arbitrary just to overshoot to ensure correctness in the isofield
    auto lower_f = position-radius-Vec3F(4, 4, 4);
    auto upper_f = position+radius+Vec3F(4, 4, 4);
    auto lower = Vec3I(lower_f.getX(), lower_f.getY(), lower_f.getZ());
    auto upper = Vec3I(upper_f.getX(), upper_f.getY(), upper_f.getZ());

    auto region = Region(lower, upper);
    return std::make_pair<Region&, voxel_function>(region, lambda);
  }


  std::pair<Region, voxel_function> clearAll(const float & density)
  {
    auto lambda = [&density](int x, int y, int z, Voxel& v) {
      v.setDensity(density);
      v.setMaterial(Vec3F(0,0,0));
    };
    //TODO FIX THIS hardcoding of max size. Should really pass 0,0,0 to 0,0,0 and it should do automatically
    auto region = Region(Vec3I(0,0,0), Vec3I(1024, 1024, 1024));
    return std::make_pair<Region&, voxel_function>(region, lambda);
  }
  

  std::pair<Region, voxel_function> makeBox(
        const Vec3F & center,
        const Vec3F & size,
        const Vec3F & color,
        const float & steep)
  {
    auto lambda = [&center, &size, &color, &steep](int x, int y, int z, Voxel& v) {
      float oldDensity = v.getDensity();
      float newDensity = - std::max( std::max(
                         std::fabs(x - center.getX()) - size.getX()/2,
                         std::fabs(y - center.getY()) - size.getY()/2),
                         std::fabs(z - center.getZ()) - size.getZ()/2) * steep;
      if (oldDensity <= newDensity) {
        v.setDensity(newDensity);
        v.setMaterial(color);
      }
    };
    //4 is arbitrary just to overshoot to ensure correctness in the isofield
    auto lower_f = center-size/Vec3F(2,2,2)-Vec3F(4, 4, 4);
    auto upper_f = center+size/Vec3F(2,2,2)+Vec3F(4, 4, 4);
    auto lower = Vec3I(lower_f.getX(), lower_f.getY(), lower_f.getZ());
    auto upper = Vec3I(upper_f.getX(), upper_f.getY(), upper_f.getZ());

    auto region = Region(lower, upper);
    return std::make_pair<Region&, voxel_function>(region, lambda);
  }
}
