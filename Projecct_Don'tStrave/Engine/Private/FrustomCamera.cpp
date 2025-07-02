#include "FrustomCamera.h"
#include "Transform.h"

CFrustomCamera::CFrustomCamera(LPDIRECT3DDEVICE9 pGraphic_Device) :
	CGameObject(pGraphic_Device)
{
}

CFrustomCamera::CFrustomCamera(const CFrustomCamera& rhs) : 
	CGameObject(rhs),
	m_fFov(rhs.m_fFov), m_fNear(rhs.m_fNear), m_fAspect(rhs.m_fAspect)
	, m_FustomPoints(rhs.m_FustomPoints)
	, m_FustomPlane(rhs.m_FustomPlane)
{
}

HRESULT CFrustomCamera::Initialize_Prototype()
{
	m_FustomPoints.reserve(8);

	m_FustomPlane.reserve(6);
	m_FustomPlane.resize(6, {});

	m_ComputeFustomPoints.reserve(8);

	//앞면 정점 4개
	m_FustomPoints.emplace_back(-1.f, 1.f, 0.f);
	m_FustomPoints.emplace_back(1.f, 1.f, 0.f);
	m_FustomPoints.emplace_back(1.f, -1.f, 0.f);
	m_FustomPoints.emplace_back(-1.f, -1.f, 0.f);

	//뒷면 정점 4개
	m_FustomPoints.emplace_back(-1.f, 1.f, 1.f);
	m_FustomPoints.emplace_back(1.f, 1.f, 1.f);
	m_FustomPoints.emplace_back(1.f, -1.f, 1.f);
	m_FustomPoints.emplace_back(-1.f, -1.f, 1.f);

	return S_OK;
}

HRESULT CFrustomCamera::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CFrustomCamera::Initialize_Late()
{
	return S_OK;
}

void CFrustomCamera::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CFrustomCamera::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CFrustomCamera::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CFrustomCamera::Render()
{
	return S_OK;
}

void CFrustomCamera::Compute_CameraMatrix()
{
	m_ComputeFustomPoints.clear();

	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_pTransformCom->Get_InverseWorldMat());
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, D3DXMatrixPerspectiveFovLH(&m_ProjMatrix, m_fFov, m_fAspect, m_fNear, m_fFar));

	for (auto iter : m_FustomPoints)
	{
		_float3	Point = {};
		_matrix	InvPorjection = {};
		D3DXMatrixInverse(&InvPorjection, NULL, &m_ProjMatrix);

		//투영 행렬의 역행렬을 곱해 view 스페이스 상의 좌표를 얻어낸다.
		D3DXVec3TransformCoord(&Point, &Point, &InvPorjection);

		//뷰스페이스의 역행렬을 곱해서 월드공간상의 좌표를 얻어낸다.
		//즉 월드 좌표를 곱해서 월드 공간상의 좌표를 얻어낸다.
		D3DXVec3TransformCoord(&Point, &Point, &m_pTransformCom->Get_World());
		m_ComputeFustomPoints.push_back(Point);
	}
}

const _float4x4& CFrustomCamera::GetInvViewMat()
{
	return m_pTransformCom->Get_World();
}

_bool CFrustomCamera::IsInObject(const _float3& Point, _float Offset)
{
	//여기에서 6개의 평면의 방정식에서 데이터를 비교한다.
	//6개의 평면의 방정식에서 모두 0 >= 값이 나오면 랜더링

	D3DXPlaneFromPoints(&m_FustomPlane[0], &m_ComputeFustomPoints[0], &m_ComputeFustomPoints[1], &m_ComputeFustomPoints[2]);
	D3DXPlaneFromPoints(&m_FustomPlane[1], &m_ComputeFustomPoints[0], &m_ComputeFustomPoints[4], &m_ComputeFustomPoints[5]);
	D3DXPlaneFromPoints(&m_FustomPlane[2], &m_ComputeFustomPoints[0], &m_ComputeFustomPoints[3], &m_ComputeFustomPoints[7]);

	D3DXPlaneFromPoints(&m_FustomPlane[3], &m_ComputeFustomPoints[6], &m_ComputeFustomPoints[2], &m_ComputeFustomPoints[1]);
	D3DXPlaneFromPoints(&m_FustomPlane[4], &m_ComputeFustomPoints[6], &m_ComputeFustomPoints[2], &m_ComputeFustomPoints[3]);
	D3DXPlaneFromPoints(&m_FustomPlane[5], &m_ComputeFustomPoints[6], &m_ComputeFustomPoints[7], &m_ComputeFustomPoints[4]);

	for (auto& iter : m_FustomPlane)
	{
		float d = (iter.a * Point.x + iter.b * Point.y + iter.c * Point.z) + iter.d;
		if (d > Offset)
			return false;
	}

	return true;
}

CGameObject* CFrustomCamera::Clone(void* pArg)
{
	return nullptr;
}

void CFrustomCamera::Free()
{
	__super::Free();
}
