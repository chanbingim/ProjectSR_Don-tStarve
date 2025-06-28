#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class ENGINE_DLL CLevel abstract : public CBase
{
protected:
	CLevel(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iLevelID);
	virtual ~CLevel() = default;

public:
	_uint Get_LevelID() const {
		return m_iID;
	}

public:
	virtual HRESULT Initialize();
	virtual void Update(_float fTimeDelta);
	virtual HRESULT Render();

	

protected:
	class CGameInstance*	m_pGameInstance = { nullptr };
	LPDIRECT3DDEVICE9		m_pGraphic_Device = { nullptr };
	_uint					m_iID = {};

protected :
	HRESULT	Parse_ObejectData(const char* LoatFilePath, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLayerLevelIndex, const _wstring& strLayerTag);
	void	LoadMapData(const char* MapFilePath, vector<BASE_DATA_STRUCT>* pOut);

public:
	virtual void Free() override;
};

NS_END