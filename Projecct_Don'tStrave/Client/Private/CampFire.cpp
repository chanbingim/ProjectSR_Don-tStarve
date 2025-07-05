#include "CampFire.h"

#include "GameInstance.h"

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
	return S_OK;
}

HRESULT CCampFire::Initialize(void* pArg)
{
	m_iFireLevel = 0;
	m_fFireOffset = 0.58f;
	m_eCurState = CCampFire::STATE::IDLE;

	if (FAILED(ADD_Components()))
		return E_FAIL;
	
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pTransformCom->SetScale(_float3(0.7f, 0.7f, 1.f));
	_float3 vPos = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);

	m_pFire = dynamic_cast<CFire*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Fire"), pArg));

	m_bEnableBillboard = true;
	Setting_Shader(L"BillBoard.fx");

	return S_OK;
}

void CCampFire::Priority_Update(_float fTimeDelta)
{
}

void CCampFire::Update(_float fTimeDelta)
{
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
	default:
		break;
	}

	m_pGameInstance->Add_RenderGroup(RENDER::ALPHATEST, this);

	HoverEvent();

	SetUp_OnTerrain(m_pTransformCom, 0.f);
	
}

void CCampFire::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CCampFire::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 50);

	class CGameObject* Obj = m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Camera"));
	auto Camera = dynamic_cast<CCamera*>(Obj);
	if (nullptr == Camera)
		return E_FAIL;


	LPDIRECT3DBASETEXTURE9 pTex = { nullptr };

	m_pGraphic_Device->GetTexture(0, &pTex);
	Excute_Billboard(Camera->GetInvViewMat(), pTex);

	m_pTexture_Com->Set_Texture(EnumToInt(m_eCurState));

	CLandObject::Render();

	m_pVIBuffer_Com->Render();
	End_Billboard();

	switch (m_eCurState)
	{
	case Client::CCampFire::STATE::IDLE:
		m_pFire->Render();
		break;
	case Client::CCampFire::STATE::DEAD:
		break;
	case Client::CCampFire::STATE::PREVIEW:
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

	if (true == dynamic_cast<CVIBuffer_Rect*>(m_pVIBuffer_Com)->Picking(m_pTransformCom, &vPickingPos))
	{
		dynamic_cast<CMouse*>(m_pGameInstance->Get_GameObject(EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_Mouse")))->Update_HoverItem(m_Item_Desc.iItemID);
		__super::ClickedEvent();
	}
}


HRESULT CCampFire::ADD_Components()
{

	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"),
		reinterpret_cast<CComponent**>(&m_pVIBuffer_Com))))
		return E_FAIL;

	Engine::CTransform::TRANSFORM_DESC Transform_Desc = { 5.f, D3DXToRadian(90.f) };

	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"),
		reinterpret_cast<CComponent**>(&m_pTransformCom), &Transform_Desc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_CampFire"),
		TEXT("Com_Texture"),
		reinterpret_cast<CComponent**>(&m_pTexture_Com))))
		return E_FAIL;


	return S_OK;
}

void CCampFire::Update_Item(_float fTimeDelta)
{
	/*switch ()
	{
	default:
		break;
	}*/
}

//void CCampFire::Update_Fire(_float fValue)
//{
//	m_fFireOffset += fValue * 0.5f;
//	_float3 vScale = m_pFireTransform_Com->GetScale();
//	m_pFireTransform_Com->SetScale(_float3(vScale.x + fValue, vScale.y + fValue, 1.f));
//}

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
