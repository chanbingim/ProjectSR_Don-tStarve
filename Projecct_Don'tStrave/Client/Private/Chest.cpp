#include "Chest.h"

#include "GameInstance.h"
#include "XML_Manager.h"

#include "Slot.h"
#include "Inventory.h"
#include "Mouse.h"
#include "ITemState.h"
#include "Camera.h"
#include "ChestUI.h"


CChest::CChest(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CItem{ pGraphic_Device }
{
}

CChest::CChest(const CChest& Prototype)
	: CItem{ Prototype }
{
}

HRESULT CChest::Initialize_Prototype()
{
	auto XML_Instance = CXML_Manager::GetInstance();
	XML_Instance->AddTexture("../Bin/Resources/Textures/Objects/Chest/treasure_chest.scml", L"../Bin/Resources/Textures/Objects/Chest/", &m_tImageVec);
	XML_Instance->LoadScml("../Bin/Resources/Textures/Objects/Chest/treasure_chest.scml", &m_tAnimation);

	return S_OK;
}

HRESULT CChest::Initialize(void* pArg)
{
	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	LoadImageFile();

	m_FrontName = TEXT("place");
	m_TailName = TEXT("");

	m_fAniTime = 0.f;
	m_iLength = 1000.f;
	m_ePreState = STATE::END;
	m_eCurState = STATE::PLACE;

	

	m_pChestUI = dynamic_cast<CChestUI*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_ChestUI")));

	return S_OK;
}

void CChest::Priority_Update(_float fTimeDelta)
{
	if (m_eCurState != STATE::OPENED)
		m_fAniTime += fTimeDelta * 700.f;
}

void CChest::Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::ALPHATEST, this);

	HoverEvent();


	switch (m_eCurState)
	{
	case Client::CChest::STATE::IDLE:
		
		break;

	case Client::CChest::STATE::PLACE:
		if (834.f <= m_fAniTime)
			m_eCurState = STATE::IDLE;
		break;

	case Client::CChest::STATE::OPEN:
		if (200.f <= m_fAniTime)
			m_eCurState = STATE::OPENED;
		if (false == __super::isInRange())
			m_eCurState = STATE::CLOSE;
		break;
	
	case Client::CChest::STATE::OPENED:
		m_pChestUI->Update(fTimeDelta);
		m_fAniTime = 199.f;
		if (false == __super::isInRange())
			m_eCurState = STATE::CLOSE;
		break;

	case Client::CChest::STATE::CLOSE:
		if (370.f <= m_fAniTime)
			m_eCurState = STATE::IDLE;
		break;

	case Client::CChest::STATE::END:
		break;

	default:
		break;
	}

	Change_State();

	SetUp_OnTerrain(m_pTransformCom, 0.f);



}

void CChest::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CChest::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	XMLRenderAnimation(m_FrontName + m_TailName);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

void CChest::HoverEvent()
{
	_float3 vPickingPos = {};

	if (true == dynamic_cast<CVIBuffer_Rect*>(m_pVIBufferCom)->Picking(m_pTransformCom, &vPickingPos))
	{
		if(STATE::OPEN != m_eCurState)
			m_pMouse->Update_Hover(L":Open", 1);
		ClickedEvent();
	}
}

void CChest::ClickedEvent()
{
	if (CChest::STATE::OPEN == m_eCurState)
		return;
	
	if (m_pGameInstance->KeyDown(VK_LBUTTON) && true == __super::isInRange())
	{
		m_eCurState = CChest::STATE::OPEN;
	}
}


HRESULT CChest::ADD_Components()
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

void CChest::Change_State()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case Client::CChest::STATE::IDLE:
			m_fAniTime = 0.f;
			m_FrontName = TEXT("chest");
			break;

		case Client::CChest::STATE::PLACE:
			m_fAniTime = 0.f;
			m_FrontName = TEXT("place");
			break;

		case Client::CChest::STATE::OPEN:
			m_fAniTime = 0.f;
			m_FrontName = TEXT("open");
			break;

		case Client::CChest::STATE::OPENED:
			m_fAniTime = 180.f;
			m_FrontName = TEXT("open");
			break;

		case Client::CChest::STATE::CLOSE:
			m_fAniTime = 0.f;
			m_FrontName = TEXT("close");
			break;

		case Client::CChest::STATE::END:
			break;

		default:
			break;
		}

		m_ePreState = m_eCurState;
	}
}


CChest* CChest::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CChest* pInstance = new CChest(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CChest");
		return nullptr;
	}

	return pInstance;
}

CGameObject* CChest::Clone(void* pArg)
{
	CGameObject* pInstance = new CChest(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone CChest");
		return nullptr;
	}

	return pInstance;
}

void CChest::Free()
{
	__super::Free();

	
	Safe_Release(m_pChestUI);

}
