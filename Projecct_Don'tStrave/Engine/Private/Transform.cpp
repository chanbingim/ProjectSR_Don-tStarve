#include "Transform.h"

CTransform::CTransform() : CComponent()
{

}

CTransform::CTransform(const CTransform& rhs) : CComponent(rhs),
m_WorldMat(rhs.m_WorldMat), m_IdentiyMat(rhs.m_IdentiyMat)
{

}

HRESULT CTransform::Initialize_Prototype()
{
	D3DXMatrixIdentity(&m_WorldMat);
	D3DXMatrixIdentity(&m_IdentiyMat);
	return S_OK;
}

HRESULT CTransform::Initialize(void* pArg)
{
	if (nullptr == pArg)
	{
		m_RateData.m_MoveSpeedSec = 5.f;
		m_RateData.m_TurnSpeedSec = D3DXToRadian(90.f);
	}
	else
		m_RateData = *(static_cast<TRANSFORM_DESC *>(pArg));

	return S_OK;
}

void CTransform::SetPosition(const _float3& _vPosition)
{
	memcpy(&m_WorldMat.m[EnumToInt(WORLDSTATE::POSITION)][0], &_vPosition, sizeof(_float3));
}

void CTransform::SetRotation(const _float3& _vRotation)
{

}

void CTransform::SetRotAxis(const _float3& _vAxis, _float _fAngle)
{
	_float vScale[3] = {};
	memcpy(&vScale, GetScale(), sizeof(_float3));

	_matrix AxisMat = {};
	D3DXMatrixRotationAxis(&AxisMat, &_vAxis, _fAngle);

	for (_uint i = 0; i < EnumToInt(WORLDSTATE::POSITION); ++i)
	{
		_float3 vNewRow = (*(_float3*)m_IdentiyMat.m[i]) * vScale[i];
		D3DXVec3TransformNormal((_float3 *)&m_WorldMat.m[i], &vNewRow, &AxisMat);
	}
}

void CTransform::TurnRate(const _float3& _vAxis, _float _fDeletaTime)
{
	_matrix AxisMat = {};
	D3DXMatrixRotationAxis(&AxisMat, &_vAxis, m_RateData.m_TurnSpeedSec * _fDeletaTime);

	for (_uint i = 0; i < EnumToInt(WORLDSTATE::POSITION); ++i)
	{
		D3DXVec3TransformNormal((_float3*)&m_WorldMat.m[i], (_float3*)&m_WorldMat.m[i], &AxisMat);
	}
}

void CTransform::LookAt(const _float3& _fDir)
{
	_float3 vScale = GetScale();
	_float3 LookVec = _fDir - GetWorldState(WORLDSTATE::POSITION);
	_float3	vUp{ 0.f, 1.f, 0.f }, vRight{};

	D3DXVec3Normalize(&LookVec, &LookVec);
	memcpy(*(_float3*)m_WorldMat.m[EnumToInt(WORLDSTATE::LOOK)], &LookVec, sizeof(_float3));
	
	D3DXVec3Cross(&vRight, &vUp, &LookVec);
	D3DXVec3Normalize(&vRight, &vRight);
	memcpy(*(_float3*)m_WorldMat.m[EnumToInt(WORLDSTATE::RIGHT)], &vRight, sizeof(_float3));

	D3DXVec3Cross(&vUp, &LookVec, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);
	memcpy(*(_float3*)m_WorldMat.m[EnumToInt(WORLDSTATE::UP)], &vUp, sizeof(_float3));

 	SetScale(vScale);
}

void CTransform::SetScale(const _float3& _vScale)
{
	_float vScale[3] = {};
	memcpy(&vScale, _vScale, sizeof(_float3));

	for (_uint i = 0; i < EnumToInt(WORLDSTATE::POSITION); ++i)
	{
		D3DXVec3Normalize((_float3 *)m_WorldMat.m[i], (_float3 *)m_WorldMat.m[i]);
		*(_float3 *)m_WorldMat.m[i] = (*(_float3*)m_WorldMat.m[i]) * vScale[i];
	}
}

void CTransform::LookInverse()
{
	*(_float3*)m_WorldMat.m[EnumToInt(WORLDSTATE::LOOK)] *= -1.f;
	*(_float3*)m_WorldMat.m[EnumToInt(WORLDSTATE::RIGHT)] *= -1.f;
}

_float3 CTransform::GetWorldState(WORLDSTATE eWorld_Sate)
{
	return *(_float3*)m_WorldMat.m[EnumToInt(eWorld_Sate)];
}

_float3 CTransform::GetRotation()
{
	return _float3(0.f, 0.f, 0.f);
}

_float3 CTransform::GetScale()
{
	return _float3( D3DXVec3Length((_float3*)m_WorldMat.m[0]),
					D3DXVec3Length((_float3*)m_WorldMat.m[1]),
					D3DXVec3Length((_float3*)m_WorldMat.m[2]));
}

_matrix& CTransform::Get_World()
{
	return m_WorldMat;
}

_matrix& CTransform::Get_InverseWorldMat()
{
	return *D3DXMatrixInverse(&m_InvWorldMat, nullptr, &m_WorldMat);;
}

void CTransform::Go_Straight(_float fTimeDelta)
{
	_float3		vPosition = GetWorldState(WORLDSTATE::POSITION);
	_float3		vLook = GetWorldState(WORLDSTATE::LOOK);

	vPosition += *D3DXVec3Normalize(&vLook, &vLook) * m_RateData.m_MoveSpeedSec * fTimeDelta;
	SetPosition(vPosition);
}

void CTransform::Go_Left(_float fTimeDelta)
{
	_float3		vPosition = GetWorldState(WORLDSTATE::POSITION);
	_float3		vLook = GetWorldState(WORLDSTATE::RIGHT);

	vPosition += *D3DXVec3Normalize(&vLook, &vLook) * m_RateData.m_MoveSpeedSec * -fTimeDelta;
	SetPosition(vPosition);
}

void CTransform::Go_Right(_float fTimeDelta)
{
	_float3		vPosition = GetWorldState(WORLDSTATE::POSITION);
	_float3		vLook = GetWorldState(WORLDSTATE::RIGHT);

	vPosition += *D3DXVec3Normalize(&vLook, &vLook) * m_RateData.m_MoveSpeedSec * fTimeDelta;
	SetPosition(vPosition);
}

void CTransform::Go_Backward(_float fTimeDelta)
{
	_float3		vPosition = GetWorldState(WORLDSTATE::POSITION);
	_float3		vLook = GetWorldState(WORLDSTATE::LOOK);

	vPosition += *D3DXVec3Normalize(&vLook, &vLook) * m_RateData.m_MoveSpeedSec * -fTimeDelta;
	SetPosition(vPosition);
}

CTransform* CTransform::Create()
{
	CTransform* pInstance = new CTransform();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("CREATE FAIL : CTRANSFORM");
	}

	return pInstance;
}

CComponent* CTransform::Clone(void* pArg)
{
	CTransform* pInstance = new CTransform(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("CLONE FAIL : CTRANSFORM");
	}

	return pInstance;
}

void CTransform::Free()
{
	__super::Free();
}
