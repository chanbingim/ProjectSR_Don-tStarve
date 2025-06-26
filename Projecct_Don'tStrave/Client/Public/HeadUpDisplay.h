#pragma once
#include "Client_Defines.h"
#include "UserInterface.h"

NS_BEGIN(Client)

class CHeadUpDisplay abstract : public CUserInterface
{
protected:
	CHeadUpDisplay(LPDIRECT3DDEVICE9 pGraphic_Device);
	CHeadUpDisplay(const CHeadUpDisplay& Prototype);
	virtual ~CHeadUpDisplay() = default;

public:
	virtual HRESULT  Initialize_Prototype()override;
	virtual HRESULT  Initialize(void* pArg)override;
	virtual void	 Priority_Update(_float fTimeDelta)override;
	virtual void	 Update(_float fTimeDelta)override;
	virtual void	 Late_Update(_float fTimeDelta)override;
	virtual HRESULT	 Render()override;

protected:
	_uint	m_iTextureIndex = {};
	_float	m_fTimeAcc = {};
	class CPlayer* m_pPlayer = {}; // Player_Info·Î ¼öÁ¤

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;

};

NS_END

