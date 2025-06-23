#pragma once

#include "Client_Defines.h"
#include "LandObject.h"
#include "UserInterface.h"
#include "PlayerAnim.h"
#include "SwapObject.h"

NS_BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
class CAnimController;
NS_END

NS_BEGIN(Client)

enum class MOTION {
	IDLE,
	IDLE_TO_RUN,
	RUN,
	RUN_TO_IDLE,
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
	END };
class CPlayer final : public CLandObject
{
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

private:
	CTransform*				m_pTransformCom = {nullptr};
	CVIBuffer_Rect*			m_pVIBufferCom = {nullptr};
	CAnimController*			m_pAnimController = {nullptr};
	CTexture*				m_pTextureCom[100] = {nullptr};
	CPlayerAnim*				m_pPlayerAnim[100] = {nullptr};
	CTransform*				m_pSwapObjectTransformCom = { nullptr };
	CVIBuffer_Rect*			m_pSwapObjectVIBufferCom = { nullptr };
	CAnimController*			m_pSwapObjectAnimController = { nullptr };
	CTexture*				m_pSwapObjectTextureCom[10] = {nullptr};
	CSwapObject*				m_pSwapObjectPlayerAnim[10] = { nullptr };
	_uint					m_iMotion = {};
	_uint					m_iSwapObject = {};
private:
	HRESULT Ready_Components();
	HRESULT Begin_RenderState();
	HRESULT End_RenderState();


public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END