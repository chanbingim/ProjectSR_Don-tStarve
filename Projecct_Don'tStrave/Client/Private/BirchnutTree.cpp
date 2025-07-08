#include "BirchnutTree.h"
#include "GameInstance.h"

#include "XML_Manager.h"
#include "DropItemComponent.h"

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
	XML_Instance->AddTexture("../Bin/Resources/Textures/Objects/Evergreen/evergreen_new.scml", L"../Bin/Resources/Textures/Objects/Evergreen/", &m_tImageVec);
	XML_Instance->LoadScml("../Bin/Resources/Textures/Objects/Evergreen/evergreen_new.scml", &m_tAnimation);

	return S_OK;
}

HRESULT CBirchnutTree::Initialize(void* pArg)
{
	if(FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_FrontName = TEXT("idle_");
	m_TailName = rand() % 2 == 1 ? TEXT("normal") : TEXT("short");

	m_EnviromentState = Enviornment_STATE::IDLE;
	m_pDropItem_Com->ADD_ItemData(38, 1);

	m_pDropItem_Com->SetCreateEffect(1);

	m_pCollision_Com->BindEnterFunction([&](CGameObject* HitActor, _float3& Dir) { BeginHitActor(HitActor, Dir); });
	m_pCollision_Com->BindOverlapFunction([&](CGameObject* HitActor, _float3& Dir) { OverlapHitActor(HitActor, Dir); });
	m_pCollision_Com->BindExitFunction([&](CGameObject* HitActor, _float3& Dir) { EndHitActor(HitActor, Dir); });

	m_EnviormentInfo.iMaxHit = 2;
	m_MaxRecoverTime = 3.0f;

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
	__super::Reset_State();
}

HRESULT CBirchnutTree::Render()
{
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

	/*Collision_Com*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_DropItem"),
		TEXT("Com_CollisionCom"), (CComponent**)&m_pDropItem_Com)))
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
