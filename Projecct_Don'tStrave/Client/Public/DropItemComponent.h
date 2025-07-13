#pragma once
#include "Client_Defines.h"
#include "Component.h"

NS_BEGIN(Engine)
class CGameObject;
NS_END

NS_BEGIN(Client)
class CItem_Manager;

class CDropItemComponent final : public CComponent
{
private :
	CDropItemComponent(LPDIRECT3DDEVICE9 pGraphic_Device);
	CDropItemComponent(const CDropItemComponent& rhs);
	virtual ~CDropItemComponent() = default;

public :
	virtual HRESULT					Initialize_Prototype() override;
	virtual HRESULT					Initialize(void* pArg) override;

	HRESULT							ADD_ItemData(_uint iItemID, _uint  iMaxCnt);
	HRESULT							DropItem(_uint iLevelID, const _wstring&	PrototypeTag, _uint iLayerID, const _wstring& LayerTag, _float3& Point);

	void							SetCreateEffect(_uint EffectType);

private :
	CItem_Manager*					m_pItem_Manager = { nullptr };
	_uint							m_iDropCnt = { };
	_uint							m_iCreateEffect = {};
	vector<pair<_uint, _uint>>		m_vecItemID = {};
	

public :
	static	CDropItemComponent*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual	CComponent*				Clone(void* pArg) override;
	virtual void					Free() override;

};
NS_END