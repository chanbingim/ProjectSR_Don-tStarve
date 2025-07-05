#pragma once

#include "Client_Defines.h"
#include "UserInterface.h"

NS_BEGIN(Client)
class CLodingSpinner final : public CUserInterface
{
public :
	typedef struct Spinner_Desc
	{
		_float2				Center = {};
		_float				fSpinnerDis = {};
		_float				fSpinnerSpeed = {};
	}SPINNER_DESC;

private:
	CLodingSpinner(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLodingSpinner() = default;

public:
	virtual HRESULT		Initialize(void* pArg);
	virtual void		Priority_Update(_float fTimeDelta);
	virtual void		Update(_float fTimeDelta);
	virtual void		Late_Update(_float fTimeDelta);
	virtual HRESULT		Render();

private:
	HRESULT				ADD_Components();

private : 
	_bool				m_IsLoadingFinish = { false };

	_float2				m_vSpinnerCenter = { };
	_float				m_fSpinnerDis = {};
	_float				m_fAngle = {};
	_float				m_fSpinnerSpeed = {};

public:
	static	CLodingSpinner*		Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg);
	virtual void				Free() override;
};
NS_END