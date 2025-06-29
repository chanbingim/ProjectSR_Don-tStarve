#include "RockObject.h"
#include "GameInstance.h"

CRockObject::CRockObject(LPDIRECT3DDEVICE9 pGraphic_Device) :
	CEnviornment_Object(pGraphic_Device)
{
}

CRockObject::CRockObject(const CRockObject& rhs) :
	CEnviornment_Object(rhs)
{
}

HRESULT CRockObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRockObject::Initialize(void* pArg)
{
	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

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
	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_WorldMat);
	m_Idle_pTexture_Com->Set_Texture(0);

	m_pVIBuffer_Com->Render();

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
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBuffer_Com))))
		return E_FAIL;

	/* Com_Idle_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), TEXT("Prototype_Component_Texture_Rock_Idle"),
		TEXT("Com_Idle_Texture"), reinterpret_cast<CComponent**>(&m_Idle_pTexture_Com))))
		return E_FAIL;

	/* Com_Damaged_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), TEXT("Prototype_Component_Texture_Rock_Damaged"),
		TEXT("Com_Damaged_Texture"), reinterpret_cast<CComponent**>(&m_Damaged_pTexture_Com))))
		return E_FAIL;

	/* Com_Broken_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), TEXT("Prototype_Component_Texture_Rock_Broken"),
		TEXT("Com_Broken_Texture"), reinterpret_cast<CComponent**>(&m_Broken_pTexture_Com))))
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

	Safe_Release(m_Damaged_pTexture_Com);
	Safe_Release(m_Broken_pTexture_Com);
}
