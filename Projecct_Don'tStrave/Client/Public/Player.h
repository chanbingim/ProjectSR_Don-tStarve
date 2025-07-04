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
	HRESULT			SetAnimation(DIR dir, MOTION motion);
	virtual void Damage() override;
	virtual void Attack() override;
	virtual void Death() override;
	virtual void		Get_Damage(_uint iAtk) override;
	PLAYER_DATA*		Get_Player();
	void			SetItem(SWAPOBJECT tItem);
	_bool		IsGhost() { return m_bIsGhost; }
private:
	MOTION					m_tMotion = {};
	_bool					m_bControll = {};
	_bool					m_bIsGhost = {};
	_bool					m_bAttack = {};
	PLAYER_DATA*				m_pPlayer = {};
private:
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