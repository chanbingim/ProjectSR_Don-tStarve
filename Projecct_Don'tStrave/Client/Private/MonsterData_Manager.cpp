#include "MonsterData_Manager.h"
#include "CUtility.h"
IMPLEMENT_SINGLETON(CMonsterData_Manager)

CMonsterData_Manager::CMonsterData_Manager()
{
}

const MONSTER_DATA& CMonsterData_Manager::Get_MonsterData(_uint iMonsterID) const
{
    if (m_iMonsterDataSize < iMonsterID)
        return m_tMonsterDataVec[0];
    return m_tMonsterDataVec[iMonsterID];
}

void CMonsterData_Manager::LoadMonsterData(const char* MapFilePath)
{
    CFile Reader = {};
    vector<string>      ReadData = {};
    ReadData.clear();
    m_iMonsterDataSize = 0;

    ReadData.reserve(500);
    Reader.ReadCSVData(MapFilePath, ',', &ReadData);

    _uint iDataSize = ReadData.size();

    for (_uint i = 11; i < iDataSize; )
    {
        MONSTER_DATA Data;
        ++m_iMonsterDataSize;
        Data.iId = static_cast<_uint>(std::stoi(ReadData[i++]));

        WCHAR szData[128];
        CUtility::ConvertUTFToWide(ReadData[i++].c_str(), szData);
        Data.strPath = szData;

        CUtility::ConvertUTFToWide(ReadData[i++].c_str(), szData);
        Data.strName = szData;

        Data.iMaxHp = static_cast<_uint>(std::stoi(ReadData[i++]));
        Data.iAtk = static_cast<_uint>(std::stoi(ReadData[i++]));
        Data.fSpeed = static_cast<_float>(std::stof(ReadData[i++]));
        Data.iAtkDistance = static_cast<_uint>(std::stoi(ReadData[i++]));
        Data.iAtkSpeed = static_cast<_uint>(std::stoi(ReadData[i++]));

        Data.iHitMax = static_cast<_uint>(std::stoi(ReadData[i++]));
        CUtility::ConvertUTFToWide(ReadData[i++].c_str(), szData);
        Data.strWilsonDial = szData;
        CUtility::ConvertUTFToWide(ReadData[i++].c_str(), szData);
        Data.strWigfridDial = szData;
        m_tMonsterDataVec.push_back(Data);
    }
}

void CMonsterData_Manager::Free()
{
    __super::Free();
}
