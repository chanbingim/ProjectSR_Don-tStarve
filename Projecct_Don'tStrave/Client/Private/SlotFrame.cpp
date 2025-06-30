#include "SlotFrame.h"

#include "GameInstance.h"
#include "Slot.h"
#include "Inventory.h"

CSlotFrame::CSlotFrame(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CUserInterface{ pGraphic_Device }
{
}

CSlotFrame::CSlotFrame(const CSlotFrame& Prototype)
    : CUserInterface{ Prototype }
{
}

HRESULT CSlotFrame::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CSlotFrame::Initialize(void* pArg)
{
    SLOTFRAME_DESC* pDesc = static_cast<SLOTFRAME_DESC*>(pArg);

    m_eSlotType = static_cast<SLOT>(pDesc->iSlotType);

    CUserInterface::UIOBJECT_DESC Desc = {};

    Desc.fX = pDesc->Desc.fX;
    Desc.fY = pDesc->Desc.fY;
    Desc.fSizeX = pDesc->Desc.fSizeX;
    Desc.fSizeY = pDesc->Desc.fSizeY;

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    __super::UpdatePosition();

    m_pSlot = static_cast<class CSlot*>(m_pGameInstance->Clone_Prototype(
        PROTOTYPE::GAMEOBJECT, EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Slot"), &pDesc->Desc));

    if (nullptr == m_pSlot)
        return E_FAIL;

    return S_OK;
}

void CSlotFrame::Priority_Update(_float fTimeDelta)
{
    m_pSlot->Priority_Update(fTimeDelta);
}

void CSlotFrame::Update(_float fTimeDelta)
{
    //m_pGameInstance->Add_RenderGroup(RENDER::ORTTHO_UI, this);

    HoverEevent();

    
    if (m_eSlotType == m_pSlot->Get_Info().eSlot)
    {
        m_pSlot->Update(fTimeDelta);
    }
    m_pSlot->Update_Count();
    
}

void CSlotFrame::Late_Update(_float fTimeDelta)
{
    m_pSlot->Late_Update(fTimeDelta);
}

HRESULT CSlotFrame::Render()
{
    m_pTexture_Com->Set_Texture(EnumToInt(m_eSlotType));

    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());

    m_pVIBuffer_Com->Render();

    m_pSlot->Render_ItemState();

    m_pSlot->Render(m_pTransform_Com);

    return S_OK;
}

void CSlotFrame::HoverEevent()
{
    RECT rc = { m_fX - m_fSizeX * 0.5f, m_fY - m_fSizeY * 0.5f, m_fX + m_fSizeX * 0.5f, m_fY + m_fSizeY * 0.5f };

    POINT pt{};
    GetCursorPos(&pt);
    ScreenToClient(g_hWnd, &pt);

    if (PtInRect(&rc, pt))
    {
        ClickedEevent();
        m_pTransform_Com->SetScale(_float3(m_fSizeX * 1.2f, m_fSizeY * 1.2f, 1.f));
    }
    else
    {
        m_pTransform_Com->SetScale(_float3(m_fSizeX, m_fSizeY, 1.f));
    }
}

void CSlotFrame::ClickedEevent()
{
    if (m_pGameInstance->KeyDown(VK_LBUTTON))
    {
        CSlot* pSlot = dynamic_cast<CSlot*>(m_pGameInstance->Chagne_Slot());

        if(m_eSlotType == SLOT::NORMAL)
        {
            if (m_pSlot->Get_ItemID() == pSlot->Get_ItemID())
                m_pSlot->Merge_Item(pSlot);
            else
                m_pSlot = dynamic_cast<CSlot*>(m_pGameInstance->Chagne_Slot(m_pSlot));
        }
        else
        {
            if (m_eSlotType == pSlot->Get_Info().eSlot)
                m_pSlot = dynamic_cast<CSlot*>(m_pGameInstance->Chagne_Slot(m_pSlot));
            else if(0 == pSlot->Get_ItemID())
                m_pSlot = dynamic_cast<CSlot*>(m_pGameInstance->Chagne_Slot(m_pSlot));
        }
    }

    if (m_pGameInstance->KeyDown(VK_RBUTTON))
    {
        CInventory* pInventory = dynamic_cast<CInventory*>(m_pGameInstance->Get_GameObject(EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_UserInterface")));
        
        CSlot* pSlot = pInventory->Find_Slot(m_pSlot->Get_Info().eSlot);
        
        if (nullptr == pSlot)
            return;

        ITEM_DESC Desc = pSlot->Get_Info();

        pSlot->Set_Info(m_pSlot->Get_Info());

        m_pSlot->Set_Info(Desc);
    }
}

HRESULT CSlotFrame::ADD_Components()
{
    // Texture Component
    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Slot"),
        TEXT("Com_Texture"),
        reinterpret_cast<CComponent**>(&m_pTexture_Com))))
        return E_FAIL;

    // VIBuffer_Rect Component
    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"),
        reinterpret_cast<CComponent**>(&m_pVIBuffer_Com))))
        return E_FAIL;


    // Transform Component
    Engine::CTransform::TRANSFORM_DESC Desc = { 5.f, D3DXToRadian(90.f) };

    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"),
        reinterpret_cast<CComponent**>(&m_pTransform_Com), &Desc)))
        return E_FAIL;


    return S_OK;
}


CSlotFrame* CSlotFrame::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CSlotFrame* pInstance = new CSlotFrame(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CSlotFrame");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CSlotFrame::Clone(void* pArg)
{
    CSlotFrame* pInstance = new CSlotFrame(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CSlotFrame");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CSlotFrame::Free()
{
    __super::Free();

    Safe_Release(m_pTexture_Com);
    Safe_Release(m_pTransform_Com);
    Safe_Release(m_pVIBuffer_Com);
    Safe_Release(m_pSlot);
}
