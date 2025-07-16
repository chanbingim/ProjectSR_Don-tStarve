#include "EnviornmentButton.h"
#include "GameInstance.h"

CEnviornmentButton::CEnviornmentButton(LPDIRECT3DDEVICE9 pGraphic_Device) :
    CEventButton(pGraphic_Device)
{
}

CEnviornmentButton::CEnviornmentButton(const CEnviornmentButton& rhs) :
    CEventButton(rhs)
{
}

HRESULT CEnviornmentButton::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CEnviornmentButton::Initialize(void* pArg)
{
    if (FAILED(ADD_Components()))
        return E_FAIL;

    if(FAILED(CButton::Initialize(pArg)))
        return E_FAIL;


    return S_OK;
}

void CEnviornmentButton::Priority_Update(_float fTimeDelta)
{
}

void CEnviornmentButton::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CEnviornmentButton::Late_Update(_float fTimeDelta)
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

HRESULT CEnviornmentButton::Render()
{
    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());
    m_pTexture_Com->Set_Texture(m_ButtonIndex);
    m_pVIBuffer_Com->Render();

    return S_OK;
}

HRESULT CEnviornmentButton::ADD_Components()
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

    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_EnviornmetButton"),
        TEXT("Com_Texture"),
        reinterpret_cast<CComponent**>(&m_pTexture_Com))))
        return E_FAIL;


    return S_OK;
}

CEnviornmentButton* CEnviornmentButton::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CEnviornmentButton* pInstance = new CEnviornmentButton(pGraphic_Device);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("CREATE FAIL : CREATE ENV BUTTON");
    }
    return pInstance;
}

CGameObject* CEnviornmentButton::Clone(void* pArg)
{
    CEnviornmentButton* pInstance = new CEnviornmentButton(*this);
    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("CLONE FAIL : CREATE ENV BUTTON");
    }
    return pInstance;
}

void CEnviornmentButton::Free()
{
    __super::Free();
}
