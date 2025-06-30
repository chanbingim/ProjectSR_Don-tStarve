#include "Item_Info.h"

#include "GameInstance.h"
#include "Item_Manager.h"

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
       static_cast<LONG>(m_fY - m_fSizeY * 0.1f + 10.f),
       static_cast<LONG>(m_fX),
       static_cast<LONG>(m_fY - m_fSizeY * 0.3f + 55.f) };

    m_rcItemInfo = {
        static_cast<LONG>(m_fX - m_fSizeX * 0.5f + 20.f),
        static_cast<LONG>(m_fY - m_fSizeY * 0.3f + 70.f),
        static_cast<LONG>(m_fX - 20.f),
        static_cast<LONG>(m_fY + m_fSizeY * 0.3f) };

    __super::UpdatePosition();

    m_SelectedItemID = 0;

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

HRESULT CItem_Info::Render(CTransform* pTransform)
{
    m_pTexture_Com->Set_Texture(m_SelectedItemID);


    _float3 vPos = pTransform->GetWorldState(WORLDSTATE::POSITION);
    _float3 vScale = pTransform->GetScale();

    pTransform->SetPosition(_float3(vPos.x + 80.f, vPos.y - 100.f, 0.f));
    pTransform->SetScale(_float3(45.f, 45.f, 1.f));
    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &pTransform->Get_World());

    m_pVIBuffer_Com->Render();

    pTransform->SetPosition(vPos);
    pTransform->SetScale(vScale);

    D3DXCOLOR white = { 1.f,1.f,1.f,1.f };
    ITEM_DATA data = CItem_Manager::GetInstance()->Get_ItemData(m_SelectedItemID);

    m_pGameInstance->Render_Font(TEXT("Font_25"), data.strName.c_str(), &m_rcItemName, white);

    m_pGameInstance->Render_Font(TEXT("Font_14"), data.strExplanation.c_str(), &m_rcItemInfo, white, DT_LEFT);


    return S_OK;
}

HRESULT CItem_Info::ADD_Components()
{
    // Texture Component
    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Item"),
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

void CItem_Info::Update_Rect(_float fX, _float fY)
{
    m_rcItemName = {
       static_cast<LONG>(fX - m_fSizeX * 0.5f),
       static_cast<LONG>(fY + 70.f),
       static_cast<LONG>(fX),
       static_cast<LONG>(fY + 100.f ) };

    m_rcItemInfo = {
        static_cast<LONG>(fX - m_fSizeX * 0.5f + 20.f),
        static_cast<LONG>(fY + 130.f ),
        static_cast<LONG>(fX + m_fSizeX * 0.5f),
        static_cast<LONG>(fY + 300.f) };
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

    //Safe_Release(m_pBtnCreate);
    //Safe_Release(m_pBtnAddToQuickSlot);
}
