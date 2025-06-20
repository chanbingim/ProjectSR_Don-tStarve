#pragma once
#include "Engine_Defines.h"

typedef struct BaseData
{
	// �⺻ ���� ������
	vector<VTXPOSTEX>	vecVertices;

	// ���� ��ȯ ������
	_float3			Scale;
	_float3			Rotation;
	_float3			Position;

	//�ؽ��� ���
	string			szTexturePath;
}BASE_DATA_STRUCT;