#include "Character_Button.h"

#include "GameInstance.h"
#include "PlayerData_Manager.h"

CCharacter_Button::CCharacter_Button(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CButton{ pGraphic_Device }
{
}

CCharacter_Button::CCharacter_Button(const CCharacter_Button& Prototype)
    :CButton{ Prototype }
{
}

HRESULT CCharacter_Button::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CCharacter_Button::Initialize(void* pArg)
{
    m_iIndex = 0;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    __super::UpdatePosition();

    

    return S_OK;
}

void CCharacter_Button::Priority_Update(_float fTimeDelta)
{

}

void CCharacter_Button::Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::ORTTHO_UI, this);

    __super::Update(fTimeDelta);

    HoverEevent();

}

void CCharacter_Button::Late_Update(_float fTimeDelta)
{
}

HRESULT CCharacter_Button::Render()
{
    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());

    m_pTexture_Com->Set_Texture(m_iIndex);

    m_pVIBuffer_Com->Render();

    return S_OK;
}

void CCharacter_Button::HoverEevent()
{
    if (true == isMouseOver())
    {
        ClickedEevent();
        m_iIndex = m_iTextureIndex * 2 + 1;
    }
    else
    {
        m_iIndex = m_iTextureIndex * 2;
    }

}

void CCharacter_Button::ClickedEevent()
{
    if (m_pGameInstance->KeyDown(VK_LBUTTON))
    {
        m_isClicked = true;
        CPlayerData_Manager::GetInstance()->Set_CharacterID(200 + m_iTextureIndex);
    }
    else
        m_isClicked = false;

}

HRESULT CCharacter_Button::ADD_Components()
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

    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::SELECT), TEXT("Prototype_Component_Texture_Character_Button"),
        TEXT("Com_Texture"),
        reinterpret_cast<CComponent**>(&m_pTexture_Com))))
        return E_FAIL;

    return S_OK;
}

CCharacter_Button* CCharacter_Button::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CCharacter_Button* pInstance = new CCharacter_Button(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Create : CCharacter_Button");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CCharacter_Button::Clone(void* pArg)
{
    CGameObject* pInstance = new CCharacter_Button(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CCharacter_Button");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CCharacter_Button::Free()
{
    __super::Free();

    Safe_Release(m_pTexture_Com);
    Safe_Release(m_pTransform_Com);
    Safe_Release(m_pVIBuffer_Com);
}
