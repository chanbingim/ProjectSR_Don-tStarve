#pragma once
#include "Client_Defines.h"
#include "Item.h"

NS_BEGIN(Engine);
class CState;
class CAnimController;
NS_END
NS_BEGIN(Client);

class CCampFire : public CItem
{
	enum class STATE {IDLE, DEAD, PREVIEW, PLACE};
private:
	CCampFire(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCampFire(const CCampFire& Prototype);
	virtual ~CCampFire() = default;

public:
	virtual HRESULT  Initialize_Prototype();
	virtual HRESULT  Initialize(void* pArg);
	virtual void	 Priority_Update(_float fTimeDelta);
	virtual void	 Update(_float fTimeDelta);
	virtual void	 Late_Update(_float fTimeDelta);
	virtual HRESULT	 Render();

	void HoverEvent();

private:
	_bool			m_bSoundPlay = {};
	class CFire* m_pFire = { nullptr };

	CCampFire::STATE	m_ePreState = {};
	CCampFire::STATE	m_eCurState = {};

private:
	HRESULT ADD_Components();
	void Change_State();

public:
	static CCampFire* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END
