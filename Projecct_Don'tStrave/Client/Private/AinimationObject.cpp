#include "AinimationObject.h"

#include "GameInstance.h"
#include "Terrian_Manager.h"
#include "Camera.h"
#include "CMath.h"

CAinimationObject::CAinimationObject(LPDIRECT3DDEVICE9 pGraphic_Device) : 
    CLandObject(pGraphic_Device),
    m_pTerrian_Manager{ CTerrian_Manager::GetInstance() }
{
    Safe_AddRef(m_pTerrian_Manager);
}

CAinimationObject::CAinimationObject(const CAinimationObject& rhs) :
    CLandObject(rhs),
    m_pTerrian_Manager{ CTerrian_Manager::GetInstance() },
    m_tAnimation{ rhs.m_tAnimation },
    m_tImageVec{ rhs.m_tImageVec }
{
    Safe_AddRef(m_pTerrian_Manager);
}

HRESULT CAinimationObject::Initialize_Prototype() 
{
	return S_OK;
}

HRESULT CAinimationObject::Initialize(void* pArg)
{
     if(FAILED(__super::Initialize(pArg)))
        return E_FAIL;

     class CGameObject* Obj = m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Camera"));
     m_pCamera = dynamic_cast<CCamera*>(Obj);
     if (nullptr == m_pCamera)
         return E_FAIL;

	return S_OK;
}

void CAinimationObject::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);

    m_fAniTime += (_uint)(fTimeDelta * 800);
}

void CAinimationObject::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
   
}

void CAinimationObject::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CAinimationObject::Render()
{
    __super::Render();
	return S_OK;
}

HRESULT CAinimationObject::LoadImageFile(vector<IMAGE_FOLDER_DESC>* tImageVec)
{
    if (nullptr == tImageVec)
    {
        for (auto& folder : m_tImageVec) {
            for (auto& file : folder.tFilesVec) {
                if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_" + file.szName),
                    TEXT("Com_" + file.szName), reinterpret_cast<CComponent**>(&file.pTexture))))
                {
                    m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_" + file.szName),
                        CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, file.szName.c_str()));
                    __super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_" + file.szName),
                        TEXT("Com_" + file.szName), reinterpret_cast<CComponent**>(&file.pTexture));
                }
            }
        }
    }
    else
    {
        for (auto& folder : *tImageVec) {
            for (auto& file : folder.tFilesVec) {
                if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_" + file.szName),
                    TEXT("Com_" + file.szName), reinterpret_cast<CComponent**>(&file.pTexture))))
                {
                    m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_" + file.szName),
                        CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, file.szName.c_str()));
                    __super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_" + file.szName),
                        TEXT("Com_" + file.szName), reinterpret_cast<CComponent**>(&file.pTexture));
                }
            }
        }
    }
    m_fAniTime = 0;
    m_iLength = 1000;
    return S_OK;
}

void CAinimationObject::XMLRenderAnimation(const wstring& animName, Entity* tEntity, vector<IMAGE_FOLDER_DESC>* AnimVec)
{
    //애니메이션을 받아온다.
    const SCML_ANIMATION_DESC* pAnim = nullptr;

    if (nullptr == tEntity)
        tEntity = &m_tAnimation;
     
    if (nullptr == AnimVec)
        AnimVec = &m_tImageVec;

    //애니메이션 저장되어있는 엔티티에서
    //모든 애니메이션 정보를 탐색하면서 animName과 같은걸 찾는다.
    for (auto& anim : (*tEntity).tAnimationsVec) {
        if (0 == wcsncmp(anim.szName.c_str(), animName.c_str(), max(animName.size(), anim.szName.size()))) {
            pAnim = &anim;
            break;
        }
    }
    
    //애니메이션 못찾으면 리턴
    if (!pAnim) return;

    //애니메이션 재생 길이를 받아온다.
    m_iLength = pAnim->iLength;

    // 부동소수점 연산으로 뭘하려했는지 알아내ㅔ야함
    // 이거 프레임 계산같음
    if (!m_bAnimPause)
        m_fAniTime = m_fAniTime % m_iLength;

    //시간에 따른 오브젝트의 순서 밑 재생프레임을 받아오려함
    vector<OBJECT_REF_DESC> timeVec = {};

    //타임 라인을 모두 받아온다.
    for (auto& ref : pAnim->tMainlinesVec.tKeysVec[0].tRefVec) {
        timeVec.push_back(ref);
    }

    //메인라인에 연결되어있는 키 값을 다 받아옴
    for (auto& mainKey : pAnim->tMainlinesVec.tKeysVec) {

        //메인 키 프레임과 애니메이션 시간을 비교
        //메인 키의 애니메이션 시간이 m_fAniTime 보다 작아지면 애니메이션 재생종료
        if (mainKey.iTime <= m_fAniTime) {
            timeVec.clear();
            for (auto& ref : mainKey.tRefVec) {
                timeVec.push_back(ref);
            }
        }

        // 메인 시간이 m_fAniTime보다 크다면 종료
        // 이거 프레임 계산같음
        if (mainKey.iTime > m_fAniTime) {
            break;
        }
    }

    //정렬을 통해서 애니메이션 순서 정해주려고 한거같음
    sort(timeVec.begin(), timeVec.end(), [](OBJECT_REF_DESC pSour, OBJECT_REF_DESC pDest)->_bool
        {
            return pSour.iZindex < pDest.iZindex;
        });

    //시간에 따른 오브젝트 재생프레임을 받아서 오브젝트의 크기및 연산을 처리함
    for (OBJECT_REF_DESC timelineId : timeVec)
    {
        // 전에 재생한 키 프레임
        const KEY_DESC* pPrevKey = nullptr;

        //다음에 재생해야하는 키 프레임
        const KEY_DESC* pNextKey = nullptr;

        //현재 선택된 애니메이션 타임라인 ID 값에 연동되어있는
        //키 프레임을 가져와서 연동함
        size_t Timeline_KeyEnd = pAnim->tTimelinesVec[timelineId.iTimeline].tKeysVec.size();
        for (size_t i = 0; i < Timeline_KeyEnd; ++i)
        {
            //현재 선택된 애니메이션에 연동되어있는 키프레임을 모두 순회하면서
            //Key  마다 재생시간이 지났는지를 검사한다.
            if (pAnim->tTimelinesVec[timelineId.iTimeline].tKeysVec[i].iTime > m_fAniTime) {
                //이전키에 현재 탐색 하는 타임 키를 넣어주고
                //탐색하는키가 0인가 확인
                // 0이면 마지막껄 가져와서 넣어주고 아니라면 i - 1로 재생함
                pPrevKey = i == 0 ? &pAnim->tTimelinesVec[timelineId.iTimeline].tKeysVec.back() : &pAnim->tTimelinesVec[timelineId.iTimeline].tKeysVec[i - 1];
               
                // 다음 재생 키프레임은 i번째의 데이터를 가져옴
                pNextKey = &pAnim->tTimelinesVec[timelineId.iTimeline].tKeysVec[i];
                break;
            }
        }

        //이전 프레임에 재생한 키나 다음에 재생할 키가 둘다 널이 아니라면
        if (!pPrevKey || !pNextKey)
        {
            //현재 선택된 애니메이션의 타임라인에 연결된게 없을경우
            //다음 오브젝트 이동
            if (pAnim->tTimelinesVec[timelineId.iTimeline].tKeysVec.empty()) {
                continue;
            }

            //키 프레임에 넣어져있는 마지막을 이전 키프레임에 넣음
            pPrevKey = &pAnim->tTimelinesVec[timelineId.iTimeline].tKeysVec.back();

            //맨 앞의 데이터를 다음 키 프레임에 넣음
            pNextKey = &pAnim->tTimelinesVec[timelineId.iTimeline].tKeysVec.front();
        }

        //이건 스케일 위치 보정하려고 한거같음
        float t = 0.f;
        //다음 재생 시작 시간 - 이전 재생시작 시간 하면 현재 재생중인 타임 값이 나옴
        int timeDiff = pNextKey->iTime - pPrevKey->iTime;

        // 0이 아니라면 애니메이션 진행중
        if (timeDiff != 0)
        {
            //현재 시간에서 이전 프레임 시간을 빼면 진행된 시간이 나옴
            int timePassed = static_cast<int>(m_fAniTime) - pPrevKey->iTime;

            //그걸 비율로 해서 t 값을 구해서 보간 함
            t = static_cast<float>(timePassed) / timeDiff;
        }

        OBJECT_DESC a = pPrevKey->tObj;
        OBJECT_DESC b = pNextKey->tObj;
        OBJECT_DESC object;
        object.iFolder = a.iFolder;
        object.iFile = a.iFile;
        D3DXVec2Lerp(&object.fPos, &a.fPos, &b.fPos, t);
        D3DXVec2Lerp(&object.fScale, &a.fScale, &b.fScale, t);
        object.fAngle = a.fAngle + (fmodf(b.fAngle - a.fAngle + 540.f, 360.f) - 180.f) * t;
        if (object.iFolder >= (*AnimVec).size() || object.iFile >= (*AnimVec)[object.iFolder].tFilesVec.size()) continue;
        IMAGE_FILE_DESC image = (*AnimVec)[object.iFolder].tFilesVec[object.iFile];

        _float x = image.fSize.x / (image.fSize.x + image.fSize.y);
        _float y = image.fSize.y / (image.fSize.x + image.fSize.y);

        D3DXMATRIX matPivot, matScale, matRotZ, matTrans, matBillboard, matWorld;
        _float3 pos = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
        D3DXMatrixTranslation(&matPivot, 0.5f - image.fPivot.x, 0.5f - image.fPivot.y, 0.f);
        D3DXMatrixScaling(&matScale, image.fSize.x * object.fScale.x / 400.f, image.fSize.y * object.fScale.y / 400.f, 1.f);
        D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(object.fAngle));

        D3DXMatrixTranslation(&matTrans, object.fPos.x / 400.f, object.fPos.y / 400.f, 0.f);

        auto Camera = m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Camera"));
        matBillboard = Camera->GetTransfrom()->Get_World();
        memcpy((_float3*)&matBillboard.m[3], pos, sizeof(_float3));

        matWorld = matPivot * matScale * matRotZ * matTrans * matBillboard;

        image.pTexture->Set_Texture(0);
        m_pGraphic_Device->SetTransform(D3DTS_WORLD, &matWorld);
        m_pVIBufferCom->Render();
    }
}

CGameObject* CAinimationObject::Clone(void* pArg)
{
	return nullptr;
}

void CAinimationObject::Free()
{
    __super::Free();
    Safe_Release(m_pTerrian_Manager);
    Safe_Release(m_pVIBufferCom);

    for (auto& Folderiter : m_tImageVec)
    {
        for (auto& Fileiter : Folderiter.tFilesVec)
            Safe_Release(Fileiter.pTexture);
    }
}
