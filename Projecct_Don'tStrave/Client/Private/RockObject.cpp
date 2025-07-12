#include "RockObject.h"
#include "GameInstance.h"

#include "XML_Manager.h"
#include "DropItemComponent.h"
#include "../../Engine/Public/CUtility.h"

#include "EffectPoolManager.h"
#include "SpriteEffect.h"

CRockObject::CRockObject(LPDIRECT3DDEVICE9 pGraphic_Device) :
	CDropItemEnviornment(pGraphic_Device)
{
	m_EnviornmentType = Enviornment_TYPE::STONE;
}

CRockObject::CRockObject(const CRockObject& rhs) :
	CDropItemEnviornment(rhs)
{
}

HRESULT CRockObject::Initialize_Prototype(const char* FilePath, const _wstring FolderName)
{
	auto XML_Instance = CXML_Manager::GetInstance();

	WCHAR FullFilePath[MAX_PATH] = {};
	wsprintf(FullFilePath, TEXT("../Bin/Resources/Textures/Objects/%s/"), FolderName.c_str());

	char XMLFullFilePath[MAX_PATH] = {};
	CUtility::ConvertWideToUTF(FullFilePath, XMLFullFilePath);
	sprintf_s(XMLFullFilePath, "%s%s", XMLFullFilePath, FilePath);

	XML_Instance->AddTexture(XMLFullFilePath, FullFilePath, &m_tImageVec);
	XML_Instance->LoadScml(XMLFullFilePath, &m_tAnimation);

	return S_OK;
}

HRESULT CRockObject::Initialize(void* pArg)
{
	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	LoadImageFile();

	m_FrontName = TEXT("full");
	m_TailName = TEXT("");

	Setting_Data();
	m_pDropItem_Com->SetCreateEffect(0);
	m_EnviormentInfo.iMaxHit = 3;

	m_pCollision_Com->BindEnterFunction([&](CGameObject* HitActor, _float3& Dir) { BeginHitActor(HitActor, Dir); });
	m_pCollision_Com->BindOverlapFunction([&](CGameObject* HitActor, _float3& Dir) { OverlapHitActor(HitActor, Dir); });
	m_pCollision_Com->BindExitFunction([&](CGameObject* HitActor, _float3& Dir) { EndHitActor(HitActor, Dir); });
	m_EnviromentState = Enviornment_STATE::IDLE;

	return S_OK;
}

void CRockObject::Priority_Update(_float fTimeDelta)
{
}

void CRockObject::Update(_float fTimeDelta)
{
	__super::HoverEevent();
}

void CRockObject::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

void CRockObject::Reset_State()
{
	
}

HRESULT CRockObject::Render()
{
	__super::Render();

	return S_OK;
}

void CRockObject::Damage(void* pArg)
{
	m_EnviormentInfo.iHit++;
	switch (m_EnviormentInfo.iMaxHit - m_EnviormentInfo.iHit)
	{
	case 0:
	{
		_float3 Pos = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
		Pos += m_pTransformCom->GetWorldState(WORLDSTATE::LOOK) * -1.f;
		Pos.y += m_pTransformCom->GetScale().y * 1.f;
		CreateDropItem(Pos);
		m_isDead = true;
	}
		break;
	case 1:
		m_FrontName = TEXT("low");
		Enviornment_STATE::DAMAGED;
		break;
	case 2 :
		m_FrontName = TEXT("med");
		Enviornment_STATE::DAMAGED;
		break;
	}

	CEffectPoolManager::GetInstance()->Add_ActiveEffect(1, (CAinimationObject**)&m_pSpirteEffect);
	m_pSpirteEffect->ReadyEffect(L"anim");

	m_pSpirteEffect->GetTransfrom()->SetPosition(m_pTransformCom->GetWorldState(WORLDSTATE::POSITION));

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

	/* Com_DropItem */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_DropItem"),
		TEXT("Com_DropItem"), reinterpret_cast<CComponent**>(&m_pDropItem_Com))))
		return E_FAIL;
	
	/* Com_Collision */
	CBox_Collision_Component::Collision_Desc Col_Desc = {};
	Col_Desc.pOwner = this;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_BoxCollision"),
		TEXT("Com_BoxCollision"), reinterpret_cast<CComponent**>(&m_pCollision_Com), &Col_Desc)))
		return E_FAIL;

	return S_OK;
}

void CRockObject::Setting_Data()
{
	switch (m_iObjectID)
	{
	case 3:
	{
		m_pDropItem_Com->ADD_ItemData(41, 1);
	}
	break;
	case 5:
	{
		m_pDropItem_Com->ADD_ItemData(39, 1);
	}
	break;
	}
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

CRockObject* CRockObject::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const char* FilePath, const _wstring FolderName)
{
	CRockObject* pInstance = new CRockObject(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype(FilePath, FolderName)))
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
