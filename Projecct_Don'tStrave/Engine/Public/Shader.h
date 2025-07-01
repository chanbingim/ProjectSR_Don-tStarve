#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class ENGINE_DLL CShader abstract : public CBase
{
protected :
	CShader(LPDIRECT3DDEVICE9 pGraphic_Deivce);
	virtual ~CShader() = default;

public :
	virtual		HRESULT		Initialize(const WCHAR* FilePath, DWORD FVF);
	virtual		HRESULT		SettingData(void* pArg);

	HRESULT					Render_Begin(_uint PassIdnex, UINT* Passes = NULL, DWORD Flag = 0);
	HRESULT					Render_End();

protected :
	LPDIRECT3DDEVICE9				m_pGraphic_Device = { nullptr };
	LPD3DXEFFECT					m_pEffect = { nullptr };
	D3DXHANDLE						m_hTechnique = { nullptr };
	LPDIRECT3DVERTEXDECLARATION9	m_pDecl = { nullptr };
	D3DVERTEXELEMENT9				m_ShaderFVF[MAX_FVF_DECL_SIZE];

public :
	virtual		void				Free();

};
NS_END