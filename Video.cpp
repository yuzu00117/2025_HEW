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

void Video::Initialize(const std::string& fileName)
{
    m_video.create(fileName.c_str());
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

        // シェーダリソースを設定
        GetDeviceContext()->PSSetShaderResources(0, 1, &m_video.getTexture()->shader_resource_view);

        // コライダーと位置情報の補正をするため
        float scale = SCREEN_SCALE;

        b2Vec2 screen_center;
        screen_center.x = SCREEN_CENTER_X;
        screen_center.y = SCREEN_CENTER_Y;


        //動画サイズ
        XMFLOAT2 size;
        size.x = height * m_video.getAspectRatio();
        size.y = height;

        //描画
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
