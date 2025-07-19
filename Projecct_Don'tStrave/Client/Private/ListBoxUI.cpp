#include "ListBoxUI.h"

#include "GameInstance.h"
#include "QuestManager.h"
#include "QuestBoxEntry.h"

CListBoxUI::CListBoxUI(LPDIRECT3DDEVICE9 pGraphic_Device) :
    CUserInterface(pGraphic_Device)
{
}

CListBoxUI::CListBoxUI(const CListBoxUI& rhs) : 
    CUserInterface(rhs)
{
}

HRESULT CListBoxUI::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CListBoxUI::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    LISTBOXUI_DESC* Desc = static_cast<LISTBOXUI_DESC*>(pArg);

    m_fRelativePos = { Desc->fX, Desc->fY };
    m_pParentTransform = Desc->pParentTransform_Com;
    m_ipreviewCnt = Desc->m_ipreviewCnt;
    m_EntryBoxSize = { m_fSizeX - 60 , 80 };
    m_iCurViewIndex = 0;

    m_fX = 0;
    m_fY = 0;

    for (_uint i = 0; i < m_ipreviewCnt; ++i)
    {
        // 프리뷰 또는 아이템을 넣고 그만큼만 보여준다.
        if (FAILED(ADD_EntryBox(i)))
            return E_FAIL;
    }

    UpdatePosition(0.9f);
    return S_OK;
}

void CListBoxUI::Priority_Update(_float fTimeDelta)
{
    if (nullptr == m_QuestList)
        return;

    auto FirstQuest = (*m_QuestList).begin();
    for (_uint i = 0; i < m_ipreviewCnt; ++i)
    {
        if (FirstQuest != (*m_QuestList).end())
        {
            m_QuestBoxVec[m_iCurViewIndex + i]->SetQuestData(m_iQuestListIndex, (*FirstQuest));
            FirstQuest++;
        }
        else
            m_QuestBoxVec[m_iCurViewIndex + i]->SetQuestData(m_iQuestListIndex, nullptr);

        m_QuestBoxVec[m_iCurViewIndex + i]->Priority_Update(fTimeDelta);
    }
}

void CListBoxUI::Update(_float fTimeDelta)
{
    for (_uint i = 0; i < m_ipreviewCnt; ++i)
    {
        m_QuestBoxVec[m_iCurViewIndex + i]->Update(fTimeDelta);
    }
}

void CListBoxUI::Late_Update(_float fTimeDelta)
{
    auto ParentPos = m_pParentTransform->GetWorldState(WORLDSTATE::POSITION);
    m_fX = ParentPos.x + m_fRelativePos.x;
    m_fY = ParentPos.y + m_fRelativePos.y;
    m_pTransform_Com->SetPosition({m_fX, m_fY, ParentPos.z});

    for (_uint i = 0; i < m_ipreviewCnt; ++i)
    {
        m_QuestBoxVec[m_iCurViewIndex + i]->Late_Update(fTimeDelta);
    }
}

void CListBoxUI::UpdateQuestList(_uint Index)
{
    m_iQuestListIndex = Index;
    switch (Index)
    {
    case 0 :
        m_QuestList = CQuestManager::GetInstance()->GetQuestList();
        break;
    case 1:
        m_QuestList = CQuestManager::GetInstance()->GetRunningQuest();
        break;
    case 2:
        m_QuestList = CQuestManager::GetInstance()->GetClearQuest();
        break;
    }

    UpdateQuestEntry();
}

HRESULT CListBoxUI::Render()
{
    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());

    for (_uint i = 0; i < m_ipreviewCnt; ++i)
    {
        m_QuestBoxVec[m_iCurViewIndex + i]->Render();
    }

    return S_OK;
}



HRESULT CListBoxUI::ADD_Components()
{
    Engine::CTransform::TRANSFORM_DESC Transform_Desc = { 5.f, D3DXToRadian(90.f) };

    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"),
        reinterpret_cast<CComponent**>(&m_pTransform_Com), &Transform_Desc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CListBoxUI::ADD_EntryBox(_uint Index)
{
    CQuestBoxEntry::QUESTBOXENTRY_DESC Desc;
    Desc.EntryIndex = Index;
    Desc.pParentTransform_Com = m_pTransform_Com;
    Desc.fX = 10;
    Desc.fY = m_EntryBoxSize.y * Index;
    Desc.fSizeX = m_EntryBoxSize.x - 50;
    Desc.fSizeY = m_EntryBoxSize.y;

    CBase* BaseObejct = m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_QuestBoxEntry"), &Desc);
    if (nullptr == BaseObejct)
        return E_FAIL;

    m_QuestBoxVec.push_back(static_cast<CQuestBoxEntry*>(BaseObejct));
    return S_OK;
}

void CListBoxUI::UpdateQuestEntry()
{
    auto FirstQuest = (*m_QuestList).begin();
    for (_uint i = 0; i < m_ipreviewCnt; ++i)
    {
        if (FirstQuest != (*m_QuestList).end())
        {
            m_QuestBoxVec[m_iCurViewIndex + i]->SetQuestData(m_iQuestListIndex, (*FirstQuest));
            FirstQuest++;
        }
        else
            m_QuestBoxVec[m_iCurViewIndex + i]->SetQuestData(m_iQuestListIndex, nullptr);
    }
}

CListBoxUI* CListBoxUI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CListBoxUI* pInstance = new CListBoxUI(pGraphic_Device);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("CREATE FAIL : LIST BOX UI");
    }

    return pInstance;
}

CGameObject* CListBoxUI::Clone(void* pArg)
{
    CListBoxUI* pInstance = new CListBoxUI(*this);
    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("CLONE FAIL : LIST BOX UI");
    }

    return pInstance;
}

void CListBoxUI::Free()
{
    __super::Free();

    for (auto iter : m_QuestBoxVec)
        Safe_Release(iter);

    Safe_Release(m_pTransform_Com);
}
