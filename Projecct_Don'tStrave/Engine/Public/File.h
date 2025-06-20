#pragma once

#include "CSVReader.h"
#include <fstream>
#include <sstream>
#include <ostream>

NS_BEGIN(Engine)
class ENGINE_DLL CFile
{
public :
	template<typename T>
	static _bool		ReadFileFromRaw(const char* FileURL, _uint FileSize, _uint _ReadMode, vector<T>* _pVecOut);

#pragma region CSV
	HRESULT		ReadCSVData(const char* FileURL, const char delimiter, vector<string>* OutListVec);

	HRESULT		SaveCSVData(const char* SaveFilePath, vector<BASE_DATA_STRUCT>* vecSaveData);
#pragma endregion

#pragma region TXT
	//HRESULT		ReadTxtData(const char* FileURL, const char delimiter, vector<string>* OutListVec);

	//template<typename T>
	//HRESULT		SaveTxtData(const char* SaveFilePath, vector<T>* vecSaveData);
#pragma endregion

	_bool		WriteFile(const char* FileURL, ifstream* _WriteData, ios_base _ReadType);

private :
	CCSVReader					m_pCSVReader;
};
NS_END

template<typename T>
inline _bool CFile::ReadFileFromRaw(const char* FileURL, _uint FileSize, _uint _ReadMode, vector<T>* _pVecOut)
{
	//byte 단위로 읽어온 데이터를 용도에 맞게 파싱해서 넘겨줌
	//일단 이런식의 구성이 맞는거같음
	vector<BYTE> ReadData(FileSize);
	ifstream OpenFile(FileURL, _ReadMode);
	if (!OpenFile.is_open())
		return false;

	OpenFile.read((char*)&ReadData[0], ReadData.size());
	OpenFile.close();

	if (nullptr == _pVecOut)
		return false;

	_pVecOut->resize(FileSize);
	for (int i = 0; i < ReadData.size(); ++i)
		(*_pVecOut)[i] = ReadData[i];

	return true;
}
