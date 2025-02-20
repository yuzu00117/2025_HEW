//-----------------------------------------------------------------------------------------------------
// #name Video.cpp
// #description ����̐�����`��̃C���^�[�t�F�[�X�H�݂����ȁA�O������
// #make 2025/2/7�@���j�S
// #update 2025/2/12
// #comment �ǉ��E�C���\��
//          
//----------------------------------------------------------------------------------------------------

#include "Video.h"
#include"scene.h"
#include"sprite.h"
#include"player_position.h"

Video::Video()
{
}

Video::~Video()
{
}

void Video::Initialize(const std::string& fileName, bool looping)
{
    m_video.create(fileName.c_str(), looping);
    m_state = Video_Start;
}

bool Video::Update()
{
    if (m_state != Video_Finished)
    {
        SceneManager& sceneManager = SceneManager::GetInstance();
        m_video.update(sceneManager.GetDeltaTime());

        if (m_video.hasFinished())
        {
            return true;
        }

        return false;
    }

    return true;
}

void Video::Draw(const XMFLOAT2& pos, float height)
{
    if (m_state != Video_Finished)
    {
        SetMovieShader();

        // �V�F�[�_���\�[�X��ݒ�
        GetDeviceContext()->PSSetShaderResources(0, 1, &m_video.getTexture()->shader_resource_view);


        //����T�C�Y
        XMFLOAT2 size;
        size.x = height * m_video.getAspectRatio();
        size.y = height;

        //�`��
        DrawSprite(
            pos,
            0.0f,
            size,
            1.0f
        );

        SetShaderNormal();

    }


}

void Video::Finalize()
{
    if (m_state != Video_Finished)
    {
        m_state = Video_Finished;
        m_video.destroy();
    }
}
