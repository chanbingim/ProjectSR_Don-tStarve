#pragma once

#include "Client_Defines.h"
#include "LandObject.h"
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
	MONSTER_DATA Get_Monster();

protected:
	CCharacter* m_pTarget = {};
	_uint		m_iId = {};
	_float		m_iAtkDistance = {};
	_uint		m_iAtkSpeed = {};

public:
	virtual void Free() override;
};

NS_END