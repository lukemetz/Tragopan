/*******************************************************************************
Copyright (c) 2005-2009 David Williams

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

    3. This notice may not be removed or altered from any source
    distribution. 	
*******************************************************************************/

#ifndef __PolyVox_ModifiedCubicSurfaceExtractorWithNormals_H__
#define __PolyVox_ModifiedCubicSurfaceExtractorWithNormals_H__
//Changed to modified
#include "ModifiedIsQuadNeeded.h"

#include <PolyVoxCore/Array.h>
#include <PolyVoxCore/SurfaceMesh.h>

namespace PolyVox
{
  //Modified added MeshType
	template<typename VolumeType, typename MeshType, typename IsQuadNeeded = ModifiedIsQuadNeeded<typename VolumeType::VoxelType, MeshType> >
	class ModifiedCubicSurfaceExtractorWithNormals
	{
	public:
		ModifiedCubicSurfaceExtractorWithNormals(VolumeType* volData, Region region, SurfaceMesh<PositionNormalData<MeshType>>* result, IsQuadNeeded isQuadNeeded = IsQuadNeeded());

		void execute();

	private:
		IsQuadNeeded m_funcIsQuadNeededCallback;

		//The volume data and a sampler to access it.
		VolumeType* m_volData;
		typename VolumeType::Sampler m_sampVolume;

		//The surface patch we are currently filling.
    //Modification, added meshType
		SurfaceMesh<PositionNormalData<MeshType>>* m_meshCurrent;

		//Information about the region we are currently processing
		Region m_regSizeInVoxels;
	};
}

#include "ModifiedCubicSurfaceExtractorWithNormals.inl"

#endif
