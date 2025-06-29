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

	CButton::BUTTON_DESC Desc = {};
	Desc.fSizeX = 50.f;
	Desc.fSizeY = 50.f;
	Desc.fX = g_iWinSizeX - 150.f;
	Desc.fY = g_iWinSizeY - 50.f;
	Desc.iTextureIndex = 0;

 	m_pButton_Left = dynamic_cast<CCamera_Button*>(m_pGameInstance->Clone_Prototype(
		PROTOTYPE::GAMEOBJECT, EnumToInt(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera_Button"), &Desc));

	Desc.fX = g_iWinSizeX - 50.f;
	Desc.fY = g_iWinSizeY - 50.f;
	Desc.iTextureIndex = 1;

	m_pButton_Right = dynamic_cast<CCamera_Button*>(m_pGameInstance->Clone_Prototype(
		PROTOTYPE::GAMEOBJECT, EnumToInt(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera_Button"), &Desc));
	
	return S_OK;
}

HRESULT CCamera::Initialize_Late()
{
	m_pPlayerTransformCom = static_cast<CTransform*>(m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Player"), TEXT("Com_Transform"), 0));
	return S_OK;
}

void CCamera::Priority_Update(_float fTimeDelta)
{
	/* 카메라의 움직임에 대한 처리를 모두 수행한다. */
	POINT		ptMouse{};

	GetCursorPos(&ptMouse);

	ScreenToClient(g_hWnd, &ptMouse);

	_float			fMove = {};

	m_pButton_Left->Update(fTimeDelta);
	m_pButton_Right->Update(fTimeDelta);


	if (GetKeyState('A') & 0x8000)
	{
		m_pTransformCom->Go_Left(fTimeDelta);
	}
	if (GetKeyState('D') & 0x8000)
	{
		m_pTransformCom->Go_Right(fTimeDelta);
	}

	m_vOldMouse = _float2(ptMouse.x, ptMouse.y);
}

void CCamera::Update(_float fTimeDelta)
{
	

}

void CCamera::Late_Update(_float fTimeDelta)
{
	if (m_pPlayerTransformCom != nullptr) {
		_float3		vPosition = m_pPlayerTransformCom->GetWorldState(WORLDSTATE::POSITION);
		_float3		vLook = m_pPlayerTransformCom->GetWorldState(WORLDSTATE::LOOK);
		_float3		vUp = m_pPlayerTransformCom->GetWorldState(WORLDSTATE::UP);
		_float3	turn = vPosition - m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
		vPosition -= *D3DXVec3Normalize(&vLook, &vLook) * 3.5f;
		vPosition += *D3DXVec3Normalize(&vUp, &vUp) * 3.5f;

		float z = sqrtf(powf(turn.x, 2) + powf(turn.y, 2));
		float player = acosf(turn.x / z);
		m_pTransformCom->SetPosition(vPosition);
		m_pTransformCom->LookAt(m_pPlayerTransformCom->GetWorldState(WORLDSTATE::POSITION));
	}
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_pTransformCom->Get_InverseWorldMat());
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, D3DXMatrixPerspectiveFovLH(&m_ProjMatrix, m_fFov, m_fAspect, m_fNear, m_fFar));
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
