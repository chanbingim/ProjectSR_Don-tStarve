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
	CTreeObject(rhs),
	m_tNormal_ImageVec{ rhs.m_tNormal_ImageVec },
	m_tNormal_Animation{ rhs.m_tNormal_Animation }
{
}

HRESULT CBirchnutTree::Initialize_Prototype()
{
	auto XML_Instance = CXML_Manager::GetInstance();

	XML_Instance->AddTexture("../Bin/Resources/Textures/Objects//Birchnut/trunk/tree_leaf_trunk_build.scml",
							 L"../Bin/Resources/Textures/Objects//Birchnut/trunk/", &m_tImageVec);

	XML_Instance->LoadScml("../Bin/Resources/Textures/Objects//Birchnut/trunk/tree_leaf_trunk_build.scml", &m_tAnimation);

	XML_Instance->AddTexture("../Bin/Resources/Textures/Objects//Birchnut/normal_trunk/tree_leaf_trunk_build.scml",
							  L"../Bin/Resources/Textures/Objects//Birchnut/normal_trunk/", &m_tNormal_ImageVec);

	XML_Instance->LoadScml("../Bin/Resources/Textures/Objects//Birchnut/normal_trunk/tree_leaf_trunk_build.scml", &m_tNormal_Animation);

	return S_OK;
}

HRESULT CBirchnutTree::Initialize(void* pArg)
{
	if(FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(CEnviornment_Object::Initialize(pArg)))
		return E_FAIL;

	m_iTreeType = rand() % 2;
	m_FrontName = TEXT("idle_");
	m_TailName = m_iTreeType == 1 ? TEXT("tall") : TEXT("normal");
	
	CTreeLeaf::LEAF_DESC TreeDesc = {};
	TreeDesc.pOwner = this;
	TreeDesc.szFrontName = m_FrontName.c_str();
	TreeDesc.szTailName = m_TailName.c_str();

	LoadImageFile();
	m_pTreeLeaf[1] = static_cast<CTreeLeaf*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY_STATIC),
		TEXT("Prototype_GameObject_Birchnut_tall_Leaf"), &TreeDesc));

	LoadImageFile(&m_tNormal_ImageVec);
	m_pTreeLeaf[0] = static_cast<CTreeLeaf*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY_STATIC),
		TEXT("Prototype_GameObject_Birchnut_Normal_Leaf"), &TreeDesc));
	
	m_pDropItem_Com->ADD_ItemData(38, 1);

	m_pDropItem_Com->SetCreateEffect(1);
	m_EnviormentInfo.iMaxHit = 2;
	m_MaxRecoverTime = 3.0f;
	m_fMaxAliveTime = 3.0f;
	m_fAliveTime = 0;
	return S_OK;
}

void CBirchnutTree::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
	m_pTreeLeaf[m_iTreeType]->Priority_Update(fTimeDelta);
}

void CBirchnutTree::Update(_float fTimeDelta)
{
	CEnviornment_Object::Update(fTimeDelta);

	Update_GrowEnvent(fTimeDelta);
	Reset_State();
}

void CBirchnutTree::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

void CBirchnutTree::Reset_State()
{
	if (m_fAniTime >= m_iLength)
	{
		if (Enviornment_STATE::RECOVERY == m_EnviromentState)
		{
			if (m_bIsNoraml)
			{
				m_iTreeType = 0;
				m_TailName = TEXT("normal");
				m_bIsNoraml = false;
			}
			else
			{
				m_iTreeType = 1;
				m_TailName = TEXT("tall");
			}
		}
	}

	__super::Reset_State();
}

HRESULT CBirchnutTree::Render()
{
	if (m_iTreeType == 0)
		XMLRenderAnimation(m_FrontName + m_TailName, &m_tNormal_Animation, &m_tNormal_ImageVec);
	else
		XMLRenderAnimation(m_FrontName + m_TailName);

	m_pTreeLeaf[m_iTreeType]->Render();
	return S_OK;
}

void CBirchnutTree::Damage(void* pArg)
{
	__super::Damage(pArg);
}

void CBirchnutTree::Death()
{

}

void CBirchnutTree::Update_GrowEnvent(_float fTimeDeleta)
{
	switch (m_EnviromentState)
	{
	case Enviornment_STATE::IDLE :
	{
		if (m_iTreeType == 0)
			m_fAliveTime += 0.01f;

		if (m_fMaxAliveTime <= m_fAliveTime)
		{
			m_FrontName = TEXT("grow_normal_to_tall");
			m_TailName = TEXT("");
			m_fAliveTime = 0;
			m_fAniTime = 0;
			m_EnviromentState = Enviornment_STATE::RECOVERY;
		}
	}
	break;
	}
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

	for(int i = 0; i < 2; ++i)
		Safe_Release(m_pTreeLeaf[i]);

	for (auto& Folderiter : m_tNormal_ImageVec)
	{
		for (auto& Fileiter : Folderiter.tFilesVec)
			Safe_Release(Fileiter.pTexture);
	}

}
