#include "Cookpot.h"

#include "GameInstance.h"
#include "XML_Manager.h"

#include "Slot.h"

#include "Mouse.h"
#include "Camera.h"
#include "CookUI.h"
#include "FoodEffect.h"

CCookpot::CCookpot(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CItem{ pGraphic_Device }
{
}

CCookpot::CCookpot(const CCookpot& Prototype)
	: CItem{ Prototype }
{
}

void CCookpot::Start_Cooking(_uint iItemID)
{
	m_eCurState = STATE::COOKING_LOOP;
	m_iFoodID = iItemID;
}

HRESULT CCookpot::Initialize_Prototype()
{
	auto XML_Instance = CXML_Manager::GetInstance();
	XML_Instance->AddTexture("../Bin/Resources/Textures/Objects/Cook_Pot/cook_pot.scml", L"../Bin/Resources/Textures/Objects/Cook_Pot/", &m_tImageVec);
	XML_Instance->LoadScml("../Bin/Resources/Textures/Objects/Cook_Pot/cook_pot.scml", &m_tAnimation);
	return S_OK;
}

HRESULT CCookpot::Initialize(void* pArg)
{
	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	LoadImageFile();

	m_FrontName = TEXT("place");
	m_TailName = TEXT("");

	m_iFoodID = 0;

	m_fAniTime = 0.f;
	m_iLength = 100.f;
	m_ePreState = STATE::END;
	m_eCurState = STATE::PLACE;

	m_pCookUI = dynamic_cast<CCookUI*>(m_pGameInstance->Clone_Prototype(
		PROTOTYPE::GAMEOBJECT, EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_CookUI"), this));

	if (nullptr == m_pCookUI)
		return E_FAIL;


	CFoodEffect::FOODEFFECT_DESC Effect_Desc = {};
	Effect_Desc.iItemID = 50;
	Effect_Desc.pTransform = m_pTransformCom;

	/*m_CFoodEffect = dynamic_cast<CFoodEffect*>(m_pGameInstance->Clone_Prototype(
		PROTOTYPE::GAMEOBJECT, EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_FoodEffect"), &Effect_Desc));*/




	return S_OK;
}

void CCookpot::Priority_Update(_float fTimeDelta)
{
	//__super::Priority_Update(fTimeDelta);
	m_fAniTime += fTimeDelta * 700.f;

}

void CCookpot::Update(_float fTimeDelta)
{
	class CGameObject* Obj = m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Camera"));
	auto Camera = dynamic_cast<CCamera*>(Obj);
	if (nullptr == Camera)
		return;

	m_pGameInstance->Add_RenderGroup(RENDER::ALPHATEST, this);

	switch (m_eCurState)
	{
	case CCookpot::STATE::IDLE_EMPTY:
			//m_pCookUI->Update(fTimeDelta);
		break;
	
	case CCookpot::STATE::IDLE_OPEN:
		m_pCookUI->Update(fTimeDelta);
		if (false == __super::isInRange(1.5f))
			m_eCurState = STATE::IDLE_EMPTY;
		break;

	case Client::CCookpot::STATE::IDLE_FULL:
		if (m_pGameInstance->KeyDown(VK_LBUTTON))
		{
			// 음식 넣어주는 거 추가
			m_eCurState = STATE::IDLE_EMPTY;
		}
		break;

	case Client::CCookpot::STATE::PLACE:
		if (735.f < m_fAniTime)
			m_eCurState = STATE::IDLE_EMPTY;
		break;

	case Client::CCookpot::STATE::COOKING_LOOP:
		m_fCookingTime += fTimeDelta;

		if (10.f < m_fCookingTime)
			m_eCurState = STATE::IDLE_FULL;
		break;

	default:
		break;
	}


	HoverEvent();

	Change_State();

	CAinimationObject::Update(fTimeDelta);

	_float3 vPos = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
	
	_float3 vRotation = m_pTransformCom->GetRotation();

}

void CCookpot::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CCookpot::Render()
{
	CAinimationObject::Render();

	XMLRenderAnimation(m_FrontName + m_TailName);

	//m_pFoodTexture->Set_Texture(50);

	//m_pVIBufferCom->Render();

	return S_OK;
}

void CCookpot::HoverEvent()
{
	_float3 vPickingPos = {};

	if (true == dynamic_cast<CVIBuffer_Rect*>(m_pVIBufferCom)->Picking(m_pTransformCom, &vPickingPos))
	{
		ClickedEvent();
	}
}

void CCookpot::ClickedEvent()
{
	if (m_pGameInstance->KeyDown(VK_LBUTTON))
	{
		if (STATE::IDLE_EMPTY == m_eCurState)
			m_eCurState = STATE::IDLE_OPEN;
		else if (STATE::IDLE_FULL == m_eCurState)
			m_eCurState = STATE::IDLE_EMPTY;
	}
}


HRESULT CCookpot::ADD_Components()
{
	/* Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{ 5.f, D3DXToRadian(90.0f) };
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Food_Transform"), reinterpret_cast<CComponent**>(&m_pFoodTransformCom), &TransformDesc)))
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

	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Item"),
		TEXT("Com_Texture"),
		reinterpret_cast<CComponent**>(&m_pFoodTexture))))
		return E_FAIL;

	return S_OK;
}

void CCookpot::Change_State()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case Client::CCookpot::STATE::IDLE_EMPTY:
			m_fAniTime = 0.f;
			m_FrontName = TEXT("idle_empty");
			if (CCookpot::STATE::IDLE_OPEN == m_ePreState)
				m_pCookUI->Change_State(CCookUI::STATE::CLOSE);
			break;

		case Client::CCookpot::STATE::IDLE_OPEN:
			m_fAniTime = 0.f;
			m_FrontName = TEXT("idle_empty");
			m_pCookUI->Change_State(CCookUI::STATE::OPEN);
			break;

		case Client::CCookpot::STATE::IDLE_FULL:
			m_fAniTime = 0.f;
			m_FrontName = TEXT("idle_full");
			break;

		case Client::CCookpot::STATE::PLACE:
			m_fAniTime = 0.f;
			m_FrontName = TEXT("place");
			break;

		case Client::CCookpot::STATE::COOKING_LOOP:
			m_fAniTime = 0.f;
			m_fCookingTime = 0.f;
			m_FrontName = TEXT("cooking_loop");
			break;

		default:
			break;
		}
		m_ePreState = m_eCurState;
	}
}

CCookpot* CCookpot::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCookpot* pInstance = new CCookpot(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone CCookpot");
		return nullptr;
	}

	return pInstance;
}

CGameObject* CCookpot::Clone(void* pArg)
{
	CGameObject* pInstance = new CCookpot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CCookpot");
		return nullptr;
	}

	return pInstance;
}

void CCookpot::Free()
{
	__super::Free();

	Safe_Release(m_pCookUI);
	Safe_Release(m_pFoodTexture);
	Safe_Release(m_pFoodTransformCom);
}
