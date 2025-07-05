#pragma once
#include "Client_Defines.h"
#include "UserInterface.h"

NS_BEGIN(Client)

class CLodingSpinner;

class CLodingInterface : public CUserInterface
{
private :
	CLodingInterface(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLodingInterface(const CLodingInterface& rhs);
	virtual ~CLodingInterface() = default;

public :
	virtual HRESULT		Initialize_Prototype();
	virtual HRESULT		Initialize(void* pArg);
	virtual void		Priority_Update(_float fTimeDelta);
	virtual void		Update(_float fTimeDelta);
	virtual void		Late_Update(_float fTimeDelta);
	virtual HRESULT		Render();


private :
	CLodingSpinner*		m_pSinner = { nullptr };

private :
	HRESULT				ADD_Components();

public :
	static	CLodingInterface*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CUserInterface*		Clone(void* pArg) override;
	virtual void				Free() override;

};
NS_END