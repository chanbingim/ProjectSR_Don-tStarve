#include "CampFire.h"

#include "GameInstance.h"
#include "XML_Manager.h"

#include "Slot.h"
#include "Inventory.h"
#include "Mouse.h"
#include "ITemState.h"
#include "Camera.h"
#include "Fire.h"

CCampFire::CCampFire(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CItem{ pGraphic_Device }
{
}

CCampFire::CCampFire(const CCampFire& Prototype)
	: CItem{ Prototype }
{
}

HRESULT CCampFire::Initialize_Prototype()
{
	auto XML_Instance = CXML_Manager::GetInstance();
	XML_Instance->AddTexture("../Bin/Resources/Textures/Objects/CampFire/campfire.scml", L"../Bin/Resources/Textures/Objects/CampFire/", &m_tImageVec);
	XML_Instance->LoadScml("../Bin/Resources/Textures/Objects/CampFire/campfire.scml", &m_tAnimation);
	return S_OK;
}

HRESULT CCampFire::Initialize(void* pArg)
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
	m_ePreState = STATE::PLACE;
	m_eCurState = STATE::PLACE;

	m_pFire = dynamic_cast<CFire*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Fire"), pArg));

	

	return S_OK;
}

void CCampFire::Priority_Update(_float fTimeDelta)
{
	m_fAniTime += fTimeDelta * 700.f;
}

void CCampFire::Update(_float fTimeDelta)
{
	class CGameObject* Obj = m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Camera"));
	auto Camera = dynamic_cast<CCamera*>(Obj);
	if (nullptr == Camera)
		return;

	m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);

	switch (m_eCurState)
	{
	case Client::CCampFire::STATE::PREVIEW:
		break;

	case Client::CCampFire::STATE::IDLE:

		if (0.f < m_Item_Desc.fDurability)
		{
			m_Item_Desc.fDurability -= 0.02f;
			m_pFire->Update_Fire(m_Item_Desc.fDurability * 0.01f);
			m_pFire->Update(fTimeDelta);
		}
		else
			m_eCurState = CCampFire::STATE::DEAD;

		break;
	case Client::CCampFire::STATE::DEAD:
		
		break;
	case Client::CCampFire::STATE::PLACE:
		if (700.f <= m_fAniTime)
			m_eCurState = STATE::IDLE;
		break;

	default:
		break;
	}
	Change_State();

	HoverEvent();

	CAinimationObject::Update(fTimeDelta);	
}

void CCampFire::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CCampFire::Render()
{
	CAinimationObject::Render();

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	XMLRenderAnimation(m_FrontName + m_TailName);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


	switch (m_eCurState)
	{
	case Client::CCampFire::STATE::IDLE:
		m_pFire->Render();
		break;
	case Client::CCampFire::STATE::DEAD:
		break;
	case Client::CCampFire::STATE::PREVIEW:
		break;

	case Client::CCampFire::STATE::PLACE:
		
		break;
	default:
		break;
	}
	


	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);

	return S_OK;
}

void CCampFire::HoverEvent()
{
	_float3 vPickingPos = {};

	if (true == dynamic_cast<CVIBuffer_Rect*>(m_pVIBufferCom)->Picking(m_pTransformCom, &vPickingPos))
	{
		dynamic_cast<CMouse*>(m_pGameInstance->Get_GameObject(EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_Mouse")))->Update_HoverItem(m_Item_Desc.iItemID);
		__super::ClickedEvent();
	}
}


HRESULT CCampFire::ADD_Components()
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

void CCampFire::Change_State()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case Client::CCampFire::STATE::IDLE:
			m_fAniTime = 0.f;
			m_FrontName = TEXT("idle");
			break;
		case Client::CCampFire::STATE::DEAD:
			m_fAniTime = 0.f;
			m_FrontName = TEXT("dead");
			break;
		case Client::CCampFire::STATE::PREVIEW:
			m_fAniTime = 0.f;
			m_FrontName = TEXT("preview");
			break;
		case Client::CCampFire::STATE::PLACE:
			m_fAniTime = 0.f;
			m_FrontName = TEXT("place");
			break;
		default:
			break;
		}
		m_ePreState = m_eCurState;
	}
}

CCampFire* CCampFire::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCampFire* pInstance = new CCampFire(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone CItem");
		return nullptr;
	}

	return pInstance;
}

CGameObject* CCampFire::Clone(void* pArg)
{
	CGameObject* pInstance = new CCampFire(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CCampFire");
		return nullptr;
	}

	return pInstance;
}

void CCampFire::Free()
{
	__super::Free();

	Safe_Release(m_pFire);
}
