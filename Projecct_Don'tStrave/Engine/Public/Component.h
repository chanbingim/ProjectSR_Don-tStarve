#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class ENGINE_DLL CComponent abstract : public CBase
{
protected:
	CComponent() = default;
	CComponent(LPDIRECT3DDEVICE9 pGraphic_Device);
	CComponent(const CComponent& Prototype);
	virtual ~CComponent() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

protected:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = { nullptr };
	class CGameInstance* m_pGameInstance = { nullptr };
	_bool					m_isCloned = { false };

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free();

};

NS_END