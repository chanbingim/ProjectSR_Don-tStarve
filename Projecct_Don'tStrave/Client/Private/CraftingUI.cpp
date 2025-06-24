#include "CraftingUI.h"
#include "GameInstance.h"
#include "Slot.h"

CCraftingUI::CCraftingUI(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CUserInterface{ pGraphic_Device }
{
}

CCraftingUI::CCraftingUI(const CCraftingUI& Prototype)
    : CUserInterface{ Prototype }
{
}

HRESULT CCraftingUI::Initialize_Prototype()
{

    return S_OK;
}

HRESULT CCraftingUI::Initialize(void* pArg)
{
    m_bHide = false;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    __super::UpdatePosition();


    return S_OK;
}

void CCraftingUI::Priority_Update(_float fTimeDelta)
{
}

void CCraftingUI::Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::ORTTHO_UI, this);
    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
    {
        _float3 vPos = m_pTransform_Com->GetWorldState(WORLDSTATE::POSITION);
        RECT rect = { m_fX - m_fSizeX * 0.5f, m_fY - m_fSizeY * 0.5f, m_fX + m_fSizeX * 0.5f, m_fY + m_fSizeY * 0.5f };

        POINT pt{};
        GetCursorPos(&pt);
        ScreenToClient(g_hWnd, &pt);

        if (true == PtInRect(&rect, pt))
        {
            if (true == m_bHide)
            {
                m_pTransform_Com->SetPosition(_float3((vPos.x + m_fSizeX * 0.8f), vPos.y, vPos.z));
                m_fX += m_fSizeX * 0.8f;
                m_bHide = false;
            }
            else
            {
                m_pTransform_Com->SetPosition(_float3((vPos.x - m_fSizeX * 0.8f), vPos.y, vPos.z));
                m_fX += m_fSizeX * -0.8f;
                m_bHide = true;
            }
        }
    }
}

void CCraftingUI::Late_Update(_float fTimeDelta)
{
}

HRESULT CCraftingUI::Render()
{
    m_pTexture_Com->Set_Texture(0);

    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());

    m_pVIBuffer_Com->Render();


    return S_OK;
}

HRESULT CCraftingUI::ADD_Components()
{
    // Texture Component
    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_SideBar"),
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

CCraftingUI* CCraftingUI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CCraftingUI* pInstance = new CCraftingUI(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CCraftingUI");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CCraftingUI::Clone(void* pArg)
{
    CCraftingUI* pInstance = new CCraftingUI(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CCraftingUI");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CCraftingUI::Free()
{
    __super::Free();

    Safe_Release(m_pTexture_Com);
    Safe_Release(m_pTransform_Com);
    Safe_Release(m_pVIBuffer_Com);
}
