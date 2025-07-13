#pragma once

#include "AinimationObject.h"
#include "Item.h"

NS_BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
class CAnimController;
class CCollision_Component;
class CGameObject;
NS_END

NS_BEGIN(Client)

class CCharacterManager;
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

protected:
	CCharacter(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCharacter(const CCharacter& Prototype);
	virtual ~CCharacter() = default;

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual HRESULT		Initialize_Late() override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;

	virtual void		Attack() = 0;
	virtual	void		Damage(void* pArg) override;
	virtual	void		Hit() = 0;
	virtual void		SetDir();
	void				RenderAnimation(const wstring& animName, Entity& tEntity, vector<IMAGE_FOLDER_DESC>& tImageVec);

	void				ResetTarget(_float iDistance);
	virtual void		GetTarget(CGameObject* actor, _float distance);

	CGameObject*		GetNearTarget() { return m_pNearTarget; }
	_bool				GetNeedItem() { return	m_bNeedItem; }

	D3DXMATRIX			GetTorchAnimation(const wstring& animName, Entity& tEntity, vector<IMAGE_FOLDER_DESC>& tImageVec);
	CHARACTER_DATA*		Get_Char() { return m_pChar; }

protected:
	virtual void OverlapHitActor(CGameObject* HitActor, _float3& _Dir);
protected:
	MOVE_DIR			m_tMoveDIr = {};
	DamageBaseDesc		m_tDamage = {};
	_uint* m_pDate = {};
	_float* m_pTime = {};

	_bool				m_bAttack;
	wstring				m_sAnim;
	DIR					m_tDir = {};
	_float				m_fNearDistance = {};
	_float3				m_fPreMove = {};
	_float3				m_fMoving = {};
	_float				m_fAngle = {};
	CHARACTER_DATA*		m_pChar = {};
	_bool				m_bCol = {};
	_bool				m_bNeedItem = {};
	CGameObject*			m_pNearTarget = {};
	CCollision_Component*		m_pCollision_Com = { nullptr };
	CCharacterManager*			m_pCharacterManager = { nullptr };
public:
	HRESULT Ready_Components();
	virtual void Free() override;
};
NS_END