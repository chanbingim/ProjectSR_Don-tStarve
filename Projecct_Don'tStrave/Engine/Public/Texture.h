#pragma once

#include "Component.h"

/* Test%d.png */

/* 텍스쳐들을 들고 있는다.  */

NS_BEGIN(Engine)

class ENGINE_DLL CTexture final : public CComponent
{
private:
	CTexture(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTexture(const CTexture& Prototype);
	virtual ~CTexture() = default;

public:
	virtual HRESULT Initialize_Prototype(TEXTURE eType, const _tchar* pTextureFilePath, _uint iNumTextures);
	virtual HRESULT Initialize_Prototype(TEXTURE eType, const _tchar* pTextureFilePath);
	virtual HRESULT Initialize(void* pArg);

public:
	HRESULT Set_Texture(_uint iTextureIndex);
	_float3	Get_Size();
	_uint	Get_Frame();
private:
	_uint									m_iNumTextures = {};
	_uint									m_iWidth = {};
	_uint									m_iHeight = {};
	_uint									m_iX = {};
	_uint									m_iFrame = {};
	vector<LPDIRECT3DBASETEXTURE9>			m_Textures;

public:
	static CTexture* Create(LPDIRECT3DDEVICE9 pGraphic_Device, TEXTURE eType, const _tchar* pTextureFilePath, _uint iNumTextures);
	static CTexture* Create(LPDIRECT3DDEVICE9 pGraphic_Device, TEXTURE eType, const _tchar* pTextureFilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END