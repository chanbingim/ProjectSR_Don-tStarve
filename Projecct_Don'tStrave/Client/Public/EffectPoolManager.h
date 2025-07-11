#pragma once
#include "Client_Defines.h"
#include "Base.h"

NS_BEGIN(Engine)
class CGameInstance;
NS_END

NS_BEGIN(Client)

class CAinimationObject;

class CEffectPoolManager : public CBase
{
	DECLARE_SINGLETON(CEffectPoolManager);
private :
	CEffectPoolManager();
	virtual ~CEffectPoolManager() = default;

public :
	HRESULT		Initailize();

	HRESULT		Add_ActiveEffect(_uint ID, CAinimationObject* pOutAnimObject);
	HRESULT		Release_ActiveEffect(_uint ID, CAinimationObject* pAnimObject);

private :
	unordered_map<_uint, list<CAinimationObject*>>	m_EffectActiveMap = {};
	unordered_map<_uint, list<CAinimationObject*>>	m_EffectUnActiveMap = {};
	CGameInstance*									m_pGameInstance = { nullptr };

private :
	HRESULT				Emplace_EffectPool();


public :
	virtual void		Free() override;
};
NS_END