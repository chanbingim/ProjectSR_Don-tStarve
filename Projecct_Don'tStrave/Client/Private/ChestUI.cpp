#include "ChestUI.h"

#include "GameInstance.h"

#include "SlotFrame.h"
#include "Slot.h"
#include "ItemState.h"


CChestUI::CChestUI(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CUserInterface{ pGraphic_Device }
{
}

CChestUI::CChestUI(const CChestUI& Prototype)
    : CUserInterface{ Prototype }
{
}

HRESULT CChestUI::Initialize_Prototype()
{

    return S_OK;
}

HRESULT CChestUI::Initialize(void* pArg)
{
    CUserInterface::UIOBJECT_DESC Desc = {};

    _float3 vPos = m_pGameInstance->GetMousePosition(0);

    Desc.fSizeX = 200.f;
    Desc.fSizeY = 200.f;
    Desc.fX = vPos.x;
    Desc.fY = vPos.y - 150.f;

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    __super::UpdatePosition();


    _float fPading = 3.f;
    _uint iOffset = {};
    CSlotFrame* pSlotFrame = { nullptr };

    CSlotFrame::SLOTFRAME_DESC Slot_Desc = {};

    for (_uint i = 0; i < 9; ++i)
    {
        if (0 == i % 3)
            iOffset += 41.f;

        Desc.fSizeX = 40.f;
        Desc.fSizeY = 40.f;
        Desc.fX = m_fX - Desc.fSizeX + (i % 3) * Desc.fSizeX;
        Desc.fY = m_fY - Desc.fSizeY + iOffset - 43.f;

        Slot_Desc.Desc = Desc;
        Slot_Desc.iSlotType = 0;

        pSlotFrame = reinterpret_cast<CSlotFrame*>(m_pGameInstance->Clone_Prototype(
            PROTOTYPE::GAMEOBJECT, EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_SlotFrame"), &Slot_Desc));


        if (nullptr == pSlotFrame)
            return E_FAIL;

        m_SlotFrames.push_back(pSlotFrame);
    }
    m_pAnimController->ChangeState(m_pState_Open);

    return S_OK;
}

void CChestUI::Priority_Update(_float fTimeDelta)
{
    for (auto pSlotFrame : m_SlotFrames)
        pSlotFrame->Priority_Update(fTimeDelta);
}

void CChestUI::Update(_float fTimeDelta)
{
    m_pAnimController->Tick(fTimeDelta);

    Update_State();

    if (CChestUI::STATE::CLOSE != m_eCurState)
    {
        for (auto pSlotFrame : m_SlotFrames)
            pSlotFrame->Update(fTimeDelta);
    }
    
}

void CChestUI::Late_Update(_float fTimeDelta)
{
    for (auto pSlotFrame : m_SlotFrames)
        pSlotFrame->Late_Update(fTimeDelta);
}

HRESULT CChestUI::Render()
{

    m_pAnimController->Render();

    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());

    m_pVIBuffer_Com->Render();

    if (CChestUI::STATE::CLOSE != m_eCurState )
    {
        for (auto pSlotFrame : m_SlotFrames)
            pSlotFrame->Render();
    }

    return S_OK;
}

void CChestUI::Change_State(CChestUI::STATE eState)
{
    m_eCurState = eState;
}

void CChestUI::Update_State()
{
    if (m_eCurState != m_ePreState)
    {
        _float3 vPos = m_pGameInstance->GetMousePosition(0);
        switch (m_eCurState)
        {
        case Client::CChestUI::STATE::OPEN:
            m_fX = vPos.x;
            m_fY = vPos.y - 150.f;

            __super::UpdatePosition();
            m_pState_Open->Reset_StateData();
            m_pAnimController->ChangeState(m_pState_Open);

            RePosition_SlotFrame();

            break;

        case Client::CChestUI::STATE::CLOSE:
            m_pAnimController->ChangeState(m_pState_Close);
            break;

        default:
            break;
        }

        m_ePreState = m_eCurState;
    }
}

void CChestUI::RePosition_SlotFrame()
{
    _uint iOffset = {};
    _float fPading = 3.f;
    _float fX{}, fY{};

    CSlotFrame* pSlotFrame = { nullptr };

    CSlotFrame::SLOTFRAME_DESC Slot_Desc = {};

    for (_uint i = 0; i < 9; ++i)
    {
        if (0 == i % 3)
            iOffset += 41.f;

        fX = m_fX - 40.f + (i % 3) * 40.f;
        fY = m_fY - 40.f + iOffset - 43.f;

        m_SlotFrames[i]->Update_SlotPosition(fX, fY);
    }
}

void CChestUI::Add_Render()
{
    m_pGameInstance->Add_RenderGroup(RENDER::ORTTHO_UI, this);
}


HRESULT CChestUI::ADD_Components()
{
    // Texture Component
    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_ChestUI"),
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

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_AnimController"),
        TEXT("Com_AnimController"), (CComponent**)&m_pAnimController)))
        return E_FAIL;

    CState::FRAME_DESC Desc = {};

    Desc.iStartFrame = 0;
    Desc.iEndFrame = 5;
    Desc.fTimeRate = 0.5f;
    Desc.pAnimTexture = m_pTexture_Com;
    Desc.bIsLoop = false;

    m_pState_Open = CItemState::Create(&Desc);

    Desc.iStartFrame = 5;
    Desc.iEndFrame = 10;
    Desc.fTimeRate = 0.5f;
    Desc.pAnimTexture = m_pTexture_Com;
    Desc.bIsLoop = false;

    m_pState_Close = CItemState::Create(&Desc);

    return S_OK;
}

CChestUI* CChestUI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CChestUI* pInstance = new CChestUI(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CChestUI");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CChestUI::Clone(void* pArg)
{
    CGameObject* pInstance = new CChestUI(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CChestUI");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CChestUI::Free()
{
    __super::Free();

    for (auto pSlotFrame : m_SlotFrames)
        Safe_Release(pSlotFrame);
    m_SlotFrames.clear();

    Safe_Release(m_pAnimController);
    Safe_Release(m_pState_Close);
    Safe_Release(m_pState_Open);

    Safe_Release(m_pTexture_Com);
    Safe_Release(m_pTransform_Com);
    Safe_Release(m_pVIBuffer_Com);
    
}
