#pragma once
#include "TypeDefs.hpp"

template<typename dataType>
class PositionNormalData
{
  public:
  PositionNormalData();
  PositionNormalData(Vec3F positionToSet, dataType materialToSet);
  PositionNormalData(Vec3F positionToSet, Vec3F normal, dataType materialToSet);

  const dataType& getMaterial(void) const;
  const Vec3F& getNormal(void) const;
  const Vec3F& getPosition(void) const;	

  void setMaterial(const dataType & materialToSet);

  void setNormal(const Vec3F& normalToSet);
  void setPosition(const Vec3F& positionToSet);

  private:
    Vec3F position;
    Vec3F normal;
    dataType data;
};

#include "PositionNormalData.inl"
