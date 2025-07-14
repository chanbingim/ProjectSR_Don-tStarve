#include "QuestFrameUI.h"

#include "GameInstance.h"
#include "QuestCategoryButton.h"
#include "ListBoxUI.h"

CQuestFrameUI::CQuestFrameUI(LPDIRECT3DDEVICE9 pGraphic_Device) : 
    CUserInterface(pGraphic_Device)
{
}

CQuestFrameUI::CQuestFrameUI(const CQuestFrameUI& rhs) :
    CUserInterface(rhs)
{
}

HRESULT CQuestFrameUI::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CQuestFrameUI::Initialize(void* pArg)
{
    UIOBJECT_DESC   Info = {};
    Info.fSizeX = 600.f;
    Info.fSizeY = 500.f;
    Info.fX = g_iWinSizeX - 300.f;
    Info.fY = 360.f;

    if (FAILED(__super::Initialize(&Info)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(ADD_CategoryButton()))
        return E_FAIL;

    CListBoxUI::LISTBOXUI_DESC Desc;

    Desc.fSizeX = m_fSizeX;
    Desc.fSizeY = m_fSizeY - 200.f;

    Desc.fX = 0.f;
    Desc.fY = 100.f;

    Desc.m_ipreviewCnt = 4;
    Desc.pParentTransform_Com = m_pTransform_Com;

    m_pListBox = static_cast<CListBoxUI*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_ListBox"), &Desc));

    ClieckedButton(0);
    m_ZOrder = 1.f;
    m_pCategoryButton[0]->SetClickEvent([&]() { ClieckedButton(0); });
    m_pCategoryButton[1]->SetClickEvent([&]() { ClieckedButton(1); });
    m_pCategoryButton[2]->SetClickEvent([&]() { ClieckedButton(2); });

    return S_OK;
}

void CQuestFrameUI::Priority_Update(_float fTimeDelta)
{
    if (m_bIsActive)
    {
        m_pListBox->Priority_Update(fTimeDelta);
    }
}

void CQuestFrameUI::Update(_float fTimeDelta)
{
    if (m_bIsActive)
    {
        m_pListBox->Update(fTimeDelta);
    }
}

void CQuestFrameUI::Late_Update(_float fTimeDelta)
{
    if (m_bIsActive)
    {
        //구성 요소 여기서 업데이트
        UpdatePosition(0.9f);
        m_FontRect = {
            static_cast<long>(m_fX - (m_fSizeX * 0.5f) + 30),
            static_cast<long>(m_fY - (m_fSizeY * 0.5f) + 10),
            static_cast<long>(m_fX - (m_fSizeX * 0.5f) + 230),
            static_cast<long>(m_fY - (m_fSizeY * 0.5f) + 70),
        };

        for (int i = 0; i < 3; ++i)
            m_pCategoryButton[i]->Late_Update(fTimeDelta);

        m_pGameInstance->Add_RenderGroup(RENDER::ORTTHO_UI, this);

        m_pListBox->Late_Update(fTimeDelta);
    }
}

HRESULT CQuestFrameUI::Render()
{
    for (int i = 0; i < 3; ++i)
        m_pCategoryButton[i]->Render();

    m_pGraphic_Device->SetTransform(D3DTS_WORLD,&m_pTransform_Com->Get_World());

    m_pTexture_Com->Set_Texture(0);
    m_pVIBuffer_Com->Render();

    D3DXCOLOR white = { 1.f,1.f,1.f,1.f };
    m_pGameInstance->Render_Font(TEXT("Date_40"), m_szTittle, &m_FontRect, white);
    m_pListBox->Render();

    return S_OK;
}

void CQuestFrameUI::ClieckedButton(_uint Index)
{
    m_pListBox->UpdateQuestList(Index);
}

void CQuestFrameUI::ToggleObject()
{
    m_bIsActive = !m_bIsActive;
}

HRESULT CQuestFrameUI::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransform_Com)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBuffer_Com)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Quest"), TEXT("Com_Texture"), (CComponent**)&m_pTexture_Com)))
        return E_FAIL;

    return S_OK;
}

HRESULT CQuestFrameUI::ADD_CategoryButton()
{
    for (int i = 0; i < 3; ++i)
    {
        CQuestCategoryButton::BUTTON_DESC Desc;
        Desc.pParentTransform = m_pTransform_Com;
        Desc.fSizeX = 50.f;
        Desc.fSizeY = 100.f;

        Desc.fRelativeX = m_fX + (Desc.fSizeX * i) + 10 * i;
        Desc.fRelativeY = m_fY - (m_fSizeY * 0.37f);
        m_pCategoryButton[i] = static_cast<CQuestCategoryButton *>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_QuestCategoryButton"), &Desc));
        m_pCategoryButton[i]->ChangeButtonTex(i);
    }

    return S_OK;
}

CQuestFrameUI* CQuestFrameUI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CQuestFrameUI* pInstance = new CQuestFrameUI(pGraphic_Device);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("CREATE FAIL : QUEST FRAME");
    }

    return pInstance;
}

CGameObject* CQuestFrameUI::Clone(void* pArg)
{
    CQuestFrameUI* pInstance = new CQuestFrameUI(*this);
    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("CLONE FAIL : QUEST FRAME");
    }

    return pInstance;
}

void CQuestFrameUI::Free()
{
    __super::Free();

    for (int i = 0; i < 3; ++i)
        Safe_Release(m_pCategoryButton[i]);

    Safe_Release(m_pListBox);
    Safe_Release(m_pTexture_Com);
    Safe_Release(m_pVIBuffer_Com);
    Safe_Release(m_pTransform_Com);
}
