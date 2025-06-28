#ifndef Engine_Struct_h__
#define Engine_Struct_h__

#include "Engine_Typedef.h"

namespace Engine
{
	typedef struct tagEngineDesc
	{
		HWND				hWnd;
		unsigned int		iNumLevels;
		unsigned int		iWinSizeX, iWinSizeY;
		WINMODE				eWindowMode;
	}ENGINE_DESC;

	/*D3DDECLUSAGE*/
	typedef struct tagVertexPositionMesh
	{
		D3DXVECTOR3			vPosition;
		D3DXVECTOR3			vNormal;

	}VTXPOSMESH;

	/*D3DDECLUSAGE*/
	typedef struct tagVertexPositionTexcoord
	{
		D3DXVECTOR3			vPosition;
		D3DXVECTOR2			vTexcoord;
	}VTXPOSTEX;
	
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
}


#endif // Engine_Struct_h__
