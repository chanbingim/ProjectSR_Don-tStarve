#include "CookedFood.h"

#include "GameInstance.h"
#include "XML_Manager.h"

#include "Slot.h"

#include "Mouse.h"
#include "Camera.h"
#include "CookUI.h"
#include "FoodEffect.h"

CCookedFood::CCookedFood(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CItem{ pGraphic_Device }
{
}

CCookedFood::CCookedFood(const CCookedFood& Prototype)
	: CItem{ Prototype }
{
}

HRESULT CCookedFood::Initialize_Prototype()
{
	auto XML_Instance = CXML_Manager::GetInstance();
	XML_Instance->AddTexture("../Bin/Resources/Textures/Objects/Food/cook_pot_food_002.scml", L"../Bin/Resources/Textures/Objects/Food/", &m_tImageVec);
	XML_Instance->LoadScml("../Bin/Resources/Textures/Objects/Food/cook_pot_food_002.scml", &m_tAnimation);
	return S_OK;
}

HRESULT CCookedFood::Initialize(void* pArg)
{
	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	LoadImageFile();

	m_FrontName = TEXT("idle");
	m_TailName = TEXT("");

	m_iFoodID = 0;

	m_fAniTime = 0.f;
	m_iLength = 1000.f;
	m_ePreState = STATE::END;
	m_eCurState = STATE::IDLE;


	return S_OK;
}

void CCookedFood::Priority_Update(_float fTimeDelta)
{

	m_fAniTime += fTimeDelta * 700.f;

}

void CCookedFood::Update(_float fTimeDelta)
{
	//m_pGameInstance->Add_RenderGroup(RENDER::ALPHATEST, this);

	switch (m_eCurState)
	{
	case CCookedFood::STATE::IDLE:

		break;

	case CCookedFood::STATE::MEATBALL:
		m_fAniTime = 0.f;
		break;

	case CCookedFood::STATE::BONESTEW:
		m_fAniTime = 0.f;
		break;

	default:
		break;
	}


	HoverEvent();

	Change_State();

	CAinimationObject::Update(fTimeDelta);

}

void CCookedFood::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CCookedFood::Render()
{
	CAinimationObject::Render();

	XMLRenderAnimation(m_FrontName + m_TailName);

	return S_OK;
}

void CCookedFood::HoverEvent()
{
	_float3 vPickingPos = {};

	if (true == dynamic_cast<CVIBuffer_Rect*>(m_pVIBufferCom)->Picking(m_pTransformCom, &vPickingPos))
	{
		ClickedEvent();
	}
}

void CCookedFood::ClickedEvent()
{
	if (m_pGameInstance->KeyDown(VK_LBUTTON))
	{
		
	}
}

void CCookedFood::Set_Food(_uint iID)
{
	switch (iID)
	{
	case 0:
		m_eCurState = STATE::IDLE;
		break;

	case 50:
		m_eCurState = STATE::MEATBALL;
		break;

	case 31:
		m_eCurState = STATE::BONESTEW;
		break;
	default:

		break;
	}
}


HRESULT CCookedFood::ADD_Components()
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

void CCookedFood::Change_State()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case Client::CCookedFood::STATE::IDLE:
			m_fAniTime = 0.f;
			m_FrontName = TEXT("idle");
			break;

		case Client::CCookedFood::STATE::MEATBALL:
			m_fAniTime = 0.f;
			m_FrontName = TEXT("meatball");
			break;
		
		case Client::CCookedFood::STATE::BONESTEW:
			m_fAniTime = 0.f;
			m_FrontName = TEXT("bonestew");
			break;

		default:
			break;
		}
		m_ePreState = m_eCurState;
	}
}

CCookedFood* CCookedFood::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCookedFood* pInstance = new CCookedFood(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone CCookedFood");
		return nullptr;
	}

	return pInstance;
}

CGameObject* CCookedFood::Clone(void* pArg)
{
	CGameObject* pInstance = new CCookedFood(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CCookedFood");
		return nullptr;
	}

	return pInstance;
}

void CCookedFood::Free()
{
	__super::Free();
}
