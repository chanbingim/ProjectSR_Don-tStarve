#include "LightComponent.h"
#include "Light_Manager.h"
#include "GameObject.h"

CLightComponent::CLightComponent() : CComponent()
{
	ZeroMemory(&m_LightData, sizeof(D3DLIGHT9));
}

CLightComponent::CLightComponent(LPDIRECT3DDEVICE9 pGraphic_Device) :CComponent(pGraphic_Device)
{
	ZeroMemory(&m_LightData, sizeof(D3DLIGHT9));
}

CLightComponent::CLightComponent(CLightComponent& rhs) :
	m_LightData(rhs.m_LightData), CComponent(rhs)
{
}

HRESULT CLightComponent::Initialize_Prototype()
{
	m_LightData.Type = D3DLIGHTTYPE::D3DLIGHT_POINT;
	m_LightData.Falloff = 1.0f;
	m_LightData.Attenuation0 = 1.0f;
	m_LightData.Attenuation1 = 0.f;
	m_LightData.Attenuation2 = 0.f;

	return S_OK;
}

HRESULT CLightComponent::Initialize(void* pArg)
{
	if (nullptr == pArg)
	{

	}
	else
	{
		LIGHT_DESC* pLightDesc = static_cast<LIGHT_DESC*>(pArg);
		m_pOwner = pLightDesc->pOwner;
		m_LightData = pLightDesc->LightData;
	}
	CLight_Manager::GetInstance()->ADD_Light((LIGHT_TYPE)m_LightData.Type, this);

	return S_OK;
}

void CLightComponent::Render_Light(_uint LightUstage)
{
	m_pGraphic_Device->SetLight(LightUstage, &m_LightData);
}

void CLightComponent::SetAttenuation(_float fAtn0, _float fAtn1, _float fAtn2)
{
	m_LightData.Attenuation0 = fAtn0;
	m_LightData.Attenuation1 = fAtn1;
	m_LightData.Attenuation2 = fAtn2;
}

CGameObject* CLightComponent::GetOwner()
{
	return m_pOwner;
}

void CLightComponent::Free()
{
	__super::Free();
}

CLightComponent* CLightComponent::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLightComponent* pLightCom = new CLightComponent(pGraphicDev);
	if (FAILED(pLightCom->Initialize_Prototype()))
	{
		Safe_Release(pLightCom);
		MSG_BOX("LightComponent Create Failed");
	}
	return pLightCom;
}

CComponent* CLightComponent::Clone(void* pArg)
{
	CLightComponent* pLightCom = new CLightComponent(*this);
	if (FAILED(pLightCom->Initialize(pArg)))
	{
		Safe_Release(pLightCom);
		MSG_BOX("LightComponent Create Failed");
	}
	return pLightCom;
}
