#include "Inventory.h"

#include "GameInstance.h"

#include "SlotFrame.h"


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
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    __super::UpdatePosition();

    CUserInterface::UIOBJECT_DESC Desc = {};

    _float fPading = 3.f;
    _uint iOffset = {};
    CSlotFrame* pSlotFrame = { nullptr };

    for (_uint i = 0; i < 15; ++i)
    {
        if (0 == i % 5)
            iOffset += 5.f;

        Desc.fSizeX = 40.f;
        Desc.fSizeY = 40.f;
        Desc.fX = (m_fX - m_fSizeX * 0.55f) + (Desc.fSizeX + fPading) * (i + 2) + iOffset;
        Desc.fY = g_iWinSizeY - 25.f;

        pSlotFrame = reinterpret_cast<CSlotFrame*>(m_pGameInstance->Clone_Prototype(
            PROTOTYPE::GAMEOBJECT, EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_SlotFrame"), &Desc));


        if (nullptr == pSlotFrame)
            return E_FAIL;

        m_Slots.push_back(pSlotFrame);
    }

    return S_OK;
}

void CInventory::Priority_Update(_float fTimeDelta)
{
    for (auto pSlot : m_Slots)
        pSlot->Priority_Update(fTimeDelta);
}

void CInventory::Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::ORTTHO_UI, this);

    for (auto pSlot : m_Slots)
        pSlot->Update(fTimeDelta);
}

void CInventory::Late_Update(_float fTimeDelta)
{
    for (auto pSlot : m_Slots)
        pSlot->Late_Update(fTimeDelta);
}

HRESULT CInventory::Render()
{
    m_pTexture_Com->Set_Texture(0);

    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());

    m_pVIBuffer_Com->Render();


    for (auto pSlot : m_Slots)
        pSlot->Render();

    return S_OK;
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
    CInventory* pInstance = new CInventory(*this);

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

    for (auto pSlot : m_Slots)
        Safe_Release(pSlot);
    m_Slots.clear();

    Safe_Release(m_pTexture_Com);
    Safe_Release(m_pTransform_Com);
    Safe_Release(m_pVIBuffer_Com);
}
