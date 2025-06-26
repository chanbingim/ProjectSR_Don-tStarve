#include "KeyManager.h"

HRESULT CKeyManager::Initialize()
{
	ZeroMemory(&m_CurFrameInput, MAX_KEY);

	return S_OK;
}

void CKeyManager::BeginKeyInput()
{
	ZeroMemory(&m_CurFrameInput, MAX_KEY);

	for (int i = 0; i < MAX_KEY; ++i)
	{
		if (GetAsyncKeyState(i) & 0x8000)
			m_CurFrameInput[i] = true;
	}
}

void CKeyManager::EndKeyInput()
{
	memcpy(m_PreFrameInput, m_CurFrameInput, MAX_KEY);
}

_bool CKeyManager::GetKeyPressed(_uint KeyNum)
{
	return m_CurFrameInput[KeyNum] && m_PreFrameInput[KeyNum];
}

_bool CKeyManager::GetKeyDown(_uint KeyNum)
{
	return m_CurFrameInput[KeyNum] && !m_PreFrameInput[KeyNum];
}

_bool CKeyManager::GetKeyUp(_uint KeyNum)
{
	return m_PreFrameInput[KeyNum] && !m_CurFrameInput[KeyNum];
}

CKeyManager* CKeyManager::Create()
{
	auto pInstance = new CKeyManager();
	if (FAILED(pInstance->Initialize()))
	{
		Safe_Release(pInstance);
		MSG_BOX("CREATE FAIL : KEY MANAGER");
	}

	return pInstance;
}

void CKeyManager::Free()
{
	__super::Free();
}
