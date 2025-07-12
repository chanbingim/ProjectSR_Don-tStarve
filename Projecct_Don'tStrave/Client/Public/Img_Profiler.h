#pragma once
#include "ImgBase.h"

NS_BEGIN(Client)
class CImg_Profiler : public CImgBase
{
private :
	CImg_Profiler();
	virtual ~CImg_Profiler() = default;

public:
	virtual HRESULT			Initialize() override;
	virtual void			Update(_float fDeletaTime) override;


private :
	string					m_TimeData;


public:
	static		CImg_Profiler*				Create();
	virtual		void						Free() override;
};
NS_END