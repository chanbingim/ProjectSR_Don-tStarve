#include "Enviornment_Object.h"
#include "GameInstance.h"

#include "Camera.h"

CEnviornment_Object::CEnviornment_Object(LPDIRECT3DDEVICE9 pGraphic_Device) :
    CLandObject(pGraphic_Device)
{
}

CEnviornment_Object::CEnviornment_Object(const CEnviornment_Object& rhs) :
    CLandObject(rhs)
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

    
}

void CEnviornment_Object::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);

    m_pGameInstance->Add_RenderGroup(RENDER::ALPHATEST, this);
}

HRESULT CEnviornment_Object::Render()
{
    class CGameObject* Obj = m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Camera"));
    auto Camera = dynamic_cast<CCamera*>(Obj);
    if (nullptr == Camera)
        return E_FAIL;
    /*if (FAILED(Begin_RenderState()))
        return E_FAIL;*/

    LPDIRECT3DBASETEXTURE9 pTex = { nullptr };

    m_pGraphic_Device->GetTexture(0, &pTex);
    Excute_Billboard(Camera->GetInvViewMat(), pTex);

    __super::Render();
    m_pVIBuffer_Com->Render();

    Safe_Release(pTex);
    End_Billboard();
    return S_OK;
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
    Safe_Release(m_Idle_pTexture_Com);
    Safe_Release(m_pVIBuffer_Com);
    Safe_Release(m_pCollision_Com);
}
