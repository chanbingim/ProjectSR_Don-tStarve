#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class CGraphic_Device : public CBase
{
private:
	CGraphic_Device();
	virtual ~CGraphic_Device() = default;

public:
	HRESULT		Ready_GraphicDev(HWND hWnd, WINMODE eMode, _uint iSizeX, _uint iSizeY, LPDIRECT3DDEVICE9* ppGraphicDev);
	void		Render_Begin(D3DXCOLOR Color);
	void		Render_End(HWND hWnd = nullptr);

private:
	LPDIRECT3D9			m_pSDK = { nullptr };		// 장치 조사 객체
	LPDIRECT3DDEVICE9	m_pGraphicDev = { nullptr };	// 그리기 담당 객체


public:
	static CGraphic_Device* Create(HWND hWnd, WINMODE eMode, _uint iSizeX, _uint iSizeY, LPDIRECT3DDEVICE9* ppGraphicDev);
	virtual void Free();

};

NS_END



