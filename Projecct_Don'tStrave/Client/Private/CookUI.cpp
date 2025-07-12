#include "CookUI.h"

#include "GameInstance.h"

#include "SlotFrame.h"
#include "Slot.h"
#include "ItemState.h"
#include "Cook_Button.h"
#include "Cookpot.h"


CCookUI::CCookUI(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CUserInterface{ pGraphic_Device }
{
}

CCookUI::CCookUI(const CCookUI& Prototype)
    : CUserInterface{ Prototype }
{
}

HRESULT CCookUI::Initialize_Prototype()
{

    return S_OK;
}

HRESULT CCookUI::Initialize(void* pArg)
{
    
    m_pCookpot = static_cast<CCookpot*>(pArg);

    if (nullptr == m_pCookpot)
        return E_FAIL;

    CUserInterface::UIOBJECT_DESC Desc = {};

    _float3 vPos = m_pGameInstance->GetMousePosition(0);

    Desc.fSizeX = 85.f;
    Desc.fSizeY = 280.f;
    Desc.fX = vPos.x;
    Desc.fY = vPos.y - 150.f;

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    __super::UpdatePosition();

    CButton::BUTTON_DESC Btn_Desc = {};

    Btn_Desc.fX;
    Btn_Desc.fY;
    Btn_Desc.fSizeX = 40.f;
    Btn_Desc.fSizeY = 25.f;
    Btn_Desc.fRelativeX = 0.f;
    Btn_Desc.fRelativeY = -80.f;
    Btn_Desc.pParentTransform = m_pTransform_Com;

    m_CookBtn = dynamic_cast<CCook_Button*>(m_pGameInstance->Clone_Prototype(
        PROTOTYPE::GAMEOBJECT, EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Cook_Button"), &Btn_Desc));

    _float fPading = 3.f;

    CSlotFrame* pSlotFrame = { nullptr };

    CSlotFrame::SLOTFRAME_DESC Slot_Desc = {};

    for (_uint i = 0; i < 4; ++i)
    {

        Desc.fSizeX = 40.f;
        Desc.fSizeY = 40.f;
        Desc.fX = m_fX;
        Desc.fY = m_fY + fPading + 40.f * i - 80.f;

        Slot_Desc.Desc = Desc;
        Slot_Desc.iSlotType = 5;

        pSlotFrame = reinterpret_cast<CSlotFrame*>(m_pGameInstance->Clone_Prototype(
            PROTOTYPE::GAMEOBJECT, EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_SlotFrame"), &Slot_Desc));


        if (nullptr == pSlotFrame)
            return E_FAIL;

        m_SlotFrames.push_back(pSlotFrame);
    }
    m_pAnimController->ChangeState(m_pState_Open);

    return S_OK;
}

void CCookUI::Priority_Update(_float fTimeDelta)
{
    for (auto pSlotFrame : m_SlotFrames)
        pSlotFrame->Priority_Update(fTimeDelta);
}

void CCookUI::Update(_float fTimeDelta)
{
    m_pAnimController->Tick(fTimeDelta);

    Update_State();

    m_pGameInstance->Add_RenderGroup(RENDER::ORTTHO_UI, this);

    if (CCookUI::STATE::CLOSE != m_eCurState)
    {
        CSlot* Foods[4] = {};

        for (_uint i = 0; i < 4; ++i)
        {
            m_SlotFrames[i]->Update(fTimeDelta);
            Foods[i] = m_SlotFrames[i]->Get_Slot();
        }


        m_CookBtn->Update(fTimeDelta);

        if (m_CookBtn->OnClick())
        {
            _uint items[4];
            for (_uint i = 0; i < 4; ++i)
            {
                _uint iItemID = {};
                if (nullptr != Foods[i])
                    iItemID = Foods[i]->Get_ItemID();

                if (0 == iItemID)
                    return;
            }
            
            m_eCurState = CCookUI::STATE::CLOSE;
            m_pCookpot->Start_Cooking(Check_Recipe(items));

            for (_uint i = 0; i < 4; ++i)
            {
                Foods[i]->Clear();
            }
        }
    }

    
}

void CCookUI::Late_Update(_float fTimeDelta)
{
    for (auto pSlotFrame : m_SlotFrames)
        pSlotFrame->Late_Update(fTimeDelta);
}

HRESULT CCookUI::Render()
{

    m_pAnimController->Render();

    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());

    m_pVIBuffer_Com->Render();

    if (CCookUI::STATE::CLOSE != m_eCurState)
    {
        for (auto pSlotFrame : m_SlotFrames)
            pSlotFrame->Render();

        m_CookBtn->Render();
    }

    return S_OK;
}

void CCookUI::Change_State(CCookUI::STATE eState)
{
    m_eCurState = eState;
}

void CCookUI::Update_State()
{
    if (m_eCurState != m_ePreState)
    {
        _float3 vPos = m_pGameInstance->GetMousePosition(0);
        switch (m_eCurState)
        {
        case Client::CCookUI::STATE::OPEN:
            m_fX = vPos.x;
            m_fY = vPos.y - 150.f;

            __super::UpdatePosition();
            m_pState_Open->Reset_StateData();
            m_pAnimController->ChangeState(m_pState_Open);

            RePosition_SlotFrame();

            break;

        case Client::CCookUI::STATE::CLOSE:
            m_pAnimController->ChangeState(m_pState_Close);
            break;

        default:
            break;
        }

        m_ePreState = m_eCurState;
    }
}

void CCookUI::RePosition_SlotFrame()
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

        m_fX = m_fX;
        m_fY = m_fY + fPading + 40.f * i - 70.f;

        m_SlotFrames[i]->Update_SlotPosition(fX, fY);
    }
}

void CCookUI::Add_Render()
{
    m_pGameInstance->Add_RenderGroup(RENDER::ORTTHO_UI, this);
}


HRESULT CCookUI::ADD_Components()
{
    // Texture Component
    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_CookUI_Close"),
        TEXT("Com_Texture"),
        reinterpret_cast<CComponent**>(&m_pTexture_Com))))
        return E_FAIL;

    // Texture Component
    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_CookUI_Open"),
        TEXT("Com_OpenTexture"),
        reinterpret_cast<CComponent**>(&m_pOpenTexture_Com))))
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
    Desc.iEndFrame = 8;
    Desc.fTimeRate = 0.5f;
    Desc.pAnimTexture = m_pOpenTexture_Com;
    Desc.bIsLoop = false;

    m_pState_Open = CItemState::Create(&Desc);

    Desc.iStartFrame = 0;
    Desc.iEndFrame = 8;
    Desc.fTimeRate = 0.5f;
    Desc.pAnimTexture = m_pTexture_Com;
    Desc.bIsLoop = false;

    m_pState_Close = CItemState::Create(&Desc);

    return S_OK;
}

_uint CCookUI::Check_Recipe(_uint Items[])
{
    _uint iFoodID = {};

    // 0 고기 1 괴물 고기 2 열매 3 기타
    _uint iFoodCnt[4] = {};

    for (_uint i = 0; i < 4; i++)
    {
        switch (Items[i])
        {
        case 44:
        case 45:
            iFoodCnt[0] += 1;
            break;

        case 46:
            iFoodCnt[1] += 1;
            break;

        case 48:
            iFoodCnt[2] += 1;
            break;
        }
    }

    if (0 < iFoodCnt[0] && 3 == iFoodCnt[2])
        iFoodID = 50; // 미트볼
    else 
        iFoodID = 31; // 괴물 음식



    return iFoodID;
}

CCookUI* CCookUI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CCookUI* pInstance = new CCookUI(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CCookUI");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CCookUI::Clone(void* pArg)
{
    CGameObject* pInstance = new CCookUI(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CCookUI");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CCookUI::Free()
{
    __super::Free();

    for (auto pSlotFrame : m_SlotFrames)
        Safe_Release(pSlotFrame);
    m_SlotFrames.clear();

    Safe_Release(m_pAnimController);
    Safe_Release(m_pState_Close);
    Safe_Release(m_pState_Open);

    Safe_Release(m_pTexture_Com);
    Safe_Release(m_pOpenTexture_Com);
    Safe_Release(m_CookBtn);

    Safe_Release(m_pTransform_Com);
    Safe_Release(m_pVIBuffer_Com);

}
