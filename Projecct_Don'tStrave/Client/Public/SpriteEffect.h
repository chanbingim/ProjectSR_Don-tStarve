#pragma once

#include "Client_Defines.h"
#include "AinimationObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)
class CSpriteEffect : public CAinimationObject
{
private:
	CSpriteEffect(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSpriteEffect(const CSpriteEffect& rhs);
	virtual ~CSpriteEffect() = default;

public:
	virtual HRESULT		Initialize_Prototype(const char* FilePath, const _wstring FolderName);
	virtual HRESULT		Initialize(void* pArg);

	virtual void		Priority_Update(_float fTimeDelta);
	virtual void		Update(_float fTimeDelta);
	virtual void		Late_Update(_float fTimeDelta);
	virtual HRESULT		Render();

	virtual void		ReadyEffect(const WCHAR* AnimName);
	virtual void		ResetObejctaData();

	_bool				IsFinished();
private :
	_wstring			m_AnimName = {TEXT("")};

private :
	HRESULT				ADD_Component();

public:
	static	CSpriteEffect*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, const char* FilePath, const _wstring FolderName);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};
NS_END