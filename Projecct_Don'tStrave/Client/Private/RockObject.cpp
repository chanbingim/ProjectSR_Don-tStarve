#include "RockObject.h"
#include "GameInstance.h"

#include"XML_Manager.h"

CRockObject::CRockObject(LPDIRECT3DDEVICE9 pGraphic_Device) :
	CEnviornment_Object(pGraphic_Device)
{
	m_EnviornmentID = 3;
}

CRockObject::CRockObject(const CRockObject& rhs) :
	CEnviornment_Object(rhs)
{
}

HRESULT CRockObject::Initialize_Prototype()
{
	//auto XML_Instance = CXML_Manager::GetInstance();
	//XML_Instance->AddTexture("../Bin/Resources/Textures/Objects/Evergreen/evergreen_new.scml", L"../Bin/Resources/Textures/Objects/Evergreen/", &m_tImageVec);
	//XML_Instance->LoadScml("../Bin/Resources/Textures/Objects/Evergreen/evergreen_new.scml", &m_tAnimation);

	return S_OK;
}

HRESULT CRockObject::Initialize(void* pArg)
{
	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	LoadImageFile();

	m_pCollision_Com->BindEnterFunction([&](CGameObject* HitActor, _float3& Dir) { BeginHitActor(HitActor, Dir); });
	m_pCollision_Com->BindOverlapFunction([&](CGameObject* HitActor, _float3& Dir) { OverlapHitActor(HitActor, Dir); });
	m_pCollision_Com->BindExitFunction([&](CGameObject* HitActor, _float3& Dir) { EndHitActor(HitActor, Dir); });

	return S_OK;
}

void CRockObject::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CRockObject::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

}

void CRockObject::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CRockObject::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CRockObject::ADD_Components()
{
	/* Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{ 5.f, D3DXToRadian(90.0f) };
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	
	/* Com_Collision */
	CBox_Collision_Component::Collision_Desc Col_Desc = {};
	Col_Desc.pOwner = this;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_BoxCollision"),
		TEXT("Com_BoxCollision"), reinterpret_cast<CComponent**>(&m_pCollision_Com), &Col_Desc)))
		return E_FAIL;

	return S_OK;
}

void CRockObject::BeginHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

void CRockObject::OverlapHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

void CRockObject::EndHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

CRockObject* CRockObject::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRockObject* pInstance = new CRockObject(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("CREATE FAIL : ROCK OBJECT");
	}

	return pInstance;
}

CGameObject* CRockObject::Clone(void* pArg)
{
	CRockObject* pInstance = new CRockObject(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("CLONE FAIL : ROCK OBJECT");
	}

	return pInstance;
}

void CRockObject::Free()
{
	__super::Free();

}
