#include "Shader.h"

CShader::CShader(LPDIRECT3DDEVICE9 pGraphic_Deivce) :
    m_pGraphic_Device(pGraphic_Deivce)
{
    ZeroMemory(&m_ShaderFVF, MAX_FVF_DECL_SIZE);
    Safe_AddRef(m_pGraphic_Device);
}

HRESULT CShader::Initialize(const WCHAR* FilePath, DWORD FVF)
{
    LPD3DXBUFFER		pErr = NULL;
    if (FAILED(D3DXCreateEffectFromFile(m_pGraphic_Device, FilePath, NULL, NULL, 0, NULL, &m_pEffect, &pErr)))
    {
        if (pErr)
        {
            OutputDebugStringA((char*)pErr->GetBufferPointer());
            MessageBoxA(nullptr, (char*)pErr->GetBufferPointer(), "FX Load Error", MB_OK);
            pErr->Release();
        }
        return E_FAIL;
    }
    else
    {
        D3DXDeclaratorFromFVF(FVF, m_ShaderFVF);
        m_hTechnique = m_pEffect->GetTechniqueByName("main");
    }

    return S_OK;
}

HRESULT CShader::Render_Begin(_uint PassIdnex, UINT* Passes, DWORD Flag)
{
    if (nullptr == m_pEffect)
        return E_FAIL;

    m_pEffect->Begin(Passes, Flag);
    m_pEffect->BeginPass(PassIdnex);

    return S_OK;
}

// 알규먼트 통해서 특정 쉐이더의 데이터를 받고
// 가상함수를 통해서 데이터 세팅을 구현해서 사용할것
// Setting -> Begin -> Render -> End 순서로 사용
HRESULT CShader::SettingData(void* pArg)
{
    
    return S_OK;
}

HRESULT CShader::Render_End()
{
    if (nullptr == m_pEffect)
        return E_FAIL;

    m_pEffect->EndPass();
    m_pEffect->End();
    m_pEffect->OnResetDevice();

    return S_OK;
}

void CShader::Free()
{
    __super::Free();
    if(m_pEffect)
        m_pEffect->OnResetDevice();

    Safe_Release(m_pEffect);
    Safe_Release(m_pGraphic_Device);
}
