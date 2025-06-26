#include "AnimController.h"
#include "State.h"

CAnimController::CAnimController() : CComponent()
{
}

CAnimController::CAnimController(const CAnimController& rhs) : CComponent(rhs)
{
}

HRESULT CAnimController::Initalize_Prototype()
{
	return S_OK;
}

HRESULT CAnimController::Initialize(void* pArg)
{
	return S_OK;
}

void CAnimController::Tick(_float fTimeDelta)
{
	if (m_CurState)
		m_CurState->Tick(fTimeDelta);
}

void CAnimController::Render()
{
	if (m_CurState)
		m_CurState->Render();
}

void CAnimController::ChangeState(CState* pNewState)
{
	//일단 여기 수업듣고 나서 이거 고민좀해봐야겠네
	//상태 초기화를 Initialize()여기서 프로토 타입을 쓸지 말지
	
	if (pNewState == m_CurState)
		return;

	if (pNewState)
	{
		m_CurState = pNewState;
		m_CurState->Reset_StateData();
	}
}

CAnimController* CAnimController::Create()
{
	CAnimController* pInstance = new CAnimController();
	if (FAILED(pInstance->Initalize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("CREATE FAIL : ANIM CONTROLLER");
	}

	return pInstance;
}

CComponent* CAnimController::Clone(void* pArg)
{
	CAnimController* pInstance = new CAnimController(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("CLONE FAIL : ANIM CONTROLLER");
	}

	return pInstance;
}

void CAnimController::Free()
{
	__super::Free();

	if (m_CurState)
		Safe_Release(m_CurState);
}
