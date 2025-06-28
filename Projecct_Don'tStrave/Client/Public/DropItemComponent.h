#pragma once
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


private :
	vector<pair<_uint, _float>>		m_iItemID = {};
	CGameObject*					m_Owner = {};

public :
	static	CDropItemComponent*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual	CComponent*				Clone(void* pArg) override;
	virtual void					Free() override;

};
NS_END