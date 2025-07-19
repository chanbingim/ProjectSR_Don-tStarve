#include "EventButton.h"
#include "GameInstance.h"

CEventButton::CEventButton(LPDIRECT3DDEVICE9 pGraphic_Device) :
    CButton(pGraphic_Device)
{
}

CEventButton::CEventButton(const CEventButton& rhs) :
    CButton(rhs)
{
}

HRESULT CEventButton::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CEventButton::Initialize(void* pArg)
{
    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CEventButton::Priority_Update(_float fTimeDelta)
{

}

void CEventButton::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);

  
}

void CEventButton::Late_Update(_float fTimeDelta)
{   
    UpdatePosition();
    if (isMouseOver())
    {
        auto vScale = m_pTransform_Com->GetScale();
        vScale *= 1.2f;
        m_pTransform_Com->SetScale(vScale);
        if (m_pGameInstance->KeyDown(VK_LBUTTON))
        {
            if (m_OnclickedEvent)
            {
                m_OnclickedEvent();
            }
        }
    }
}

HRESULT CEventButton::Render()
{
    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());
    m_pTexture_Com->Set_Texture(m_ButtonIndex);
    m_pVIBuffer_Com->Render();

    return S_OK;
}

void CEventButton::SetClickEvent(function<void()> Func)
{
    m_OnclickedEvent = Func;
}


void CEventButton::ChangeButtonTex(_uint Index)
{
    m_ButtonIndex = Index;
}

HRESULT CEventButton::ADD_Components()
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

    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_QuestButton"),
        TEXT("Com_Texture"),
        reinterpret_cast<CComponent**>(&m_pTexture_Com))))
        return E_FAIL;

    return S_OK;
}

CEventButton* CEventButton::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CEventButton* pInstance = new CEventButton(pGraphic_Device);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("CREATE FAIL : EVENT BUTTON");
    }

    return pInstance;
}

CGameObject* CEventButton::Clone(void* pArg)
{
    CEventButton* pInstance = new CEventButton(*this);
    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("CLONE FAIL : EVENT BUTTON");
    }

    return pInstance;
}

void CEventButton::Free()
{
    __super::Free();

    Safe_Release(m_pTransform_Com);
    Safe_Release(m_pVIBuffer_Com);
    Safe_Release(m_pTexture_Com);

}
