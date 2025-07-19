#ifndef Engine_Struct_h__
#define Engine_Struct_h__

#include "Engine_Typedef.h"
#define D3DFVF_CUSTOMVERTEX (D3DFVF_TEX1) // 변경 했음

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
	
	struct PaticleAtrribute
	{
		_float3							m_Position = {};     // 월드스페이스 내 파티클 위치
		D3DXCOLOR						color = {};        // 파티클의 색

		_float							ParticleSize = {};
		_float2							TextureUV = { 0.5f, 0.5f };

		_float3							velocity = {};     // 초당 이동 속도
		_float3							acceleration = {}; // 초당 가속 속도

		_float							lifeTime = {};     // 파티클 생명시간
		_float							AliveTime = {};    // 파티클의 현재 나이
		
		D3DXCOLOR						colorFade = {};    // 파티클 색이 퇴색하는 방법
		_bool							m_isAlive = { true };      // 파티클 생존 여부
	};

	typedef	struct	tagVertexPariticle
	{
		D3DXVECTOR3			vPosition;
		D3DCOLOR    		Color;
	}VTXPOSPARTICLE;

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
		_int			iID;
	
	}BASE_DATA_STRUCT;


}


#endif // Engine_Struct_h__
