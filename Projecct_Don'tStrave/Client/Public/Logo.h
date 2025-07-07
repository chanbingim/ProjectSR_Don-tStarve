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
	virtual HRESULT  Initialize_Prototype()override;
	virtual HRESULT  Initialize(void* pArg)override;
	virtual void	 Priority_Update(_float fTimeDelta)override;
	virtual void	 Update(_float fTimeDelta)override;
	virtual void	 Late_Update(_float fTimeDelta)override;
	virtual HRESULT	 Render()override;


private:
	HRESULT ADD_Components();

public:
	static CLogo* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free() override;
};

NS_END
