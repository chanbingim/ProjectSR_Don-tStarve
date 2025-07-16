#include "Logo.h"

#include "GameInstance.h"

#include "Level_Loading.h"

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
	m_iButtonIndex = 5;

	CUserInterface::UIOBJECT_DESC Desc = {};

	Desc.fSizeX = 1280.f;
	Desc.fSizeY = 720.f;
	Desc.fX  = g_iWinSizeX * 0.5f;
	Desc.fY  = g_iWinSizeY * 0.5f;

	m_Color = D3DXCOLOR(0.749f, 0.737f, 0.416f, 1.0f);
	for (long i = 0; i < 4; i++)
	{
		m_rcButton[i] = { 150, 400 + 50 * i, 300, 440+ 50 * i };
	}

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	__super::UpdatePosition();

	m_pButton_Transform_Com->SetScale(_float3(220.f, 60.f, 1.f));
	m_pButton_Transform_Com->SetPosition(_float3(-420.f, -65.f, 1.f));

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
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 50);

	m_pTexture_Com->Set_Texture(0);

	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());

	m_pVIBuffer_Com->Render();

	POINT       ptMouse = {};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	for(_uint i = 0; i < 4 ; ++i)
	{
		if (PtInRect(&m_rcButton[i], ptMouse))
		{
			m_iButtonIndex = i;
			m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pButton_Transform_Com->Get_World());
			m_pButton_Texture_Com->Set_Texture(0);
			m_pVIBuffer_Com->Render();
			m_pButton_Transform_Com->SetPosition(_float3(-420.f, -65.f - i * 50.f, 1.f));
		}
	}
	

	m_pGameInstance->Render_Font(TEXT("Date_40"), TEXT("Game Start"), &m_rcButton[0], m_Color);
	m_pGameInstance->Render_Font(TEXT("Date_40"), TEXT("Option"), &m_rcButton[1], m_Color);
	m_pGameInstance->Render_Font(TEXT("Date_40"), TEXT("Mods"), &m_rcButton[2], m_Color);
	m_pGameInstance->Render_Font(TEXT("Date_40"), TEXT("Quit"), &m_rcButton[3], m_Color);


	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);

	return S_OK;
}

HRESULT CLogo::ADD_Components()
{
	// Texture Component
	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::LOGO), TEXT("Prototype_Component_Texture_Logo"),
		TEXT("Com_Texture"),
		reinterpret_cast<CComponent**>(&m_pTexture_Com))))
		return E_FAIL;

	// Texture Component
	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::LOGO), TEXT("Prototype_Component_Texture_Logo_Button"),
		TEXT("Com_Button_Texture"),
		reinterpret_cast<CComponent**>(&m_pButton_Texture_Com))))
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

	// Transform Component
	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform_Button"),
		reinterpret_cast<CComponent**>(&m_pButton_Transform_Com), &Transform_Desc)))
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
	Safe_Release(m_pButton_Texture_Com);
	Safe_Release(m_pButton_Transform_Com);
}
