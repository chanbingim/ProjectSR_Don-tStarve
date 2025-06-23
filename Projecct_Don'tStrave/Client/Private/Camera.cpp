#include "Camera.h"

#include "GameInstance.h"

CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject { pGraphic_Device }
{
}

CCamera::CCamera(const CCamera& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CCamera::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera::Initialize(void* pArg)
{
	CAMERA_DESC* pDesc = static_cast<CAMERA_DESC*>(pArg);
	m_fFov = pDesc->fFov;
	m_fAspect = static_cast<_float>(g_iWinSizeX) / g_iWinSizeY;
	m_fNear = pDesc->fNear;
	m_fFar = pDesc->fFar;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	/* 카메라의 월드 상태를 트랜스폼 ㅋㅓㅁ포넌트에게 동기화한다.  */
	/* 뷰스페이스 변환행렬을 구한다. */
	/*D3DXMatrixLookAtLH();*/
	m_pTransformCom->SetPosition(pDesc->vEye);
	m_pTransformCom->LookAt(pDesc->vAt);

	/* 투영스페이스 변환행렬 */
	D3DXMatrixPerspectiveFovLH(&m_ProjMatrix, m_fFov, m_fAspect, m_fNear, m_fFar);

	POINT		ptMouse{};

	GetCursorPos(&ptMouse);

	ScreenToClient(g_hWnd, &ptMouse);

	m_vOldMouse = _float2(ptMouse.x, ptMouse.y);
	
	return S_OK;
}

void CCamera::Priority_Update(_float fTimeDelta)
{
	/* 카메라의 움직임에 대한 처리를 모두 수행한다. */
	POINT		ptMouse{};

	GetCursorPos(&ptMouse);

	ScreenToClient(g_hWnd, &ptMouse);

	_float			fMove = {};


	if (GetKeyState('W') & 0x8000)
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
	}
	if (GetKeyState('S') & 0x8000)
	{
		m_pTransformCom->Go_Backward(-fTimeDelta);
	}
	if (GetKeyState('A') & 0x8000)
	{
		m_pTransformCom->Go_Left(-fTimeDelta);
	}
	if (GetKeyState('D') & 0x8000)
	{
		m_pTransformCom->Go_Right(fTimeDelta);
	}


	if (fMove = ptMouse.x - m_vOldMouse.x)
	{
		m_pTransformCom->TurnRate(_float3(0.f, 1.f, 0.f), fTimeDelta * fMove * m_fSensor);
	}

	if (fMove = ptMouse.y - m_vOldMouse.y)
	{
		m_pTransformCom->TurnRate(m_pTransformCom->GetWorldState(WORLDSTATE::RIGHT), fTimeDelta * fMove * m_fSensor);
	}


	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_pTransformCom->Get_InverseWorldMat());
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, D3DXMatrixPerspectiveFovLH(&m_ProjMatrix, m_fFov, m_fAspect, m_fNear, m_fFar));


	m_vOldMouse = _float2(ptMouse.x, ptMouse.y);
}

void CCamera::Update(_float fTimeDelta)
{

}

void CCamera::Late_Update(_float fTimeDelta)
{

}

HRESULT CCamera::Render()
{
	return S_OK;
}

HRESULT CCamera::Ready_Components(void* pArg)
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), pArg)))
		return E_FAIL;
	return S_OK;
}

CCamera* CCamera::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera* pInstance = new CCamera(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera::Clone(void* pArg)
{
	CCamera* pInstance = new CCamera(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
}
