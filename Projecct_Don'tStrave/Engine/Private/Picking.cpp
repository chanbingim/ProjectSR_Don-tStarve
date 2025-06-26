#include "Picking.h"

CPicking::CPicking(LPDIRECT3DDEVICE9 pGraphicDevice)
    : m_pGraphic_Device { pGraphicDevice }
{
    Safe_AddRef(m_pGraphic_Device);
}

HRESULT CPicking::Initialize(HWND hWnd)
{
    m_hWnd = hWnd;

    D3DVIEWPORT9        Viewport{};
    m_pGraphic_Device->GetViewport(&Viewport);

    m_iWinSizeX = Viewport.Width;
    m_iWinSizeY = Viewport.Height;    

    return S_OK;
}

void CPicking::Update()
{
    /* 월드상에서의 마우스의 정보를 구한다 .*/


    /* 뷰포트 상의 마우스 좌표를 구한다. */
    POINT       ptMouse = {};
    GetCursorPos(&ptMouse);
    ScreenToClient(m_hWnd, &ptMouse);

    _float4         vMousePos{};

    /* 투영스페이스 상의 마우스 좌표를 구한다. (0, 0) -> (-1, 1) (wsx,wsy) -> (1, -1) */
    vMousePos.x = ptMouse.x / (m_iWinSizeX * 0.5f) - 1.f;
    vMousePos.y = ptMouse.y / -(m_iWinSizeY * 0.5f) + 1.f;
    vMousePos.z = 0.f; /* near를 클릭한 것이기 때문에 .*/
    vMousePos.w = 1.f; /* w로 나눠서 결정된 값이기 대문에. */

    /* 뷰스페이스 상의 마우스 좌표를 구한다. */
    _float4x4       ProjMatrixInv{};
    m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &ProjMatrixInv);
    D3DXMatrixInverse(&ProjMatrixInv, nullptr, &ProjMatrixInv);
    D3DXVec4Transform(&vMousePos, &vMousePos, &ProjMatrixInv);

    _float3         RayPos{}, RayDir{};
    RayPos = _float3(0.f, 0.f, 0.f);
    RayDir = _float3(vMousePos.x, vMousePos.y, vMousePos.z);

    /* 월드스페이스 상의 마우스 좌표를 구한다. */
    _float4x4       ViewMatrixInv{};
    m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrixInv);
    D3DXMatrixInverse(&ViewMatrixInv, nullptr, &ViewMatrixInv);   

    D3DXVec3TransformCoord(&m_vRayPos[ENUM_CLASS(RAY::WORLD)], &RayPos, &ViewMatrixInv);
    D3DXVec3TransformNormal(&m_vRayDir[ENUM_CLASS(RAY::WORLD)], &RayDir, &ViewMatrixInv);  

    D3DXVec3Normalize(&m_vRayDir[ENUM_CLASS(RAY::WORLD)], &m_vRayDir[ENUM_CLASS(RAY::WORLD)]);
}

void CPicking::Transform_ToLocalSpace(const _float4x4* pWorldMatrixInverse)
{
    D3DXVec3TransformCoord(&m_vRayPos[ENUM_CLASS(RAY::VIEW)], &m_vRayPos[ENUM_CLASS(RAY::WORLD)], pWorldMatrixInverse);
    D3DXVec3TransformNormal(&m_vRayDir[ENUM_CLASS(RAY::VIEW)], &m_vRayDir[ENUM_CLASS(RAY::WORLD)], pWorldMatrixInverse);

    D3DXVec3Normalize(&m_vRayDir[ENUM_CLASS(RAY::VIEW)], &m_vRayDir[ENUM_CLASS(RAY::VIEW)]);
}

_float3 CPicking::GetMousePosition(_uint ID)
{
    switch (ID)
    {
    case 0:
    {
        POINT       ptMouse = {};
        GetCursorPos(&ptMouse);
        ScreenToClient(m_hWnd, &ptMouse);
        return _float3((_float)ptMouse.x, (_float)ptMouse.y, 0.f);
    }
    case 1:
        return m_vRayPos[ENUM_CLASS(RAY::VIEW)];

    case 2:
        return m_vRayPos[ENUM_CLASS(RAY::WORLD)];

    default :
        return _float3(FLT_MAX, FLT_MAX, FLT_MAX);
    }
}

_bool CPicking::Picking_InWorldSpace(const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut)
{
    _float      fU{}, fV{}, fDist{};

    _bool       isColl = D3DXIntersectTri(&vPointA, &vPointB, &vPointC, &m_vRayPos[ENUM_CLASS(RAY::WORLD)], &m_vRayDir[ENUM_CLASS(RAY::WORLD)], &fU, &fV, &fDist);

    if (true == isColl)
    {
        *pOut = m_vRayPos[ENUM_CLASS(RAY::WORLD)] + m_vRayDir[ENUM_CLASS(RAY::WORLD)] * fDist;
    }

    return isColl;
}

_bool CPicking::Picking_InLocalSpace(const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut)
{
    _float      fU{}, fV{}, fDist{};

    _bool       isColl = D3DXIntersectTri(&vPointA, &vPointB, &vPointC, &m_vRayPos[ENUM_CLASS(RAY::VIEW)], &m_vRayDir[ENUM_CLASS(RAY::VIEW)], &fU, &fV, &fDist);

    if (true == isColl)
    {
        *pOut = m_vRayPos[ENUM_CLASS(RAY::VIEW)] + m_vRayDir[ENUM_CLASS(RAY::VIEW)] * fDist;
    }

    return isColl;
}

CPicking* CPicking::Create(LPDIRECT3DDEVICE9 pGraphicDevice, HWND Handle)
{
    CPicking* pInstance = new CPicking(pGraphicDevice);
    if (FAILED(pInstance->Initialize(Handle)))
    {
        Safe_Release(pInstance);
        MSG_BOX("CREATE FAIL : PICKING");
    }

    return pInstance;
}

void CPicking::Free()
{
    __super::Free();

    Safe_Release(m_pGraphic_Device);
}
 