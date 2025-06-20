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

void CTransform::LerpTurn(const _float3& _vAxis, _float _TofAngle, _float _fDurTime)
{

}

void CTransform::LookAt(const _float3& _fDir)
{
	_float3 vScale = GetScale();
	*(_float3*)m_WorldMat.m[EnumToInt(WORLDSTATE::LOOK)] = _fDir - GetWorldState(WORLDSTATE::POSITION);
	auto vLook = _fDir - GetWorldState(WORLDSTATE::POSITION);
	D3DXVec3Cross(	(_float3*)m_WorldMat.m[EnumToInt(WORLDSTATE::RIGHT)],
					(_float3*)m_WorldMat.m[EnumToInt(WORLDSTATE::UP)],
					(_float3*)m_WorldMat.m[EnumToInt(WORLDSTATE::LOOK)]);

	D3DXVec3Cross(  (_float3*)m_WorldMat.m[EnumToInt(WORLDSTATE::UP)],
					(_float3*)m_WorldMat.m[EnumToInt(WORLDSTATE::LOOK)],
					(_float3*)m_WorldMat.m[EnumToInt(WORLDSTATE::RIGHT)]);

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
