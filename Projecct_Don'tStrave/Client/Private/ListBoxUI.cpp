#include "ListBoxUI.h"

#include "GameInstance.h"
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

    for (int i = 0; i < m_ipreviewCnt; ++i)
    {
        // 프리뷰 또는 아이템을 넣고 그만큼만 보여준다.
        if (FAILED(ADD_EntryBox(i)))
            return E_FAIL;
    }

    return S_OK;
}

void CListBoxUI::Priority_Update(_float fTimeDelta)
{
    for (_uint i = 0; i < m_ipreviewCnt; ++i)
    {
        m_QuestBoxVec[m_iCurViewIndex + i]->Priority_Update(fTimeDelta);
    }
}

void CListBoxUI::Update(_float fTimeDelta)
{
    for (_uint i = 0; i < m_ipreviewCnt; ++i)
    {
        m_QuestBoxVec[m_iCurViewIndex + i]->Priority_Update(fTimeDelta);
    }
}

void CListBoxUI::Late_Update(_float fTimeDelta)
{
    for (_uint i = 0; i < m_ipreviewCnt; ++i)
    {
        m_QuestBoxVec[m_iCurViewIndex + i]->Priority_Update(fTimeDelta);
    }
}

HRESULT CListBoxUI::Render()
{
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
    Desc.fX =  m_fX;
    Desc.fY = m_fY + (m_fWinSizeY / m_ipreviewCnt * Index);
    Desc.fSizeX = m_fWinSizeX;
    Desc.fSizeX = m_fWinSizeY / m_ipreviewCnt;

    CBase* BaseObejct = m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Gameplay_Object_QuestBoxEntry"), &Desc);
    if (nullptr == BaseObejct)
        return E_FAIL;

    m_QuestBoxVec.push_back(static_cast<CQuestBoxEntry*>(BaseObejct));
    return S_OK;
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

    for (_uint i = 0; i < m_ipreviewCnt; ++i)
        Safe_Release(m_QuestBoxVec[i]);

    Safe_Release(m_pTransform_Com);
}
