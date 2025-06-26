#pragma once
#include "Client_Defines.h"
#include "UserInterface.h"

NS_BEGIN(Client)

class CButton abstract : public CUserInterface
{
public:
	typedef struct Button_Desc
	{
		_float		fX{}, fY{}, fSizeX{}, fSizeY{};
		_float		fRelativeX = {};
		_float		fRelativeY = {};
		CTransform* pParentTransform = { nullptr };
	}BUTTON_DESC;
protected:
	CButton(LPDIRECT3DDEVICE9 pGraphic_Device);
	CButton(const CButton& Prototype);
	virtual ~CButton() = default;

public:
	virtual HRESULT  Initialize_Prototype()override;
	virtual HRESULT  Initialize(void* pArg)override;
	virtual void	 Priority_Update(_float fTimeDelta)override;
	virtual void	 Update(_float fTimeDelta)override;
	virtual void	 Late_Update(_float fTimeDelta)override;
	virtual HRESULT	 Render()override;

	_bool	OnClick() { return m_isClicked; };

protected:
	_bool		m_isClicked = {};
	_float		m_fRelativeX = {};
	_float		m_fRelativeY = {};
	CTransform* m_pParentTransform = { nullptr };

protected:
	virtual HRESULT ADD_Components();
	_bool isMouseOver();

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

NS_END
