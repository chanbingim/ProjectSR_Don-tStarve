#pragma once

#include "Base.h"

NS_BEGIN(Engine)
class CLightComponent;

class CLight_Manager final : public CBase
{
	DECLARE_SINGLETON(CLight_Manager);

private :
	CLight_Manager();
	virtual ~CLight_Manager() = default;

public :
	HRESULT		Initialize(LPDIRECT3DDEVICE9 pGraphic_Device);
	void		UpdateLight();

	HRESULT		ADD_Light(LIGHT_TYPE LightType ,CLightComponent* pLight);
	void		DeadLight(LIGHT_TYPE LightType, CLightComponent* pLight);

	//라이트 설정을 끄거나 킬수있게 한다.
	void		Enable_Light();
	void		UnEnable_Light();
	
private :
	//카메라 기준 Light 처리
	void		Sorting_Light(_uint type);

private :
	list<CLightComponent*>				m_Lightlist[ENUM_CLASS(LIGHT_TYPE::END)];
	list<CLightComponent*>				m_DeadLight = {};
	LPDIRECT3DDEVICE9					m_pGraphic_Device = { nullptr };

public :
	virtual		void					Free() override;

};
NS_END