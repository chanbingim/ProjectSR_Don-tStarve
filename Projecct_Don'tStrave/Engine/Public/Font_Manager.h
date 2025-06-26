#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CFont_Manager final: public CBase
{
private:
	CFont_Manager(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CFont_Manager() = default;

public:
	HRESULT Initialize();
	void Render_Font(const _wstring strFontTag, const _tchar* pText, RECT* pRect);

	HRESULT Add_Font(const _wstring strFontTag, _uint iSize, const _tchar* pFontName);

private:
	LPDIRECT3DDEVICE9	m_pGraphic_Device = { nullptr };

	map<const _wstring, LPD3DXFONT> m_pFonts = {};
	
	LPD3DXSPRITE		m_pSprite = { nullptr };

public:
	static CFont_Manager* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

NS_END