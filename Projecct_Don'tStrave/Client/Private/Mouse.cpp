#include "Mouse.h"

#include "GameInstance.h"
#include "Item_Manager.h"
#include "Slot.h"
#include "Item.h"

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

    m_MouseMessage = L"";

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    m_pTransform_Com->SetScale(_float3(m_fSizeX, m_fSizeY, 1.f));

    if (FAILED(Add_Slot()))
        return E_FAIL;
    m_fSizeX = 40.f;
    m_fSizeY = 40.f;

    __super::UpdatePosition();

    m_pPlayerTransform_Com = static_cast<CTransform*>(m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Player"), TEXT("Com_Transform"), 0));

    Safe_AddRef(m_pPlayerTransform_Com);

    return S_OK;
}

void CMouse::Priority_Update(_float fTimeDelta)
{
    
    m_pSlot = dynamic_cast<CSlot*>(m_pGameInstance->Chagne_Slot());
    m_pSlot->Priority_Update(fTimeDelta);

    m_MouseMessage = L"";
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
    

#pragma region TestCode
    ITEM_DESC Desc = {};
    if (GetKeyState('1') & 0x8000)
    {
        Desc.iItemID = 1;
        Desc.eItemType = ITEM_TYPE::MERTARIAL;
        Desc.eSlot = SLOT::NORMAL;
        Desc.iNumItem = 1;
        Desc.fDurability = 100.f;

        m_pSlot->Set_Info(Desc);
    }
    if (GetKeyState('2') & 0x8000)
    {
        Desc.iItemID = 2;
        Desc.eItemType = ITEM_TYPE::MERTARIAL;
        Desc.eSlot = SLOT::NORMAL;
        Desc.iNumItem = 1;
        Desc.fDurability = 100.f;

        m_pSlot->Set_Info(Desc);
    }
    if (GetKeyState('3') & 0x8000)
    {
        Desc.iItemID = 3;
        Desc.eItemType = ITEM_TYPE::EQUIPMENT;
        Desc.eSlot = SLOT::HAND;
        Desc.iNumItem = 1;
        Desc.fDurability = 100.f;

        m_pSlot->Set_Info(Desc);
    }
    if (GetKeyState('5') & 0x8000)
    {
        Desc.iItemID = 5;
        Desc.eItemType = ITEM_TYPE::FOOD;
        Desc.eSlot = SLOT::NORMAL;
        Desc.iNumItem = 1;
        Desc.fDurability = 100.f;

        m_pSlot->Set_Info(Desc);
    }
#pragma endregion
    
}

void CMouse::Late_Update(_float fTimeDelta)
{
    if (SLOT::NORMAL == m_pSlot->Get_Info().eSlot)
        m_pSlot->Update(fTimeDelta);

    ClickedEevent();
    m_pGameInstance->Add_RenderGroup(RENDER::ORTTHO_UI, this);
}

HRESULT CMouse::Render()
{
    
    //m_pTexture_Com->Set_Texture(0);

    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());

    //m_pVIBuffer_Com->Render();
     RECT Rect = { LONG(m_fX - m_fSizeX), LONG(m_fY - m_fSizeY - 20.f),LONG(m_fX + m_fSizeX), LONG(m_fY + m_fSizeY - 20.f) };
    m_pSlot->Render(m_pTransform_Com);

    Rect = { LONG(m_fX - m_fSizeX), LONG(m_fY - m_fSizeY - 20.f),LONG(m_fX + m_fSizeX), LONG(m_fY + m_fSizeY - 20.f) };
    
    D3DXCOLOR white = { 1.f, 1.f, 1.f, 1.f };
    m_pGameInstance->Render_Font(TEXT("Font_18"), m_MouseMessage.c_str(), &Rect, white);
    //m_pGameInstance->Render_Font(TEXT("Font_18"), TEXT("나무 \n 줍기"), &Rect);
    
    
    

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
                if (true == dynamic_cast<CVIBuffer_Terrain*>(m_pGameInstance->Get_Component(
                    EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_BackGround"), TEXT("Com_VIBuffer")
                ))->Picking(dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(EnumToInt(LEVEL::GAMEPLAY),
                    TEXT("Layer_BackGround"), TEXT("Com_Transform"))), &vPickingPos))
                {

                    _float3 vDistance = vPickingPos - m_pPlayerTransform_Com->GetWorldState(WORLDSTATE::POSITION);

                    if (1.f <= D3DXVec3Length(&vDistance))
                    {
                        m_bPutDown = false;
                        return;
                    }


                    ITEM_DESC Desc = m_pSlot->Get_Info();
                    Desc.vPosition = vPickingPos;

                    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Item"),
                        EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_Item"), &Desc)))
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
void CMouse::Update_HoverObject(CItem* pItem)
{
    m_MouseMessage = CItem_Manager::GetInstance()->Get_ItemData(pItem->Get_Info().iItemID).strName;
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
    Safe_Release(m_pPlayerTransform_Com);
    Safe_Release(m_pTexture_Com);
    Safe_Release(m_pTransform_Com);
    Safe_Release(m_pVIBuffer_Com);

}
