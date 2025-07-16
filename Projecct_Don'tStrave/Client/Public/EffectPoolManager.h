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

	HRESULT		Add_ActiveEffect(_uint ID, CAinimationObject** pOutAnimObject, void* pArg = nullptr);


	void		UpdateActvieEffect(_float fTimeDeleta);
	void				Clear();
private :
	unordered_map<_uint, list<CAinimationObject*>*>	m_EffectActiveMap = {};
	unordered_map<_uint, list<CAinimationObject*>*>	m_EffectUnActiveMap = {};

	CGameInstance*									m_pGameInstance = { nullptr };

	

private :
	HRESULT				Release_ActiveEffect(_uint ID, CAinimationObject* pAnimObject);
	HRESULT				Emplace_EffectPool();

	void				ADD_Effect(_uint ID, _wstring PrototypeTag);

public :
	virtual void		Free() override;
};
NS_END