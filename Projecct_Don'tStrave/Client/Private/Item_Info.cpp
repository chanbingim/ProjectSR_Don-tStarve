#include "Item_Info.h"

#include "GameInstance.h"

CItem_Info::CItem_Info(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CUserInterface{ pGraphic_Device }
{
}

CItem_Info::CItem_Info(const CItem_Info& Prototype)
    : CUserInterface{ Prototype }
{
}

HRESULT CItem_Info::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CItem_Info::Initialize(void* pArg)
{
    UIOBJECT_DESC* pDesc = static_cast<UIOBJECT_DESC*>(pArg);

    UIOBJECT_DESC Desc = {};
    Desc.fX = pDesc->fX;
    Desc.fY = pDesc->fY + 145.f;
    Desc.fSizeX = pDesc->fSizeX - 60.f;
    Desc.fSizeY = 160.f;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    m_rcItemName = {
       static_cast<LONG>(m_fX - m_fSizeX * 0.5f),
       static_cast<LONG>(m_fY - m_fSizeY * 0.5f + 10.f),
       static_cast<LONG>(m_fX),
       static_cast<LONG>(m_fY - m_fSizeY * 0.5f + 30.f) };

    m_rcItemInfo = {
        static_cast<LONG>(m_fX - m_fSizeX * 0.5f),
        static_cast<LONG>(m_fY - m_fSizeY * 0.5f),
        static_cast<LONG>(m_fX),
        static_cast<LONG>(m_fY + m_fSizeY * 0.5f) };

    __super::UpdatePosition();

    return S_OK;
}

void CItem_Info::Priority_Update(_float fTimeDelta)
{
}

void CItem_Info::Update(_float fTimeDelta)
{

}

void CItem_Info::Late_Update(_float fTimeDelta)
{
}

HRESULT CItem_Info::Render()
{
    m_pTexture_Com->Set_Texture(0);

    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());

    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    //m_pVIBuffer_Com->Render();

    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    // Test Code
    /*RECT rect = { m_fX - m_fSizeX * 0.5f, m_fY - m_fSizeY * 0.5f,m_fX + m_fSizeX * 0.5f,m_fY + m_fSizeY * 0.5f };
    wstring s = to_wstring(m_iDate) + L"ÀÏ";
    m_pGameInstance->Render_Font(TEXT("Date_40"), s.c_str(), &rect);*/
    
    m_pGameInstance->Render_Font(TEXT("Font_18"), TEXT("È½ºÒ"), &m_rcItemName);

    m_pGameInstance->Render_Font(TEXT("Font_14"), TEXT("¼³¸í..."), &m_rcItemInfo);


    return S_OK;
}

HRESULT CItem_Info::ADD_Components()
{
    // Texture Component
    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Slot"),
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

CItem_Info* CItem_Info::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CItem_Info* pInstance = new CItem_Info(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CItem_Info");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CItem_Info::Clone(void* pArg)
{
    CGameObject* pInstance = new CItem_Info(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CItem_Info");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CItem_Info::Free()
{
    __super::Free();

    Safe_Release(m_pTexture_Com);
    Safe_Release(m_pTransform_Com);
    Safe_Release(m_pVIBuffer_Com);
}
