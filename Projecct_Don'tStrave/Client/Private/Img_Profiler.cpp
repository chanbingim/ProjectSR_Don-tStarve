#include "Img_Profiler.h"
#include "GameInstance.h"

CImg_Profiler::CImg_Profiler()
{
}

HRESULT CImg_Profiler::Initialize()
{
	return S_OK;
}

void CImg_Profiler::Update(_float fDeletaTime)
{
	auto time = m_pGameInstance->GetProfileTime();
	if (ImGui::Begin("PROFILER"))
	{
		auto frameSec = g_GameSetting.iMaxFrame;
		char overlay[16];
		ImGui::Text("Priority : ");
		ImGui::SameLine();
		sprintf_s(overlay, "%.2f", time.Priority_Time / frameSec * 100.f);
		ImGui::ProgressBar(time.Priority_Time / frameSec * 100.f, ImVec2(200.f, 30.f), overlay);

		ImGui::Text("Update : ");
		ImGui::SameLine();
		sprintf_s(overlay, "%.2f", time.Update_Time / frameSec * 100.f);
		ImGui::ProgressBar(time.Update_Time / frameSec * 100.f, ImVec2(200.f, 30.f), overlay);

		ImGui::Text("LdateUpdate : ");
		ImGui::SameLine();
		sprintf_s(overlay, "%.2f", time.LateUpdate_Time / frameSec * 100.f);
		ImGui::ProgressBar(time.LateUpdate_Time / frameSec, ImVec2(200.f, 30.f), overlay);

		ImGui::Text("CollisionTime : ");
		ImGui::SameLine();
		sprintf_s(overlay, "%.2f", time.ColUpdate_Time / frameSec * 100.f);
		ImGui::ProgressBar(time.ColUpdate_Time / frameSec, ImVec2(200.f, 30.f), overlay);

		ImGui::Text("Render : ");
		ImGui::SameLine();
		sprintf_s(overlay, "%.2f",  time.Render_Time / frameSec * 100);
		ImGui::ProgressBar(time.Render_Time / frameSec, ImVec2(200.f, 30.f), overlay);

		ImGui::End();
	}
}

CImg_Profiler* CImg_Profiler::Create()
{
	CImg_Profiler* pInstance = new CImg_Profiler();
	if (FAILED(pInstance->Initialize()))
	{
		Safe_Release(pInstance);
		MSG_BOX("CREATE FAIL : PROFILER");
	}

	return pInstance;
}

void CImg_Profiler::Free()
{
	__super::Free();

	
}
