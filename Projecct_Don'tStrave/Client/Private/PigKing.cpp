#include "PigKing.h"

#include "GameInstance.h"
#include "XML_Manager.h"
#include "DropItemComponent.h"
#include "QuestManager.h"

#include "Script.h"
#include "Clock.h"
#include "Slot.h"

CPigKing::CPigKing(LPDIRECT3DDEVICE9 pGraphic_Device) : 
    CDropItemEnviornment(pGraphic_Device)
{
}

CPigKing::CPigKing(const CPigKing& rhs) : 
    CDropItemEnviornment(rhs)
{
}

HRESULT CPigKing::Initialize_Prototype()
{
    auto XML_Instance = CXML_Manager::GetInstance();
    XML_Instance->AddTexture("../Bin/Resources/Textures/Npc/PigKing/Pig_King.scml", L"../Bin/Resources/Textures/Npc/PigKing/", &m_tImageVec);
    XML_Instance->LoadScml("../Bin/Resources/Textures/Npc/PigKing/Pig_King.scml", &m_tAnimation);

    return S_OK;
}

HRESULT CPigKing::Initialize(void* pArg)
{
    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    LoadImageFile();

    Change_Motion(pigKingState::IDLE);
    m_pDropItem_Com->ADD_ItemData(39, 1);

    return S_OK;
}

void CPigKing::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);

    auto AllUserInterface = m_pGameInstance->GetAllObejctsToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_UserInterface"));
    for (auto& iter : *AllUserInterface)
    {
        auto Clock = dynamic_cast<CClock*>(iter);
        if (Clock)
            m_pClock = Clock;
    }

    if (m_pVIBufferCom->Picking(m_pTransformCom, &m_PickingPos))
    {
        if (m_pGameInstance->KeyPressed(VK_LBUTTON))
        {
            //마우스 슬롯확인 아이템 던져줄거
            if (pigKingState::IDLE == m_PigState)
                Change_Motion(UNIMPREESED);
               
        }
        else if (m_pGameInstance->KeyDown(VK_RBUTTON))
        {
            if (!m_bIsQuestStart)
            {
                m_pGameInstance->ChangeGameState(GAMESTATE::SINEMATIC);
                ShowFirstClickMotion();
            }
        }
    }
    
    ChangeTimeState();
}

void CPigKing::Update(_float fTimeDelta)
{

}

void CPigKing::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);

    Reset_State();
}

void CPigKing::Reset_State()
{
    if (m_fAniTime >= m_iLength)
    {
        if (pigKingState::UNIMPREESED >= m_PigState)
        {
            Change_Motion(IDLE);
        }
        else if (pigKingState::F_SLEEP == m_PigState)
        {
            Change_Motion(L_SLEEP);
        }
        m_fAniTime = 0;
    }
}

HRESULT CPigKing::Render()
{
    __super::Render();
    return S_OK;
}

HRESULT CPigKing::ADD_Components()
{
    /* Com_Transform */
    CTransform::TRANSFORM_DESC		TransformDesc{ 5.f, D3DXToRadian(90.0f) };
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
        return E_FAIL;

    /* Com_VIBuffer */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_DropItem"),
        TEXT("Com_DropItem"), reinterpret_cast<CComponent**>(&m_pDropItem_Com))))
        return E_FAIL;


    return S_OK;
}

void CPigKing::Change_Motion(pigKingState ChangeState)
{
    m_PigState = ChangeState;
    m_fAniTime = 0;

    switch (m_PigState)
    {
    case pigKingState::IDLE :
    {
        m_FrontName = TEXT("idle");
    }
    break;
    case pigKingState::HAPPY:
    {
        m_FrontName = TEXT("happy");
    }
    break;
    case pigKingState::COINTOSS:
    {
        m_FrontName = TEXT("cointoss");
    }
    break;
    case pigKingState::F_SLEEP:
    {
        m_FrontName = TEXT("sleep_pre");
    }
    break;
    case pigKingState::L_SLEEP:
    {
        m_FrontName = TEXT("sleep_loop");
    }
    break;
    case pigKingState::E_SLEEP:
    {
        m_FrontName = TEXT("sleep_pst");
    }
    break;
    case pigKingState::UNIMPREESED:
    {
        m_FrontName = TEXT("unimpressed");
    }
    break;
    }
}

void CPigKing::ChangeTimeState()
{
    switch(m_pClock->GetClockState())
    {
    case CClock::MORNING :
        if (pigKingState::L_SLEEP == m_PigState)
        {
            Change_Motion(pigKingState::E_SLEEP);
        }
        break;
    case CClock::LUNCH:
       
        break;
    case CClock::NIGHT:
        if (pigKingState::IDLE == m_PigState)
        {
            Change_Motion(pigKingState::F_SLEEP);
        }
        break;

    }

}

void CPigKing::ChangeMeetToGold()
{
    auto slot = dynamic_cast<CSlot*>(m_pGameInstance->Chagne_Slot());
    if (slot)
    {
        auto ItemData = slot->Get_Info();
        if (44 == ItemData.iItemID)
        {
            if (0 <= ItemData.iNumItem - m_TradeCnt)
            {
                _float3 Pos = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
                Pos -= m_pTransformCom->GetWorldState(WORLDSTATE::LOOK) * 0.2f;
                CreateDropItem(Pos);
            }
        }
    }
}

void CPigKing::ShowFirstClickMotion()
{
    CScript::SCRIPT_DESC Scirpt_Desc;
    Scirpt_Desc.DataScript.push_back(L"오, 이 몸을 찾아온 손님이로군!");
    Scirpt_Desc.DataScript.push_back(L"반갑다. 이 몸의 이름은 돼지왕");
    Scirpt_Desc.DataScript.push_back(L"황금을 가져오면 너의 소원을 들어주겠다.");
    Scirpt_Desc.DataScript.push_back(L"단, 맛난 음식은 내가 먹을 테니, 탐내지 마라!");

    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(EnumToInt(LEVEL::GAMEPLAY),
        TEXT("Prototype_GameObject_ScriptUI"), EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_UserInterface"), &Scirpt_Desc)))
        return;

    CQuestManager::GetInstance()->QuestStartEvent();
    m_bIsQuestStart = true;
}

CPigKing* CPigKing::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CPigKing* pInstance = new CPigKing(pGraphic_Device);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("CLONE FAIL : PIG King");
    }
    return pInstance;
}

CGameObject* CPigKing::Clone(void* pArg)
{
    CPigKing* pInstance = new CPigKing(*this);
    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("CLONE FAIL : PIG King");
    }
    return pInstance;
}

void CPigKing::Free()
{
    __super::Free();
}
