#include "ResurrectionStone.h"

#include "GameInstance.h"
#include "XML_Manager.h"

#include "Player.h"
#include "Monster.h"

CResurrectionStone::CResurrectionStone(LPDIRECT3DDEVICE9 pGraphic_Device) :
	CEnviornment_Object(pGraphic_Device)
{
}

CResurrectionStone::CResurrectionStone(const CResurrectionStone& rhs) :
	CEnviornment_Object(rhs)
{
}

HRESULT CResurrectionStone::Initialize_Prototype()
{
	auto XML_Instance = CXML_Manager::GetInstance();
	XML_Instance->AddTexture("../Bin/Resources/Textures/Objects/Resurrection_Stone/resurrection_stone.scml", L"../Bin/Resources/Textures/Objects/Resurrection_Stone/", &m_tImageVec);
	XML_Instance->LoadScml("../Bin/Resources/Textures/Objects/Resurrection_Stone/resurrection_stone.scml", &m_tAnimation);

	return S_OK;
}

HRESULT CResurrectionStone::Initialize(void* pArg)
{
	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	LoadImageFile();
	m_EnviornmentType = Enviornment_TYPE::RESERREECTION;
	m_EnviromentState = Enviornment_STATE::DAMAGED;
	m_FrontName = TEXT("idle_off");
	m_TailName = TEXT("");

	m_pCollision_Com->BindEnterFunction([&](CGameObject* HitActor, _float3& Dir) { BeginHitActor(HitActor, Dir); });
	m_pCollision_Com->BindOverlapFunction([&](CGameObject* HitActor, _float3& Dir) { OverlapHitActor(HitActor, Dir); });
	m_pCollision_Com->BindExitFunction([&](CGameObject* HitActor, _float3& Dir) { EndHitActor(HitActor, Dir); });

	return S_OK;
}

void CResurrectionStone::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CResurrectionStone::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	Reset_State();
}

void CResurrectionStone::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

void CResurrectionStone::Reset_State()
{
	if (m_iLength <= m_fAniTime)
	{
		if (Enviornment_STATE::RECOVERY == m_EnviromentState)
		{
			m_fAniTime = 0;
			m_EnviromentState = Enviornment_STATE::IDLE;
			m_FrontName = TEXT("idle_activate");
		}

		if (Enviornment_STATE::BROKEN == m_EnviromentState)
		{
			m_fAniTime = 0;
			m_EnviromentState = Enviornment_STATE::BROKEN_IDLE;
			m_FrontName = TEXT("idle_broken");
		}
	}
}

HRESULT CResurrectionStone::Render()
{
	__super::Render();
	return S_OK;
}

void CResurrectionStone::Damage(void* pArg)
{
	if (nullptr == pArg)
	{
#ifdef _DEBUG
		MSG_BOX("Not Found Data : Resurrection");
#endif 
		return;
	}
	DAMAGE_DATA_BASE Data = *static_cast<DAMAGE_DATA_BASE*>(pArg);
	auto Attacker = static_cast<CGameObject*>(Data.Attacker);

	auto Monster = dynamic_cast<CMonster*>(Attacker);
	if (Monster)
		Damageed(1, Data.Damage);

	auto Player = dynamic_cast<CPlayer*>(Attacker);
	if (Player || Attacker == this)
		Damageed(0, Data.Damage);

}

void CResurrectionStone::Death()
{
	m_fAniTime = 0;
	m_EnviromentState = Enviornment_STATE::BROKEN;
	m_FrontName = TEXT("resurrect");
}

HRESULT CResurrectionStone::ADD_Components()
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

void CResurrectionStone::Damageed(_uint itype, _uint iDamage)
{
	switch (itype)
	{
	case 0 :
		if (m_EnviromentState == Enviornment_STATE::DAMAGED )
		{
			m_fAniTime = 0;
			m_EnviromentState = Enviornment_STATE::RECOVERY;
			m_FrontName = TEXT("activate");
		}
		else if (m_EnviromentState == Enviornment_STATE::IDLE)
		{
			Death();
		}
		break;
	case 1:
		m_EnviormentInfo.iHp -= iDamage;
		if (0 >= m_EnviormentInfo.iHp)
			Death();
		break;
	}
}

void CResurrectionStone::BeginHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

void CResurrectionStone::OverlapHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

void CResurrectionStone::EndHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

CResurrectionStone* CResurrectionStone::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CResurrectionStone* pInstance = new CResurrectionStone(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("CREATE FAIL : RESURRECTION STONE OBJECT");
	}

	return pInstance;
}

CGameObject* CResurrectionStone::Clone(void* pArg)
{
	CResurrectionStone* pInstance = new CResurrectionStone(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("CLONE FAIL : RESURRECTION STONE OBJECT");
	}

	return pInstance;
}

void CResurrectionStone::Free()
{
	__super::Free();
}
