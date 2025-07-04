#pragma once

#include "Client_Defines.h"
#include "Base.h"

NS_BEGIN(Engine)
class CGameInstance;
NS_END

NS_BEGIN(Client)

class CXML_Manager : public CBase
{
	DECLARE_SINGLETON(CXML_Manager);

private :                     
	CXML_Manager();
	virtual ~CXML_Manager() = default;

public :
	HRESULT		Initialize(LPDIRECT3DDEVICE9 pGraphic_Device);

	HRESULT		LoadScml(const char* filename, void* tAnimation);
	HRESULT		AddTexture(const _char* pScmlFilePath, const _tchar* pTextureFilePath, void* pOutImageVec);

private :
	CGameInstance*		m_pGameInstance = {nullptr};
	LPDIRECT3DDEVICE9	m_pGraphic_Device = { nullptr };

	WCHAR				m_ConvertStr[MAX_PATH] = {};

public :
	virtual				void			Free() override;
};
NS_END