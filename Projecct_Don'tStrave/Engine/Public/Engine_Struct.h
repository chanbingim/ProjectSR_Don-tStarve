#ifndef Engine_Struct_h__
#define Engine_Struct_h__

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
	
}


#endif // Engine_Struct_h__
