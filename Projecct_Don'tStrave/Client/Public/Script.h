#pragma once

#include "Client_Defines.h"
#include "UserInterface.h"

NS_BEGIN(Client)
class CScript : public CUserInterface
{
public :
	typedef struct ScriptDesc
	{
		vector<_wstring>		DataScript;
	}SCRIPT_DESC;

private :
	CScript(LPDIRECT3DDEVICE9 pGraphic_Device);
	CScript(const CScript& rhs);
	virtual ~CScript() = default;

public :
	virtual HRESULT		Initialize_Prototype();
	virtual HRESULT		Initialize(void* pArg);
	virtual void		Priority_Update(_float fTimeDelta);
	virtual void		Update(_float fTimeDelta);
	virtual void		Late_Update(_float fTimeDelta);
	virtual HRESULT		Render();

private :
	_float				m_fAnimationTime = {};
	_bool				m_StartTextView = { false };


private :
	void				PlayAnimation(_uint type);
	void				MovePositionAnim();
	void				ChangeScaleAnim();

	HRESULT				ADD_Components();

public :
	static		CScript*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual		CGameObject*		Clone(void* pArg) override;
	virtual		void				Free() override;

};
NS_END