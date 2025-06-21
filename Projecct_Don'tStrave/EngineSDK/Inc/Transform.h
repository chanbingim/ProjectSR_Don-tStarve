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
	//�̰� ���ʹϾ� �����ؼ� �Ẹ��
	void							SetRotation(const _float3& _vRotation);

	//���� �������� ȸ����Ű�� �Լ�
	void							SetRotAxis(const _float3& _vAxis, _float _fAngle);	

	//���� ȸ������ �ʴ� ȸ������ Rateȸ���� ����
	void							TurnRate(const _float3& _vAxis, _float _fDeletaTime);

	//���� ȸ���������� ������ ȸ�������� �ɶ����� ȸ��
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