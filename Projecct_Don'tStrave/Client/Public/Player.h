#pragma once

#include "Client_Defines.h"
#include "LandObject.h"
#include "UserInterface.h"
#include "PlayerAnim.h"
#include "Character.h"
#include "Health.h"
#include "Hunger.h"

NS_BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
class CAnimController;
class CCollision_Component;
NS_END

NS_BEGIN(Client)

enum SWAPOBJECT {
	SWAPOBJECT_NONE,
	SWAPOBJECT_AXE,
	SWAPOBJECT_GOLDAXE,
	SWAPOBJECT_PICKAXE,
	SWAPOBJECT_GOLDPICKAXE,
	SWAPOBJECT_SHOVEL,
	SWAPOBJECT_GOLDSHOVEL,
	SWAPOBJECT_TORCH,
	SWAPOBJECT_END
};
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
	HRESULT			SetAnimation(_uint i, DIR dir, MOTION motion);
	HRESULT			AddAnimation(_uint i, DIR dir, MOTION motion);
	virtual void Damage() override;
	virtual void Attack() override;
	virtual void Death() override;
	_uint		Get_Hp();
	_uint		Get_Hunger();
	void			SetItem(SWAPOBJECT tItem);
	_bool		IsGhost() { return m_bIsGhost; }
private:
	CTexture*				m_pTextureCom[2][DIR::DIR_END][MOTION::MOTION_END] = {nullptr};
	CPlayerAnim*			m_pPlayerAnim[2][DIR::DIR_END][MOTION::MOTION_END] = {nullptr};
	CTransform*				m_pSwapObjectTransformCom = { nullptr };
	CAnimController*		m_pSwapObjectAnimController = { nullptr };
	CTexture*				m_pSwapObjectTextureCom[SWAPOBJECT_END][DIR::DIR_END][MOTION::MOTION_END] = {nullptr};
<<<<<<< HEAD
	CPlayerAnim*			m_pSwapObjectPlayerAnim[SWAPOBJECT_END][DIR::DIR_END][MOTION::MOTION_END] = { nullptr };
	CCollision_Component*	m_pCollision_Com = { nullptr };
=======
	CPlayerAnim*				m_pSwapObjectPlayerAnim[SWAPOBJECT_END][DIR::DIR_END][MOTION::MOTION_END] = { nullptr };
	CGameObject*				m_pWorkObject = { nullptr };
	CCollision_Component	*	m_pCollision_Com = { nullptr };
>>>>>>> origin/0628_kjh
	MOTION					m_tMotion = {};
	DIR						m_tDir = {};
	SWAPOBJECT				m_tItem = {};
	_uint					m_iDirection = {};
	_uint					m_iSwapObject = {};
	_uint					m_iHunger = {};
	_bool					m_bControll = {};
	_bool					m_bIsGhost = {};
	_bool					m_bAttack = {};
private:
	HRESULT Ready_Components();
	HRESULT Begin_RenderState();
	HRESULT End_RenderState();

	void BeginHitActor(CGameObject* HitActor, _float3& _Dir);
	void OverlapHitActor(CGameObject* HitActor, _float3& _Dir);
	void EndHitActor(CGameObject* HitActor, _float3& _Dir);
public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END