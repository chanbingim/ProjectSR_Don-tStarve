#include "Cookpot.h"

#include "GameInstance.h"
#include "XML_Manager.h"

#include "Slot.h"

#include "Mouse.h"
#include "Camera.h"

CCookpot::CCookpot(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CItem{ pGraphic_Device }
{
}

CCookpot::CCookpot(const CCookpot& Prototype)
	: CItem{ Prototype }
{
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

	m_fAniTime = 0.f;
	m_iLength = 100.f;
	m_ePreState = STATE::END;
	m_eCurState = STATE::PLACE;


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
		
		break;

	case Client::CCookpot::STATE::IDLE_FULL:
		
		break;

	case Client::CCookpot::STATE::PLACE:
		if (735.f < m_fAniTime)
			m_eCurState = STATE::IDLE_EMPTY;
		break;

	case Client::CCookpot::STATE::COOKING_LOOP:
		
		break;

	default:
		break;
	}

	Change_State();

	CAinimationObject::Update(fTimeDelta);
}

void CCookpot::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CCookpot::Render()
{
	CAinimationObject::Render();

	XMLRenderAnimation(m_FrontName + m_TailName);

	return S_OK;
}

void CCookpot::HoverEvent()
{

}


HRESULT CCookpot::ADD_Components()
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

void CCookpot::Change_State()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case Client::CCookpot::STATE::IDLE_EMPTY:
			m_fAniTime = 0.f;
			m_FrontName = TEXT("idle_empty");
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


}
