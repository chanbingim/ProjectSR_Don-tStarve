#include "Fire.h"

#include "GameInstance.h"
#include "XML_Manager.h"

#include "Slot.h"
#include "Inventory.h"
#include "Mouse.h"
#include "ITemState.h"
#include "Camera.h"

CFire::CFire(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CItem{ pGraphic_Device }
{
}

CFire::CFire(const CFire& Prototype)
	: CItem{ Prototype }
{
}

HRESULT CFire::Initialize_Prototype()
{
	auto XML_Instance = CXML_Manager::GetInstance();
	XML_Instance->AddTexture("../Bin/Resources/Textures/Objects/CampFire_Fire/campfire_fire.scml", L"../Bin/Resources/Textures/Objects/CampFire_Fire/", &m_tImageVec);
	XML_Instance->LoadScml("../Bin/Resources/Textures/Objects/CampFire_Fire/campfire_fire.scml", &m_tAnimation);
	return S_OK;
}

HRESULT CFire::Initialize(void* pArg)
{
	m_fDurability = 100.f;
	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	LoadImageFile();

	m_pLight_Com->SetLightType(D3DLIGHT_POINT);

	m_Color = D3DXCOLOR(1.0f, 0.8f, 0.8f, 1.f);
	_float3	vPos = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
	m_pLight_Com->SetDiffuseColor(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f));
	m_pLight_Com->SetAmbientColor(D3DXCOLOR(0.f, 0.f, 0.f, 0.f));
	m_pLight_Com->SetPosition(vPos);
	m_pLight_Com->SetMaxRange(3.3f);

	m_pLight_Com->SetAttenuation(0.f, 0.3f, 0.8f);

	m_FrontName = TEXT("level3");
	m_TailName = TEXT("");

	m_fAniTime = 0.f;
	m_iLength = 100.f;
	m_ePreState = STATE::LEVEL3;
	m_eCurState = STATE::LEVEL3;

	return S_OK;
}

void CFire::Priority_Update(_float fTimeDelta)
{
}

void CFire::Update(_float fTimeDelta)
{
	m_fAniTime += fTimeDelta * 700.f;
	//_float3	vPos = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
	//m_pLight_Com->SetPosition(vPos);
	_float fValue = m_fDurability * 0.01f;
	//m_pLight_Com->Render_Light();
	m_pLight_Com->SetAmbientColor(D3DXCOLOR(m_Color.r * fValue, m_Color.g * fValue, m_Color.b * fValue, m_Color.b));
	//m_pLight_Com->Render_Light();

	switch (m_eCurState)
	{
	case Client::CFire::STATE::LEVEL1:
		if (30.f < m_fDurability)
			m_eCurState = CFire::STATE::LEVEL2;
		break;

	case Client::CFire::STATE::LEVEL2:
		if (60.f < m_fDurability)
			m_eCurState = CFire::STATE::LEVEL3;
		else if (30.f >= m_fDurability)
			m_eCurState = CFire::STATE::LEVEL1;
		break;

	case Client::CFire::STATE::LEVEL3:
		if (100.f < m_fDurability)
			m_eCurState = CFire::STATE::LEVEL4;
		else if (60.f >= m_fDurability)
			m_eCurState = CFire::STATE::LEVEL2;
		break;

	case Client::CFire::STATE::LEVEL4:
		if (100.f >= m_fDurability)
			m_eCurState = CFire::STATE::LEVEL3;
		break;

	default:
		break;
	}

	m_pGameInstance->Add_RenderGroup(RENDER::ALPHATEST, this);

	Change_State();

	CAinimationObject::Update(fTimeDelta);
}

void CFire::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CFire::Render()
{

	XMLRenderAnimation(m_FrontName + m_TailName);

	return S_OK;
}

HRESULT CFire::ADD_Components()
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

	CLightComponent::LIGHT_DESC Light_Desc = {};

	m_Light.Type = D3DLIGHT_POINT;

	m_Light.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_Light.Ambient = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.f);

	Light_Desc.LightData = m_Light;
	Light_Desc.pOwner = this;

	// Transform Component
	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_Light"),
		TEXT("Com_Light"),
		reinterpret_cast<CComponent**>(&m_pLight_Com), &Light_Desc)))
		return E_FAIL;


	return S_OK;
}

void CFire::Change_State()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case Client::CFire::STATE::LEVEL1:
			m_fAniTime = 0.f;
			m_FrontName = TEXT("level1");
			break;

		case Client::CFire::STATE::LEVEL2:
			m_fAniTime = 0.f;
			m_FrontName = TEXT("level2");
			break;

		case Client::CFire::STATE::LEVEL3:
			m_fAniTime = 0.f;
			m_FrontName = TEXT("level3");
			break;

		case Client::CFire::STATE::LEVEL4:
			m_fAniTime = 0.f;
			m_FrontName = TEXT("level4");
			break;

		default:
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CFire::Update_Fire(_float fValue)
{
	m_fDurability = fValue;
}

CFire* CFire::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CFire* pInstance = new CFire(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone CFire");
		return nullptr;
	}

	return pInstance;
}

CGameObject* CFire::Clone(void* pArg)
{
	CGameObject* pInstance = new CFire(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CFire");
		return nullptr;
	}

	return pInstance;
}

void CFire::Free()
{
	__super::Free();

	Safe_Release(m_pLight_Com);
}
