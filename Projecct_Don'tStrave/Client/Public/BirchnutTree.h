#pragma once
#include "TreeObject.h"

NS_BEGIN(Client)
class CTreeLeaf;

class CBirchnutTree : public CTreeObject
{
private:
	CBirchnutTree(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBirchnutTree(const CBirchnutTree& rhs);
	virtual ~CBirchnutTree() = default;

public:
	virtual HRESULT				Initialize_Prototype() override;
	virtual HRESULT				Initialize(void* pArg) override;
	virtual void				Priority_Update(_float fTimeDelta) override;
	virtual void				Update(_float fTimeDelta) override;
	virtual void				Late_Update(_float fTimeDelta) override;
	virtual	void				Reset_State() override;
	virtual HRESULT				Render() override;

	virtual void				Damage(void* pArg) override;
	virtual void				Death()override;

private :
	_int						m_iTreeType = {}; // 0 : Small 1 : Tall
	_float						m_fAliveTime = {};
	_float						m_fMaxAliveTime = {};
	_bool						m_bIsNoraml = { false };

	CTreeLeaf*					m_pTreeLeaf[2] = {nullptr, nullptr};

	Entity						m_tNormal_Animation = {};
	vector<IMAGE_FOLDER_DESC>	m_tNormal_ImageVec = {};

private:
	void				Update_GrowEnvent(_float fTimeDeleta);
	HRESULT				ADD_Components();

	void				BeginHitActor(CGameObject* HitActor, _float3& _Dir);
	void				OverlapHitActor(CGameObject* HitActor, _float3& _Dir);
	void				EndHitActor(CGameObject* HitActor, _float3& _Dir);

public:
	static		CBirchnutTree*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject*					Clone(void* pArg) override;
	void							Free() override;

};
NS_END