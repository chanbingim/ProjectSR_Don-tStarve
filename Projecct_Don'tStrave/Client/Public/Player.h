#pragma once

#include "Character.h"
#include "TorchFire.h"
NS_BEGIN(Client)
class CSlotFrame;
class CSlot;
class CPlayer final : public CCharacter
{
	enum MOTION {
		BUCKED,
		BUCK_PST,
		IDLE,
		IDLE_TO_RUN,
		ITEM_IN,
		ITEM_OUT,
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
		IDLE_TO_ATTACK,
		ATTACK,
		IDLE_TO_SPEAR,
		SPEAR,
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
	virtual HRESULT Initialize_Late() override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual void SetDir() override;
	virtual HRESULT Render() override;

	virtual void Damage(void* pArg);
	virtual void Hit() override;
	virtual void Attack() override;
	virtual void Death() override;
	void Dead();
	void ChargeAttack();
	HRESULT			SetAnimation(DIR dir, MOTION motion);
	PLAYER_DATA*		Get_Player();
	void				SetItem(SWAPOBJECT tItem);
	_bool			Eat(void* pArg);
	void				LightningAttack(_float3 fAttack,_float fPower);
	HRESULT				Set_EquipmentSlot(CSlotFrame* pSlotFrame);

	void				MakeItem(_wstring prototype, ITEM_DESC itemDesc);
	void				MakeMaterialItem(CSlot* slot, ITEM_DESC itemDesc);

private:
	MOTION					m_tMotion = {};
	_bool					m_bControll = {};
	_bool					m_bAttack = {};
	_bool					m_bLightningAttack = {};
	_bool					m_bCrawling = {};
	_bool					m_bTerrorbeak = {};
	_bool					m_bEat = {};
	_float					m_fHungTime = {};
	_float					m_fFightTime = {};
	_float3					m_fLightning = {};
	_int						m_iDarkTime = {};
	_int						m_iHealthChange = {};
	_int						m_iSanityChange = {};
	_int						m_iHungerChange = {};
	_wstring					m_sItem = {};
	ITEM_DESC				m_tItem = {};
	vector<IMAGE_FOLDER_DESC>	m_tGhostImageVec = {};
	vector<IMAGE_FOLDER_DESC>	m_tWigfridImageVec = {};
	vector<IMAGE_FOLDER_DESC>	m_tWigfridGhostImageVec = {};
	vector<IMAGE_FOLDER_DESC>	m_tItemImageVec[ENUM_CLASS(SWAPOBJECT::NONE)] = {};
	vector<IMAGE_FOLDER_DESC>	m_tMakeImageVec = {};
	Entity						m_tWigfridAnimation = {};
	Entity						m_tItemAnimation = {};
	Entity						m_tMakeAnimation = {};
	CTorchFire*					m_pTorchFire = {};
	PLAYER_DATA*				m_pPlayer = {};

	CSlotFrame*					m_pEquipment_Slot = { nullptr };
	CSlot*					m_pSlot = {};
	SWAPOBJECT				m_tSwapItem;
	CGameObject*				m_pTarget = { nullptr };
private:

	void BeginHitActor(CGameObject* HitActor, _float3& _Dir);
	virtual void OverlapHitActor(CGameObject* HitActor, _float3& _Dir) override;
	void EndHitActor(CGameObject* HitActor, _float3& _Dir);
public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END