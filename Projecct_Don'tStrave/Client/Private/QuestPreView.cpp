#include "QuestPreView.h"

#include "GameInstance.h"
#include "QuestManager.h"
#include "QuestNextButton.h"
#include "QuestData.h"

CQuestPreView::CQuestPreView(LPDIRECT3DDEVICE9 pGraphic_Device) :
    CUserInterface(pGraphic_Device)
{
}

CQuestPreView::CQuestPreView(const CQuestPreView& rhs) :
    CUserInterface(rhs)
{
}

HRESULT CQuestPreView::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CQuestPreView::Initialize(void* pArg)
{
    UIOBJECT_DESC   Info = {};
    Info.fSizeX = 300.f;
    Info.fSizeY = 100.f;
    Info.fX = g_iWinSizeX - 300.f;
    Info.fY = 100.f;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(__super::Initialize(&Info)))
        return E_FAIL;

    if (FAILED(ADD_NextButtons()))
        return E_FAIL;

    m_pNextButton[0]->SetClickEvent([&]() { ClickedButtonEvent(0); });
    m_pNextButton[1]->SetClickEvent([&]() { ClickedButtonEvent(1); });

    m_pRunningList = CQuestManager::GetInstance()->GetRunningQuest();
    
    return S_OK;
}

void CQuestPreView::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CQuestPreView::Update(_float fTimeDelta)
{
    for (int i = 0; i < 2; ++i)
        m_pNextButton[i]->Update(fTimeDelta);

    UpdateViewer();
}

void CQuestPreView::Late_Update(_float fTimeDelta)
{
    UpdatePosition();
    if (m_pViewData)
    {
        size_t textSize = m_pViewData->SubTitle.length();
        m_FontRect = {
               static_cast<long>(m_fX - (m_fSizeX * 0.5f) + 10),
               static_cast<long>(m_fY - (m_fSizeY * 0.5f) + 40),
               static_cast<long>(m_fX - (m_fSizeX * 0.5f) + (textSize * 20)),
               static_cast<long>(m_fY - (m_fSizeY * 0.5f) + 70),
        };
    }

    for (int i = 0; i < 2; ++i)
        m_pNextButton[i]->Late_Update(fTimeDelta);

    m_pGameInstance->Add_RenderGroup(RENDER::ORTTHO_UI, this);
}

HRESULT CQuestPreView::Render()
{
    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());

    D3DXCOLOR white = { 1.f,1.f,1.f,1.f };
    m_pTexture_Com->Set_Texture(0);
    m_pVIBuffer_Com->Render();

    if (m_pViewData)
    {
        m_pGameInstance->Render_Font(TEXT("Font_18"), m_pViewData->SubTitle.c_str(), &m_FontRect, white, DT_LEFT);
        
        m_FontRect.top    += 20;
        m_FontRect.bottom += 20;
        m_pGameInstance->Render_Font(TEXT("Font_18"), (CQuestManager::GetInstance()->GetPercentData(m_pViewData)).c_str(), &m_FontRect, white, DT_LEFT);
    }

    for (int i = 0; i < 2; ++i)
        m_pNextButton[i]->Render();

    return S_OK;
}

void CQuestPreView::ClickedButtonEvent(_uint index)
{
    if (nullptr == m_pRunningList)
        return;

    if (1 == index)
        m_ViewIndex++;
    else
        m_ViewIndex--;

    if (m_pRunningList->size() <= m_ViewIndex)
    {
        m_ViewIndex = m_pRunningList->size() - 1;
        return;
    }
    else if(0 > m_ViewIndex)
    {
        m_ViewIndex = 0;
        return;
    }
}

void CQuestPreView::UpdateViewer()
{
    if (nullptr == m_pRunningList)
        return;

    auto iter = m_pRunningList->begin();
    if (iter == m_pRunningList->end())
    {
        m_pViewData = nullptr;
        return;
    }

    for (_uint i = 0; i < m_ViewIndex; ++i)
        iter++;

    m_pViewData = (*iter);
}

HRESULT CQuestPreView::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransform_Com)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBuffer_Com)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_QuestPreView"), TEXT("Com_Texture"), (CComponent**)&m_pTexture_Com)))
        return E_FAIL;

    return S_OK;
}

HRESULT CQuestPreView::ADD_NextButtons()
{
    for (int i = 0; i < 2; ++i)
    {
        CQuestNextButton::BUTTON_DESC Desc;
        Desc.pParentTransform = m_pTransform_Com;
        Desc.fSizeX = 20.f;
        Desc.fSizeY = 35.f;

        Desc.fRelativeX = m_fSizeX * 0.24f + (Desc.fSizeX * i) + 20 * i;
        Desc.fRelativeY = -(m_fSizeY * 0.3f);
        m_pNextButton[i] = static_cast<CQuestNextButton*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_QuestNextButton"), &Desc));
        m_pNextButton[i]->ChangeButtonTex(i);
    }
    return S_OK;
}

CQuestPreView* CQuestPreView::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CQuestPreView* pInstance = new CQuestPreView(pGraphic_Device);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("CREATE FAIL : QUEST PREVIEW");
    }
    return pInstance;
}

CGameObject* CQuestPreView::Clone(void* pArg)
{
    CGameObject* pInstance = new CQuestPreView(*this);
    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("CLONE FAIL : QUEST PREVIEW");
    }
    return pInstance;
}

void CQuestPreView::Free()
{
    __super::Free();

    for (int i = 0; i < 2; ++i)
    {
        Safe_Release(m_pNextButton[i]);
    }

    Safe_Release(m_pTexture_Com);
    Safe_Release(m_pVIBuffer_Com);
    Safe_Release(m_pTransform_Com);
}
