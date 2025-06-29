#pragma once
#include "Client_Defines.h"
#include "Component.h"

NS_BEGIN(Engine)
class CGameObject;
NS_END

NS_BEGIN(Client)
class CDropItemComponent final : public CComponent
{
private :
	CDropItemComponent(LPDIRECT3DDEVICE9 pGraphic_Device);
	CDropItemComponent(const CDropItemComponent& rhs);
	virtual ~CDropItemComponent() = default;

public :
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

	HRESULT					ADD_ItemData(_uint iItemID, _float  fProbability);
	HRESULT					DropItem();

private :
	_uint							m_iDropCnt = { };
	vector<pair<_uint, _float>>		m_vecItemID = {};
	CGameObject*					m_Owner = {};

	vector<_uint>					m_DropItemVec = {};

private :
	HRESULT					ComputeDropItem();

public :
	static	CDropItemComponent*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual	CComponent*				Clone(void* pArg) override;
	virtual void					Free() override;

};
NS_END