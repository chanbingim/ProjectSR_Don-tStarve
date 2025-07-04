#pragma once

#include "Client_Defines.h"
#include "AinimationObject.h"
#include "UserInterface.h"
#include "PlayerAnim.h"

#include "Character_Manager.h"

NS_BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
class CAnimController;
class CCollision_Component;
NS_END

NS_BEGIN(Client)

class CCharacter_Manager;
class CTerrian_Manager;

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

class CCharacter : public CAinimationObject
{
	typedef struct Character_desc {
		_uint			m_iMaxHp = {};
		_uint			m_iTemp = {};
		_uint			m_iAtk = {};
		_uint			m_iDef = {};
		_uint			m_iMaxHit = {};
	}CHARACTER_DESC;

protected:
	CCharacter(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCharacter(const CCharacter& Prototype);
	virtual ~CCharacter() = default;

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;

	virtual void		Attack() = 0;
	virtual void		Get_Damage(_uint iAtk);
	void				SetDir();

	void				RenderAnimation(const wstring& animName);

protected:
	_uint			m_iMaxHp = {};
	_uint			m_iTemp = {};
	_uint			m_iAtk = {};
	_float			m_fSpeed = {};
	_uint			m_iMaxHit = {};
	_float3			m_fMoving = {};
	_float			m_fAngle = {};
	_float			m_fCamDistance = {};
	MOVE_DIR		m_tMoveDIr = {};
	DIR				m_tDir = {};
	_uint			m_iDir = {};
	_int			m_iHp = {};
	_int			m_iHit = {};
	_bool			m_bAttack;
	wstring			m_sAnim;


	CCharacter_Manager*			m_pCharacterInstance = { nullptr };
	CCollision_Component*		m_pCollision_Com = { nullptr };

public:
	HRESULT Ready_Components();
	virtual void Free() override;
};
NS_END