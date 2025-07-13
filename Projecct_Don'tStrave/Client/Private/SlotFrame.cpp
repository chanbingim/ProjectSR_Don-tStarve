#include "SlotFrame.h"

#include "GameInstance.h"
#include "Item_Manager.h"

#include "Slot.h"
#include "Inventory.h"
#include "Mouse.h"
#include "Player.h"

CSlotFrame::CSlotFrame(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CUserInterface{ pGraphic_Device }
{
}

CSlotFrame::CSlotFrame(const CSlotFrame& Prototype)
    : CUserInterface{ Prototype }
{
}

void CSlotFrame::Update_SlotPosition(_float fX, _float fY)
{
    m_fX = fX;
    m_fY = fY;

    __super::UpdatePosition();

    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());
}

HRESULT CSlotFrame::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CSlotFrame::Initialize(void* pArg)
{
    SLOTFRAME_DESC* pDesc = static_cast<SLOTFRAME_DESC*>(pArg);

    m_eSlotType = static_cast<SLOT>(pDesc->iSlotType);

    if(SLOT::HAND ==  m_eSlotType)
    {
        CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(
            EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_Player")));

        pPlayer->Set_EquipmentSlot(this);
    }

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

    m_pMouse = dynamic_cast<CMouse*>(m_pGameInstance->Get_GameObject(EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_Mouse")));

    if (nullptr == m_pMouse)
        return E_FAIL;

    

    return S_OK;
}

void CSlotFrame::Priority_Update(_float fTimeDelta)
{
    m_pSlot->Priority_Update(fTimeDelta);
}

void CSlotFrame::Update(_float fTimeDelta)
{

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

void CSlotFrame::Update_IceBox(_float fTimeDelta)
{
    HoverEevent();

    if (m_eSlotType == m_pSlot->Get_Info().eSlot)
    {
        m_pSlot->Update_IceBox(fTimeDelta);
    }
    m_pSlot->Update_Count();
}

void CSlotFrame::Update_Equipment()
{
    ITEM_DESC Desc = m_pSlot->Get_Info();

    if (Desc.iItemID == 0)
        return;

    if(10 > Desc.iItemID)
        Desc.fDurability -= 5.f;
    else
        Desc.fDurability -= 1.f;

    if (0 >= Desc.fDurability)
    {
        Swap_HandObject(0);
        m_pSlot->Clear();
        return;
    }
    m_pSlot->Set_Info(Desc);
}

void CSlotFrame::Swap_HandObject(_uint iItemID)
{
    CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_Player")));
    PLAYER_DATA* pData = pPlayer->Get_Player();
    switch (iItemID)
    {
    case 0:
        pData->tItem = SWAPOBJECT::NONE;
        pData->iAtk = 1;
        break;

    case 1:  // 1 번 도끼
        pData->tItem = SWAPOBJECT::AXE;
        pData->iAtk = 27;
        break;

    case 2:  // 곡괭이 
        pData->tItem = SWAPOBJECT::PICKAXE;
        pData->iAtk = 27;
        break;

    case 3:  //삽
        pData->tItem = SWAPOBJECT::SHOVEL;
        pData->iAtk = 10;
        break;

    case 4:  // 횃불
        pData->tItem = SWAPOBJECT::TORCH;
        pData->iAtk = 17;
        break;

    case 8:  // 창
        pData->tItem = SWAPOBJECT::SPEAR;
        pData->iAtk = 34;
        break;

    case 19:  // 금 도끼
        pData->tItem = SWAPOBJECT::GOLDAXE;
        pData->iAtk = 27;
        break;

    case 20:  // 금 곡괭이
        pData->tItem = SWAPOBJECT::GOLDPICKAXE;
        pData->iAtk = 27;
        break;

    case 21:  // 금 삽
        pData->tItem = SWAPOBJECT::GOLDSHOVEL;
        pData->iAtk = 10;
        break;

    case 22:  // 뇌전창
        pPlayer->Get_Player()->tItem = SWAPOBJECT::LIGHTNINGSPEAR;
        pData->iAtk = 50;
        break;

    default:
        break;
    }
    
}

void CSlotFrame::HoverEevent()
{
    RECT rc = { (LONG)(m_fX - m_fSizeX * 0.5f), (LONG)(m_fY - m_fSizeY * 0.5f), (LONG)(m_fX + m_fSizeX * 0.5f), (LONG)(m_fY + m_fSizeY * 0.5f) };

    POINT pt{};
    GetCursorPos(&pt);
    ScreenToClient(g_hWnd, &pt);

    if (PtInRect(&rc, pt))
    {
        ClickedEevent();
        _uint iItemID = m_pSlot->Get_ItemID();
        if (0 != iItemID)
            m_pMouse->Update_HoverSlot(iItemID);
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

        if(m_eSlotType == SLOT::NORMAL) // 인반 인벤토리
        {
            if (m_pSlot->Get_ItemID() == pSlot->Get_ItemID())
                m_pSlot->Merge_Item(pSlot);
            else
                m_pSlot = dynamic_cast<CSlot*>(m_pGameInstance->Chagne_Slot(m_pSlot));
        }
        else if (m_eSlotType == SLOT::POT) // 요리솥
        {
            ITEM_DESC Desc = pSlot->Get_Info();

            // 슬롯이 비었고 손에 음식이나 재료
            if ((Desc.eItemType == ITEM_TYPE::FOOD || Desc.eItemType == ITEM_TYPE::MERTARIAL) && 0 == m_pSlot->Get_ItemID())
            {
                if (1 == Desc.iNumItem) // 손에 있는게 1개
                {
                    m_pSlot->Set_Info(Desc);
                    pSlot->Clear();
                }
                else // 여러개
                {
                    Desc.iNumItem -= 1;
                    pSlot->Set_Info(Desc);

                    Desc.iNumItem = 1;
                    m_pSlot->Set_Info(Desc);
                }
               
            }
            else if (0 == Desc.iItemID && 0 != m_pSlot->Get_ItemID()) // 마우스에만 없을 때
            {
                pSlot->Set_Info(m_pSlot->Get_Info());

                m_pSlot->Clear();
                return;
            }
            else
            {
                return;
            }
                
        } 
        else if (m_eSlotType == SLOT::HAND)
        {
            _uint iItemID = pSlot->Get_ItemID();

            // 무기 변경
            Swap_HandObject(iItemID);

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
        if (SLOT::NORMAL == m_eSlotType)
        {
            ITEM_DESC Desc = m_pSlot->Get_Info();
            
            if (ITEM_TYPE::FOOD == Desc.eItemType)
            {
                ITEM_DATA Item_Data = CItem_Manager::GetInstance()->Get_ItemData(Desc.iItemID);

                CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_Player")));

                if (pPlayer->Eat(&Item_Data)) {
                    m_pSlot->Use_One();
                }

            }
            else if (ITEM_TYPE::EQUIPMENT == Desc.eItemType)
            {
                CInventory* pInventory = dynamic_cast<CInventory*>(m_pGameInstance->Get_GameObject(EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_UserInterface")));

                CSlot* pSlot = pInventory->Find_Slot(m_pSlot->Get_Info().eSlot);

                Swap_HandObject(m_pSlot->Get_ItemID());

                ITEM_DESC Desc = pSlot->Get_Info();

                pSlot->Set_Info(m_pSlot->Get_Info());

                m_pSlot->Set_Info(Desc);
            }
        }
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
