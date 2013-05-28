
template<typename dataType>
PositionData<dataType>::PositionData()
{
}

template<typename dataType>
PositionData<dataType>::PositionData(PolyVox::Vector3DFloat positionToSet, dataType dataToSet)
  :position(positionToSet),
  data(dataToSet)
{
}


template<typename dataType>
const dataType& PositionData<dataType>::getMaterial() const
{
  return data;
}

template<typename dataType>
const PolyVox::Vector3DFloat& PositionData<dataType>::getPosition() const
{
  return position;
}

template<typename dataType>
void PositionData<dataType>::setPosition(const PolyVox::Vector3DFloat& positionToSet)
{
  position = positionToSet;
}

template<typename dataType>
void PositionData<dataType>::setMaterial(const dataType& dataToSet)
{
  data = dataToSet;
}
