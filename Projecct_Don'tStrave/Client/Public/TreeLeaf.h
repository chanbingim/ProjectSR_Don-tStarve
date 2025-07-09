#pragma once
#include "Enviornment_Object.h"

NS_BEGIN(Client)
class CEnviornment_Object;

class CTreeLeaf : public CEnviornment_Object
{
public :
	typedef struct Leaf_Desc : CLandObject::LANDOBJECT_DESC
	{
		CEnviornment_Object*		pOwner;
		const WCHAR*				szFrontName;
		const WCHAR*				szTailName;
	}LEAF_DESC;

protected:
	CTreeLeaf(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTreeLeaf(const CEnviornment_Object& rhs);
	virtual ~CTreeLeaf() = default;

public:
	virtual HRESULT					Initialize_Prototype(const char* XMLFilePath, const _wstring FolderName);
	virtual HRESULT					Initialize(void* pArg) override;
	virtual void					Priority_Update(_float fTimeDelta) override;
	virtual void					Update(_float fTimeDelta) override;
	virtual void					Late_Update(_float fTimeDelta) override;
	virtual	void					Reset_State();
	virtual HRESULT					Render() override;

private :
	HRESULT							ADD_Components();

private :
	CEnviornment_Object*			m_pOwner = { nullptr };

public:
	static		CTreeLeaf*			Create(LPDIRECT3DDEVICE9 pGraphic_Device, const char* XMLFilePath, const _wstring FolderName);
	CGameObject*					Clone(void* pArg) override;
	void							Free() override;
};
NS_END