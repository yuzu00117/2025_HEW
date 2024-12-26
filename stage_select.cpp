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


constexpr float SCALE = 30.0f; // ピクセルからメートルへの変換スケール
//テクスチャのダウンロード グローバル変数にしてる
ID3D11ShaderResourceView* g_stage_select_Texture = NULL;

// メンバ変数として保持

StagePointFactory m_stagePointFactory;



void StageSelectScene::Initialize()
{
	g_stage_select_Texture = InitTexture(L"asset\\texture\\sample_texture\\sample_stage_select.png");


	//ワールドをつくる
	 // Box2Dワールドの作成
	b2Vec2 gravity(0.0f,0.0f); // 重力なし
	m_world = new b2World(gravity);

	
	StageSelectPlayer& m_player = StageSelectPlayer::GetInstance();
	//プレイヤーの定義
	m_player.Initialize(m_world, 640, 300);

	//初期のポジションを定義
	m_player.SetTouchStagePointBody(m_player.GetBody());

	// ステージポイントの生成
	m_stagePointFactory.CreateStagePoint(m_world, 400.0f, 560.0f, 40.0f,1);
	m_stagePointFactory.CreateStagePoint(m_world, 1000.0f, 360.0f, 40.0f,2);
	m_stagePointFactory.CreateStagePoint(m_world, 1200.0f, 360.0f, 40.0f,3);

	StageSelectContactListener& listener = StageSelectContactListener::GetInstance();
	m_world->SetContactListener(&listener);


	
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
	
	

		switch (m_player.GetTouchStageSelectNum())
		{
		case 0:
			break;

		case 1:
			if (Keyboard_IsKeyDown(KK_SPACE) || (state.buttonA))
			{
				SceneManager& sceneManager = SceneManager::GetInstance();
				sceneManager.ChangeScene(SCENE_GAME);
			};
			break;

		case 2:
			if (Keyboard_IsKeyDown(KK_SPACE) || (state.buttonA))
			{
				SceneManager& sceneManager = SceneManager::GetInstance();
				sceneManager.ChangeScene(SCENE_GAME);
			};
			break;

		case 3:
			if (Keyboard_IsKeyDown(KK_SPACE) || (state.buttonA))
			{
				SceneManager& sceneManager = SceneManager::GetInstance();
				sceneManager.ChangeScene(SCENE_GAME);
			};
			break;

		}


	
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

		StageSelectPlayer& m_player = StageSelectPlayer::GetInstance();
		m_player.Draw();


		// ステージポイントの描画
		m_stagePointFactory.Draw();

		b2Vec2 pos = m_player.GetTouchStagePointBody()->GetPosition();

		switch (m_player.GetTouchStageSelectNum())
		{
		case 0:
			break;

		case 1:
			
			// シェーダリソースを設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_stage_select_Texture);

			DrawSpriteOld(
				XMFLOAT2(pos.x*SCALE,(pos.y*SCALE)-100),
				0.0f,
				XMFLOAT2(200, 200)
			);


			break;
		case 2:
		
			// シェーダリソースを設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_stage_select_Texture);

			DrawSpriteOld(
				XMFLOAT2(pos.x * SCALE, (pos.y * SCALE) - 100),
				0.0f,
				XMFLOAT2(200, 200)
			);

			break;
		case 3:
			
			// シェーダリソースを設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_stage_select_Texture);

			DrawSpriteOld(
				XMFLOAT2(pos.x * SCALE, (pos.y * SCALE) - 100),
				0.0f,
				XMFLOAT2(200, 200)
			);

			break;
		default:
			break;
		}

		//バックバッファ、フロントバッファ入れ替え
		Present();
	
}

void StageSelectScene::Finalize()
{
	StageSelectPlayer& m_player = StageSelectPlayer::GetInstance();
	m_player.Finalize();
	/*m_stagePointFactory.Finalize();*/


	UnInitTexture(g_stage_select_Texture);
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