#pragma once

#include "Base.h"

/* �������� ����ؾ� �� ��ü���� �и��Ͽ� �����Ѵ�. */

NS_BEGIN(Engine)
class CGameObject;

class CObject_Manager final : public CBase
{
private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	class CComponent* Get_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex = 0);

public:
	HRESULT Initialize(_uint iNumLevels);

	/* ������ ã�� �����Ͽ� ���̾ �߰��Ѵ�. */
	HRESULT Add_GameObject_ToLayer(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLayerLevelIndex, const _wstring& strLayerTag, void* pArg);
	list<CGameObject*>*			GetAllObejctsToLayer(_uint iLayerLevelIndex, const _wstring& strLayerTag);

	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);	
	void Clear(_uint iLevelIndex);
	void Clear_DeadObj();

private:

	_uint								m_iNumLevels = {};
	map<const _wstring, class CLayer*>* m_pLayers = { nullptr };
	class CGameInstance*				m_pGameInstance = { nullptr };

private:
	class CLayer* Find_Layer(_uint iLayerLevelIndex, const _wstring& strLayerTag);

public:
	static CObject_Manager* Create(_uint iNumLevels);
	virtual void Free() override;

};

NS_END