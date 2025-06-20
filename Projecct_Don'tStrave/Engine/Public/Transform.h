#pragma once
#include "Component.h"

NS_BEGIN(Engine)
class ENGINE_DLL CTransform final : public CComponent
{
public :
	typedef struct Transform_Desc
	{
		_float		m_TurnSpeedSec = {};
		_float		m_MoveSpeedSec = {};

	}TRANSFORM_DESC;

protected :
	CTransform();
	CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public :
	virtual		HRESULT				Initialize_Prototype() override;
	virtual		HRESULT				Initialize(void* pArg) override;

	void							SetPosition(const _float3& _vPosition);
	//이건 쿼터니언 공부해서 써보기
	void							SetRotation(const _float3& _vRotation);

	//축을 기준으로 회전시키는 함수
	void							SetRotAxis(const _float3& _vAxis, _float _fAngle);	

	//기존 회전에서 초당 회전각도 Rate회전을 적용
	void							TurnRate(const _float3& _vAxis, _float _fDeletaTime);

	//기존 회전각도에서 정해진 회전각도가 될때까지 회전
	void							LookAt(const _float3& _fDir);

	void							SetScale(const _float3& _vScale);

	_float3							GetWorldState(WORLDSTATE eWorld_Sate);
	_float3							GetRotation();
	_float3							GetScale();

	_matrix&						Get_World();
	_matrix&						Get_InverseWorldMat();

private :
	TRANSFORM_DESC					m_RateData;

	_matrix							m_WorldMat = {};
	_matrix							m_InvWorldMat = {};
	_matrix							m_IdentiyMat = {};

public :
	static		CTransform*			Create();
	virtual		CComponent*			Clone(void* pArg = nullptr) override;
	virtual		void				Free()  override;
};
NS_END