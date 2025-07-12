#pragma once

#include "Client_Defines.h"
#include "Base.h"

NS_BEGIN(Engine)
class CGameInstance;
NS_END

NS_BEGIN(Client)
class CImgBase : public CBase
{
protected :
	CImgBase();
	virtual ~CImgBase() = default;

public :
	virtual HRESULT			Initialize();
	virtual void			Update(_float fDeletaTime);

protected : 
	CGameInstance*			m_pGameInstance = { };

public :
	virtual		CImgBase*		Clone(void* pArg);
	virtual		void			Free() override;
};
NS_END