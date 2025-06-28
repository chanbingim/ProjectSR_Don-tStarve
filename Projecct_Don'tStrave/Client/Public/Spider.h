#pragma once
#include "Monster.h"

NS_BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
class CAnimController;
NS_END

NS_BEGIN(Client)

class CSpider : public CMonster
{
	enum MOTION {
		IDLE,
		IDLE_TO_RUN,
		RUN,
		RUN_TO_IDLE,
		ATTACK,
		IDLE_TO_SLEEP,
		SLEEP,
		SLEEP_TO_IDLE,
		IDLE_TO_EAT,
		EAT,
		EAT_TO_IDLE,
		DAMAGE,
		IDLE_TO_COWER,
		COWER,
		COWER_TO_IDLE,
		TAUNT,
		DEATH,
		MOTION_END
	};
private:
	CSpider(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSpider(const CSpider& Prototype);
	virtual ~CSpider() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	HRESULT			SetAnimation(DIR dir, MOTION motion);
	HRESULT			AddAnimation(DIR dir, MOTION motion);
	virtual void Damage() override;
	virtual void Attack() override;
	virtual void Death() override;
private:
	CTexture*				m_pTextureCom[DIR::DIR_END][MOTION::MOTION_END] = { nullptr };
	CPlayerAnim*				m_pSpiderAnim[DIR::DIR_END][MOTION::MOTION_END] = { nullptr };
	MOTION					m_tMotion = {};
	DIR						m_tDir = {};
	_bool					m_bMove = {};
private:
	HRESULT Ready_Components();
	HRESULT Begin_RenderState();
	HRESULT End_RenderState();
public:
	static CSpider* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END