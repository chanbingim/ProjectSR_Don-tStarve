#include "ParticleShader.h"

CParticleShader::CParticleShader(LPDIRECT3DDEVICE9 pGraphic_Deivce) :
    CShader(pGraphic_Deivce)
{
}

HRESULT CParticleShader::Initialize(const WCHAR* FilePath, DWORD FVF)
{
    if(FAILED(__super::Initialize(FilePath, FVF)))
        return E_FAIL;

    return S_OK;
}

HRESULT CParticleShader::SettingData(void* pArg)
{
    PARTICLE_SHADER_DESC Des = *static_cast<PARTICLE_SHADER_DESC*>(pArg);
    LPDIRECT3DBASETEXTURE9  Tex = nullptr;

    m_pGraphic_Device->GetTexture(0, &Tex);
    m_pGraphic_Device->CreateVertexDeclaration(m_ShaderFVF, &m_pDecl);
    m_pEffect->SetTechnique(m_hTechnique);

    m_pEffect->SetFloat("vScale", Des.Size);
    m_pEffect->SetMatrix("WorldMat", &Des.WorldMat);
    m_pEffect->SetMatrix("ViewMat", &Des.ViewMat);
    m_pEffect->SetMatrix("ProjdMat", &Des.ProjdMat);
    m_pEffect->SetTexture("Tex", Tex);

    m_pGraphic_Device->SetVertexDeclaration(m_pDecl);
    Safe_Release(Tex);
    return S_OK;
}

CParticleShader* CParticleShader::Create(const WCHAR* FilePath, LPDIRECT3DDEVICE9 pGraphic_Deivce, DWORD FVF)
{
    CParticleShader* pInstance = new CParticleShader(pGraphic_Deivce);
    if (FAILED(pInstance->Initialize(FilePath, FVF)))
    {
        Safe_Release(pInstance);
        MSG_BOX("CREATE FAIL : PARTICLE SHADER");
    }

    return pInstance;
}

void CParticleShader::Free()
{
    __super::Free();
}
