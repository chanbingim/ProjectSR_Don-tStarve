#include "Particle.h"

CParticle::CParticle(LPDIRECT3DDEVICE9 pGraphic_Device) :
    CVIBuffer(pGraphic_Device)
{
}

CParticle::CParticle(const CParticle& rhs) : 
    CVIBuffer(rhs)
{
}

HRESULT CParticle::Initialize_Prototype()
{
    m_iNumVertices = 1;
    m_iVertexStride = sizeof(VTXPOSPARTICLE);
    m_iFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
    m_ePrimitiveType = D3DPT_POINTLIST;
    m_iNumPrimitive = 1;

    m_pVertexPositions = new _float3[m_iNumVertices];

    if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iNumVertices * m_iVertexStride, 0, m_iFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
        return E_FAIL;

    VTXPOSPARTICLE* pVertices = { nullptr };
    m_pVB->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);

    pVertices[0].vPosition = {0.f, 0.f, 0.f};
    pVertices[0].Color = D3DXCOLOR(1.f,1.f,1.f,1.f);

    m_pVB->Unlock();


    return S_OK;
}

HRESULT CParticle::Initialize(void* pArg)
{
    return S_OK;
}

void CParticle::Update(_float fDeletaTime)
{
    if (lifeTime <= AliveTime)
        m_isAlive = false;
    else
    {
        m_Position += (velocity  + acceleration) * fDeletaTime;
    }
}

HRESULT CParticle::Render()
{
    __super::Render();

    return S_OK;
}

void CParticle::ResetParitcle(_float3 ResetPoint)
{
    m_Position = ResetPoint;
    AliveTime = 0.f;
    m_isAlive = true;
}

CComponent* CParticle::Clone(void* pArg)
{
    return nullptr;
}

void CParticle::Free()
{
    __super::Free();
}
