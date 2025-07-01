#pragma once

#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CParticleSystemComponent final : public CComponent
{
public : 
	typedef		struct		ParticleSystem_Desc
	{
		_float							MaxParticleSize;
		_float							MinParticleSize;
		_float							fBoundMin = {};
		_float							fBoundMax = {};
		_float							fRespawnTime = {};

		// 한 화면에서 시스템이 가질 수 있는 최대 파티클 수
		_uint							iMaxParticles = {};
	
	}PARTICLE_SYSTEM_DESC;

private:
	CParticleSystemComponent(LPDIRECT3DDEVICE9 pGraphic_Device);
	CParticleSystemComponent(const CParticleSystemComponent& rhs);
	virtual ~CParticleSystemComponent() = default;

public :
	virtual HRESULT					Initialize_Prototype() override;
	virtual HRESULT					Initialize(void* pArg) override;

	virtual HRESULT					Update(_float fDeletaTime);
	virtual HRESULT					Render();

	//	리스트에 파티클 추가 함수
	void							ADD_Particle(PaticleAtrribute _Particle);
	
	//	리스트에 있는 파티클 초기화 함수 호출
	void							ResetParitcle(PaticleAtrribute& ResetParticle);

	HRESULT							Begin_RenderState();
	HRESULT							End_RenderState();

private :
	PARTICLE_SYSTEM_DESC			m_pSystemData = {};

	vector<PaticleAtrribute>		m_particles;

	// 파티클 버텍스 버퍼
	LPDIRECT3DVERTEXBUFFER9			m_pVertexBuffer;
	// 복사를 시작할 다음 파티클의 인덱스
	DWORD							m_vertexBufferOffset;

	// 한 파티클 드로잉 단계에 정의된 파티클의 수
	DWORD							m_vertexBufferBatchSize;

private :
	HRESULT							RemoveDeadParitcle();

public :
	static	CParticleSystemComponent*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent*						Clone(void* pArg) override;
	virtual void							Free() override;

};
NS_END