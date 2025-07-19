#include "Select_Character.h"

#include "GameInstance.h"
#include "Character_Button.h"
#include "Character_Info.h"

CSelect_Character::CSelect_Character(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CUserInterface{ pGraphic_Device }
{
}

CSelect_Character::CSelect_Character(const CSelect_Character& Prototype)
    : CUserInterface{ Prototype }
{
}

HRESULT CSelect_Character::Initialize_Prototype()
{

    return S_OK;
}

HRESULT CSelect_Character::Initialize(void* pArg)
{
    CUserInterface::UIOBJECT_DESC Desc = {};

    Desc.fSizeX = 1290.f;
    Desc.fSizeY = 725.f;
    Desc.fX = g_iWinSizeX * 0.5f;
    Desc.fY = g_iWinSizeY * 0.5f;

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    __super::UpdatePosition();

    CButton::BUTTON_DESC Button_Desc = {};

    Button_Desc.iTextureIndex = 0;
    Button_Desc.fX = m_fX;
    Button_Desc.fY = m_fY;
    Button_Desc.fSizeX = 150.f;
    Button_Desc.fSizeY = 150.f;
    Button_Desc.fRelativeX = -170.f;
    Button_Desc.fRelativeY = 150.f;
    Button_Desc.pParentTransform = m_pTransform_Com;


    m_pButton_Wilson = dynamic_cast<CCharacter_Button*>(m_pGameInstance->Clone_Prototype(
        PROTOTYPE::GAMEOBJECT, EnumToInt(LEVEL::SELECT), TEXT("Prototype_GameObject_Character_Button"), &Button_Desc));

    Button_Desc.iTextureIndex = 1;
    Button_Desc.fRelativeX = 0.f;
    Button_Desc.fRelativeY = 150.f;
    
    m_pButton_Wigfrid = dynamic_cast<CCharacter_Button*>(m_pGameInstance->Clone_Prototype(
        PROTOTYPE::GAMEOBJECT, EnumToInt(LEVEL::SELECT), TEXT("Prototype_GameObject_Character_Button"), &Button_Desc));
    
    Button_Desc.iTextureIndex = 0;
    Button_Desc.fSizeX = 414.f;
    Button_Desc.fSizeY = 720.f;
    Button_Desc.fRelativeX = 433.f;
    Button_Desc.fRelativeY = 0.f;

    m_pCharater_Info = dynamic_cast<CCharacter_Info*>(m_pGameInstance->Clone_Prototype(
        PROTOTYPE::GAMEOBJECT, EnumToInt(LEVEL::SELECT), TEXT("Prototype_GameObject_Character_Info"), &Button_Desc));


  
    return S_OK;
}

void CSelect_Character::Priority_Update(_float fTimeDelta)
{

}

void CSelect_Character::Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::ORTTHO_UI, this);

    m_pButton_Wilson->Update(fTimeDelta);
    m_pButton_Wigfrid->Update(fTimeDelta);
    m_pCharater_Info->Update(fTimeDelta);

    if (m_pButton_Wilson->OnClick())
        m_pCharater_Info->Set_Button(0);
    else if (m_pButton_Wigfrid->OnClick())
        m_pCharater_Info->Set_Button(1);
}

void CSelect_Character::Late_Update(_float fTimeDelta)
{
   
}

HRESULT CSelect_Character::Render()
{
    m_pTexture_Com->Set_Texture(0);

    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());

    m_pVIBuffer_Com->Render();

    return S_OK;
}

HRESULT CSelect_Character::ADD_Components()
{
    // Texture Component
    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::SELECT), TEXT("Prototype_Component_Texture_Select"),
        TEXT("Com_Texture"),
        reinterpret_cast<CComponent**>(&m_pTexture_Com))))
        return E_FAIL;


    // VIBuffer_Rect Component
    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"),
        reinterpret_cast<CComponent**>(&m_pVIBuffer_Com))))
        return E_FAIL;

    Engine::CTransform::TRANSFORM_DESC Transform_Desc = { 5.f, D3DXToRadian(90.f) };

    // Transform Component
    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"),
        reinterpret_cast<CComponent**>(&m_pTransform_Com), &Transform_Desc)))
        return E_FAIL;


    return S_OK;
}

CSelect_Character* CSelect_Character::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CSelect_Character* pInstance = new CSelect_Character(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CSelect_Character");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CSelect_Character::Clone(void* pArg)
{
    CGameObject* pInstance = new CSelect_Character(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CSelect_Character");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CSelect_Character::Free()
{
    __super::Free();

    Safe_Release(m_pButton_Wilson);
    Safe_Release(m_pButton_Wigfrid);
    Safe_Release(m_pCharater_Info);

    Safe_Release(m_pTexture_Com);
    Safe_Release(m_pTransform_Com);
    Safe_Release(m_pVIBuffer_Com);

}
