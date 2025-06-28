#include "Clock.h"

#include "GameInstance.h"

CClock::CClock(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CUserInterface{ pGraphic_Device }
{
}

CClock::CClock(const CClock& Prototype)
    : CUserInterface{ Prototype }
{
}

HRESULT CClock::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CClock::Initialize(void* pArg)
{
    m_iDate = 1;
    CUserInterface::UIOBJECT_DESC Desc = {};

    Desc.fX = g_iWinSizeX - 80.f;
    Desc.fY = 70.f;
    Desc.fSizeX = 120.f;
    Desc.fSizeY = 120.f;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    __super::UpdatePosition();

    m_pTransform_Com_Clock->SetScale(_float3(m_fSizeX*0.8f, m_fSizeY* 0.8f, 1.f));
    m_pTransform_Com_Clock->SetPosition(_float3(m_fX - m_fWinSizeX * 0.5f, -m_fY + m_fWinSizeY * 0.5f, 0.f));

    return S_OK;
}

void CClock::Priority_Update(_float fTimeDelta)
{
}

void CClock::Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::ORTTHO_UI, this);

    m_fGameTime += fTimeDelta;

   if (60.f <= m_fGameTime)
    {
        m_iDate += 1;
        m_fGameTime = 0.f;
    }
   m_pTransform_Com_Clock->SetRotAxis(_float3(0.f, 0.f, 1.f), -0.1f * m_fGameTime);
}

void CClock::Late_Update(_float fTimeDelta)
{
}

HRESULT CClock::Render()
{
    m_pTexture_Com->Set_Texture(0);

    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());

    m_pVIBuffer_Com->Render();

    m_pTexture_Com_Clock->Set_Texture(0);

    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com_Clock->Get_World());

    m_pVIBuffer_Com->Render();

    // Test Code
    RECT rect = { m_fX - m_fSizeX*0.5f, m_fY - m_fSizeY * 0.5f,m_fX + m_fSizeX * 0.5f,m_fY + m_fSizeY * 0.5f };
    wstring s = to_wstring(m_iDate) + L"ÀÏ";

    D3DXCOLOR white = { 1.f,1.f,1.f,1.f };
    m_pGameInstance->Render_Font(TEXT("Date_40"), s.c_str(), &rect, white);

    return S_OK;
}

HRESULT CClock::ADD_Components()
{
    // Texture Component
    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Clock_Frame"),
        TEXT("Com_Texture"),
        reinterpret_cast<CComponent**>(&m_pTexture_Com))))
        return E_FAIL;

    // Texture Component
    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Clock_Point"),
        TEXT("Com_Texture_Clock"),
        reinterpret_cast<CComponent**>(&m_pTexture_Com_Clock))))
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
        TEXT("Com_Transform_Clock"),
        reinterpret_cast<CComponent**>(&m_pTransform_Com_Clock), &Transform_Desc)))
        return E_FAIL;


    return S_OK;
}

CClock* CClock::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CClock* pInstance = new CClock(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CClock");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CClock::Clone(void* pArg)
{
    CGameObject* pInstance = new CClock(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CClock");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CClock::Free()
{
    __super::Free();

    Safe_Release(m_pTexture_Com);
    Safe_Release(m_pTransform_Com);
    Safe_Release(m_pVIBuffer_Com);

    Safe_Release(m_pTexture_Com_Clock);
    Safe_Release(m_pTransform_Com_Clock);
}
