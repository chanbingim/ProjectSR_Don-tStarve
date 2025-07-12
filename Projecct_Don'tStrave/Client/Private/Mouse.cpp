#include "Mouse.h"

#include "GameInstance.h"
#include "Item_Manager.h"

#include "Terrian_Manager.h"
#include "Terrain.h"

#include "Slot.h"
#include "Item.h"
#include "Player.h"
#include "Grid.h"
#include "SkillIndicator.h"

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
    m_bPutDown = false;
    m_iMouseState = 0;
    m_strInfoMessage = L"";
    m_strInteraction = L"";

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    m_pTransform_Com->SetScale(_float3(m_fSizeX, m_fSizeY, 1.f));


    if (FAILED(Add_Slot()))
        return E_FAIL;
    m_fSizeX = 80.f;
    m_fSizeY = 80.f;

    __super::UpdatePosition();

    m_pBlend_Texture_Com->Set_Texture(0, 1);

    m_pPlayerTransform_Com = static_cast<CTransform*>(m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Player"), TEXT("Com_Transform"), 0));

    m_pPlayer_Data = static_cast<CPlayer*>(m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Player"), 0))->Get_Player();

    Safe_AddRef(m_pPlayerTransform_Com);

    m_pGrid = dynamic_cast<CGrid*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Grid")));

    m_pSkillIndicator = dynamic_cast<CSkillIndicator*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_SkillIndicator")));

    return S_OK;
}

void CMouse::Priority_Update(_float fTimeDelta)
{
    
    m_pSlot = dynamic_cast<CSlot*>(m_pGameInstance->Chagne_Slot());
    m_pSlot->Priority_Update(fTimeDelta);

    m_strInfoMessage = L"";
    m_iMouseState = 0;

}

void CMouse::Update(_float fTimeDelta)
{

    m_pSlot->Update_Count();

    POINT pt{};
    GetCursorPos(&pt);
    ScreenToClient(g_hWnd, &pt);

    m_fX = (_float)pt.x;
    m_fY = (_float)pt.y;
 
    //m_pTransform_Com->SetPosition(_float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
    __super::UpdatePosition();

    m_pSkillIndicator->Update(fTimeDelta);
    
#pragma region TestCode
    ITEM_DESC Desc = {};
    if (GetKeyState('1') & 0x8000)
    {
        Desc.iItemID = 35;
        Desc.eItemType = ITEM_TYPE::MERTARIAL;
        Desc.eSlot = SLOT::NORMAL;
        Desc.iNumItem = 1;
        Desc.fDurability = 100.f;

        m_pSlot->Set_Info(Desc);
    }
    if (GetKeyState('2') & 0x8000)
    {
        Desc.iItemID = 9;
        Desc.eItemType = ITEM_TYPE::EQUIPMENT;
        Desc.eSlot = SLOT::HAND;
        Desc.iNumItem = 1;
        Desc.fDurability = 100.f;

        m_pSlot->Set_Info(Desc);
    }
    if (GetKeyState('3') & 0x8000)
    {
        Desc.iItemID = 19;
        Desc.eItemType = ITEM_TYPE::EQUIPMENT;
        Desc.eSlot = SLOT::HAND;
        Desc.iNumItem = 1;
        Desc.fDurability = 100.f;

        m_pSlot->Set_Info(Desc);
    }
    if (GetKeyState('4') & 0x8000)
    {
        Desc.iItemID = 19;
        Desc.eItemType = ITEM_TYPE::EQUIPMENT;
        Desc.eSlot = SLOT::HAND;
        Desc.iNumItem = 1;
        Desc.fDurability = 100.f;

        m_pSlot->Set_Info(Desc);
    }
    if (GetKeyState('5') & 0x8000)
    {
        Desc.iItemID = 14;
        Desc.eItemType = ITEM_TYPE::STRUCTURE;
        Desc.eSlot = SLOT::NORMAL;
        Desc.iNumItem = 1;
        Desc.fDurability = 100.f;

        m_pSlot->Set_Info(Desc);
    }
    if (GetKeyState('6') & 0x8000)
    {
        Desc.iItemID = 7;
        Desc.eItemType = ITEM_TYPE::STRUCTURE;
        Desc.eSlot = SLOT::NORMAL;
        Desc.iNumItem = 1;
        Desc.fDurability = 100.f;

        m_pSlot->Set_Info(Desc);
    }
    if (GetKeyState('7') & 0x8000)
    {
        Desc.iItemID = 46;
        Desc.eItemType = ITEM_TYPE::FOOD;
        Desc.eSlot = SLOT::NORMAL;
        Desc.iNumItem = 1;
        Desc.fDurability = 100.f;

        m_pSlot->Set_Info(Desc);
    }
    if (GetKeyState('8') & 0x8000)
    {
        Desc.iItemID = 5;
        Desc.eItemType = ITEM_TYPE::STRUCTURE;
        Desc.eSlot = SLOT::NORMAL;
        Desc.iNumItem = 1;
        Desc.fDurability = 100.f;

        m_pSlot->Set_Info(Desc);
    }
    if (GetKeyState('9') & 0x8000)
    {
        Desc.iItemID = 12;
        Desc.eItemType = ITEM_TYPE::STRUCTURE;
        Desc.eSlot = SLOT::NORMAL;
        Desc.iNumItem = 1;
        Desc.fDurability = 100.f;

        m_pSlot->Set_Info(Desc);
    }
    if (GetKeyState('0') & 0x8000)
    {
        Desc.iItemID = 13;
        Desc.eItemType = ITEM_TYPE::STRUCTURE;
        Desc.eSlot = SLOT::NORMAL;
        Desc.iNumItem = 1;
        Desc.fDurability = 100.f;

        m_pSlot->Set_Info(Desc);
    }
#pragma endregion
    
    ITEM_DESC Slot_Desc = m_pSlot->Get_Info();

    m_eType = Slot_Desc.eItemType;

    if (ITEM_TYPE::STRUCTURE == m_eType)
    {
        m_pGrid->Update(fTimeDelta);
    }
    else if (ITEM_TYPE::FOOD == m_eType)
    {
        m_pSlot->Update(fTimeDelta);
    }

        
}

void CMouse::Late_Update(_float fTimeDelta)
{
    ClickedEevent();
    m_pGameInstance->Add_RenderGroup(RENDER::ORTTHO_UI, this);

}

HRESULT CMouse::Render()
{
    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());

    if (ITEM_TYPE::STRUCTURE == m_eType)
    {
        m_pGraphic_Device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADD);
        m_pGraphic_Device->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT); // Stage0 °á°ú
        m_pGraphic_Device->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE | D3DTA_ALPHAREPLICATE);

        m_pSlot->Render(m_pTransform_Com);

        m_pGraphic_Device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
        m_pGraphic_Device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    }
    else
    {
        m_pSlot->Render(m_pTransform_Com);
    }

    if (0 != m_iMouseState)
    {
        m_fX -= 30.f;
        m_fY -= 40.f;
        m_pTexture_Com->Set_Texture(m_iMouseState - 1);
        __super::UpdatePosition();
        m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());
        m_pVIBuffer_Com->Render();


        D3DXCOLOR white = { 0.95f, 0.95f, 0.95f, 1.f };
        RECT Rect = { LONG(m_fX+ 30.f - m_fSizeX), LONG(m_fY - m_fSizeY - 40),LONG(m_fX + 30.f + m_fSizeX), LONG(m_fY + m_fSizeY - 40) };
        m_pGameInstance->Render_Font(TEXT("MouseInfo_40"), m_strInfoMessage.c_str(), &Rect, white);

        Rect = { LONG(m_fX + 60.f - m_fSizeX* 0.5f), LONG(m_fY - m_fSizeY),LONG(m_fX + 50.f + m_fSizeX), LONG(m_fY + m_fSizeY) };
        m_pGameInstance->Render_Font(TEXT("MouseInfo_40"), m_strInteraction.c_str(), &Rect, white, DT_LEFT | DT_SINGLELINE | DT_VCENTER);


        m_fY += 40.f;
        m_fX += 50.f;
    }
    m_pSkillIndicator->Render();
    return S_OK;
}
void CMouse::ClickedEevent()
{
    _float3 vPickingPos = {};
    _uint iItemID = m_pSlot->Get_ItemID();

    if (m_pGameInstance->KeyDown(VK_LBUTTON))
    {
        if (false == m_bPutDown)
        {
            if(0 != iItemID)
            {
                auto Player_Pos = m_pPlayerTransform_Com->GetWorldState(WORLDSTATE::POSITION);
                auto Terrian = CTerrian_Manager::GetInstance()->GetOnTerrian(Player_Pos);
                auto VIbuffer = Terrian->GetCurVIBuffer();
                auto Transform = Terrian->GetTransfrom();
                if (VIbuffer->Picking(Transform, &vPickingPos))
                {
                    _float3 vDistance = vPickingPos - m_pPlayerTransform_Com->GetWorldState(WORLDSTATE::POSITION);

                    if (1.f <= D3DXVec3Length(&vDistance))
                    {
                        m_bPutDown = false;
                        return;
                    }


                    ITEM_DESC Desc = m_pSlot->Get_Info();
                    vPickingPos.y = 0.f;
                    Desc.vPosition = vPickingPos;

                    ITEM_DATA Item_Data = CItem_Manager::GetInstance()->Get_ItemData(Desc.iItemID);
                    ITEM_TYPE eType = Item_Data.eItemType;

                    if (ITEM_TYPE::MERTARIAL == eType)
                    {
                        if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Material_Item"),
                            EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_Item"), &Desc)))
                        {
                            MSG_BOX("Failed to Add Item");
                        }
                    }
                    else if (ITEM_TYPE::EQUIPMENT == eType)
                    {
                        if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Equipment"),
                            EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_Item"), &Desc)))
                        {
                            MSG_BOX("Failed to Add Item");
                        }
                    }
                    else if (ITEM_TYPE::FOOD == eType)
                    {
                        if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Food"),
                            EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_Item"), &Desc)))
                        {
                            MSG_BOX("Failed to Add Item");
                        }
                    }
                    else if (5 == iItemID)
                    {
                        if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_CamFire"),
                            EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_Item"), &Desc)))
                        {
                            MSG_BOX("Failed to Add Item");
                        }
                    }
                    else if (14 == iItemID)
                    {
                        if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Chest"),
                            EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_Item"), &Desc)))
                        {
                            MSG_BOX("Failed to Add Item");
                        }
                    }
                    else if (7 == iItemID)
                    {
                        if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_ResearchLap"),
                            EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_Item"), &Desc)))
                        {
                            MSG_BOX("Failed to Add Item");
                        }
                    }
                    else if (12 == iItemID)
                    {
                        if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Cookpot"),
                            EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_Item"), &Desc)))
                        {
                            MSG_BOX("Failed to Add Item");
                        }
                    }
                    else if (13 == iItemID)
                    {
                        if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_IceBox"),
                            EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_Item"), &Desc)))
                        {
                            MSG_BOX("Failed to Add Item");
                        }
                    }
                    else
                    { 
                         MSG_BOX("Failed to Add Item");
                    }
                    

                    m_pSlot->Clear();
                    m_bPutDown = true;
                    return;
                }
            }
        }
        else
        {
            m_bPutDown = false;
        }
    }
   
}
void CMouse::Update_HoverItem(_uint itemID)
{
    ITEM_DATA Data = CItem_Manager::GetInstance()->Get_ItemData(itemID);
    m_strInfoMessage = Data.strName;

    switch (Data.eItemType)
    {
    case ITEM_TYPE::MERTARIAL:
        m_iMouseState = 2;
        m_strInteraction = L":Pick";
        break;

    case ITEM_TYPE::FOOD:
        m_iMouseState = 2;
        m_strInteraction = L":Pick";
        break;

    case ITEM_TYPE::EQUIPMENT:
        m_iMouseState = 2;
        m_strInteraction = L":Pick";
        break;

    case ITEM_TYPE::STRUCTURE:
        
        break;

    default:
        break;
    }
       
}
void CMouse::Update_HoverSlot(_uint itemID)
{
    ITEM_DATA Data = CItem_Manager::GetInstance()->Get_ItemData(itemID);
    m_strInfoMessage = Data.strName;

    switch (Data.eItemType)
    {
    case ITEM_TYPE::MERTARIAL:
        m_iMouseState = 2;
        m_strInteraction = L":Inspect";
        break;

    case ITEM_TYPE::FOOD:
        m_iMouseState = 2;
        m_strInteraction = L":Eat";
        break;

    case ITEM_TYPE::EQUIPMENT:
        m_iMouseState = 2;
        m_strInteraction = L":Equip";
        break;

    default:
        break;
    }
}
void CMouse::Update_Hover(_uint itemID)
{
    ITEM_DATA Data = CItem_Manager::GetInstance()->Get_ItemData(itemID);
    m_strInfoMessage = Data.strName;
}

void CMouse::Update_Hover(const wstring strMessage, const _uint iMouseState)
{
    m_iMouseState = iMouseState;
    m_strInteraction = strMessage;
}

void CMouse::Update_HoverEnv(_uint iObjectID)
{
    SWAPOBJECT SwapObject = m_pPlayer_Data->tItem;

    if (2 == iObjectID || 7 == iObjectID || 8 == iObjectID)
    {
        m_strInteraction = L"PICK";
        m_iMouseState = 1;
        return;
    }

    switch (SwapObject)
    {
    case Client::SWAPOBJECT::AXE:
    case Client::SWAPOBJECT::GOLDAXE:
        if (4 == iObjectID || 9 == iObjectID)
        {
            m_strInteraction = L"CHOP";
            m_iMouseState = 1;
        }

        break;
    case Client::SWAPOBJECT::PICKAXE:
    case Client::SWAPOBJECT::GOLDPICKAXE:
        if (3 == iObjectID || 5 == iObjectID)
        {
            m_strInteraction = L"DIG";
            m_iMouseState = 1;
        }
        break;
    case Client::SWAPOBJECT::SHOVEL:
    case Client::SWAPOBJECT::GOLDSHOVEL:

        break;

    case Client::SWAPOBJECT::SPEAR:
        break;

    case Client::SWAPOBJECT::TORCH:
        if (4 == iObjectID)
        {
            m_strInteraction = L"LIGHT";
            m_iMouseState = 1;
        }
        break;
        break;

    case Client::SWAPOBJECT::NONE:
        break;

    default:
        break;
    }
   

}

HRESULT CMouse::ADD_Components()
{
    // Texture Component
    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Mouse"),
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

    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Scale"),
        TEXT("Com_BlendTexture"),
        reinterpret_cast<CComponent**>(&m_pBlend_Texture_Com))))
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
    Safe_Release(m_pGrid);
    Safe_Release(m_pSkillIndicator);

    Safe_Release(m_pPlayerTransform_Com);
    Safe_Release(m_pTexture_Com);
    Safe_Release(m_pBlend_Texture_Com);
    Safe_Release(m_pTransform_Com);
    Safe_Release(m_pVIBuffer_Com);

}
