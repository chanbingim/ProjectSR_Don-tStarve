#include "Texture.h"  
#include <Windows.h>  

CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphic_Device)  
: CComponent{ pGraphic_Device }  
{  
}  

CTexture::CTexture(const CTexture& Prototype)
	: CComponent{ Prototype }
	, m_iNumTextures{ Prototype.m_iNumTextures }
	, m_Textures{ Prototype.m_Textures }
	, m_iWidth{ Prototype.m_iWidth }
	, m_iHeight{ Prototype.m_iHeight }
	, m_iX{ Prototype.m_iX }
	, m_iFrame{ Prototype.m_iFrame }
{
	for (auto& pTexture : m_Textures)
		Safe_AddRef(pTexture);
}

HRESULT CTexture::Initialize_Prototype(TEXTURE eType, const _tchar* pTextureFilePath, _uint iNumTextures)
{
	m_iNumTextures = iNumTextures;

	m_Textures.reserve(m_iNumTextures);

	for (size_t i = 0; i < iNumTextures; i++)
	{
		LPDIRECT3DBASETEXTURE9		pTexture = { nullptr };
		_tchar		szFileName[MAX_PATH] = {};

		wsprintf(szFileName, pTextureFilePath, i);

		HRESULT hr = TEXTURE::PLANE == eType ?
			D3DXCreateTextureFromFile(m_pGraphic_Device, szFileName, reinterpret_cast<LPDIRECT3DTEXTURE9*>(&pTexture)) :
			D3DXCreateCubeTextureFromFile(m_pGraphic_Device, szFileName, reinterpret_cast<LPDIRECT3DCUBETEXTURE9*>(&pTexture));
		if (FAILED(hr))
			return E_FAIL;
		m_Textures.push_back(pTexture);
	}

	_tchar		szFileName[MAX_PATH] = {};
	LPDIRECT3DTEXTURE9 pTexture = { nullptr };
	wsprintf(szFileName, pTextureFilePath, 0);
	D3DXIMAGE_INFO imageInfo;
	if (SUCCEEDED(D3DXGetImageInfoFromFile(szFileName, &imageInfo))) {
		m_iWidth = imageInfo.Width;
		m_iHeight = imageInfo.Height;
	}
	else {
		m_iWidth = 0;
		m_iHeight = 0;
	}
	return S_OK;
}

HRESULT CTexture::Initialize_Prototype(TEXTURE eType, const _tchar* pTextureFilePath)  
{  
	m_iNumTextures = 1;  
	
	m_Textures.reserve(m_iNumTextures);  
	

	const _tchar* fileName = {};
	_tchar		szFileName[MAX_PATH] = {};

	//wsprintf(szFileName, pTextureFilePath, "\\*.dds");
	wstring str = pTextureFilePath;
	str += L"/*.dds";
	WIN32_FIND_DATAW data;
	HANDLE hFind;
	if ((hFind = FindFirstFileW(str.c_str(), &data)) != INVALID_HANDLE_VALUE)
	{
		str = pTextureFilePath;
		str += L"/";
		str += data.cFileName;
		swscanf_s(data.cFileName, L"%d_%d.dds", &m_iX, &m_iFrame);
	}

	LPDIRECT3DBASETEXTURE9		pTexture = { nullptr };  

	HRESULT hr = TEXTURE::PLANE == eType ?  
		D3DXCreateTextureFromFile(m_pGraphic_Device, str.c_str(), reinterpret_cast<LPDIRECT3DTEXTURE9*>(&pTexture)) :
		D3DXCreateCubeTextureFromFile(m_pGraphic_Device, str.c_str(), reinterpret_cast<LPDIRECT3DCUBETEXTURE9*>(&pTexture));
	if (FAILED(hr))  
		return E_FAIL;  
	m_Textures.push_back(pTexture);  
	
	D3DXIMAGE_INFO imageInfo;  
	if (SUCCEEDED(D3DXGetImageInfoFromFile(str.c_str(), &imageInfo))) {
		m_iWidth = imageInfo.Width;  
		m_iHeight = imageInfo.Height;  
	}
	return S_OK;  
}  

HRESULT CTexture::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CTexture::Set_Texture(_uint iTextureIndex)
{
	if (iTextureIndex >= m_iNumTextures)
		return E_FAIL;
	m_pGraphic_Device->SetTexture(0, m_Textures[iTextureIndex]);

	return S_OK;
}

_float3 CTexture::Get_Size()
{
	return _float3(m_iWidth, m_iHeight, m_iX);
}

_uint CTexture::Get_Frame()
{
	return m_iFrame;
}

CTexture* CTexture::Create(LPDIRECT3DDEVICE9 pGraphic_Device, TEXTURE eType, const _tchar* pTextureFilePath, _uint iNumTextures)
{
	CTexture* pInstance = new CTexture(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eType, pTextureFilePath, iNumTextures)))
	{
		MSG_BOX("Failed to Created : CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CTexture* CTexture::Create(LPDIRECT3DDEVICE9 pGraphic_Device, TEXTURE eType, const _tchar* pTextureFilePath)
{
	CTexture* pInstance = new CTexture(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eType, pTextureFilePath)))
	{
		MSG_BOX("Failed to Created : CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CTexture::Clone(void* pArg)
{
	CTexture* pInstance = new CTexture(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTexture::Free()
{
	__super::Free();

	for (auto& pTexture : m_Textures)
		Safe_Release(pTexture);
}
