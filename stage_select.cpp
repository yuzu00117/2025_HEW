//-----------------------------------------------------------------------------------------------------
// #name stage_select.cpp
// #description ステージセレクトのCPP
// #make 2024/12/28　　永野義也
// #update 2024/12/28
// #comment 追加・修正予定
//          ・タイトル作ったさねー
//           
//----------------------------------------------------------------------------------------------------

#include"scene.h"
#include"sprite.h"
#include"texture.h"
#include"Xinput_controller.h"
#include"Xinput.h"
#include"keyboard.h"
#include"main.h"
#include"collider_type.h"
#include"stage_select.h"
#include"stage_select_player.h"
#include"stage_point.h"
#include"stage_point_factory.h"
#include"stage_select_contact_list.h"
#include"tool.h"
#include"easing.h"
#include"sound.h"


constexpr float SCALE = 30.0f; // ピクセルからメートルへの変換スケール
//テクスチャのダウンロード グローバル変数にしてる

//背景の表示
ID3D11ShaderResourceView* g_stage_select_background_Texture = NULL;


//Aボタン押した時のエフェクト
ID3D11ShaderResourceView* g_stage_select_tap_effect_Texture = NULL;

//コインのキラキラのエフェクト
ID3D11ShaderResourceView* g_stage_select_coin_effect_Texture = NULL;
//コインのキラキラのエフェクト1
ID3D11ShaderResourceView* g_stage_select_coin_effect_Texture1 = NULL;
//コインのキラキラのエフェクト2
ID3D11ShaderResourceView* g_stage_select_coin_effect_Texture2 = NULL;
//コインのキラキラのエフェクト3
ID3D11ShaderResourceView* g_stage_select_coin_effect_Texture3 = NULL;
//コインのキラキラのエフェクト4
ID3D11ShaderResourceView* g_stage_select_coin_effect_Texture4= NULL;

//チュートリアルステージのやホップアップ
ID3D11ShaderResourceView* g_stage_select_hopup_tutorial_Texture = NULL;
//１－１のホップアップ
ID3D11ShaderResourceView* g_stage_select_hopup_1_1_Texture = NULL;
//まだ解放してないマップのホップアップ
ID3D11ShaderResourceView* g_stage_select_hopup_unknow_Texture = NULL;

//管理用に使う数字
ID3D11ShaderResourceView* g_stage_select_number_Texture = NULL;


//決定した時に使うズームイン
ID3D11ShaderResourceView* g_stage_select_fade_black_Texture = NULL;

//決定してステージにとぶまでにテクスチャなくなる問題を潰すためのくろ
ID3D11ShaderResourceView* g_stage_select_black_Texture = NULL;



// メンバ変数として保持

StagePointFactory m_stagePointFactory;



void StageSelectScene::Initialize()
{
	//背景の初期化
	g_stage_select_background_Texture = InitTexture(L"asset\\texture\\stage_select_texture\\stage_select_background.png");

	g_stage_select_coin_effect_Texture= InitTexture(L"asset\\texture\\stage_select_texture\\stage_select_coin_effect.png");

	g_stage_select_coin_effect_Texture1 = InitTexture(L"asset\\texture\\stage_select_texture\\stage_select_coin_effect1.png");

	g_stage_select_coin_effect_Texture2 = InitTexture(L"asset\\texture\\stage_select_texture\\stage_select_coin_effect2.png");

	g_stage_select_coin_effect_Texture3 = InitTexture(L"asset\\texture\\stage_select_texture\\stage_select_coin_effect3.png");

	g_stage_select_coin_effect_Texture4 = InitTexture(L"asset\\texture\\stage_select_texture\\stage_select_coin_effect4.png");

	g_stage_select_tap_effect_Texture= InitTexture(L"asset\\texture\\stage_select_texture\\stage_select_tap_effect.png");

	g_stage_select_hopup_tutorial_Texture= InitTexture(L"asset\\texture\\stage_select_texture\\stage_select_hopup_tutorial.png");

	g_stage_select_hopup_1_1_Texture= InitTexture(L"asset\\texture\\stage_select_texture\\stage_select_hopup_1-1.png");

	g_stage_select_hopup_unknow_Texture=InitTexture(L"asset\\texture\\stage_select_texture\\stage_select_hopup_unknow.png");

	g_stage_select_number_Texture = InitTexture(L"asset\\texture\\stage_select_texture\\stage_select_number.png");

	g_stage_select_fade_black_Texture= InitTexture(L"asset\\texture\\sample_texture\\sample_fade_black.png");

	g_stage_select_black_Texture= InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_block.png");


	//ワールドをつくる
	 // Box2Dワールドの作成
	b2Vec2 gravity(0.0f,0.0f); // 重力なし
	m_world = new b2World(gravity);

	
	StageSelectPlayer& m_player = StageSelectPlayer::GetInstance();
	//プレイヤーの定義
	m_player.Initialize(m_world, 250, 500);

	//初期のポジションを定義
	m_player.SetTouchStagePointBody(m_player.GetBody());

	// ステージポイントの生成
	m_stagePointFactory.CreateStagePoint(m_world, 322.0f, 335.0f, 110.0f,1);
	m_stagePointFactory.CreateStagePoint(m_world, 530.0f, 210.0f, 110.0f,2);
	m_stagePointFactory.CreateStagePoint(m_world, 700.0f, 360.0f,100.0f,3);
	m_stagePointFactory.CreateStagePoint(m_world, 980.0f, 440.0f, 100.0f, 4);

	StageSelectContactListener& listener = StageSelectContactListener::GetInstance();
	m_world->SetContactListener(&listener);
	
	
	//全ての音を止める
	app_atomex_stop_player();

	//曲を流す
	app_atomex_start(STAGESELECT_BGM);
}

void StageSelectScene::Update()
{

		//コントローラーの入力の受け取り
		ControllerState state = GetControllerInput();

		if (Keyboard_IsKeyDown(KK_SPACE) || (state.buttonA))

		{
			/*SceneManager& sceneManager = SceneManager::GetInstance();
			sceneManager.ChangeScene(SCENE_GAME);*/
		};


		// Box2Dワールドの更新処理（必要なら追加可能）
		if (m_world) {
			float timeStep = 1.0f / 60.0f; // 更新間隔
			int32 velocityIterations = 6;
			int32 positionIterations = 2;
			m_world->Step(timeStep, velocityIterations, positionIterations);
		}

		StageSelectPlayer& m_player = StageSelectPlayer::GetInstance();

		m_player.Update();

		/* ステージポイントの更新*/
		m_stagePointFactory.Update();
	

		//タップのエフェクト用の
		if (Keyboard_IsKeyDown(KK_SPACE) || (state.buttonA))
		{
			if (g_tap_addition == 0)
			{

				int random_num;
				random_num = g_coin_effect_random_number = GetRandomInt(1, 360);

				g_tap_effect_angle = DegreesToRadians(random_num);

				g_tap_addition = 1;

				app_atomex_start(Decision_Sound);

				m_player.SetTapTextureFlag(true);
			}
		}
	

		//カーソルをステージポイントにあててる時
		if (m_player.GetTouchStageSelectNum() != 0)
		{
			
			if ((Keyboard_IsKeyDown(KK_SPACE) || (state.buttonA))&& (Stage_select_flag==false))
			{
				Stage_select_flag = true;
				fade_rate = 0.01;
				app_atomex_start(Select_Sound);
			}
		}


		

		
		//フェードの関数になったら
		if (1<=fade_rate)
		{
			SceneManager& sceneManager = SceneManager::GetInstance();
		
			switch (m_player.GetTouchStageSelectNum())
			{
			case 0:
				break;
			case 1:
				sceneManager.SetStageName(STAGE_TUTORIAL);
				sceneManager.ChangeScene(SCENE_GAME);
				break;
			case 2:
				sceneManager.SetStageName(STAGE_1_1);
				sceneManager.ChangeScene(SCENE_GAME);
				break;

			case 3:
				sceneManager.SetStageName(STAGE_TEST);
				sceneManager.ChangeScene(SCENE_GAME);
				break;

			case 4:
				sceneManager.SetStageName(STAGE_BOSS);
				sceneManager.ChangeScene(SCENE_GAME);
				break;
			}
		}


	
}

void StageSelectScene::Draw()
{
	
		//バッファクリア
		Clear();

		//2D描画なので深度無効
		SetDepthEnable(false);

		//プレイヤーのインスタンスの確保
		StageSelectPlayer& m_player = StageSelectPlayer::GetInstance();


		//-------------------------------------------------------------------------------------------
		//背景
		if (g_stage_select_background_Texture != nullptr)
		{
			// シェーダリソースを設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_stage_select_background_Texture);

			DrawSpriteOld(
				XMFLOAT2(SCREEN_XCENTER, SCREEN_YCENTER),
				0.0f,
				XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT)
			);
		}
		//------------------------------------------------------------------------------------------------------
		
		


		
		//------------------------------------------------------------------------------------------------------
		//コイン描画
		
		//大変ばちぼこに頭悪いかも

		if (g_coin_effect_random_number == 0)
		{
			g_coin_effect_random_number = GetRandomInt(1,10);
		}

		if (g_coin_effect_random_number1 == 0)
		{
			g_coin_effect_random_number1 = GetRandomInt(1, 10);
		}

		if (g_coin_effect_random_number2 == 0)
		{
			g_coin_effect_random_number2 = GetRandomInt(1, 10);
		}

		if (g_coin_effect_random_number3 == 0)
		{
			g_coin_effect_random_number3 = GetRandomInt(1, 10);
		}

		if (g_coin_effect_random_number4 == 0)
		{
			g_coin_effect_random_number4 = GetRandomInt(1, 10);
		}



		g_coin_addition++;
		g_coin_addition1++;
		g_coin_addition2++;
		g_coin_addition3++;
		g_coin_addition4++;

		
		// シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_stage_select_coin_effect_Texture);

		switch (g_coin_effect_random_number)
		{
		case 1:
			//左上金貨　左下
			DrawDividedSprite(XMFLOAT2(140,100), 0.0f,XMFLOAT2(70,70), 5, 6, g_coin_addition/2, 1.0);
		
			break;
		case 2:
			//左下錨
			DrawDividedSprite(XMFLOAT2(220, 550), 30.0f, XMFLOAT2(90, 90), 5, 6, g_coin_addition / 2, 1.0);

			break;
		case 3:
			//錨鎖　左上
			DrawDividedSprite(XMFLOAT2(30, 250), 0.0f, XMFLOAT2(50, 50), 5, 6, g_coin_addition / 2, 1.0);

			break;
		case 4:
			//錨鎖　した
			DrawDividedSprite(XMFLOAT2(30, 400), 0.0f, XMFLOAT2(50, 50), 5, 6, g_coin_addition / 2, 1.0);

			break;
		case 5:
			//左上金貨　左上
			DrawDividedSprite(XMFLOAT2(40, 30), 0.0f, XMFLOAT2(70, 70), 5, 6, g_coin_addition / 2, 1.0);

			break;
		case 6:
			//右上 がち右上
			DrawDividedSprite(XMFLOAT2(1180, 30), 0.0f, XMFLOAT2(70, 70), 5, 6, g_coin_addition / 2, 1.0);

			break;
		case 7:
			//右上　ちと右上
			DrawDividedSprite(XMFLOAT2(1050, 75), 0.0f, XMFLOAT2(70, 70), 5, 6, g_coin_addition / 2, 1.0);
			break;
		case 8:
			//右上　ちとちと右上
			DrawDividedSprite(XMFLOAT2(1100, 275), 0.0f, XMFLOAT2(70, 70), 5, 6, g_coin_addition / 2, 1.0);
			break;
		case 9:
			//右下　がち
			DrawDividedSprite(XMFLOAT2(1250, 680), 0.0f, XMFLOAT2(70, 70), 5, 6, g_coin_addition / 2, 1.0);
			break;

		case 10:
			//右下　ちと
			DrawDividedSprite(XMFLOAT2(1110, 480), 0.0f, XMFLOAT2(70, 70), 5, 6, g_coin_addition / 2, 1.0);
			break;
		default:
			break;
		}





		// シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_stage_select_coin_effect_Texture1);

		switch (g_coin_effect_random_number1)
		{
		case 1:
			//左上金貨　左下
			DrawDividedSprite(XMFLOAT2(140, 100), 0.0f, XMFLOAT2(70, 70), 5, 5, g_coin_addition1 / 2, 1.0);

			break;
		case 2:
			//左下錨
			DrawDividedSprite(XMFLOAT2(220, 550), 30.0f, XMFLOAT2(90, 90), 5, 5, g_coin_addition1 / 2, 1.0);

			break;
		case 3:
			//錨鎖　左上
			DrawDividedSprite(XMFLOAT2(30, 250), 0.0f, XMFLOAT2(50, 50), 5, 5, g_coin_addition1 / 2, 1.0);

			break;
		case 4:
			//錨鎖　した
			DrawDividedSprite(XMFLOAT2(30, 400), 0.0f, XMFLOAT2(50, 50), 5, 5, g_coin_addition1 / 2, 1.0);

			break;
		case 5:
			//左上金貨　左上
			DrawDividedSprite(XMFLOAT2(40, 30), 0.0f, XMFLOAT2(70, 70), 5, 5, g_coin_addition1 / 2, 1.0);

			break;
		case 6:
			//右上 がち右上
			DrawDividedSprite(XMFLOAT2(1180, 30), 0.0f, XMFLOAT2(70, 70), 5, 5, g_coin_addition1 / 2, 1.0);

			break;
		case 7:
			//右上　ちと右上
			DrawDividedSprite(XMFLOAT2(1050, 75), 0.0f, XMFLOAT2(70, 70), 5, 5, g_coin_addition1 / 2, 1.0);
			break;
		case 8:
			//右上　ちとちと右上
			DrawDividedSprite(XMFLOAT2(1100, 275), 0.0f, XMFLOAT2(70, 70), 5, 5, g_coin_addition1 / 2, 1.0);
			break;
		case 9:
			//右下　がち
			DrawDividedSprite(XMFLOAT2(1250, 680), 0.0f, XMFLOAT2(70, 70), 5, 5, g_coin_addition1 / 2, 1.0);
			break;

		case 10:
			//右下　ちと
			DrawDividedSprite(XMFLOAT2(1110, 480), 0.0f, XMFLOAT2(70, 70), 5, 5, g_coin_addition1 / 2, 1.0);
			break;
		default:
			break;
		}


		// シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_stage_select_coin_effect_Texture2);

		switch (g_coin_effect_random_number2)
		{
		case 1:
			//左上金貨　左下
			DrawDividedSprite(XMFLOAT2(140, 100), 0.0f, XMFLOAT2(70, 70), 5, 5, g_coin_addition2 / 2, 1.0);

			break;
		case 2:
			//左下錨
			DrawDividedSprite(XMFLOAT2(220, 550), 30.0f, XMFLOAT2(90, 90), 5, 5, g_coin_addition2 / 2, 1.0);

			break;
		case 3:
			//錨鎖　左上
			DrawDividedSprite(XMFLOAT2(30, 250), 0.0f, XMFLOAT2(50, 50), 5, 5, g_coin_addition2 / 2, 1.0);

			break;
		case 4:
			//錨鎖　した
			DrawDividedSprite(XMFLOAT2(30, 400), 0.0f, XMFLOAT2(50, 50), 5, 5, g_coin_addition2 / 2, 1.0);

			break;
		case 5:
			//左上金貨　左上
			DrawDividedSprite(XMFLOAT2(40, 30), 0.0f, XMFLOAT2(70, 70), 5, 5, g_coin_addition2 / 2, 1.0);

			break;
		case 6:
			//右上 がち右上
			DrawDividedSprite(XMFLOAT2(1180, 30), 0.0f, XMFLOAT2(70, 70), 5, 5, g_coin_addition2 / 2, 1.0);

			break;
		case 7:
			//右上　ちと右上
			DrawDividedSprite(XMFLOAT2(1050, 75), 0.0f, XMFLOAT2(70, 70), 5, 5, g_coin_addition2 / 2, 1.0);
			break;
		case 8:
			//右上　ちとちと右上
			DrawDividedSprite(XMFLOAT2(1100, 275), 0.0f, XMFLOAT2(70, 70), 5, 5, g_coin_addition2 / 2, 1.0);
			break;
		case 9:
			//右下　がち
			DrawDividedSprite(XMFLOAT2(1250, 680), 0.0f, XMFLOAT2(70, 70), 5, 5, g_coin_addition2 / 2, 1.0);
			break;

		case 10:
			//右下　ちと
			DrawDividedSprite(XMFLOAT2(1110, 480), 0.0f, XMFLOAT2(70, 70), 5, 5, g_coin_addition2 / 2, 1.0);
			break;
		default:
			break;
		}


		// シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_stage_select_coin_effect_Texture3);

		switch (g_coin_effect_random_number3)
		{
		case 1:
			//左上金貨　左下
			DrawDividedSprite(XMFLOAT2(140, 100), 0.0f, XMFLOAT2(70, 70), 5, 5, g_coin_addition3 / 2, 1.0);

			break;
		case 2:
			//左下錨
			DrawDividedSprite(XMFLOAT2(220, 550), 30.0f, XMFLOAT2(90, 90), 5, 5, g_coin_addition3 / 2, 1.0);

			break;
		case 3:
			//錨鎖　左上
			DrawDividedSprite(XMFLOAT2(30, 250), 0.0f, XMFLOAT2(50, 50), 5, 5, g_coin_addition3 / 2, 1.0);

			break;
		case 4:
			//錨鎖　した
			DrawDividedSprite(XMFLOAT2(30, 400), 0.0f, XMFLOAT2(50, 50), 5, 5, g_coin_addition3 / 2, 1.0);

			break;
		case 5:
			//左上金貨　左上
			DrawDividedSprite(XMFLOAT2(40, 30), 0.0f, XMFLOAT2(70, 70), 5, 5, g_coin_addition3 / 2, 1.0);

			break;
		case 6:
			//右上 がち右上
			DrawDividedSprite(XMFLOAT2(1180, 30), 0.0f, XMFLOAT2(70, 70), 5, 5, g_coin_addition3 / 2, 1.0);

			break;
		case 7:
			//右上　ちと右上
			DrawDividedSprite(XMFLOAT2(1050, 75), 0.0f, XMFLOAT2(70, 70), 5, 5, g_coin_addition3 / 2, 1.0);
			break;
		case 8:
			//右上　ちとちと右上
			DrawDividedSprite(XMFLOAT2(1100, 275), 0.0f, XMFLOAT2(70, 70), 5, 5, g_coin_addition3 / 2, 1.0);
			break;
		case 9:
			//右下　がち
			DrawDividedSprite(XMFLOAT2(1250, 680), 0.0f, XMFLOAT2(70, 70), 5, 5, g_coin_addition3 / 2, 1.0);
			break;

		case 10:
			//右下　ちと
			DrawDividedSprite(XMFLOAT2(1110, 480), 0.0f, XMFLOAT2(70, 70), 5, 5, g_coin_addition3 / 2, 1.0);
			break;
		default:
			break;
		}



		// シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_stage_select_coin_effect_Texture4);

		switch (g_coin_effect_random_number4)
		{
		case 1:
			//左上金貨　左下
			DrawDividedSprite(XMFLOAT2(140, 100), 0.0f, XMFLOAT2(70, 70), 5, 5, g_coin_addition4 / 2, 1.0);

			break;
		case 2:
			//左下錨
			DrawDividedSprite(XMFLOAT2(220, 550), 30.0f, XMFLOAT2(90, 90), 5, 5, g_coin_addition4 / 2, 1.0);

			break;
		case 3:
			//錨鎖　左上
			DrawDividedSprite(XMFLOAT2(30, 250), 0.0f, XMFLOAT2(50, 50), 5, 5, g_coin_addition4 / 2, 1.0);

			break;
		case 4:
			//錨鎖　した
			DrawDividedSprite(XMFLOAT2(30, 400), 0.0f, XMFLOAT2(50, 50), 5, 5, g_coin_addition4 / 2, 1.0);

			break;
		case 5:
			//左上金貨　左上
			DrawDividedSprite(XMFLOAT2(40, 30), 0.0f, XMFLOAT2(70, 70), 5, 5, g_coin_addition4 / 2, 1.0);

			break;
		case 6:
			//右上 がち右上
			DrawDividedSprite(XMFLOAT2(1180, 30), 0.0f, XMFLOAT2(70, 70), 5, 5, g_coin_addition4 / 2, 1.0);

			break;
		case 7:
			//右上　ちと右上
			DrawDividedSprite(XMFLOAT2(1050, 75), 0.0f, XMFLOAT2(70, 70), 5, 5, g_coin_addition4 / 2, 1.0);
			break;
		case 8:
			//右上　ちとちと右上
			DrawDividedSprite(XMFLOAT2(1100, 275), 0.0f, XMFLOAT2(70, 70), 5, 5, g_coin_addition4 / 2, 1.0);
			break;
		case 9:
			//右下　がち
			DrawDividedSprite(XMFLOAT2(1250, 680), 0.0f, XMFLOAT2(70, 70), 5, 5, g_coin_addition4 / 2, 1.0);
			break;

		case 10:
			//右下　ちと
			DrawDividedSprite(XMFLOAT2(1110, 480), 0.0f, XMFLOAT2(70, 70), 5, 5, g_coin_addition4 / 2, 1.0);
			break;
		default:
			break;
		}
		

		if (60 < g_coin_addition)
		{
			g_coin_addition = 0;
			g_coin_effect_random_number = 0;
		}

		if (50 < g_coin_addition1)
		{
			g_coin_addition1 = 0;
			g_coin_effect_random_number1 = 0;
		}

		if (50 < g_coin_addition2)
		{
			g_coin_addition2 = 0;
			g_coin_effect_random_number2 = 0;
		}

		if (50 < g_coin_addition3)
		{
			g_coin_addition3 = 0;
			g_coin_effect_random_number3 = 0;
		}

		if (50 < g_coin_addition4)
		{
			g_coin_addition4 = 0;
			g_coin_effect_random_number4 = 0;
		}


		//------------------------------------------------------------------------------------------------------


			// ステージポイントの描画
		m_stagePointFactory.Draw();



		//-------------------------------------------------------------------------------------------
		//ポップアップの描画

		b2Vec2 pos = m_player.GetTouchStagePointBody()->GetPosition();

		rate += 0.03;
		// イージングがかかった値を保存する変数
		double easingRate;
		easingRate = Ease::InCubic(rate);



		//---------------------------------------
		//ホップアップに表示する数字の管理
		int max_score = max_coin_cnt;//最大数の確保
		int now_score;

		

		if (1 < easingRate)
		{
			easingRate = 1;
		}
		
		switch (m_player.GetTouchStageSelectNum())
		{
		case 0:
			rate = 0;
			break;

		case 1:
			//----------------------------------------------------------------------------
			//チュートリアル


			// シェーダリソースを設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_stage_select_hopup_tutorial_Texture);

			DrawSpriteOld(
				XMFLOAT2(pos.x*SCALE,(pos.y*SCALE)-(195*easingRate)),
				0.0f,
				XMFLOAT2(300*easingRate, 230*easingRate),
				easingRate
			);


			if (0.9 < easingRate)
			{
				//--------------------------------------------------------------------------
				// 最大数の管理
				// -------------------------------------------------------------------------
				// シェーダリソースを設定
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_stage_select_number_Texture);

				for (int i = 0; i < 2; i++) {
					DrawDividedSprite(XMFLOAT2((pos.x * SCALE) + 92 + (-15 * i), (pos.y * SCALE) - 128), 0.0f, XMFLOAT2(15, 15), 10, 1, max_score);
					max_score /= 10;
				}

				//------------------------------------------------------------------------------
				//取得数管理
				//-----------------------------------------------------------------------------
				now_score = get_tutorial_coin_cnt;

				GetDeviceContext()->PSSetShaderResources(0, 1, &g_stage_select_number_Texture);

				for (int i = 0; i < 2; i++) {
					DrawDividedSprite(XMFLOAT2((pos.x * SCALE) + 45 + (-15 * i), (pos.y * SCALE) - 128), 0.0f, XMFLOAT2(15, 15), 10, 1, now_score);
					max_score /= 10;
				}

			}
			break;
		case 2:
		
			// シェーダリソースを設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_stage_select_hopup_1_1_Texture);


			DrawSpriteOld(
				XMFLOAT2(pos.x * SCALE, (pos.y * SCALE) + (195 * easingRate)),
				0.0f,
				XMFLOAT2(300 * easingRate, 230 * easingRate),
				easingRate
			);

			if (0.9 < easingRate)
			{
			//--------------------------------------------------------------------------
			// 最大数の管理
			// -------------------------------------------------------------------------
			// シェーダリソースを設定
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_stage_select_number_Texture);

				for (int i = 0; i < 2; i++) {
					DrawDividedSprite(XMFLOAT2((pos.x * SCALE) + 92 + (-15 * i), (pos.y * SCALE) + 270), 0.0f, XMFLOAT2(15, 15), 10, 1, max_score);
					max_score /= 10;
				}

				//------------------------------------------------------------------------------
				//取得数管理
				//-----------------------------------------------------------------------------
				now_score = get_tutorial_coin_cnt;

				GetDeviceContext()->PSSetShaderResources(0, 1, &g_stage_select_number_Texture);

				for (int i = 0; i < 2; i++) {
					DrawDividedSprite(XMFLOAT2((pos.x * SCALE) + 45 + (-15 * i), (pos.y * SCALE) + 270), 0.0f, XMFLOAT2(15, 15), 10, 1, now_score);
					max_score /= 10;
				}

			}

			break;
		case 3:
			
			// シェーダリソースを設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_stage_select_hopup_unknow_Texture);

			DrawSpriteOld(
				XMFLOAT2(pos.x * SCALE, (pos.y * SCALE) - (195 * easingRate)),
				0.0f,
				XMFLOAT2(300 * easingRate, 230 * easingRate),
				easingRate
			);


			if (0.9 < easingRate)
			{
				//--------------------------------------------------------------------------
				// 最大数の管理
				// -------------------------------------------------------------------------
				// シェーダリソースを設定
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_stage_select_number_Texture);

				for (int i = 0; i < 2; i++) {
					DrawDividedSprite(XMFLOAT2((pos.x * SCALE) + 92 + (-15 * i), (pos.y * SCALE) - 128), 0.0f, XMFLOAT2(15, 15), 10, 1, max_score);
					max_score /= 10;
				}

				//------------------------------------------------------------------------------
				//取得数管理
				//-----------------------------------------------------------------------------
				now_score = get_tutorial_coin_cnt;

				GetDeviceContext()->PSSetShaderResources(0, 1, &g_stage_select_number_Texture);

				for (int i = 0; i < 2; i++) {
					DrawDividedSprite(XMFLOAT2((pos.x * SCALE) + 45 + (-15 * i), (pos.y * SCALE) - 128), 0.0f, XMFLOAT2(15, 15), 10, 1, now_score);
					max_score /= 10;
				}

			}
			break;

		case 4:

			// シェーダリソースを設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_stage_select_hopup_unknow_Texture);

			DrawSpriteOld(
				XMFLOAT2(pos.x* SCALE, (pos.y* SCALE) - (195 * easingRate)),
				0.0f,
				XMFLOAT2(300 * easingRate, 230 * easingRate),
				easingRate
			);


			if (0.9 < easingRate)
			{
				//--------------------------------------------------------------------------
				// 最大数の管理
				// -------------------------------------------------------------------------
				// シェーダリソースを設定
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_stage_select_number_Texture);

				for (int i = 0; i < 2; i++) {
					DrawDividedSprite(XMFLOAT2((pos.x * SCALE) + 92 + (-15 * i), (pos.y * SCALE) - 128), 0.0f, XMFLOAT2(15, 15), 10, 1, max_score);
					max_score /= 10;
				}

				//------------------------------------------------------------------------------
				//取得数管理
				//-----------------------------------------------------------------------------
				now_score = get_tutorial_coin_cnt;

				GetDeviceContext()->PSSetShaderResources(0, 1, &g_stage_select_number_Texture);

				for (int i = 0; i < 2; i++) {
					DrawDividedSprite(XMFLOAT2((pos.x * SCALE) + 45 + (-15 * i), (pos.y * SCALE) - 128), 0.0f, XMFLOAT2(15, 15), 10, 1, now_score);
					max_score /= 10;
				}

			}
			break;
		default:
			break;
		}


	



		//-----------------------------------------------------------------------------//
		//タップした時にでるエフェクト
		// シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_stage_select_tap_effect_Texture);
		if (g_tap_addition != 0)
		{


			b2Vec2 pos = m_player.GetBody()->GetPosition();
			g_tap_addition++;


		
			DrawDividedSprite(XMFLOAT2((pos.x*SCALE)-20, (pos.y*SCALE)-20),g_tap_effect_angle, XMFLOAT2(100, 100), 6, 3 ,g_tap_addition/2, 2.0);

			if (32 < g_tap_addition)
			{
				g_tap_addition = 0;
				m_player.SetTapTextureFlag(false);
			}

		}
		//----------------------------------------------------------------------------------//

	
		m_player.Draw();

		double fadeeasingRate=0;
		if (fade_rate != 0)
		{
			fade_rate += 0.03;
			fadeeasingRate = Ease::InCubic(fade_rate);
			if (1 < fadeeasingRate)
			{
				fadeeasingRate = 1;
				
			}
			
			// シェーダリソースを設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_stage_select_fade_black_Texture);

			DrawSpriteOld(
				XMFLOAT2(pos.x* SCALE, pos.y* SCALE),
				0.0f,
				XMFLOAT2(2000-(2000*fadeeasingRate), 2000-(2000 * fadeeasingRate))
			);
			DrawSpriteOld(
				XMFLOAT2(pos.x* SCALE, pos.y* SCALE),
				0.0f,
				XMFLOAT2(2000 , 2000)
			);

			if (0.9 < fade_rate)
			{
				// シェーダリソースを設定
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_stage_select_black_Texture);
				DrawSpriteOld(
					XMFLOAT2(pos.x * SCALE, pos.y * SCALE),
					0.0f,
					XMFLOAT2(2000, 2000)
				);
			}
		}

		//----------------------------------------------------------------------------------------------

		//バックバッファ、フロントバッファ入れ替え
		Present();
	
}

void StageSelectScene::Finalize()
{
	StageSelectPlayer& m_player = StageSelectPlayer::GetInstance();
	m_player.Finalize();
	m_stagePointFactory.Finalize();

	if (g_stage_select_background_Texture) UnInitTexture(g_stage_select_background_Texture);

	if (g_stage_select_tap_effect_Texture) UnInitTexture(g_stage_select_tap_effect_Texture);

	if (g_stage_select_coin_effect_Texture) UnInitTexture(g_stage_select_coin_effect_Texture);
	if (g_stage_select_coin_effect_Texture1) UnInitTexture(g_stage_select_coin_effect_Texture1);
	if (g_stage_select_coin_effect_Texture2) UnInitTexture(g_stage_select_coin_effect_Texture2);
	if (g_stage_select_coin_effect_Texture3) UnInitTexture(g_stage_select_coin_effect_Texture3);
	if (g_stage_select_coin_effect_Texture4) UnInitTexture(g_stage_select_coin_effect_Texture4);

	if (g_stage_select_hopup_tutorial_Texture) UnInitTexture(g_stage_select_hopup_tutorial_Texture);
	if (g_stage_select_hopup_1_1_Texture) UnInitTexture(g_stage_select_hopup_1_1_Texture);
	if (g_stage_select_hopup_unknow_Texture) UnInitTexture(g_stage_select_hopup_unknow_Texture);

	if (g_stage_select_number_Texture) UnInitTexture(g_stage_select_number_Texture);

	if (g_stage_select_fade_black_Texture) UnInitTexture(g_stage_select_fade_black_Texture);
	if (g_stage_select_black_Texture) UnInitTexture(g_stage_select_black_Texture);

	
	// ワールド解放
	if (m_world) {
		DestroyWorld(m_world);
	}

	
}


void DestroyWorld(b2World*& world) {
	if (!world) return;

	// ワールド内のすべてのボディを削除
	b2Body* body = world->GetBodyList();
	while (body) {
		b2Body* nextBody = body->GetNext();
		world->DestroyBody(body);
		body = nextBody;
	}

	// ワールド自体を解放
	delete world;
	world = nullptr;
}