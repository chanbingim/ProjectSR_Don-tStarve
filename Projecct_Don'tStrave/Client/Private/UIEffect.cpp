#include "UIEffect.h"

#include "Slot.h"
#include "GameInstance.h"

CUIEffect::CUIEffect(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CUserInterface{pGraphic_Device}
{
}

CUIEffect::CUIEffect(const CUIEffect& Prototype)
    :CUserInterface{ Prototype }, m_fTimeAcc{0.f}
{
}

HRESULT CUIEffect::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUIEffect::Initialize(void* pArg)
{
    if (FAILED(Add_Components()))
        return E_FAIL;

    CUIEffect::UIEFFECT_DESC* pDesc = static_cast<UIEFFECT_DESC*>(pArg);

    m_iTextureIndex = pDesc->iItemID;
    m_pSlot = pDesc->pSlot;

    memcpy(&m_Item_Desc, &pDesc->Item_Desc, sizeof(ITEM_DESC));

    _float3 vPos = pDesc->vPositon;
    _float4x4 matView = {};
    _float4x4 matProjection = {};

    m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
    m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProjection);

    D3DXVec3TransformCoord(&vPos, &vPos, &matView);
    D3DXVec3TransformCoord(&vPos, &vPos, &matProjection);
    

    _float3 vTargetPos = m_pSlot->Get_Position();

    CUserInterface::UIOBJECT_DESC Desc = {};

    Desc.fSizeX = 60.f;
    Desc.fSizeY = 60.f;
    Desc.fX = vPos.x * (g_iWinSizeX * 0.5f) + g_iWinSizeX * 0.5f;
    Desc.fY = -vPos.y * (g_iWinSizeY * 0.5f) + g_iWinSizeY * 0.5f;

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    m_TargetDir = _float3((vTargetPos.x + g_iWinSizeX * 0.5f) - Desc.fX, -(vTargetPos.y + g_iWinSizeY * 0.5f) - Desc.fY, 0.f);

    __super::UpdatePosition();
    
    return S_OK;
}

void CUIEffect::Priority_Update(_float fTimeDelta)
{
    
}

void CUIEffect::Update(_float fTimeDelta)
{
    m_fTimeAcc += fTimeDelta;
    _float3 vPosition = m_pTransform_Com->GetWorldState(WORLDSTATE::POSITION);
    vPosition += m_TargetDir * fTimeDelta * 4.f;
    m_pTransform_Com->SetPosition(vPosition);

    if (0.25f < m_fTimeAcc)
    {
        m_isDead = true;

        _uint iItemID = m_pSlot->Get_ItemID();
        if (0 == iItemID)
        {
            m_pSlot->Set_Info(m_Item_Desc);
        }
        else if (m_iTextureIndex == iItemID)
        {
            m_pSlot->Merge_Item(m_Item_Desc);
            m_isDead = true;
        }
    }


    
}

void CUIEffect::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::ORTTHO_UI, this);
}

HRESULT CUIEffect::Render()
{
    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());

    m_pTexture_Com->Set_Texture(m_iTextureIndex);

    m_pVIBuffer_Com->Render();

    return S_OK;
}

HRESULT CUIEffect::Add_Components()
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

    if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Item"),
        TEXT("Com_Texture"),
        reinterpret_cast<CComponent**>(&m_pTexture_Com))))
        return E_FAIL;

    return S_OK;
}

CUIEffect* CUIEffect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CUIEffect* pInstance = new CUIEffect(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CUIEffect");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CUIEffect::Clone(void* pArg)
{
    CUIEffect* pInstance = new CUIEffect(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CUIEffect");
        return nullptr;
    }

    return pInstance;
}

void CUIEffect::Free()
{
    __super::Free();

    Safe_Release(m_pTransform_Com);
    Safe_Release(m_pVIBuffer_Com);
    Safe_Release(m_pTexture_Com);
}
