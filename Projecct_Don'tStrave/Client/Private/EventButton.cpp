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
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CEventButton::Priority_Update(_float fTimeDelta)
{

}

void CEventButton::Update(_float fTimeDelta)
{
    m_ButtonMat = m_pTransform_Com->Get_World() * m_pParentTransform->Get_World();

    if (m_isClicked)
    {
        if (m_OnclickedEvent)
        {
            m_OnclickedEvent();
        }
    }
}

void CEventButton::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::ORTTHO_UI, this);
}

HRESULT CEventButton::Render()
{
    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_ButtonMat);
    m_pTexture_Com->Set_Texture(0);
    m_pVIBuffer_Com->Render();

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
}
