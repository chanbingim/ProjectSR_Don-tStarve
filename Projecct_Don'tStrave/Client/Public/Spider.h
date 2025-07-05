#pragma once

#include "Client_Defines.h"
#include "Monster.h"

NS_BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
class CCollision_Component;
NS_END

NS_BEGIN(Client)
class CSpiderHouse;
class CSpider : public CMonster
{
protected:
	CSpider(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSpider(const CSpider& Prototype);
	virtual ~CSpider() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT	Initialize_Late() override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void		OutHouse() = 0;

	virtual void Damage(void* pArg) override;
protected:
	_bool					m_bOutHouse = {};
	CSpiderHouse*			m_pHouse = {};
public:
	virtual void Free() override;
};

NS_END