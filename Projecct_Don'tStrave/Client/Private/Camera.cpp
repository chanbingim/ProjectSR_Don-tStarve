#include "Camera.h"

#include "GameInstance.h"
#include "Camera_Button.h"

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

	/* 카메라의 월드 상태를 트랜스폼 컴포넌트에게 동기화한다.  */
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

	CCamera_Button::CameraButton_Desc Desc = {};
	Desc.fSizeX = 50.f;
	Desc.fSizeY = 50.f;
	Desc.fX = g_iWinSizeX - 150.f;
	Desc.fY = g_iWinSizeY - 50.f;
	Desc.iTextIndex = 0;

 	m_pButton_Left = dynamic_cast<CCamera_Button*>(m_pGameInstance->Clone_Prototype(
		PROTOTYPE::GAMEOBJECT, EnumToInt(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera_Button"), &Desc));

	Desc.fX = g_iWinSizeX - 50.f;
	Desc.fY = g_iWinSizeY - 50.f;
	Desc.iTextIndex = 1;

	m_pButton_Right = dynamic_cast<CCamera_Button*>(m_pGameInstance->Clone_Prototype(
		PROTOTYPE::GAMEOBJECT, EnumToInt(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera_Button"), &Desc));
	
	return S_OK;
}

void CCamera::Priority_Update(_float fTimeDelta)
{


	if (GetKeyState(VK_UP) & 0x8000)
	{

		_float3		vPosition = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
		_float3		vLook = m_pTransformCom->GetWorldState(WORLDSTATE::LOOK);

		vPosition += *D3DXVec3Normalize(&vLook, &vLook) * 5.f * fTimeDelta;

		m_pTransformCom->SetPosition(vPosition);
	}
	if (GetKeyState(VK_DOWN) & 0x8000)
	{
		_float3		vPosition = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
		_float3		vLook = m_pTransformCom->GetWorldState(WORLDSTATE::LOOK);

		vPosition -= *D3DXVec3Normalize(&vLook, &vLook) * 5.f * fTimeDelta;

		m_pTransformCom->SetPosition(vPosition);
	}

	/* 카메라의 움직임에 대한 처리를 모두 수행한다. */
	POINT		ptMouse{};

	GetCursorPos(&ptMouse);

	ScreenToClient(g_hWnd, &ptMouse);

	_float			fMove = {};

	m_pButton_Left->Update(fTimeDelta);
	m_pButton_Right->Update(fTimeDelta);

	if (true == m_pButton_Right->OnClick())
	{
		m_pTransformCom->TurnRate(_float3(0.f, 1.f, 0.f), fTimeDelta * 10.f);
	}
	else if (true == m_pButton_Left->OnClick())
	{
		m_pTransformCom->TurnRate(_float3(0.f, 1.f, 0.f), -fTimeDelta * 10.f);
	}

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
		//m_pTransformCom->TurnRate(_float3(0.f, 1.f, 0.f), fTimeDelta * fMove * m_fSensor);
	}

	if (fMove = ptMouse.y - m_vOldMouse.y)
	{
		//m_pTransformCom->TurnRate(m_pTransformCom->GetWorldState(WORLDSTATE::RIGHT), fTimeDelta * fMove * m_fSensor);
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
	m_pButton_Left->Render();
	m_pButton_Right->Render();

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

	Safe_Release(m_pButton_Left);
	Safe_Release(m_pButton_Right);
}
