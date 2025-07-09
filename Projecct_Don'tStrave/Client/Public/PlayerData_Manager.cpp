#include "PlayerData_Manager.h"
#include "CUtility.h"
IMPLEMENT_SINGLETON(CPlayerData_Manager)

CPlayerData_Manager::CPlayerData_Manager()
{
}

const PLAYER_DESC& CPlayerData_Manager::Get_PlayerData(_uint iPlayerID) const
{
    if (m_iPlayerDataSize < m_iCharacterID - 200)
        return m_tPlayerDataVec[0];
    return m_tPlayerDataVec[m_iCharacterID - 200];
}

void CPlayerData_Manager::LoadPlayerData(const char* MapFilePath)
{
    CFile Reader = {};
    vector<string>      ReadData = {};
    ReadData.clear();
    m_iPlayerDataSize = 0;

    ReadData.reserve(500);
    Reader.ReadCSVData(MapFilePath, ',', &ReadData);

    _uint iDataSize = ReadData.size();

    for (_uint i = 0; i < iDataSize; )
    {
        PLAYER_DESC Data;
        ++m_iPlayerDataSize;
        Data.iId = static_cast<_uint>(std::stoi(ReadData[i++]));
        WCHAR szData[128];
        CUtility::ConvertUTFToWide(ReadData[i++].c_str(), szData);
        Data.strName = szData;
        Data.iMaxHp = static_cast<_uint>(std::stoi(ReadData[i++]));
        Data.iMaxHunger = static_cast<_uint>(std::stoi(ReadData[i++]));
        Data.iMaxMental = static_cast<_uint>(std::stoi(ReadData[i++]));
        Data.fAtk = static_cast<_float>(std::stoi(ReadData[i++]));
        Data.fDef = static_cast<_float>(std::stoi(ReadData[i++]));
        m_tPlayerDataVec.push_back(Data);
    }
}

void CPlayerData_Manager::Free()
{
    __super::Free();
}
