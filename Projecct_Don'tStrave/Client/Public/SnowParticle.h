#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CTexture;
class CParticleSystemComponent;
NS_END

NS_BEGIN(Client)
class CParticleShader;

class CSnowParticle final : public CGameObject
{
private :
	CSnowParticle(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSnowParticle(const CSnowParticle& rhs);
	virtual ~CSnowParticle() = default;

public :
	virtual HRESULT		Initialize_Prototype();
	virtual HRESULT		Initialize(void* pArg);
	virtual void		Priority_Update(_float fTimeDelta);
	virtual void		Update(_float fTimeDelta);
	virtual void		Late_Update(_float fTimeDelta);
	virtual HRESULT		Render();

private :
	CTexture*					m_Texture_Com = { nullptr };
	CParticleSystemComponent*	m_ParticleSys_Com = { nullptr };

	CParticleShader*			m_Snow_Shader = { nullptr };

	_float						m_MaxBound{}, m_MinBound{};

private:
	HRESULT						ADD_Components();
	HRESULT						ADD_Shader();

public :
	static		CSnowParticle*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual		CGameObject*		Clone(void* pArg) override;
	virtual		void				Free();

};
NS_END