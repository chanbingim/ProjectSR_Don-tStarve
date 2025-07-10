#pragma once
#include "Component.h"

NS_BEGIN(Engine)
class CGameObject;
class CTransform;

class ENGINE_DLL CLightComponent final : public CComponent
{
public :
	typedef struct Light_Desc
	{
		CGameObject*	pOwner = { nullptr };
		CTransform*		PlayerPoint = { nullptr };
		D3DLIGHT9		LightData;

	}LIGHT_DESC;

protected:
	CLightComponent();
	CLightComponent(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLightComponent(CLightComponent& rhs);
	virtual ~CLightComponent() = default;

public:
	virtual		HRESULT				Initialize_Prototype();
	virtual		HRESULT				Initialize(void* pArg);
	
	
	virtual		void				Render_Light(_uint LightUstage = 0);

	const	D3DLIGHT9&	GetLightData() { return m_LightData; }

	//Default Option
	void	SetLightType(D3DLIGHTTYPE _Type)  { m_LightData.Type = _Type; }
	void	SetAmbientColor(D3DXCOLOR Color)  { m_LightData.Ambient = Color; }
	void	SetDiffuseColor(D3DXCOLOR Color)  { m_LightData.Diffuse = Color; }
	void	SetSpecularColor(D3DXCOLOR Color) { m_LightData.Specular = Color; }
	void	SetPosition(_float3 _vPos)		  { m_LightData.Position = _vPos; }

	// not Use Point Light
	void	SetDirecation(_float3 _vDir)		  { m_LightData.Direction = _vDir; }

	// Not Use DirectionLight
	void	SetMaxRange(_float _fRange)		  { m_LightData.Range = _fRange; }

	// Only Use SoptLight
	void	SetInnearLight(_float _fTheta)	  { m_LightData.Theta = _fTheta; }
	void	SetOutLight(_float _fPhi)		  { m_LightData.Phi = _fPhi; }

	// ShaderSettingData
	void	SetAttenuation(_float fAtn0, _float fAtn1, _float fAtn2);

	CGameObject*					GetOwner();
	FLOAT							GetDistance();
	FLOAT							Compute_LightDistance();

protected:
	CGameObject*					m_pOwner = { nullptr };
	CTransform*						m_PlayerPoint = { nullptr };

	D3DLIGHT9						m_LightData;
	FLOAT							m_Distance;
public : 
	static		CLightComponent*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual		CComponent*			Clone(void* pArg = nullptr);
	virtual		void				Free() override;
};
NS_END