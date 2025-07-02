#include "Chest.h"

#include "GameInstance.h"

#include "Slot.h"
#include "Inventory.h"
#include "Mouse.h"
#include "ITemState.h"
#include <Camera.h>


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
	return S_OK;
}

HRESULT CChest::Initialize(void* pArg)
{
	m_ePreState = m_eCurState = CChest::STATE::PLACE;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pAnimController->ChangeState(m_State_Com[EnumToInt(m_eCurState)]);

	m_pTransformCom->SetScale(_float3(1.5f, 0.9f, 1.f));
	_float3 vPos = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);

	m_bEnableBillboard = true;
	Setting_Shader(L"BillBoard.fx");

	return S_OK;
}

void CChest::Priority_Update(_float fTimeDelta)
{
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
		break;

	case Client::CChest::STATE::OPEN:
		if (false == __super::isInRange())
			m_eCurState = CChest::STATE::CLOSE;
		break;

	case Client::CChest::STATE::CLOSE:
		break;

	case Client::CChest::STATE::END:
		break;

	default:
		break;
	}

	SetUp_OnTerrain(m_pTransformCom, 0.f);

	Change_State();

	m_pAnimController->Tick(fTimeDelta);

}

void CChest::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CChest::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 100);


	class CGameObject* Obj = m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Camera"));
	auto Camera = dynamic_cast<CCamera*>(Obj);
	if (nullptr == Camera)
		return E_FAIL;

	LPDIRECT3DBASETEXTURE9 pTex = { nullptr };
	m_pAnimController->Render();

	m_pGraphic_Device->GetTexture(0, &pTex);
	Excute_Billboard(Camera->GetInvViewMat(), pTex);

	CLandObject::Render();
	m_pVIBuffer_Com->Render();
	End_Billboard();


	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);

	return S_OK;
}

void CChest::HoverEvent()
{
	_float3 vPickingPos = {};

	if (true == dynamic_cast<CVIBuffer_Rect*>(m_pVIBuffer_Com)->Picking(m_pTransformCom, &vPickingPos))
	{
		dynamic_cast<CMouse*>(m_pGameInstance->Get_GameObject(EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_Mouse")))->Update_HoverItem(m_Item_Desc.iItemID);
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

	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"),
		reinterpret_cast<CComponent**>(&m_pVIBuffer_Com))))
		return E_FAIL;

	Engine::CTransform::TRANSFORM_DESC Transform_Desc = { 5.f, D3DXToRadian(90.f) };

	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"),
		reinterpret_cast<CComponent**>(&m_pTransformCom), &Transform_Desc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_AnimController"),
		TEXT("Com_AnimController"), (CComponent**)&m_pAnimController)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Chest_IDLE"),
		TEXT("Com_IDLETexture"),
		reinterpret_cast<CComponent**>(&m_Texture_Com[EnumToInt(CChest::STATE::IDLE)]))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Chest_PLACE"),
		TEXT("Com_PLACETexture"),
		reinterpret_cast<CComponent**>(&m_Texture_Com[EnumToInt(CChest::STATE::PLACE)]))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Chest_OPEN"),
		TEXT("Com_OPENTexture"),
		reinterpret_cast<CComponent**>(&m_Texture_Com[EnumToInt(CChest::STATE::OPEN)]))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Chest_CLOSE"),
		TEXT("Com_CLOSETexture"),
		reinterpret_cast<CComponent**>(&m_Texture_Com[EnumToInt(CChest::STATE::CLOSE)]))))
		return E_FAIL;


	CState::FRAME_DESC Desc = {};

	Desc.iStartFrame = 0;
	Desc.iEndFrame = 0;
	Desc.fTimeRate = 1.f;
	Desc.pAnimTexture = m_Texture_Com[EnumToInt(CChest::STATE::IDLE)];
	Desc.bIsLoop = false;

	m_State_Com[EnumToInt(CChest::STATE::IDLE)] = CItemState::Create(&Desc);

	Desc.iStartFrame = 0;
	Desc.iEndFrame = 9;
	Desc.fTimeRate = 1.f;
	Desc.pAnimTexture = m_Texture_Com[EnumToInt(CChest::STATE::PLACE)];
	Desc.bIsLoop = false;

	m_State_Com[EnumToInt(CChest::STATE::PLACE)] = CItemState::Create(&Desc);

	Desc.iStartFrame = 0;
	Desc.iEndFrame = 4;
	Desc.fTimeRate = 1.f;
	Desc.pAnimTexture = m_Texture_Com[EnumToInt(CChest::STATE::OPEN)];
	Desc.bIsLoop = false;

	m_State_Com[EnumToInt(CChest::STATE::OPEN)] = CItemState::Create(&Desc);

	Desc.iStartFrame = 0;
	Desc.iEndFrame = 7;
	Desc.fTimeRate = 1.f;
	Desc.pAnimTexture = m_Texture_Com[EnumToInt(CChest::STATE::CLOSE)];
	Desc.bIsLoop = false;

	m_State_Com[EnumToInt(CChest::STATE::CLOSE)] = CItemState::Create(&Desc);
	

	return S_OK;
}

void CChest::Change_State()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case Client::CChest::STATE::IDLE:
			break;
		case Client::CChest::STATE::PLACE:
			break;
		case Client::CChest::STATE::OPEN:

			break;
		case Client::CChest::STATE::CLOSE:

			break;
		case Client::CChest::STATE::END:
			break;
		default:
			break;
		}

		m_pAnimController->ChangeState(m_State_Com[EnumToInt(m_eCurState)]);
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

	Safe_Release(m_pAnimController);

	for (_uint i = 0; i < EnumToInt(CChest::STATE::END); ++i)
	{
		Safe_Release(m_Texture_Com[i]);
		Safe_Release(m_State_Com[i]);
	}

}
