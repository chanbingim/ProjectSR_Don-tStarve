#pragma once

#include "Client_Defines.h"
#include "AlphaObject.h"
#include "UserInterface.h"
#include "PlayerAnim.h"
#include "Character_Manager.h"


NS_BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
class CAnimController;
NS_END

NS_BEGIN(Client)

enum DIR {
	DOWN,
	SIDE,
	UP,
	DIR_END
};

enum MOVE_DIR {
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_UP,
	MOVE_END
};

class CCharacter : public CAlphaObject
{
protected:
	CCharacter(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCharacter(const CCharacter& Prototype);
	virtual ~CCharacter() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void		Attack() = 0;
	void		Get_Damage(_uint iAtk);
	void		SetDir();
protected:
	_uint			m_iMaxHp = {};
	_uint			m_iTemp = {};
	_uint			m_iAtk = {};
	_uint			m_iDef = {};
	_uint			m_iMaxHit = {};
	_float3			m_fMoving = {};
	_float			m_fAngle = {};
	_float			m_fCamDistance = {};
	MOVE_DIR			m_tMoveDIr = {};
	_uint			m_iDir = {};
	_int				m_iHp = {};
	_int				m_iHit = {};
	_bool			m_bAttack;
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CAnimController* m_pAnimController = { nullptr };
	CTransform* m_pAnimTransformCom = { nullptr };
	CCharacter_Manager* m_pCharacterInstance = { nullptr };
public:
	virtual void Free() override;
};
NS_END