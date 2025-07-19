#include "QuestCategoryButton.h"
#include "GameInstance.h"

CQuestCategoryButton::CQuestCategoryButton(LPDIRECT3DDEVICE9 pGraphic_Device) :
    CButton(pGraphic_Device)
{
}

CQuestCategoryButton::CQuestCategoryButton(const CQuestCategoryButton& rhs) :
    CButton(rhs)
{
}

HRESULT CQuestCategoryButton::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CQuestCategoryButton::Initialize(void* pArg)
{
    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CQuestCategoryButton::Priority_Update(_float fTimeDelta)
{

}

void CQuestCategoryButton::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);

  
}

void CQuestCategoryButton::Late_Update(_float fTimeDelta)
{   
    UpdatePosition();
    if (isMouseOver())
    {
        auto vPos = m_pTransform_Com->GetWorldState(WORLDSTATE::POSITION);
        vPos.y += 10.f;
        m_pTransform_Com->SetPosition(vPos);
        if (m_pGameInstance->KeyDown(VK_LBUTTON))
        {
            if (m_OnclickedEvent)
            {
                m_OnclickedEvent();
            }
        }
    }

    m_pGameInstance->Add_RenderGroup(RENDER::ORTTHO_UI, this);
}

HRESULT CQuestCategoryButton::Render()
{
    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());
    m_pTexture_Com->Set_Texture(m_ButtonIndex);
    m_pVIBuffer_Com->Render();

    return S_OK;
}

void CQuestCategoryButton::SetClickEvent(function<void()> Func)
{
    m_OnclickedEvent = Func;
}

void CQuestCategoryButton::ChangeButtonTex(_uint Index)
{
    m_ButtonIndex = Index;
}

HRESULT CQuestCategoryButton::ADD_Components()
{
    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"),
        reinterpret_cast<CComponent**>(&m_pVIBuffer_Com))))
        return E_FAIL;

    Engine::CTransform::TRANSFORM_DESC Transform_Desc = { 5.f, D3DXToRadian(90.f) };

    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"),
        reinterpret_cast<CComponent**>(&m_pTransform_Com), &Transform_Desc)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_QuestCategory"),
        TEXT("Com_Texture"),
        reinterpret_cast<CComponent**>(&m_pTexture_Com))))
        return E_FAIL;

    return S_OK;
}

CQuestCategoryButton* CQuestCategoryButton::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CQuestCategoryButton* pInstance = new CQuestCategoryButton(pGraphic_Device);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("CREATE FAIL : EVENT BUTTON");
    }

    return pInstance;
}

CGameObject* CQuestCategoryButton::Clone(void* pArg)
{
    CQuestCategoryButton* pInstance = new CQuestCategoryButton(*this);
    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("CLONE FAIL : EVENT BUTTON");
    }

    return pInstance;
}

void CQuestCategoryButton::Free()
{
    __super::Free();

    Safe_Release(m_pTransform_Com);
    Safe_Release(m_pVIBuffer_Com);
    Safe_Release(m_pTexture_Com);

}
