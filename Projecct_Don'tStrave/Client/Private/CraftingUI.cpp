#include "CraftingUI.h"
#include "GameInstance.h"
#include "Slot.h"
#include "Crafting_Button.h"
#include "QuickSlot_Button.h"
#include "Category_Button.h"
#include "Item_Info.h"
#include "Item_Button.h"

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

    m_iCategoryIndex = 0;

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

    for(_uint i = 0; i < 6; ++i)
    {
        Desc.iTextureIndex = i;
        Desc.fX = m_fX;
        Desc.fY = m_fY;
        Desc.fSizeX = 30.f;
        Desc.fSizeY = 30.f;
        Desc.fRelativeX = -m_fSizeX * 0.35f + i * 35.f;
        Desc.fRelativeY = m_fSizeY * 0.35f;
        Desc.pParentTransform = m_pTransform_Com;
        m_pCategorys.push_back(dynamic_cast<CCategory_Button*>(m_pGameInstance->Clone_Prototype(
            PROTOTYPE::GAMEOBJECT, EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Category_Button"), &Desc)));
    }

    _uint ItemButtons = 1;

    for (_uint i = 0; i < 5; ++i)
    {
        Desc.iTextureIndex = ItemButtons++;
        Desc.fX = m_fX;
        Desc.fY = m_fY;
        Desc.fSizeX = 50.f;
        Desc.fSizeY = 50.f;
        Desc.fRelativeX = -m_fSizeX * 0.34f + i * 60.f;
        Desc.fRelativeY = m_fSizeY * 0.15f;
        Desc.pParentTransform = m_pTransform_Com;
        CItem_Button* pItem_Button = dynamic_cast<CItem_Button*>(m_pGameInstance->Clone_Prototype(
            PROTOTYPE::GAMEOBJECT, EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Item_Button"), &Desc));

        if (nullptr != pItem_Button)
            m_pItem_Buttons[0].push_back(pItem_Button);
    }
    for (_uint i = 0; i < 3; ++i)
    {
        Desc.iTextureIndex = ItemButtons++;
        Desc.fX = m_fX;
        Desc.fY = m_fY;
        Desc.fSizeX = 50.f;
        Desc.fSizeY = 50.f;
        Desc.fRelativeX = -m_fSizeX * 0.34f + i * 60.f;
        Desc.fRelativeY = m_fSizeY * 0.15f;
        Desc.pParentTransform = m_pTransform_Com;
        CItem_Button* pItem_Button = dynamic_cast<CItem_Button*>(m_pGameInstance->Clone_Prototype(
            PROTOTYPE::GAMEOBJECT, EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Item_Button"), &Desc));

        if (nullptr != pItem_Button)
            m_pItem_Buttons[1].push_back(pItem_Button);
    }
    for (_uint i = 0; i < 2; ++i)
    {
        Desc.iTextureIndex = ItemButtons++;
        Desc.fX = m_fX;
        Desc.fY = m_fY;
        Desc.fSizeX = 50.f;
        Desc.fSizeY = 50.f;
        Desc.fRelativeX = -m_fSizeX * 0.34f + i * 60.f;
        Desc.fRelativeY = m_fSizeY * 0.15f;
        Desc.pParentTransform = m_pTransform_Com;
        CItem_Button* pItem_Button = dynamic_cast<CItem_Button*>(m_pGameInstance->Clone_Prototype(
            PROTOTYPE::GAMEOBJECT, EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Item_Button"), &Desc));

        if (nullptr != pItem_Button)
        {
            m_pItem_Buttons[2].push_back(pItem_Button);
        }
    }
    for (_uint i = 0; i < 2; ++i)
    {
        Desc.iTextureIndex = ItemButtons++;
        Desc.fX = m_fX;
        Desc.fY = m_fY;
        Desc.fSizeX = 50.f;
        Desc.fSizeY = 50.f;
        Desc.fRelativeX = -m_fSizeX * 0.34f + i * 60.f;
        Desc.fRelativeY = m_fSizeY * 0.15f;
        Desc.pParentTransform = m_pTransform_Com;
        CItem_Button* pItem_Button = dynamic_cast<CItem_Button*>(m_pGameInstance->Clone_Prototype(
            PROTOTYPE::GAMEOBJECT, EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Item_Button"), &Desc));

        if (nullptr != pItem_Button)
        {
            m_pItem_Buttons[3].push_back(pItem_Button);
        }
    }
    for (_uint i = 0; i < 2; ++i)
    {
        Desc.iTextureIndex = ItemButtons++;
        Desc.fX = m_fX;
        Desc.fY = m_fY;
        Desc.fSizeX = 50.f;
        Desc.fSizeY = 50.f;
        Desc.fRelativeX = -m_fSizeX * 0.34f + i * 60.f;
        Desc.fRelativeY = m_fSizeY * 0.15f;
        Desc.pParentTransform = m_pTransform_Com;
        CItem_Button* pItem_Button = dynamic_cast<CItem_Button*>(m_pGameInstance->Clone_Prototype(
            PROTOTYPE::GAMEOBJECT, EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Item_Button"), &Desc));

        if (nullptr != pItem_Button)
        {
            m_pItem_Buttons[4].push_back(pItem_Button);
        }
    }
    for (_uint i = 0; i < 4; ++i)
    {
        Desc.iTextureIndex = ItemButtons++;
        Desc.fX = m_fX;
        Desc.fY = m_fY;
        Desc.fSizeX = 50.f;
        Desc.fSizeY = 50.f;
        Desc.fRelativeX = -m_fSizeX * 0.34f + i * 60.f;
        Desc.fRelativeY = m_fSizeY * 0.15f;
        Desc.pParentTransform = m_pTransform_Com;
        CItem_Button* pItem_Button = dynamic_cast<CItem_Button*>(m_pGameInstance->Clone_Prototype(
            PROTOTYPE::GAMEOBJECT, EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Item_Button"), &Desc));

        if (nullptr != pItem_Button)
        {
            m_pItem_Buttons[5].push_back(pItem_Button);
        }
    }
    
    m_pCategorys[0]->Select_Button();
    for (_uint i = 0; i < 6; ++i)
    {
        m_pItem_Buttons[i][0]->Select_Button();
        m_iItemBtnIndex[i] = 0;
    }
    

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

    m_pItem_Info->Update_Rect(m_fX, m_fY);
    
    // 카테고리 버튼 업데이트
    _uint iNumCategory = m_pCategorys.size();

    for (_uint i = 0; i < iNumCategory; ++i)
    {
        m_pCategorys[i]->Update(fTimeDelta);
        if (m_iCategoryIndex != i && m_pCategorys[i]->OnClick())
        {
            m_pCategorys[m_iCategoryIndex]->Select_Button();
            m_pCategorys[i]->Select_Button();
            m_iCategoryIndex = i;
            break;
        }
    }

    // 아이템 버튼 업데이트
    _uint iNumItemBtn = m_pItem_Buttons[m_iCategoryIndex].size();

    for (_uint i = 0; i < iNumItemBtn; ++i)
    {
        m_pItem_Buttons[m_iCategoryIndex][i]->Update(fTimeDelta);
        if (m_iItemBtnIndex[m_iCategoryIndex] != i && m_pItem_Buttons[m_iCategoryIndex][i]->OnClick())
        {
            m_pItem_Buttons[m_iCategoryIndex][m_iItemBtnIndex[m_iCategoryIndex]]->Select_Button();
            m_pItem_Buttons[m_iCategoryIndex][i]->Select_Button();
            m_iItemBtnIndex[m_iCategoryIndex] = i;
            break;
        }
    }
    m_pItem_Info->Set_ITem(m_pItem_Buttons[m_iCategoryIndex][m_iItemBtnIndex[m_iCategoryIndex]]->Get_ItemID());

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

    m_pVIBuffer_Com->Render();

    m_pItem_Info->Render(m_pTransform_Com);

    for (auto pButton : m_pCategorys)
        pButton->Render();

    for (auto pItemBtn : m_pItem_Buttons[m_iCategoryIndex])
        pItemBtn->Render();

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

    for (auto pButton : m_pCategorys)
        Safe_Release(pButton);
    m_pCategorys.clear();

    for (_uint i = 0; i < 6; ++i)
    {
        for (auto pButton : m_pItem_Buttons[i])
            Safe_Release(pButton);
        m_pItem_Buttons[i].clear();
    }
    
}
