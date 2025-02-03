#include"scene.h"
#include"sprite.h"
#include"texture.h"
#include"Xinput_controller.h"
#include"Xinput.h"
#include"keyboard.h"
#include"main.h"
#include"Item_Coin_UI.h"
#include"1-1_boss.h"


//テクスチャのダウンロード グローバル変数にしてる
ID3D11ShaderResourceView* g_result_Texture = NULL;

//スコアランクのテクスチャ
ID3D11ShaderResourceView* coin_score_texture = NULL;		//コインスコアランクのテクスチャ格納用
ID3D11ShaderResourceView* clear_time_score_texture = NULL;	//コインスコアランクのテクスチャ格納用
ID3D11ShaderResourceView* g_score_rank_C_Texture = NULL;	//Cランク
ID3D11ShaderResourceView* g_score_rank_B_Texture = NULL;	//Bランク
ID3D11ShaderResourceView* g_score_rank_A_Texture = NULL;	//Aランク
ID3D11ShaderResourceView* g_score_rank_S_Texture = NULL;	//Sランク

//スコアランクの描画サイズ
#define SCORE_RANK_SIZE (100)

void ResulttScene::Initialize()
{
	//リザルト画面の背景テクスチャの読み込み
	g_result_Texture = InitTexture(L"asset\\texture\\sample_texture\\sample_result.png");
	

	//スコアランクのテクスチャの読み込み
	g_score_rank_C_Texture = InitTexture(L"asset\\texture\\sample_texture\\stamp01_c.png");
	g_score_rank_B_Texture = InitTexture(L"asset\\texture\\sample_texture\\stamp01_b.png");
	g_score_rank_A_Texture = InitTexture(L"asset\\texture\\sample_texture\\stamp01_a.png");
	g_score_rank_S_Texture = InitTexture(L"asset\\texture\\sample_texture\\stamp01_s.png");
}

void ResulttScene::Update()
{
    // コントローラーの入力の受け取り
    ControllerState state = GetControllerInput();

    // コインの枚数を取得
    int coinCount = Item_Coin_UI::GetNowCoinCount();
    int scorePoints = 0;

    // コインの枚数に応じてスコアランクのテクスチャとポイントを設定
    if (coinCount >= 30) {
        coin_score_texture = g_score_rank_A_Texture;
        scorePoints = 100;
    } else if (coinCount >= 20) {
        coin_score_texture = g_score_rank_B_Texture;
        scorePoints = 75;
    } else if (coinCount >= 10) {
        coin_score_texture = g_score_rank_C_Texture;
        scorePoints = 50;
    } else {
        coin_score_texture = g_score_rank_C_Texture; // デフォルトはCランク
        scorePoints = 50;
    }

    // スコアポイントを変数に加算
    total_score_points += scorePoints;



    // ボスのクリアタイムを取得
	Boss_1_1& boss = Boss_1_1::GetInstance();
    float bossClearTime = boss.GetBossElapsedTime();

    // クリアタイムに応じたランクとスコアポイントの設定
    if (bossClearTime >= 180.0f) { // 3分以上
        clear_time_score_texture = g_score_rank_C_Texture;
        scorePoints = 50;
    } else if (bossClearTime >= 120.0f) { // 2分以上
        clear_time_score_texture = g_score_rank_B_Texture;
        scorePoints = 75;
    } else { // 2分未満
        clear_time_score_texture = g_score_rank_A_Texture;
        scorePoints = 100;
    }

    // スコアポイントを変数に加算
    total_score_points += scorePoints;

    if (Keyboard_IsKeyDown(KK_SPACE) || (state.buttonA))
    {
        SceneManager& sceneManager = SceneManager::GetInstance();
        sceneManager.ChangeScene(SCENE_STAGE_SELECT);
    }
}

void ResulttScene::Draw()
{

	//バッファクリア
	Clear();

	//2D描画なので深度無効
	SetDepthEnable(false);

	//リザルト画面の背景の描画
	if (g_result_Texture != nullptr)
	{
		// シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_result_Texture);

		DrawSpriteOld(
			XMFLOAT2(SCREEN_XCENTER, SCREEN_YCENTER),
			0.0f,
			XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT)
		);
	}

	//ボスのクリアタイムスコアを描画
	//シェーダリソースを設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &clear_time_score_texture);

    if (clear_time_score_texture != NULL) {
        DrawSpriteOld(
			XMFLOAT2(550, 325), //一旦マジックナンバーで管理。後で変える
			0.0f,
			XMFLOAT2(SCORE_RANK_SIZE, SCORE_RANK_SIZE)
		);
    }

	//スコアランクを描画
	//シェーダリソースを設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &coin_score_texture);

    if (coin_score_texture != NULL) {
        DrawSpriteOld(
			XMFLOAT2(550, 245), //一旦マジックナンバーで管理。後で変える
			0.0f,
			XMFLOAT2(SCORE_RANK_SIZE, SCORE_RANK_SIZE)
		);
    }


	//バックバッファ、フロントバッファ入れ替え
	Present();

}

void ResulttScene::Finalize()
{
	UnInitTexture(g_result_Texture);
}