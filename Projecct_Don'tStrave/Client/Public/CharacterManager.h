#pragma once
#include "Client_Defines.h"
#include "Base.h"

NS_BEGIN(Engine)
class CCollision_Component;
class CGameInstance;
NS_END

NS_BEGIN(Client)
class CPlayer;
class CCharacter;

class CCharacterManager final : public CBase
{
	DECLARE_SINGLETON(CCharacterManager);

private:
	CCharacterManager();
	virtual ~CCharacterManager() = default;

public:
	HRESULT		Initialize();
	void		Update();
	void		SetPlayer(CPlayer* pPlayer);
	void		AddObject(CCharacter* pCharacter);
private:
	list<CCharacter*> m_pCharacterList;
	CGameInstance* m_pGameInstance = { nullptr };
	CPlayer* m_pPlayer = {};
private:

public:
	virtual		void				Free();
};
NS_END