#include "FoodEffect.h"

#include "Slot.h"
#include "GameInstance.h"

CFoodEffect::CFoodEffect(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CUserInterface{ pGraphic_Device }
{
}

CFoodEffect::CFoodEffect(const CFoodEffect& Prototype)
    :CUserInterface{ Prototype }, m_fTimeAcc{ 0.f }
{
}

HRESULT CFoodEffect::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CFoodEffect::Initialize(void* pArg)
{
    if (FAILED(Add_Components()))
        return E_FAIL;

    CFoodEffect::FOODEFFECT_DESC* pDesc = static_cast<FOODEFFECT_DESC*>(pArg);

    m_iTextureIndex = pDesc->iItemID;
    m_pTargetTransform = pDesc->pTransform;

    CUserInterface::UIOBJECT_DESC Desc = {};

    Desc.fSizeX = 60.f;
    Desc.fSizeY = 60.f;
    Desc.fX = 30.f;
    Desc.fX = 30.f;
    

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    __super::UpdatePosition();

   

    return S_OK;
}

void CFoodEffect::Priority_Update(_float fTimeDelta)
{

}

void CFoodEffect::Update(_float fTimeDelta)
{

}

void CFoodEffect::Late_Update(_float fTimeDelta)
{
    
}

HRESULT CFoodEffect::Render()
{
    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());

    m_pTexture_Com->Set_Texture(m_iTextureIndex);

    m_pVIBuffer_Com->Render();

    return S_OK;
}

HRESULT CFoodEffect::Add_Components()
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

CFoodEffect* CFoodEffect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CFoodEffect* pInstance = new CFoodEffect(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CUIEffect");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CFoodEffect::Clone(void* pArg)
{
    CGameObject* pInstance = new CFoodEffect(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CUIEffect");
        return nullptr;
    }

    return pInstance;
}

void CFoodEffect::Free()
{
    __super::Free();

    //Safe_Release(m_pSlot);
    Safe_Release(m_pTransform_Com);
    Safe_Release(m_pVIBuffer_Com);
    Safe_Release(m_pTexture_Com);
}
