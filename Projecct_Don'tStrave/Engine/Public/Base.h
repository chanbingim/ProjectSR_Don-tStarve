#pragma once

/* ���۷��� ī��Ʈ�� ����� �ڽİ�ü���� �ο����ֱ� ���ؼ�. */
/* ���۷��� ī��Ʈ�� ������Ű��. */
/* ���۷��� ī��Ʈ�� ���ҽ�Ų��. or �蠫����. */
#include "Engine_Defines.h"

NS_BEGIN(Engine)

class ENGINE_DLL CBase abstract
{
protected:
	CBase();
	virtual ~CBase() = default;

public:
	/* ���۷��� ī��Ʈ�� ������Ų��.  */
	/* ���� : ������Ų ��� ��. */
	_uint AddRef();

	/* ���۷��� ī��Ʈ�� ���ҽ�Ų��. or �蠫����. */
	/* ���� : ���ҽ�Ű�� ������ ��. */
	_uint Release();

private:
	_uint		m_iRefCnt = {};

public:
	virtual void Free();
};

NS_END



