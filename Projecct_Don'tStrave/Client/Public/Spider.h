#pragma once
#include "Monster.h"

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
	virtual void		OutHouse(CCharacter* pCharacter);

	virtual void Damage(void* pArg) override;
protected:
	CSpiderHouse*			m_pHouse = {};
	_bool					m_bHouse = {};
public:
	virtual void Free() override;
};

NS_END