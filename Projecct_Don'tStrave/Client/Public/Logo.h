#include "Client_Defines.h"
#include "UserInterface.h"

NS_BEGIN(Client)

class CLogo final : public CUserInterface
{
private:
	CLogo(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLogo(const CLogo& Prototype);
	virtual ~CLogo() = default;

public:
	_uint Get_ID() { return m_iButtonIndex; }

public:
	virtual HRESULT  Initialize_Prototype()override;
	virtual HRESULT  Initialize(void* pArg)override;
	virtual void	 Priority_Update(_float fTimeDelta)override;
	virtual void	 Update(_float fTimeDelta)override;
	virtual void	 Late_Update(_float fTimeDelta)override;
	virtual HRESULT	 Render()override;

private:
	_uint		m_iButtonIndex = {};
	RECT		m_rcButton[4] = {};
	D3DXCOLOR	m_Color = {};
	CTexture*	m_pButton_Texture_Com = { nullptr };
	CTransform* m_pButton_Transform_Com = { nullptr };
	

private:
	HRESULT ADD_Components();

public:
	static CLogo* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free() override;
};

NS_END
