#include"scene.h"
#include"sprite.h"
#include"texture.h"
#include"Xinput_controller.h"
#include"Xinput.h"
#include"keyboard.h"
#include"main.h"
#include"Item_Coin_UI.h"
#include"1-1_boss.h"
#include"gokai.h"


//テクスチャのダウンロード グローバル変数にしてる
ID3D11ShaderResourceView* g_result_Texture = NULL;

//スコアランクのテクスチャ
ID3D11ShaderResourceView* total_score_texture = NULL;		//トータルランクのテクスチャ格納用
ID3D11ShaderResourceView* coin_score_texture = NULL;		//コインランクのテクスチャ格納用
ID3D11ShaderResourceView* clear_time_score_texture = NULL;	//コインランクのテクスチャ格納用
ID3D11ShaderResourceView* gokai_score_texture = NULL;       //豪快ランクのテクスチャ格納用
ID3D11ShaderResourceView* g_score_rank_C_Texture = NULL;	//Cランク
ID3D11ShaderResourceView* g_score_rank_B_Texture = NULL;	//Bランク
ID3D11ShaderResourceView* g_score_rank_A_Texture = NULL;	//Aランク
ID3D11ShaderResourceView* g_score_rank_S_Texture = NULL;	//Sランク

//スコアランクの描画サイズ
#define RANK_SIZE (110)
#define TOTAL_RANK_SIZE (200)

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
    //コントローラーの入力の受け取り
    ControllerState state = GetControllerInput();

    //スコアポイントの一時格納用変数
    int total_score_points = 0;



    //豪快度を取得して、ランクに反映する
    int gokai_count = Gokai_UI::GetNowGokaiCount(); //豪快度を取得
    int gokai_score_points = 0; //豪快度のスコアポイント格納用

    //豪快度に応じて豪快ランクのテクスチャとポイントを設定
    if (gokai_count >= 3000) {
        gokai_score_texture = g_score_rank_A_Texture;
        gokai_score_points = 100;
    }
    else if (gokai_count >= 2000)
    {
        gokai_score_texture = g_score_rank_B_Texture;
        gokai_score_points = 75;
    } else {
        gokai_score_texture = g_score_rank_C_Texture;
        gokai_score_points = 50;
    }



    //コインの枚数を取得して、ランクに反映
    int coin_count = Item_Coin_UI::GetNowCoinCount(); //コインの枚数を取得
    int coin_score_points = 0; //コインのスコアポイント格納用

    //コインの枚数に応じてスコアランクのテクスチャとポイントを設定
    if (coin_count >= 30)
    {
        coin_score_texture = g_score_rank_A_Texture;
        coin_score_points = 100;
    }
    else if (coin_count >= 20)
    {
        coin_score_texture = g_score_rank_B_Texture;
        coin_score_points = 75;
    } else {
        coin_score_texture = g_score_rank_C_Texture;
        coin_score_points = 50;
    }



    //ボスのクリアタイムを取得して、ランクに反映
	Boss_1_1& boss = Boss_1_1::GetInstance(); //ボスのインスタンスを取得
    float boss_clear_time = boss.GetBossElapsedTime(); //ボスのクリアタイムを取得
    int boss_score_points = 0; //ボスのスコアポイント格納用

    //クリアタイムに応じたランクとスコアポイントの設定
    if (boss_clear_time >= 180.0f)
    { //3分以上
        clear_time_score_texture = g_score_rank_C_Texture;
        boss_score_points = 50;
    }
    else if (boss_clear_time >= 120.0f)
    { //2分以上
        clear_time_score_texture = g_score_rank_B_Texture;
        boss_score_points = 75;
    } else { //2分未満
        clear_time_score_texture = g_score_rank_A_Texture;
        boss_score_points = 100;
    }

    //トータルのスコアポイントを計算
    total_score_points = (gokai_score_points + coin_score_points + boss_score_points);

    //トータルのスコアポイントに応じてランクを設定
    if (total_score_points >= 300)
    { //300以上ならSランク
        total_score_texture = g_score_rank_S_Texture;
    }
    else if (total_score_points >= 250)
    { //250以上ならAランク
        total_score_texture = g_score_rank_A_Texture;
    }
    else if (total_score_points >= 200)
    { //200以上ならBランク
        total_score_texture = g_score_rank_B_Texture;
    } else { //200以下ならCランク
        total_score_texture = g_score_rank_C_Texture;
    }

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

    //豪快度のランクを描画
	//シェーダリソースを設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &gokai_score_texture);

    if (gokai_score_texture != NULL) {
        DrawSpriteOld(
			XMFLOAT2(600, 160), //一旦マジックナンバーで管理。後で変える
			0.0f,
			XMFLOAT2(RANK_SIZE, RANK_SIZE)
		);
    }

	//コインのランクを描画
	//シェーダリソースを設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &coin_score_texture);

    if (coin_score_texture != NULL) {
        DrawSpriteOld(
			XMFLOAT2(600, 240), //一旦マジックナンバーで管理。後で変える
			0.0f,
			XMFLOAT2(RANK_SIZE, RANK_SIZE)
		);
    }

    //ボスのクリアタイムランクを描画
	//シェーダリソースを設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &clear_time_score_texture);

    if (clear_time_score_texture != NULL) {
        DrawSpriteOld(
			XMFLOAT2(600, 320), //一旦マジックナンバーで管理。後で変える
			0.0f,
			XMFLOAT2(RANK_SIZE, RANK_SIZE)
		);
    }

    //トータルのランクを描画
	//シェーダリソースを設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &total_score_texture);

    if (total_score_texture != NULL) {
        DrawSpriteOld(
			XMFLOAT2(185, 620), //一旦マジックナンバーで管理。後で変える
			0.0f,
			XMFLOAT2(TOTAL_RANK_SIZE, TOTAL_RANK_SIZE)
		);
    }

	//バックバッファ、フロントバッファ入れ替え
	Present();

}

void ResulttScene::Finalize()
{
	UnInitTexture(g_result_Texture);
}