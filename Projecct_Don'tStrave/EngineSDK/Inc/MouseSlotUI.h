#pragma once
#include "UserInterface.h"

NS_BEGIN(Engine)
class ENGINE_DLL CMouseSlotUI abstract : public CUserInterface
{
protected :
	CMouseSlotUI(LPDIRECT3DDEVICE9  pGraphic_Device);
	CMouseSlotUI(const CMouseSlotUI& rhs);
	virtual ~CMouseSlotUI() = default;

public :
	virtual HRESULT		Initialize_Prototype();
	virtual HRESULT		Initialize(void* pArg);
	virtual void		Priority_Update(_float fTimeDelta);
	virtual void		Update(_float fTimeDelta);
	virtual void		Late_Update(_float fTimeDelta);
	virtual HRESULT		Render();

	HRESULT				UpdateSlotData(CGameObject* pBase, const _wstring& BaseVIBufferTag, const _wstring& BaseTextureTag);

protected :
	CVIBuffer*			m_pVISlotBuffer_Com = { nullptr };
	CTexture*			m_pSlotTexture_Com = { nullptr };

public :
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;

};
NS_END