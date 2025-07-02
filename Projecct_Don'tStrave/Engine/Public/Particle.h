#pragma once

#include "VIBuffer.h"

NS_BEGIN(Engine)
class ENGINE_DLL CParticle
{
public :
	typedef struct Particle_Desc
	{
		_float							ParticleSize = {}; // 파티클 사이즈
		_float3							velocity = {};     // 초당 이동 속도
		_float3							acceleration = {}; // 초당 가속 속도

		_float							lifeTime = {};     // 파티클 생명시간
		_float							AliveTime = {};    // 파티클의 현재 나이

		D3DXCOLOR						color = {};        // 파티클의 색
		D3DXCOLOR						colorFade = {};    // 파티클 색이 퇴색하는 방법
	}PARTICLE_DESC;

private :
	CParticle(LPDIRECT3DDEVICE9 pGraphic_Device);
	CParticle(const CParticle& rhs);
	virtual ~CParticle() = default;

public :
	virtual HRESULT					Initialize_Prototype() override;
	virtual HRESULT					Initialize(void* pArg) override;

	virtual void					Update(_float fDeletaTime);

	virtual HRESULT					Render() override;
	virtual void					ResetParitcle(_float3 ResetPoint);

	_bool							IsAlive() { return m_isAlive; }

private :
	


public :
	virtual CComponent*				Clone(void* pArg) override;
	virtual void					Free() override;
};

struct PaticleAtrribute
{
	_float							ParticleSize = {};
	_float3							m_Position = {};     // 월드스페이스 내 파티클 위치

	_float3							velocity = {};     // 초당 이동 속도
	_float3							acceleration = {}; // 초당 가속 속도

	_float							lifeTime = {};     // 파티클 생명시간
	_float							AliveTime = {};    // 파티클의 현재 나이

	D3DXCOLOR						color = {};        // 파티클의 색
	D3DXCOLOR						colorFade = {};    // 파티클 색이 퇴색하는 방법
	_bool							m_isAlive = { true };      // 파티클 생존 여부
};
NS_END