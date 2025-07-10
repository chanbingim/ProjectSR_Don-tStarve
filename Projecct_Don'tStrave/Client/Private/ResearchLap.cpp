#include "ResearchLap.h"

#include "GameInstance.h"
#include "XML_Manager.h"

#include "Slot.h"
#include "Inventory.h"
#include "Mouse.h"
#include "Camera.h"
#include "CraftingUI.h"
#include "Item_Button.h"

CResearchLap::CResearchLap(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CItem{ pGraphic_Device }
{
}

CResearchLap::CResearchLap(const CResearchLap& Prototype)
	: CItem{ Prototype }
{
}

HRESULT CResearchLap::Initialize_Prototype()
{
	auto XML_Instance = CXML_Manager::GetInstance();
	XML_Instance->AddTexture("../Bin/Resources/Textures/Objects/ResearchLab/researchlab.scml", L"../Bin/Resources/Textures/Objects/ResearchLab/", &m_tImageVec);
	XML_Instance->LoadScml("../Bin/Resources/Textures/Objects/ResearchLab/researchlab.scml", &m_tAnimation);
	return S_OK;
}

HRESULT CResearchLap::Initialize(void* pArg)
{
	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	LoadImageFile();

	m_FrontName = TEXT("place");
	m_TailName = TEXT("");
	
	m_fAniTime = 0.f;
	m_iLength = 100.f;
	m_ePreState = STATE::END;
	m_eCurState = STATE::PLACE;
	
	m_pItem_Buttons = dynamic_cast<CCraftingUI*>(m_pGameInstance->Get_GameObject(
		EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_UserInterface"), 5))->Get_ItemBtn();

	if (nullptr == m_pItem_Buttons)
		return E_FAIL;

	return S_OK;
}

void CResearchLap::Priority_Update(_float fTimeDelta)
{
	//__super::Priority_Update(fTimeDelta);
	m_fAniTime += fTimeDelta * 700.f;

}

void CResearchLap::Update(_float fTimeDelta)
{
	class CGameObject* Obj = m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Camera"));
	auto Camera = dynamic_cast<CCamera*>(Obj);
	if (nullptr == Camera)
		return;

	m_pGameInstance->Add_RenderGroup(RENDER::ALPHATEST, this);


	switch (m_eCurState)
	{
	case Client::CResearchLap::STATE::IDLE:
		if (true == __super::isInRange(1.5f))
			m_eCurState = STATE::USE;
		break;

	case Client::CResearchLap::STATE::USE:
		if (2200.f < m_fAniTime)
			m_eCurState = STATE::LOOP;
		break;

	case Client::CResearchLap::STATE::PLACE:
		if (700.f < m_fAniTime)
			m_eCurState = STATE::IDLE;
		break;

	case Client::CResearchLap::STATE::LOOP:
		if (false == __super::isInRange(1.5f))
			m_eCurState = STATE::IDLE;
		break;

	default:
		break;
	}

	Change_State();

	CAinimationObject::Update(fTimeDelta);
}

void CResearchLap::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CResearchLap::Render()
{
	CAinimationObject::Render();

	XMLRenderAnimation(m_FrontName + m_TailName);

	return S_OK;
}

void CResearchLap::HoverEvent()
{
	
}


HRESULT CResearchLap::ADD_Components()
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

void CResearchLap::Change_State()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case Client::CResearchLap::STATE::IDLE:
			m_fAniTime = 0.f;
			m_FrontName = TEXT("idle");

			for (_uint i = 0; i < EnumToInt(CATEGORY::END); ++i)
			{
				for (auto pButton : m_pItem_Buttons[i])
				{
					pButton->InResearchLap(false);
				}
			}
			break;
			
		case Client::CResearchLap::STATE::USE:
			m_fAniTime = 0.f;
			m_FrontName = TEXT("use");
			for (_uint i = 0; i < EnumToInt(CATEGORY::END); ++i)
			{
				for (auto pButton : m_pItem_Buttons[i])
				{
					pButton->InResearchLap(true);
				}
			}
			break;

		case Client::CResearchLap::STATE::PLACE:
			m_fAniTime = 0.f;
			m_FrontName = TEXT("place");
			break;

		case Client::CResearchLap::STATE::LOOP:
			m_fAniTime = 0.f;
			m_FrontName = TEXT("proximity_loop");
			break;

		default:
			break;
		}
		m_ePreState = m_eCurState;
	}
}

CResearchLap* CResearchLap::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CResearchLap* pInstance = new CResearchLap(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone CResearchLap");
		return nullptr;
	}

	return pInstance;
}

CGameObject* CResearchLap::Clone(void* pArg)
{
	CGameObject* pInstance = new CResearchLap(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CResearchLap");
		return nullptr;
	}

	return pInstance;
}

void CResearchLap::Free()
{
	__super::Free();

	
}
