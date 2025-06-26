#pragma once

#include "Base.h"

NS_BEGIN(Engine)
class CKeyManager : public CBase
{
private :
	CKeyManager() = default;
	virtual ~CKeyManager() = default;

public :
	HRESULT		Initialize();
	//키입력 시작
	void		BeginKeyInput();

	//키입력 종료
	void		EndKeyInput();

	_bool		GetKeyPressed(_uint KeyNum);
	_bool		GetKeyDown(_uint KeyNum);
	_bool		GetKeyUp(_uint KeyNum);

private :
	_bool		m_CurFrameInput[MAX_KEY];
	_bool		m_PreFrameInput[MAX_KEY];

public :
	static	CKeyManager*	Create();
	virtual void			Free() override;

};
NS_END