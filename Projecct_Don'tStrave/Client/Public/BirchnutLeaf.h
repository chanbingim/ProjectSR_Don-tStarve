#pragma once
#include "Enviornment_Object.h"

NS_BEGIN(Client)
class CBirchnutLeaf : public CEnviornment_Object
{
protected:
	CBirchnutLeaf(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBirchnutLeaf(const CEnviornment_Object& rhs);
	virtual ~CBirchnutLeaf() = default;

public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize(void* pArg) override;
	virtual void			Priority_Update(_float fTimeDelta) override;
	virtual void			Update(_float fTimeDelta) override;
	virtual void			Late_Update(_float fTimeDelta) override;
	virtual	void			Reset_State();
	virtual HRESULT			Render() override;

private :
	HRESULT					ADD_Components();

public:
	static		CBirchnutLeaf*		Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _wstring FolderName, const _wstring FilePath);
	CGameObject*					Clone(void* pArg) override;
	void							Free() override;
};
NS_END