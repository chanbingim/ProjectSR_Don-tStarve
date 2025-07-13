#include "CampFire.h"

#include "GameInstance.h"
#include "XML_Manager.h"

#include "Slot.h"
#include "Inventory.h"
#include "Mouse.h"
#include "ITemState.h"
#include "Camera.h"
#include "Fire.h"
#include "UIEffect.h"

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

	m_bSoundPlay = false;

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

	m_pGameInstance->Add_RenderGroup(RENDER::ALPHATEST, this);

	switch (m_eCurState)
	{
	case Client::CCampFire::STATE::PREVIEW:
		break;

	case Client::CCampFire::STATE::IDLE:
		if(false == m_bSoundPlay  && __super::isInRange(1.5f))
		{
			m_pGameInstance->Manager_PlaySound(L"campfire_level1.wav", CHANNELID::SOUND_ITEM, 10.f);
			m_bSoundPlay = true;
		}

		if (5600.f < m_fAniTime)
		{
			m_bSoundPlay = false;
			m_fAniTime = 0.f;
		}

		if (0.f < m_Item_Desc.fDurability)
		{
			m_Item_Desc.fDurability -= 1.f * fTimeDelta;
			m_pFire->Update_Fire(m_Item_Desc.fDurability);
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

	XMLRenderAnimation(m_FrontName + m_TailName);

	return S_OK;
}

void CCampFire::HoverEvent()
{
	_float3 vPickingPos = {};

	if (true == dynamic_cast<CVIBuffer_Rect*>(m_pVIBufferCom)->Picking(m_pTransformCom, &vPickingPos))
	{
		CMouse* pMouse = dynamic_cast<CMouse*>(m_pGameInstance->Get_GameObject(EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_Mouse")));
		CSlot* pSlot = pMouse->Get_Slot();
		ITEM_DESC Desc = pSlot->Get_Info();

		if (0 == Desc.iItemID)
			return;

		switch (Desc.iItemID)
		{
		case 44:
		case 46:
		case 48:
			pMouse->Update_Hover(L"요리하기", 2);
			if(m_pGameInstance->KeyDown(VK_RBUTTON))
				Cook(Desc.iItemID, pSlot, Desc);
			break;			
		case 17:
		case 36:
		case 39:
			break;

		default:
			pMouse->Update_Hover(L"연료 넣기", 2);
			if (m_pGameInstance->KeyDown(VK_RBUTTON))
				Add_Fuel(Desc.iItemID, pSlot);
			break;
		}

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
			m_pGameInstance->Manager_PlaySound(L"add_fuel.wav", CHANNELID::SOUND_ITEM, 10.f);
			break;
		case Client::CCampFire::STATE::DEAD:
			m_fAniTime = 0.f;
			m_FrontName = TEXT("dead");
			m_pGameInstance->Manager_PlaySound(L"fire_out.wav", CHANNELID::SOUND_ITEM, 10.f);
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

void CCampFire::Cook(const _uint iItemID, CSlot* pSlot,ITEM_DESC& Item_Desc)
{
	_uint iCookedItemID = iItemID + 1;

	CInventory* pInventory = dynamic_cast<CInventory*>(m_pGameInstance->Get_GameObject(EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_UserInterface"), 0));
	CSlot* pFindSlot = pInventory->Find_Item(iCookedItemID);

	if (nullptr == pFindSlot)
		return;
	else
	{
		CUIEffect::UIEFFECT_DESC Desc = {};
		
		Desc.iItemID = iCookedItemID;
		Desc.pSlot = pFindSlot;
		Desc.vPositon = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);

		Item_Desc.iItemID = iCookedItemID;
		Item_Desc.iNumItem = 1;
		
		memcpy(&Desc.Item_Desc, &Item_Desc, sizeof(ITEM_DESC));

		m_pGameInstance->Add_GameObject_ToLayer(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_UIEffect"),
			EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_UIEffect"), &Desc);

		pSlot->Use_One();
	}
}

void CCampFire::Add_Fuel(const _uint iItemID, CSlot* pSlot)
{
	_float fFuel = { 10.f };

	if (37 == iItemID)
		fFuel = 20.f;
	else if (16 == iItemID)
		fFuel = 60.f;
	
	m_Item_Desc.fDurability += fFuel;

	m_pGameInstance->Manager_PlaySound(L"add_fuel.wav", CHANNELID::SOUND_ITEM, 10.f);

	pSlot->Use_One();
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
