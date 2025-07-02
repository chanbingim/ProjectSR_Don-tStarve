#pragma once

#include "GameObject.h"

NS_BEGIN(Engine)
class ENGINE_DLL CFrustomCamera abstract : public CGameObject
{
protected :
	CFrustomCamera(LPDIRECT3DDEVICE9 pGraphic_Device);
	CFrustomCamera(const CFrustomCamera& rhs);
	virtual ~CFrustomCamera() = default;

public :
	virtual HRESULT					Initialize_Prototype();
	virtual HRESULT					Initialize(void* pArg);
	virtual HRESULT					Initialize_Late();

	virtual void					Priority_Update(_float fTimeDelta);
	virtual void					Update(_float fTimeDelta);
	virtual void					Late_Update(_float fTimeDelta);
	virtual HRESULT					Render();
	
	//함수를 호출행야 카메라 행렬관련된 연산을한다.
	//절투체 좌표 변환, 카메라의 월드행렬을 이용한 투영, 뷰 행렬 계산
	void							Compute_CameraMatrix();


	const	_float4x4&				GetInvViewMat();
	_bool							IsInObject(const _float3& Point, _float Offset = 0);

protected:
	vector<_float3>					m_FustomPoints;
	vector<_float3>					m_ComputeFustomPoints;
	vector<D3DXPLANE>				m_FustomPlane;

	_float4x4						m_ProjMatrix = { };
	_float4x4						m_ViewMatrix = { };

	_float							m_fFov = {};
	_float							m_fAspect = {};
	_float							m_fNear = {};
	_float							m_fFar = {};

public :
	virtual		CGameObject*		Clone(void* pArg) override;
	virtual		void				Free() override;

};
NS_END