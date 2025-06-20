#pragma once

#include "Base.h"

NS_BEGIN(Engine)
class CTexture;

class ENGINE_DLL CState abstract : public CBase
{
public :
	typedef struct Frame
	{
		CTexture*			pAnimTexture = { nullptr };

		_uint				iStartFrame = {};
		_uint				iEndFrame = {};

		_float				fTimeRate = { 1.f };
		_bool				bIsLoop = { false };
	}FRAME_DESC;

protected :
	CState() = default;
	virtual ~CState() = default;

public :
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);

	virtual HRESULT			Reset_StateData();

	//여기서 텍스쳐 컴포넌트의 정보를 가지고있어도 되는거같은데
	virtual void			Tick(_float fTimeDelta);
	virtual void			Render();

	void					SetFrameRate(_float _ftimeRate);

protected :
	FRAME_DESC					m_Frame = {};

	_float						m_AccTime = {};
	_float						m_fFrameTime = {};

	vector<function<void()>>	m_Notify;

public :
	virtual		void		Free() override;
};
NS_END