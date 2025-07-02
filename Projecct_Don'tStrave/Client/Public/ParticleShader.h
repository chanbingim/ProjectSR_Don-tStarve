#pragma once
#include "Client_Defines.h"
#include "Shader.h"

NS_BEGIN(Client)
class CParticleShader final : public CShader
{
public :
	typedef struct ParticleShader_Desc
	{
		_float		Size;
		_float4x4	WorldMat;
		_float4x4	ViewMat;
		_float4x4	ProjdMat;

	}PARTICLE_SHADER_DESC;

private:
	CParticleShader(LPDIRECT3DDEVICE9 pGraphic_Deivce);
	virtual ~CParticleShader() = default;

public:
	virtual		HRESULT		Initialize(const WCHAR* FilePath, DWORD FVF);
	virtual		HRESULT		SettingData(void* pArg) override;

public:
	static		CParticleShader*	Create(const WCHAR* FilePath, LPDIRECT3DDEVICE9 pGraphic_Deivce, DWORD FVF);
	virtual		void				Free() override;

};
NS_END