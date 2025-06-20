#pragma once

#include "GameObject.h"

NS_BEGIN(Engine)
class CTransform;
class CVIBuffer;
class CTexture;

class ENGINE_DLL CUserInterface abstract : public CGameObject
{
public :
	typedef struct tagUIObject
	{
		_float		fX{}, fY{}, fSizeX{}, fSizeY{};
	}UIOBJECT_DESC;

protected:
	CUserInterface(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUserInterface(const CUserInterface& rhs);
	virtual ~CUserInterface() = default;

public:
	virtual HRESULT		Initialize_Prototype();
	virtual HRESULT		Initialize(void* pArg);
	virtual void		Priority_Update(_float fTimeDelta);
	virtual void		Update(_float fTimeDelta);
	virtual void		Late_Update(_float fTimeDelta);
	virtual HRESULT		Render();

	virtual	void		HoverEevent();
	virtual	void		ClickedEevent();

	void				UpdatePosition();

protected :
	_float					m_fX{}, m_fY{}, m_fSizeX{}, m_fSizeY{};
	_float					m_fWinSizeX{}, m_fWinSizeY{};

	CTransform*				m_pTransform_Com = { nullptr };
	CVIBuffer*				m_pVIBuffer_Com = { nullptr };
	CTexture*				m_pTexture_Com = { nullptr };

public :
	virtual CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;

};
NS_END