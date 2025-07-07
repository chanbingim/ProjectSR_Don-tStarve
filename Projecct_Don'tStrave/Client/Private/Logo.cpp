#include "Logo.h"

#include "GameInstance.h"



CLogo::CLogo(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUserInterface{ pGraphic_Device }
{
}

CLogo::CLogo(const CLogo& Prototype)
	: CUserInterface{ Prototype }
{
}

HRESULT CLogo::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLogo::Initialize(void* pArg)
{
	
	CUserInterface::UIOBJECT_DESC Desc = {};

	Desc.fSizeX = 1280.f;
	Desc.fSizeY = 720.f;
	Desc.fX  = g_iWinSizeX * 0.5f;
	Desc.fY  = g_iWinSizeY * 0.5f;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	__super::UpdatePosition();


	return S_OK;
}

void CLogo::Priority_Update(_float fTimeDelta)
{
	

}

void CLogo::Update(_float fTimeDelta)
{
	

	

}

void CLogo::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::ORTTHO_UI, this);
}

HRESULT CLogo::Render()
{
	m_pTexture_Com->Set_Texture(0);

	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());

	m_pVIBuffer_Com->Render();

	return S_OK;
}

HRESULT CLogo::ADD_Components()
{
	// Texture Component
	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Logo"),
		TEXT("Com_Texture"),
		reinterpret_cast<CComponent**>(&m_pTexture_Com))))
		return E_FAIL;

	// VIBuffer_Rect Component
	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"),
		reinterpret_cast<CComponent**>(&m_pVIBuffer_Com))))
		return E_FAIL;

	Engine::CTransform::TRANSFORM_DESC Transform_Desc = { 5.f, D3DXToRadian(90.f) };

	// Transform Component
	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"),
		reinterpret_cast<CComponent**>(&m_pTransform_Com), &Transform_Desc)))
		return E_FAIL;

	return S_OK;
}


CLogo* CLogo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLogo* pInstance = new CLogo(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLogo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLogo::Clone(void* pArg)
{
	CGameObject* pInstance = new CLogo(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CLogo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLogo::Free()
{
	__super::Free();

	Safe_Release(m_pTexture_Com);
	Safe_Release(m_pTransform_Com);
	Safe_Release(m_pVIBuffer_Com);
}
