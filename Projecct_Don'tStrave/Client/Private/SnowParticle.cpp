#include "SnowParticle.h"
#include "GameInstance.h"

#include "ParticleShader.h"
#include "Camera.h"

CSnowParticle::CSnowParticle(LPDIRECT3DDEVICE9 pGraphic_Device) :
    CGameObject(pGraphic_Device)
{
}

CSnowParticle::CSnowParticle(const CSnowParticle& rhs) :
    CGameObject(rhs)
{
}

HRESULT CSnowParticle::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CSnowParticle::Initialize(void* pArg)
{
    if(FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(ADD_Shader()))
        return E_FAIL;

    for (int i = 0; i < 500; ++i)
    {
        PaticleAtrribute     Attripute;

        Attripute.m_Position.x = m_pGameInstance->Random(m_MinBound, m_MaxBound);
        Attripute.m_Position.y = m_pGameInstance->Random(1, 4);
        Attripute.m_Position.z = m_pGameInstance->Random(m_MinBound, m_MaxBound);


        Attripute.lifeTime = 9999.f;
        Attripute.velocity = { 0.f,0.f,0.f};
        Attripute.velocity.y = -1 * m_pGameInstance->Random(1.f, 3.f);
        Attripute.ParticleSize = 0.2f;
        Attripute.color = {0.f, 1.f, 0.f, 1.f};
        Attripute.acceleration = { 0.f, 0.f, 0.f };

        m_ParticleSys_Com->ADD_Particle(Attripute);
    }
    

    return S_OK;
}

void CSnowParticle::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);

}

void CSnowParticle::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);

    m_ParticleSys_Com->Update(fTimeDelta);
}

void CSnowParticle::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);

    m_pGameInstance->Add_RenderGroup(RENDER::PARTICLE, this);
}

HRESULT CSnowParticle::Render()
{
    __super::Render();

   /* auto Camera = dynamic_cast<CCamera*>(m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Camera")));
    CParticleShader::PARTICLE_SHADER_DESC Shader_Desc = {};
    Shader_Desc.Size = 7.f;
    Shader_Desc.WorldMat = Camera->GetInvViewMat();

    m_pGraphic_Device->GetTransform(D3DTS_VIEW, &Shader_Desc.ViewMat);
    m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &Shader_Desc.ProjdMat);
    m_Snow_Shader->SettingData(&Shader_Desc);*/
    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_World());

    m_Texture_Com->Set_Texture(0);

    m_ParticleSys_Com->Begin_RenderState();
    m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    //m_Snow_Shader->Render_Begin(0);

    m_ParticleSys_Com->Render();

   // m_Snow_Shader->Render_End();
    m_ParticleSys_Com->End_RenderState();
    return S_OK;
}

HRESULT CSnowParticle::ADD_Components()
{
    /* Com_Transform */
    CTransform::TRANSFORM_DESC		TransformDesc{ 5.f, D3DXToRadian(90.0f) };
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
        return E_FAIL;

    CParticleSystemComponent::PARTICLE_SYSTEM_DESC Particle_Desc = {};
    m_MinBound = 1.f;
    m_MaxBound = 30.f;

    Particle_Desc.fBoundMax = m_MaxBound;
    Particle_Desc.fBoundMin = m_MinBound;
    Particle_Desc.fRespawnTime = 10.f;
    Particle_Desc.iMaxParticles = 500.f;
    Particle_Desc.MinParticleSize = 0.f;
    Particle_Desc.MaxParticleSize = 0.15f;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Particle_System"),
        TEXT("Com_Particle_Sys"), reinterpret_cast<CComponent**>(&m_ParticleSys_Com), &Particle_Desc)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Snow_Texture"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_Texture_Com))))
        return E_FAIL;

    return S_OK;
}

HRESULT CSnowParticle::ADD_Shader()
{
    m_Snow_Shader = CParticleShader::Create(L"Particle.fx", m_pGraphic_Device, D3DFVF_PARTICLEVERTEX);

    return S_OK;
}

CSnowParticle* CSnowParticle::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CSnowParticle* pInstance = new CSnowParticle(pGraphic_Device);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("CREATE FAIL : SNOW PARTICLE");
    }
    return pInstance;
}

CGameObject* CSnowParticle::Clone(void* pArg)
{
    CSnowParticle* pInstance = new CSnowParticle(*this);
    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("CLONE FAIL : SNOW PARTICLE");
    }
    return pInstance;
}

void CSnowParticle::Free()
{
    __super::Free();

    Safe_Release(m_pTransformCom);
    Safe_Release(m_Texture_Com);
    Safe_Release(m_ParticleSys_Com);
    Safe_Release(m_Snow_Shader);
}
