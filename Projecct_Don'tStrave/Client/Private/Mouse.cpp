#include "Mouse.h"

#include "GameInstance.h"
#include "Slot.h"

CMouse::CMouse(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CUserInterface{ pGraphic_Device }
{
}

CMouse::CMouse(const CMouse& Prototype)
    : CUserInterface{ Prototype }
{
}

HRESULT CMouse::Initialize_Prototype()
{

    return S_OK;
}

HRESULT CMouse::Initialize(void* pArg)
{

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    m_pTransform_Com->SetScale(_float3(m_fSizeX, m_fSizeY, 1.f));

    if (FAILED(Add_Slot()))
        return E_FAIL;

    return S_OK;
}

void CMouse::Priority_Update(_float fTimeDelta)
{
    m_pSlot = dynamic_cast<CSlot*>(m_pGameInstance->Chagne_Slot());
    m_pSlot->Priority_Update(fTimeDelta);
}

void CMouse::Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::ORTTHO_UI, this);

    POINT pt{};
    GetCursorPos(&pt);
    ScreenToClient(g_hWnd, &pt);
    m_fX = pt.x - g_iWinSizeX * 0.5f;
    m_fY = -pt.y + g_iWinSizeY * 0.5f;
    
    
    m_pTransform_Com->SetPosition(_float3(m_fX, m_fY, 0.f));

    if (GetKeyState('1') & 0x8000)
    {
        m_pSlot->Set_Info(1, 1, 50.f);
    }
    if (GetKeyState('2') & 0x8000)
    {
        m_pSlot->Set_Info(2, 1, 50.f);

    }
    if (GetKeyState('5') & 0x8000)
    {
        m_pSlot->Set_Info(5, 1, 50.f);
    }

    m_pSlot->Update(fTimeDelta);
}

void CMouse::Late_Update(_float fTimeDelta)
{

}

HRESULT CMouse::Render()
{
    m_pTexture_Com->Set_Texture(0);

    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());

    m_pVIBuffer_Com->Render();

    m_pSlot->Render();

    return S_OK;
}
HRESULT CMouse::ADD_Components()
{
    // Texture Component
    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Item"),
        TEXT("Com_Texture_Item"),
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

HRESULT CMouse::Add_Slot()
{
    CUserInterface::UIOBJECT_DESC Desc = {};

    Desc.fSizeX = 50.f;
    Desc.fSizeY = 50.f;
    Desc.fX = m_fX;
    Desc.fY = m_fY;

    m_pSlot = static_cast<CSlot*>(m_pGameInstance->Clone_Prototype(
        PROTOTYPE::GAMEOBJECT, EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Slot"), &Desc));

    if (nullptr == m_pSlot)
        return E_FAIL;

    m_pGameInstance->Chagne_Slot(m_pSlot);

    return S_OK;
}

CMouse* CMouse::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CMouse* pInstance = new CMouse(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CMouse");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CMouse::Clone(void* pArg)
{
    CMouse* pInstance = new CMouse(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CMouse");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMouse::Free()
{
    __super::Free();

    Safe_Release(m_pSlot);
    Safe_Release(m_pTexture_Com);
    Safe_Release(m_pTransform_Com);
    Safe_Release(m_pVIBuffer_Com);

}
