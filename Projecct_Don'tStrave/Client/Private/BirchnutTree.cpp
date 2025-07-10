#include "BirchnutTree.h"
#include "GameInstance.h"

#include "XML_Manager.h"
#include "DropItemComponent.h"
#include "TreeLeaf.h"

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
	auto XML_Instance = CXML_Manager::GetInstance();

	XML_Instance->AddTexture("../Bin/Resources/Textures/Objects//Birchnut/trunk/tree_leaf_trunk_build.scml",
							 L"../Bin/Resources/Textures/Objects//Birchnut/trunk/", &m_tImageVec);

	XML_Instance->LoadScml("../Bin/Resources/Textures/Objects//Birchnut/trunk/tree_leaf_trunk_build.scml", &m_tAnimation);

	return S_OK;
}

HRESULT CBirchnutTree::Initialize(void* pArg)
{
	if(FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(CEnviornment_Object::Initialize(pArg)))
		return E_FAIL;

	m_FrontName = TEXT("idle_");
	m_TailName = rand() % 2 == 1 ? TEXT("tall") : TEXT("short");

	LoadImageFile();
	
	CTreeLeaf::LEAF_DESC TreeDesc = {};
	TreeDesc.pOwner = this;
	TreeDesc.szFrontName = m_FrontName.c_str();
	TreeDesc.szTailName = m_TailName;
	
 	m_pTreeLeaf = static_cast<CTreeLeaf *>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY_STATIC),
					TEXT("Prototype_GameObject_Birchnut_Leaf"), &TreeDesc));
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
	m_pTreeLeaf->Late_Update(fTimeDelta);
}

void CBirchnutTree::Reset_State()
{
	__super::Reset_State();
}

HRESULT CBirchnutTree::Render()
{
	__super::Render();
	m_pTreeLeaf->Render();
	return S_OK;
}

void CBirchnutTree::Damage(void* pArg)
{
	__super::Damage(pArg);
}

void CBirchnutTree::Death()
{

}

HRESULT CBirchnutTree::ADD_Components()
{
	/*Transform Com*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/*VI_Buffer Com*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	CCollision_Component::COL_DESC ColDesc = {};
	ColDesc.pOwner = this;
	/*Collision_Com*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_SphereCollision"),
		TEXT("Com_CollisionCom"), (CComponent**)&m_pCollision_Com, &ColDesc)))
		return E_FAIL;

	/*Collision_Com*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_DropItem"),
		TEXT("Com_DropItem"), (CComponent**)&m_pDropItem_Com)))
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

CBirchnutTree* CBirchnutTree::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
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
	Safe_Release(m_pTreeLeaf);
}
