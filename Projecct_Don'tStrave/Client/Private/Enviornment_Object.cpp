#include "Enviornment_Object.h"
#include "GameInstance.h"

#include "Camera.h"

CEnviornment_Object::CEnviornment_Object(LPDIRECT3DDEVICE9 pGraphic_Device) :
    CAinimationObject(pGraphic_Device)
{
}

CEnviornment_Object::CEnviornment_Object(const CEnviornment_Object& rhs) :
    CAinimationObject(rhs),
    m_EnviornmentID(rhs.m_EnviornmentID)
{
}

HRESULT CEnviornment_Object::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CEnviornment_Object::Initialize(void* pArg)
{
    if (nullptr == pArg)
    {

    }
    else
    {
        GAMEOBJECT_DESC* TeerrainDesc = static_cast<GAMEOBJECT_DESC*>(pArg);

        m_pTransformCom->SetPosition(TeerrainDesc->vPosition);
        m_pTransformCom->SetScale(TeerrainDesc->vScale);
        m_pTransformCom->SetRotation(TeerrainDesc->vRotation);
    }

    m_bEnableBillboard = true;
    Setting_Shader(L"BillBoard.fx");

    return S_OK;
}

void CEnviornment_Object::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CEnviornment_Object::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);

    _float3 Pos;
    if (m_pVIBufferCom->Picking(m_pTransformCom, &Pos) && m_pGameInstance->KeyDown(VK_LBUTTON))
    {
        Damage(nullptr);
    }
}

void CEnviornment_Object::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
    class CGameObject* Obj = m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Camera"));
    auto Camera = dynamic_cast<CCamera*>(Obj);
    if (nullptr == Camera)
        return;

  
    //if(Camera->IsInObject(m_pTransformCom->GetWorldState(WORLDSTATE::POSITION)))
    m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);
}

void CEnviornment_Object::Reset_State()
{
}

HRESULT CEnviornment_Object::Render()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    __super::Render();
    XMLRenderAnimation(m_FrontName + m_TailName);

    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    return S_OK;
}

void CEnviornment_Object::Damage(void* pArg)
{
}

void CEnviornment_Object::Death()
{
}

_wstring CEnviornment_Object::GetEnviornmnetName()
{
    switch (m_EnviornmentID)
    {
    case 1 :
        return TEXT("Portal");
    case 2:
        return TEXT("Grass");
    case 3:
        return TEXT("Rock");
    case 4:
        return TEXT("Tree");
    case 5:
        return TEXT("Gold Vein");
    }

    return TEXT("");;
}

_uint CEnviornment_Object::GetEnviormentID()
{
    return m_EnviornmentID;
}

CEnviornment_Object* CEnviornment_Object::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CEnviornment_Object* pInstance = new CEnviornment_Object(pGraphic_Device);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("CREATE FAIL : ENVIORNMENT");
    }

    return pInstance;
}

CGameObject* CEnviornment_Object::Clone(void* pArg)
{
    CEnviornment_Object* pInstance = new CEnviornment_Object(*this);
    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("CLONE FAIL : ENVIORNMENT");
    }

    return pInstance;
}

void CEnviornment_Object::Free()
{
    __super::Free();

    Safe_Release(m_pTransformCom);
    Safe_Release(m_pCollision_Com);
}
