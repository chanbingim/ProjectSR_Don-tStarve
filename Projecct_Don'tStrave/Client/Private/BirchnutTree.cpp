#include "BirchnutTree.h"

CBirchnutTree::CBirchnutTree(LPDIRECT3DDEVICE9 pGraphic_Device) :
	CTreeObject(pGraphic_Device)
{
}

CBirchnutTree::CBirchnutTree(const CBirchnutTree& rhs) : 
	CTreeObject(rhs)
{
}

HRESULT CBirchnutTree::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBirchnutTree::Initialize(void* pArg)
{
	if(FAILED(ADD_Components()))
		return E_FAIL;


	return S_OK;
}

void CBirchnutTree::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CBirchnutTree::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CBirchnutTree::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

void CBirchnutTree::Reset_State()
{

}

HRESULT CBirchnutTree::Render()
{
	return S_OK;
}

void CBirchnutTree::Damage(void* pArg)
{

}

void CBirchnutTree::Death()
{

}

HRESULT CBirchnutTree::ADD_Components()
{
	/*Transform Com*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/*VI_Buffer Com*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/*Collision_Com*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_SphereCollision"),
		TEXT("Com_CollisionCom"), (CComponent**)&m_pCollision_Com)))
		return E_FAIL;

	return S_OK;
}

void CBirchnutTree::BeginHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

void CBirchnutTree::OverlapHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

void CBirchnutTree::EndHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

CBirchnutTree* CBirchnutTree::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _wstring FolderName, const _wstring FilePath)
{
	CBirchnutTree* pInstance = new CBirchnutTree(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("CREATE FAIL : BIRCHNUT TREE");
	}

	return pInstance;
}

CGameObject* CBirchnutTree::Clone(void* pArg)
{
	CBirchnutTree* pInstance = new CBirchnutTree(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("CLONE FAIL : BIRCHNUT TREE");
	}

	return pInstance;
}

void CBirchnutTree::Free()
{
	__super::Free();

}
