#pragma once

#include "Character.h"

NS_BEGIN(Client)

class CPlayer final : public CCharacter
{
	enum MOTION {
		BUCKED,
		BUCK_PST,
		IDLE,
		IDLE_TO_RUN,
		RUN,
		RUN_TO_IDLE,
		DIAL,
		IDLE_TO_BUILD,
		BUILD,
		BUILD_TO_IDLE,
		HUNGRY,
		EAT,
		FASTEAT,
		IDLE_TO_AXE,
		AXE,
		IDLE_TO_PICKAXE,
		PICKAXE,
		PICKAXE_TO_IDLE,
		IDLE_TO_SHOVEL,
		SHOVEL,
		SHOVEL_TO_IDLE,
		ATTACK,
		PICKUP,
		GIVE,
		DAMAGE,
		DEATH1,
		DEATH2,
		WAKEUP,
		GHOST_APPEAR,
		GHOST_IDLE,
		GHOST_DISSIPATE,
		MOTION_END
	};

private:
	CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPlayer(const CPlayer& Prototype);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void Hit() override;
	virtual void Attack() override;
	virtual void Death() override;
	void Dead();
	HRESULT			SetAnimation(DIR dir, MOTION motion);
	PLAYER_DATA*		Get_Player();
	void				SetItem(SWAPOBJECT tItem);
	void				Eat(void* pArg);
	_bool			IsGhost() { return m_bIsGhost; }
private:
	MOTION					m_tMotion = {};
	_bool					m_bControll = {};
	_bool					m_bIsGhost = {};
	_bool					m_bAttack = {};
	_float					m_fHungTime = {};
	_int						m_iHealthChange = {};
	_int						m_iSanityChange = {};
	_int						m_iHungerChange = {};
	vector<IMAGE_FOLDER_DESC>	m_tGhostImageVec = {};
	vector<IMAGE_FOLDER_DESC>	m_tWigfridImageVec = {};
	vector<IMAGE_FOLDER_DESC>	m_tWigfridGhostImageVec = {};
	vector<IMAGE_FOLDER_DESC>	m_tItemImageVec[ENUM_CLASS(SWAPOBJECT::NONE)] = {};
	vector<IMAGE_FOLDER_DESC>	m_tMakeImageVec = {};
	Entity						m_tWigfridAnimation = {};
	Entity						m_tItemAnimation = {};
	Entity						m_tMakeAnimation = {};
	PLAYER_DATA*				m_pPlayer = {};
private:

	void BeginHitActor(CGameObject* HitActor, _float3& _Dir);
	void OverlapHitActor(CGameObject* HitActor, _float3& _Dir);
	void EndHitActor(CGameObject* HitActor, _float3& _Dir);
public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END