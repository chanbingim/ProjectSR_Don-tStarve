#pragma once
#include "Engine_Defines.h"

typedef struct BaseData
{
	// 기본 정점 데이터
	vector<VTXPOSTEX>	vecVertices;

	// 월드 변환 데이터
	_float3			Scale;
	_float3			Rotation;
	_float3			Position;

	//텍스쳐 경로
	string			szTexturePath;
}BASE_DATA_STRUCT;