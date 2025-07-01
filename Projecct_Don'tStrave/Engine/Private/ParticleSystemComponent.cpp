#include "ParticleSystemComponent.h"
#include "GameInstance.h"

#include "CMath.h"

CParticleSystemComponent::CParticleSystemComponent(LPDIRECT3DDEVICE9 pGraphic_Device) :
	CComponent(pGraphic_Device)
{
}

CParticleSystemComponent::CParticleSystemComponent(const CParticleSystemComponent& rhs) :
	CComponent(rhs)
{
}

HRESULT CParticleSystemComponent::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticleSystemComponent::Initialize(void* pArg)
{
	if (nullptr == pArg)
	{

	}
	else
	{
		m_pSystemData = *static_cast<PARTICLE_SYSTEM_DESC*>(pArg);

		if (FAILED(m_pGraphic_Device->CreateVertexBuffer(
			m_pSystemData.iMaxParticles * sizeof(PaticleAtrribute),
			D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
			D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_PSIZE,
			D3DPOOL_DEFAULT,
			&m_pVertexBuffer,
			0)))
			return E_FAIL;

		m_particles.reserve(m_pSystemData.iMaxParticles * 2);
	}

	return S_OK;
}

HRESULT CParticleSystemComponent::Update(_float fDeletaTime)
{
	auto iter_End = m_particles.end();
	for (auto& iter : m_particles)
	{
		iter.m_Position += iter.velocity * fDeletaTime;
		iter.AliveTime += fDeletaTime;
		
		if (iter.m_Position.y <= -1.f)
		{
			ResetParitcle(iter);
		}

		if (iter.AliveTime >= iter.lifeTime)
		{
			iter.m_isAlive = false;
		}
	}

	RemoveDeadParitcle();
	return S_OK;
}

HRESULT CParticleSystemComponent::Render()
{
	//여기서 파티클의 랜더링을 관리한다.
	PaticleAtrribute* vertex = {};

	m_pGraphic_Device->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(PaticleAtrribute));

	m_pVertexBuffer->Lock(0, m_pSystemData.iMaxParticles * sizeof(PaticleAtrribute),
						  (void**)&vertex, D3DLOCK_DISCARD);

	memcpy(vertex, &m_particles[0], m_pSystemData.iMaxParticles * sizeof(PaticleAtrribute));
	m_pVertexBuffer->Unlock();

	m_pGraphic_Device->DrawPrimitive(
						D3DPT_POINTLIST,
						0, m_pSystemData.iMaxParticles);

	return S_OK;
}

void CParticleSystemComponent::ADD_Particle(PaticleAtrribute _Particle)
{
	m_particles.push_back(_Particle);
}

void CParticleSystemComponent::ResetParitcle(PaticleAtrribute& ResetParticle)
{
	ResetParticle.m_Position = { m_pGameInstance->Random(m_pSystemData.fBoundMin, m_pSystemData.fBoundMax),
								m_pGameInstance->Random(m_pSystemData.fBoundMin, m_pSystemData.fBoundMax),
								m_pGameInstance->Random(m_pSystemData.fBoundMin, m_pSystemData.fBoundMax) };

	ResetParticle.velocity.y = -1 * m_pGameInstance->Random(1.f, 3.f);
	ResetParticle.AliveTime = 0;
	ResetParticle.m_isAlive = true;
}

HRESULT CParticleSystemComponent::Begin_RenderState()
{
	//랜더 스테이트 지정
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSIZE, CMath::FtoDw(m_pSystemData.MaxParticleSize));
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSIZE_MIN, CMath::FtoDw(m_pSystemData.MinParticleSize));

	// POINTSCALE_A,B,C를 통해 거리에 따른 파티클 크기를 제어하자
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALE_A, CMath::FtoDw(0.f));
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALE_B, CMath::FtoDw(0.f));
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALE_C, CMath::FtoDw(1.f));

	// 알파 블렌딩을 적용시키자
	// 텍스쳐의 알파채널을 받아와 블렌딩을 해준다
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	return S_OK;
}

HRESULT CParticleSystemComponent::End_RenderState()
{
	//랜더 스테이트 지정

	return S_OK;
}

HRESULT CParticleSystemComponent::RemoveDeadParitcle()
{
	auto iter_End = m_particles.end();
	for (auto iter = m_particles.begin(); iter != iter_End;)
	{
		if (!(*iter).m_isAlive)
		{
			iter = m_particles.erase(iter);
		}
		else
			iter++;
	}

	return S_OK;
}

CParticleSystemComponent* CParticleSystemComponent::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CParticleSystemComponent* pInstance = new CParticleSystemComponent(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("CREATE FAIL : PARTICLE SYSTEM");
	}
	return pInstance;
}

CComponent* CParticleSystemComponent::Clone(void* pArg)
{
	CParticleSystemComponent* pInstance = new CParticleSystemComponent(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("CLONE FAIL : PARTICLE SYSTEM");
	}
	return pInstance;
}

void CParticleSystemComponent::Free()
{
	__super::Free();
	m_particles.clear();
}
