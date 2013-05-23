#pragma once

template<typename dataType>
class PositionNormalData
{
  public:
  PositionNormalData();
  PositionNormalData(PolyVox::Vector3DFloat positionToSet, dataType materialToSet);
  PositionNormalData(PolyVox::Vector3DFloat positionToSet, PolyVox::Vector3DFloat normal, dataType materialToSet);

  const dataType& getMaterial(void) const;
  const PolyVox::Vector3DFloat& getNormal(void) const;
  const PolyVox::Vector3DFloat& getPosition(void) const;	

  void setMaterial(const dataType & materialToSet);

  void setNormal(const PolyVox::Vector3DFloat& normalToSet);
  void setPosition(const PolyVox::Vector3DFloat& positionToSet);

  private:
    PolyVox::Vector3DFloat position;
    PolyVox::Vector3DFloat normal;
    dataType data;
};

#include "PositionNormalData.inl"
