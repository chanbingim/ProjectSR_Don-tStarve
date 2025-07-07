#include "Enviornment_Object.h"

#include "Terrian_Manager.h"
#include "Terrain.h"
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
        CLandObject::LANDOBJECT_DESC Arg = {};
        GAMEOBJECT_DESC* TeerrainDesc = static_cast<GAMEOBJECT_DESC*>(pArg);

        m_pTransformCom->SetPosition(TeerrainDesc->vPosition);
        m_pTransformCom->SetScale(TeerrainDesc->vScale);
        m_pTransformCom->SetRotation(TeerrainDesc->vRotation);

        auto Terrian = m_pTerrian_Manager->GetOnTerrian(TeerrainDesc->vPosition);
        if (nullptr == Terrian)
            int a = 10;

        Arg.pLandVIBuffer = Terrian->GetCurVIBuffer();
        Arg.pLandTransform = Terrian->GetTransfrom();

        __super::Initialize(&Arg);
        SetUp_OnTerrain(m_pTransformCom);
    }

    m_bEnableBillboard = true;
    m_pCollision_Com->Enable(false);

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
    if ( m_pGameInstance->KeyDown(VK_LBUTTON) && m_pVIBufferCom->Picking(m_pTransformCom, &Pos))
    {
        DamageBaseDesc  damage;
        damage.Attacker = this;

        Damage(&damage);
    }
}

void CEnviornment_Object::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
    
    if(m_pCamera->IsInObject(m_pTransformCom->GetWorldState(WORLDSTATE::POSITION)))
        m_pGameInstance->Add_RenderGroup(RENDER::ALPHATEST, this);
}

void CEnviornment_Object::Reset_State()
{
}

HRESULT CEnviornment_Object::Render()
{
    //m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);

    __super::Render();
    XMLRenderAnimation(m_FrontName + m_TailName);

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
    case 6:
        return TEXT("Resurrection Stone");
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
