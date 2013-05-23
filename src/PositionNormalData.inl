
template<typename dataType>
PositionNormalData<dataType>::PositionNormalData()
{
}

template<typename dataType>
PositionNormalData<dataType>::PositionNormalData(PolyVox::Vector3DFloat positionToSet, dataType dataToSet)
  :position(positionToSet),
  data(dataToSet)
{
}

template<typename dataType>
PositionNormalData<dataType>::PositionNormalData(PolyVox::Vector3DFloat positionToSet, PolyVox::Vector3DFloat normalToSet, dataType dataToSet)
  :position(positionToSet),
  normal(normalToSet),
  data(dataToSet)
{

}

template<typename dataType>
const dataType& PositionNormalData<dataType>::getMaterial() const
{
  return data;
}

template<typename dataType>
const PolyVox::Vector3DFloat& PositionNormalData<dataType>::getNormal() const
{
  return normal;
}

template<typename dataType>
const PolyVox::Vector3DFloat& PositionNormalData<dataType>::getPosition() const
{
  return position;
}

template<typename dataType>
void PositionNormalData<dataType>::setNormal(const PolyVox::Vector3DFloat& normalToSet)
{
  normal = normalToSet;
}	

template<typename dataType>
void PositionNormalData<dataType>::setPosition(const PolyVox::Vector3DFloat& positionToSet)
{
  position = positionToSet;
}

template<typename dataType>
void PositionNormalData<dataType>::setMaterial(const dataType& dataToSet)
{
  data = dataToSet;
}
