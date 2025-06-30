#pragma once

#include "Client_Defines.h"
#include "Level.h"

NS_BEGIN(Client)
class CTutorialLevel : public CLevel
{
private:
	CTutorialLevel(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevelID);
	virtual ~CTutorialLevel() = default;

public:
	virtual HRESULT		Initialize() override;
	virtual void		Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;

private:
	HRESULT				Ready_Layer_BackGround(const _wstring& strLayerTag);
	HRESULT				Ready_Layer_Camera(const _wstring& strLayerTag);
	HRESULT				Ready_Layer_Enviornment(const _wstring& strLayerTag);
	HRESULT				Ready_Layer_Player(const _wstring& strLayerTag);
	HRESULT				Ready_Layer_Monster(const _wstring& strLayerTag);
	

public:
	static CTutorialLevel*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevelID);
	virtual void			Free() override;

};
NS_END