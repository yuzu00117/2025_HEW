#include"scene.h"
#include"sprite.h"
#include"texture.h"
#include"Xinput_controller.h"
#include"Xinput.h"
#include"keyboard.h"
#include"main.h"


//テクスチャのダウンロード グローバル変数にしてる
ID3D11ShaderResourceView* g_stage_select_Texture = NULL;

void StageSelectScene::Initialize()
{
	g_stage_select_Texture = InitTexture(L"asset\\texture\\sample_texture\\sample_stage_select.png");
}

void StageSelectScene::Update()
{
	//コントローラーの入力の受け取り
	ControllerState state = GetControllerInput();

	if (Keyboard_IsKeyDown(KK_SPACE) || (state.buttonA))
	{
		SceneManager& sceneManager = SceneManager::GetInstance();
		sceneManager.ChangeScene(SCENE_GAME);
	};
}

void StageSelectScene::Draw()
{

	//バッファクリア
	Clear();

	//2D描画なので深度無効
	SetDepthEnable(false);

	if (g_stage_select_Texture != nullptr)
	{
		// シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_stage_select_Texture);

		DrawSpriteOld(
			XMFLOAT2(SCREEN_XCENTER, SCREEN_YCENTER),
			0.0f,
			XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT)
		);
	}

	//バックバッファ、フロントバッファ入れ替え
	Present();

}

void StageSelectScene::Finalize()
{
	UnInitTexture(g_stage_select_Texture);
}