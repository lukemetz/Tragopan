#pragma once
#include "TypeDefs.hpp"

template<typename dataType>
class PositionData
{
  public:
  PositionData();
  PositionData(Vec3F positionToSet, dataType materialToSet);

  const dataType& getMaterial(void) const;
  const Vec3F& getPosition(void) const;	

  void setMaterial(const dataType & materialToSet);
  void setPosition(const Vec3F& positionToSet);

  private:
    Vec3F position;
    dataType data;
};

#include "PositionData.inl"
