#include "Inventory.h"

#include "GameInstance.h"

#include "SlotFrame.h"
#include "Slot.h"

CInventory::CInventory(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CUserInterface{ pGraphic_Device }
{
}

CInventory::CInventory(const CInventory& Prototype)
    : CUserInterface{ Prototype }
{
}

HRESULT CInventory::Initialize_Prototype()
{

    return S_OK;
}

HRESULT CInventory::Initialize(void* pArg)
{
    CUserInterface::UIOBJECT_DESC Desc = {};

    Desc.fSizeX = 900.f;
    Desc.fSizeY = 150.f;
    Desc.fX = g_iWinSizeX * 0.5f;
    Desc.fY = g_iWinSizeY;

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    __super::UpdatePosition();


    _float fPading = 3.f;
    _uint iOffset = {};
    CSlotFrame* pSlotFrame = { nullptr };

    CSlotFrame::SLOTFRAME_DESC Slot_Desc = {};

    for (_uint i = 0; i < 15; ++i)
    {
        if (0 == i % 5)
            iOffset += 5.f;

        Desc.fSizeX = 40.f;
        Desc.fSizeY = 40.f;
        Desc.fX = (m_fX - m_fSizeX * 0.55f) + (Desc.fSizeX + fPading) * (i + 2) + iOffset;
        Desc.fY = g_iWinSizeY - 25.f;

        Slot_Desc.Desc = Desc;
        Slot_Desc.iSlotType = 0;

        pSlotFrame = reinterpret_cast<CSlotFrame*>(m_pGameInstance->Clone_Prototype(
            PROTOTYPE::GAMEOBJECT, EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_SlotFrame"), &Slot_Desc));


        if (nullptr == pSlotFrame)
            return E_FAIL;

        m_SlotFrames.push_back(pSlotFrame);
    }

    for (_uint i = 15; i < 19; ++i)
    {
        if (0 == i % 3)
            iOffset += 5.f;

        Desc.fSizeX = 40.f;
        Desc.fSizeY = 40.f;
        Desc.fX = (m_fX - m_fSizeX * 0.55f) + (Desc.fSizeX + fPading) * (i + 2) + iOffset;
        Desc.fY = g_iWinSizeY - 25.f;

        Slot_Desc.Desc = Desc;
        Slot_Desc.iSlotType = i - 14;

        pSlotFrame = reinterpret_cast<CSlotFrame*>(m_pGameInstance->Clone_Prototype(
            PROTOTYPE::GAMEOBJECT, EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_SlotFrame"), &Slot_Desc));

        if (nullptr == pSlotFrame)
            return E_FAIL;

        m_SlotFrames.push_back(pSlotFrame);
    }

    return S_OK;
}

void CInventory::Priority_Update(_float fTimeDelta)
{
    for (auto pSlotFrame : m_SlotFrames)
        pSlotFrame->Priority_Update(fTimeDelta);
}

void CInventory::Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::ORTTHO_UI, this);

    for (auto pSlotFrame : m_SlotFrames)
        pSlotFrame->Update(fTimeDelta);
}

void CInventory::Late_Update(_float fTimeDelta)
{
    for (auto pSlotFrame : m_SlotFrames)
        pSlotFrame->Late_Update(fTimeDelta);
}

HRESULT CInventory::Render()
{
    m_pTexture_Com->Set_Texture(0);

    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());

    m_pVIBuffer_Com->Render();


    for (auto pSlotFrame : m_SlotFrames)
        pSlotFrame->Render();

    return S_OK;
}

CSlot* CInventory::Find_Item(_uint _iItemID)
{
    _bool bEmpty = { false };
    CSlot* pEmptySlot = { nullptr };

    for (_uint i = 0; i < 15; ++i)
    {
        CSlot* pSlot = m_SlotFrames[i]->Get_Slot();
        _uint iItemID = pSlot->Get_ItemID();

        if (0 == iItemID && false == bEmpty)
        {
            bEmpty = true;
            pEmptySlot = pSlot;
        }
        if (_iItemID == pSlot->Get_ItemID())
            return pSlot;
    }

    return pEmptySlot;
}

CSlot* CInventory::Find_Slot(SLOT eSlot)
{
    switch (eSlot)
    {
    case Client::SLOT::NORMAL:
        return nullptr;
        break;

    case Client::SLOT::HAND:
        return m_SlotFrames[15]->Get_Slot();
        break;

    case Client::SLOT::HAT:
        return m_SlotFrames[16]->Get_Slot();
        break;

    case Client::SLOT::ARMOR:
        return m_SlotFrames[17]->Get_Slot();
        break;

    default:
        break;
    }
    return nullptr;
}

HRESULT CInventory::ADD_Components()
{
    // Texture Component
    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Inventory"),
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

CInventory* CInventory::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CInventory* pInstance = new CInventory(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CInventory");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CInventory::Clone(void* pArg)
{
    CGameObject* pInstance = new CInventory(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CInventory");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CInventory::Free()
{
    __super::Free();

    for (auto pSlotFrame : m_SlotFrames)
        Safe_Release(pSlotFrame);
    m_SlotFrames.clear();

    Safe_Release(m_pTexture_Com);
    Safe_Release(m_pTransform_Com);
    Safe_Release(m_pVIBuffer_Com);
}
