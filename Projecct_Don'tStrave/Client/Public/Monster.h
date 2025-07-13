#pragma once

#include "Character.h"

NS_BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
class CCollision_Component;
NS_END

NS_BEGIN(Client)

class CMonster : public CCharacter
{
protected:
	CMonster(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMonster(const CMonster& Prototype);
	virtual ~CMonster() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT	Initialize_Late() override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual	void		SetDir() override;
	void				SetRandomMove();

	MONSTER_DATA*	Get_Monster();
	_bool Get_Active() { return m_bActive; }

protected:
	MONSTER_DATA*		m_pMonsterData = {};
	_float3				m_fMove = {};
	_float				m_fMoveTime = {};
	_float				m_fMoveStart = {};
	_float				m_fMoveDelay = {};
	_float				m_fAttackTime = {};
	_bool				m_bTarget = {};
	_bool				m_bActive = {};
public:
	virtual void Free() override;
};

NS_END