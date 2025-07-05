#pragma once
#include "Enviornment_Object.h"

NS_BEGIN(Client)
class CDropItemComponent;

class CDropItemEnviornment : public CEnviornment_Object
{
protected:
	CDropItemEnviornment(LPDIRECT3DDEVICE9 pGraphic_Device);
	CDropItemEnviornment(const CDropItemEnviornment& rhs);
	virtual ~CDropItemEnviornment() = default;

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;

	virtual HRESULT		Render() override;
	HRESULT				CreateDropItem( const _float3& Pos);

protected :
	CDropItemComponent* m_pDropItem_Com = { nullptr };
public:
	CGameObject*		Clone(void* pArg) override;
	void				Free() override;
};
NS_END