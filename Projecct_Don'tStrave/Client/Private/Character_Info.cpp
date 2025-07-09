#include "Character_Info.h"

#include "GameInstance.h"
#include "PlayerData_Manager.h"

CCharacter_Info::CCharacter_Info(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CButton{ pGraphic_Device }
{
}

CCharacter_Info::CCharacter_Info(const CCharacter_Info& Prototype)
    :CButton{ Prototype }
{
}

HRESULT CCharacter_Info::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CCharacter_Info::Initialize(void* pArg)
{
    m_iTextureIndex = 0;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    __super::UpdatePosition();

    return S_OK;
}

void CCharacter_Info::Priority_Update(_float fTimeDelta)
{

}

void CCharacter_Info::Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::ORTTHO_UI, this);

    __super::Update(fTimeDelta);

}

void CCharacter_Info::Late_Update(_float fTimeDelta)
{
}

HRESULT CCharacter_Info::Render()
{
    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());

    m_pTexture_Com->Set_Texture(m_iTextureIndex);

    m_pVIBuffer_Com->Render();

    return S_OK;
}

HRESULT CCharacter_Info::ADD_Components()
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

    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::SELECT), TEXT("Prototype_Component_Texture_Character_Info"),
        TEXT("Com_Texture"),
        reinterpret_cast<CComponent**>(&m_pTexture_Com))))
        return E_FAIL;

    return S_OK;
}

CCharacter_Info* CCharacter_Info::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CCharacter_Info* pInstance = new CCharacter_Info(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Create : CCharacter_Info");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CCharacter_Info::Clone(void* pArg)
{
    CGameObject* pInstance = new CCharacter_Info(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CCharacter_Button");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CCharacter_Info::Free()
{
    __super::Free();

    Safe_Release(m_pTexture_Com);
    Safe_Release(m_pTransform_Com);
    Safe_Release(m_pVIBuffer_Com);
}
