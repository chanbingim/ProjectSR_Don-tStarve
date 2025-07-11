#include "Client_Defines.h"
#include "UserInterface.h"

NS_BEGIN(Client)

class CMiniMap final : public CUserInterface
{
private:
	CMiniMap(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMiniMap(const CMiniMap& Prototype);
	virtual ~CMiniMap() = default;

public:
	virtual HRESULT  Initialize_Prototype()override;
	virtual HRESULT  Initialize(void* pArg)override;
	virtual void	 Priority_Update(_float fTimeDelta)override;
	virtual void	 Update(_float fTimeDelta)override;
	virtual void	 Late_Update(_float fTimeDelta)override;
	virtual HRESULT	 Render()override;

private:
	_bool					m_IsVisible = {};
	_float					m_fNumTileX = {};
	_float					m_fNumTileY = {};
	_float					m_fZoom = {};
	UIOBJECT_DESC			m_Panel_Desc = {};
	_float3					m_vKeyDownPos = {};

	CTransform*				m_pPanel_Transform_Com = { nullptr };
	CVIBuffer*				m_pPanel_VIBuffer_Com = { nullptr };

	class CMiniMap_Button*	m_pMiniMap_Btn = { nullptr };
	vector<class CMiniMap_Icon*>	m_Icons = {};

private:
	HRESULT ADD_Components();
	HRESULT Update_Objects();

public:
	static CMiniMap* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free() override;
};

NS_END