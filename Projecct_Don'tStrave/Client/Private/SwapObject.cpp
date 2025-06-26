#include "SwapObject.h"
#include "GameInstance.h"
#include "CMath.h"

HRESULT CSwapObject::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CSwapObject::Initialize(void* pArg)
{
    if (nullptr == pArg)
        return E_FAIL;

    SwapObject_Desc* UI_MoveDesc = static_cast<SwapObject_Desc*>(pArg);
    if (UI_MoveDesc)
    {
        __super::Initialize(pArg);
        m_pParentTransformCom = UI_MoveDesc->pParentTransformCom;
        Safe_AddRef(m_pParentTransformCom);
        m_pTransformCom = UI_MoveDesc->pTransformCom;
        Safe_AddRef(m_pTransformCom);
        m_StartPoint = UI_MoveDesc->StartPoint;
    }

    return S_OK;
}

HRESULT CSwapObject::Reset_StateData()
{
    if (FAILED(__super::Reset_StateData()))
        return E_FAIL;

    return S_OK;
}

void CSwapObject::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CSwapObject::Render()
{

    _float3		vPosition = m_pParentTransformCom->GetWorldState(WORLDSTATE::POSITION);
    _float3		vLook = m_pParentTransformCom->GetWorldState(WORLDSTATE::UP);

    vPosition += *D3DXVec3Normalize(&vLook, &vLook) * m_StartPoint.y;

    vLook = m_pParentTransformCom->GetWorldState(WORLDSTATE::RIGHT);

    vPosition += *D3DXVec3Normalize(&vLook, &vLook) * m_StartPoint.x;

    m_pTransformCom->SetPosition(vPosition);

    //m_pTransformCom->SetPosition(m_pParentTransformCom->GetWorldState(WORLDSTATE::POSITION) + m_StartPoint);
    m_Frame.pAnimTexture->Set_Texture(m_Frame.iStartFrame);
}

CSwapObject* CSwapObject::Create(void* pArg)
{
    CSwapObject* pInstance = new CSwapObject();
    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("CREAT FAIL : PLAYER ANIMATION");
    }

    return pInstance;
}

void CSwapObject::Free()
{
    __super::Free();
    Safe_Release(m_pParentTransformCom);
    Safe_Release(m_pTransformCom);
}
