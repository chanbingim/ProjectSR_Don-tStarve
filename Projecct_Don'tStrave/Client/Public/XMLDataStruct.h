#pragma once
#include "Client_Defines.h"

NS_BEGIN(Engine)
class CTexture;
NS_END

NS_BEGIN(Client)
typedef struct Object_desc {
	_uint	iFolder;						// Sprite에 이미지가 저장되어있는 폴더 번호
	_uint	iFile;							// 해당 폴더 내의 파일 ID
	_float2 fPos;							// 오브젝트 위치
	_float	fAngle;							// 오브젝트 회전
	_float2 fScale;							// 오브젝트 크기
}OBJECT_DESC;

typedef struct Object_Ref_desc {
	_uint iTimeline;						// 시간
	_uint iZindex;							// 오브젝트 앞뒤 판별
}OBJECT_REF_DESC;

typedef struct Key {
	_uint	iId;							// Key ID 를 통해서 판별
	_uint	iTime;							// 키 재생 시간
	OBJECT_DESC tObj;						// 오브젝트 정보
}KEY_DESC;

//MainLine에 들어가있는 키형태의 프레임
typedef struct MainKey {
	_uint iTime;							// 메인키에대한 ID 번호
	vector<OBJECT_REF_DESC> tRefVec;		// 메인키안에 Obejct_Ref라는 데이터가 있음
}MAINKEY_DESC;

typedef struct Mainline {
	vector<MAINKEY_DESC> tKeysVec;			//여기에 메인프레임에 달아져있는 Key의 값이 들어간다.
}MAINLINE_DESC;

//이거 각 부위별로 애니메이션 시간 받아올려고 한거같음
typedef struct Timeline {
	_uint	iId;							// 이건 부위별 ID
	wstring szName;							// 부위 별 ID 와 부위 이름
	vector<KEY_DESC> tKeysVec;				// 메인 라인 안에 타임 라인이 있다.
											// 메인 라인 밑에있는 모든 타임라인을 저장
}TIMELINE_DESC;

typedef struct Scml_Animation {
	wstring szName;							// 애니메이션 이름
	_uint iLength;							// 애니메이션 재생 길이
											// 여기에 interval 재생속도도 있음
	MAINLINE_DESC tMainlinesVec;			// 메인라인 <- 이 밑에 애니메이션 Key가 들어감
	vector<TIMELINE_DESC> tTimelinesVec;	// 각 애니메이션 정보에 타임라인이 있는데 그거에 대한 정보임
}SCML_ANIMATION_DESC;

typedef struct Entity {
	wstring szName;									// 이게 제일 사위의 노드같은 거임
	vector<SCML_ANIMATION_DESC> tAnimationsVec;		// 애니메이션 정보를 구성할 녀석
}ENTITY_DESC;

typedef struct Image_File {
	_uint iId;							// 파일 이미지
	wstring szName;						// 파일 이름
	CTexture* pTexture;					// 텍스쳐
	_float2 fSize;						// 텍스처 사이즈
	_float2 fPivot;						// 중점 위치
}IMAGE_FILE_DESC;

//이미지 폴더 하려고 한거같음
typedef struct Image_Folder {
	_uint iId;							// 폴더 ID 이름
	wstring szName;						// 폴더 이름
	vector<IMAGE_FILE_DESC> tFilesVec;  // 파일 넣으려고 만든 STL
}IMAGE_FOLDER_DESC;
NS_END