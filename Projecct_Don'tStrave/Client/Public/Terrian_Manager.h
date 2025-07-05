#pragma once
#include "Client_Defines.h"
#include "Base.h"

NS_BEGIN(Client)
class CTerrain;

class CTerrian_Manager final : public CBase
{
	DECLARE_SINGLETON(CTerrian_Manager);

private :
	CTerrian_Manager();
	virtual ~CTerrian_Manager() = default;

public :
	HRESULT				Initialize(const _float2& TileSize);
	CTerrain*			GetOnTerrian(_float3& Pos);
	vector<CTerrain*>*	GetTerrains();
	void				ADD_Terrian(CTerrain* pTerrian);
	void				Clear();
	
private :
	_float2						m_Size = {};
	vector<CTerrain*>			m_pTerrian = { nullptr };
	
public :
	virtual			void		Free() override;

};
NS_END