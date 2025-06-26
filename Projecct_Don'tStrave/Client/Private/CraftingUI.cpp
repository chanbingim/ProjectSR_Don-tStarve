#include "CraftingUI.h"
#include "GameInstance.h"
#include "Slot.h"
#include "Crafting_Button.h"
#include "QuickSlot_Button.h"
#include "Item_Info.h"

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
    m_pQuickSlots.reserve(5);

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    __super::UpdatePosition();

    CButton::BUTTON_DESC Desc = {};
    Desc.fX = m_fX;
    Desc.fY = m_fY;
    Desc.fSizeX = 60.f;
    Desc.fSizeY = 60.f;
    Desc.fRelativeX = m_fSizeX * 0.52f;
    Desc.fRelativeY = m_fSizeY * 0.5f - 60.f;
    Desc.pParentTransform = m_pTransform_Com;

    m_pOpenButton = dynamic_cast<CCrafting_Button*>(m_pGameInstance->Clone_Prototype(
        PROTOTYPE::GAMEOBJECT, EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Crafting_Button"), &Desc));

    for (_uint i = 0; i < 5; ++i)
    {
        Desc.fX = m_fX;
        Desc.fY = m_fY;
        Desc.fSizeX = 60.f;
        Desc.fSizeY = 60.f;
        Desc.fRelativeX = m_fSizeX * 0.52f;
        Desc.fRelativeY = m_fSizeY * 0.5f - 120.f - i * 60.f;
        Desc.pParentTransform = m_pTransform_Com;

        m_pQuickSlots.push_back(dynamic_cast<CQuickSlot_Button*>(m_pGameInstance->Clone_Prototype(
            PROTOTYPE::GAMEOBJECT, EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_QuickSlot_Button"), &Desc)));
    }

    CUserInterface::UIOBJECT_DESC UI_Desc = {};

    UI_Desc.fX = m_fX;
    UI_Desc.fY = m_fY;
    UI_Desc.fSizeX = m_fSizeX;
    UI_Desc.fSizeY = m_fSizeY;

    m_pItem_Info = dynamic_cast<CItem_Info*>(m_pGameInstance->Clone_Prototype(
        PROTOTYPE::GAMEOBJECT, EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Item_Info"), &UI_Desc));

    return S_OK;
}

void CCraftingUI::Priority_Update(_float fTimeDelta)
{
}

void CCraftingUI::Update(_float fTimeDelta)
{
    HoverEevent();

    ClickedEevent();

    m_pOpenButton->Update(fTimeDelta);

    for (auto qButton : m_pQuickSlots)
        qButton->Update(fTimeDelta);

    m_pGameInstance->Add_RenderGroup(RENDER::ORTTHO_UI, this);
}

void CCraftingUI::Late_Update(_float fTimeDelta)
{
}

HRESULT CCraftingUI::Render()
{
    m_pOpenButton->Render();

    for (auto qButton : m_pQuickSlots)
        qButton->Render();

    m_pTexture_Com->Set_Texture(0);

    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());

    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    m_pVIBuffer_Com->Render();

    m_pItem_Info->Render();

    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    
    return S_OK;
}

void CCraftingUI::HoverEevent()
{
}

void CCraftingUI::ClickedEevent()
{
    if (true == m_pOpenButton->OnClick())
    {
        _float3 vPos = m_pTransform_Com->GetWorldState(WORLDSTATE::POSITION);
        
        if (true == m_bHide)
        {
            m_pTransform_Com->SetPosition(_float3((vPos.x + m_fSizeX * 0.95f), vPos.y, vPos.z));
            m_fX += m_fSizeX;
            m_bHide = false;
        }
        else
        {
            m_pTransform_Com->SetPosition(_float3((vPos.x - m_fSizeX * 0.95f), vPos.y, vPos.z));
            m_fX += m_fSizeX * -1.f;
            m_bHide = true;
        }
    }
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

    Safe_Release(m_pOpenButton);
    Safe_Release(m_pItem_Info);

    for (auto pButton : m_pQuickSlots)
        Safe_Release(pButton);

    m_pQuickSlots.clear();
}
