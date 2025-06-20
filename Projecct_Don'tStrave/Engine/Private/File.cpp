#include "File.h"

HRESULT CFile::ReadCSVData(const char* FileURL, const char delimiter, vector<string>* OutListVec)
{
	return m_pCSVReader.ReadCSVData(FileURL, delimiter, OutListVec);
}

HRESULT CFile::SaveCSVData(const char* SaveFilePath, vector<BASE_DATA_STRUCT>* vecSaveData)
{
	return m_pCSVReader.SaveCSVData(SaveFilePath, vecSaveData);
}

_bool CFile::WriteFile(const char* FileURL, ifstream* _WriteData, ios_base _ReadType)
{
	//이건 아직 수정중

	return true;
}