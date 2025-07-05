#include "Client_Defines.h"
#include "UserInterface.h"

NS_BEGIN(Client)

class CMiniMap_Icon final : public CUserInterface
{
public:
	typedef struct Icon_Desc : public UIOBJECT_DESC
	{
		_uint	iTextureNum{};
		_float fRX{}, fRY{};
		_float fParentCX{}, fParentCY{};
	}ICON_DESC;
private:
	CMiniMap_Icon(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMiniMap_Icon(const CMiniMap_Icon& Prototype);
	virtual ~CMiniMap_Icon() = default;

public:
	virtual HRESULT  Initialize_Prototype()override;
	virtual HRESULT  Initialize(void* pArg)override;
	virtual void	 Priority_Update(_float fTimeDelta)override;
	virtual void	 Update(_float fTimeDelta)override;
	virtual void	 Late_Update(_float fTimeDelta)override;
	HRESULT	 Render();

	void Update_Position(CTransform* pTransform);
	void Update_Scale(_float fScale, CTransform* pTransform);

private:
	_uint	m_iTextureNum = {};
	_float	m_fRX{}, m_fRY{};
private:
	HRESULT ADD_Components();

public:
	static CMiniMap_Icon* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free() override;
};

NS_END
