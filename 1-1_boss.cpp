//-----------------------------------------------------------------------------------------------------
// #name 1-1_boss.cpp
// #description bossの動作を管理する
// #make 2025/01/07　　作成者
// #update 2025/01/21
// #comment 追加・修正
//          ・修正内容
//----------------------------------------------------------------------------------------------------
#include "1-1_boss.h"
#include "include/box2d/box2d.h"
#include "texture.h"
#include "sprite.h"
#include "main.h"
#include "world_box2d.h"
#include "create_filter.h"
#include "player_position.h"
#include "display.h"
#include "collider_type.h"
#include "keyboard.h"
#include "1_1boss_state_debug.h"
#include "scene.h"
#include "anchor.h"
#include "camera_shake.h"
#include "hit_stop.h"
#include "Item_Spirit.h"

// 使用するテクスチャファイルの定義
static ID3D11ShaderResourceView *g_mini_boss_Texture = NULL;				 // ミニゴーレムのテクスチャ
static ID3D11ShaderResourceView *g_boss_shock_wave_sheet1_Texture = NULL;	 // 衝撃波のテクスチャ1
static ID3D11ShaderResourceView *g_boss_shock_wave_sheet2_Texture = NULL;	 // 衝撃波のテクスチャ2
static ID3D11ShaderResourceView *g_boss_charge_attack_sheet1_Texture = NULL; // 突進攻撃のテクスチャ1
static ID3D11ShaderResourceView *g_boss_charge_attack_sheet2_Texture = NULL; // 突進攻撃のテクスチャ2

static ID3D11ShaderResourceView *g_boss_walk_sheet1_Texture = NULL; // ゴーレムの歩行アニメーション1
static ID3D11ShaderResourceView *g_boss_walk_sheet2_Texture = NULL; // ゴーレムの歩行アニメーション2
static ID3D11ShaderResourceView *g_boss_jump_sheet1_Texture = NULL; // ゴーレムのジャンプアニメーション1
static ID3D11ShaderResourceView *g_boss_jump_sheet2_Texture = NULL; // ゴーレムのジャンプアニメーション2
static ID3D11ShaderResourceView *g_boss_panic_sheet_Texture = NULL; // ゴーレムのパニックアニメーション

static ID3D11ShaderResourceView *g_mini_boss_create_sheet1_Texture = NULL; // ミニゴーレム生成時のボスアニメーション1
static ID3D11ShaderResourceView *g_mini_boss_create_sheet2_Texture = NULL; // ミニゴーレム生成時のボスアニメーション2

// ボス周辺のエフェクトのテクスチャ
static ID3D11ShaderResourceView *g_boss_charge_effect = NULL;		 // ボスの突進エフェクト
static ID3D11ShaderResourceView *g_boss_charge_attack_effect = NULL; // ボスの突進攻撃エフェクト
static ID3D11ShaderResourceView *g_mini_golem_break_effect = NULL;	 // ミニゴーレムが破壊された時のエフェクト
static ID3D11ShaderResourceView *g_boss_panic_effect = NULL;		 // ボスがパニックになった時のエフェクト
static ID3D11ShaderResourceView *g_boss_shock_wave_effect = NULL;	 // ボスの衝撃波エフェクト
static ID3D11ShaderResourceView *g_boss_down_sheet = NULL;			 // ボスのダウン状態
//-------------------------------------------------------------------------------------------
// デバッグ用の画像
static ID3D11ShaderResourceView *g_debug_color = NULL; // デバッグ用

static ID3D11ShaderResourceView *g_debug_boss_body_color = NULL; // デバッグ用

static ID3D11ShaderResourceView *g_debug_attack_color = NULL; // デバッグ用

static ID3D11ShaderResourceView *g_debug_core = NULL; // デバッグ用

// ボスのCPPファイルの実装
b2Body *outside_boss_body;

Boss_1_1::Boss_1_1()
{
}

Boss_1_1::~Boss_1_1()
{
}

void Boss_1_1::Initialize(b2Vec2 position, b2Vec2 bodysize, bool left)
{
	// ボディのサイズ
	bodysize.x = bodysize.x * BOSS_SIZE_SCALE;
	bodysize.y = bodysize.y * BOSS_SIZE_SCALE;

	if (g_mini_boss_create_sheet1_Texture == NULL)
	{
		g_mini_boss_Texture = InitTexture(L"asset\\texture\\boss_1_1\\mini_boss.png");									  // ミニゴーレムのインクルード
		g_boss_shock_wave_sheet1_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_shock_wave_sheet1.png");		  // 衝撃波のインクルードシート1
		g_boss_shock_wave_sheet2_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_shock_wave_sheet2.png");		  // 衝撃波のインクルードシート2
		g_boss_charge_attack_sheet1_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_charge_sheet1.png");			  // 突進攻撃のシート1
		g_boss_charge_attack_sheet2_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_charge_sheet2_.png");		  // 突進攻撃のシート2
		g_mini_boss_create_sheet1_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_create_mini_golem_sheet1.png");  // ミニゴーレムの生成ボスシート1
		g_mini_boss_create_sheet2_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_create_mini_golem_sheet2_.png"); // ミニゴーレムの生成ボスシート2
		g_boss_walk_sheet1_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_walk_sheet1.png");					  // ゴーレムの歩行アニメーション1
		g_boss_walk_sheet2_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_walk_sheet2.png");					  // ゴーレムの歩行アニメーション2
		g_boss_jump_sheet1_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_jump_new_sheet1.png");				  // ゴーレムのジャンプアニメーション1
		g_boss_jump_sheet2_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_jump_new_sheet2.png");				  // ゴーレムのジャンプアニメーション2
		g_boss_panic_sheet_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_panic_sheet1.png");					  // ゴーレムのパニックアニメーション
		g_boss_down_sheet			=InitTexture(L"asset\\texture\\boss_1_1\\boss_down_sheet.png");					  // ゴーレムのダウンアニメーション
		
		
		// エフェクト
		g_boss_charge_attack_effect = InitTexture(L"asset\\texture\\boss_1_1\\boss_charge_attack_effect.png"); // ボスのチャージアタックエフェクト
		g_boss_charge_effect = InitTexture(L"asset\\texture\\boss_1_1\\boss_charge_effect.png");			   // ボスのチャージエフェクト
		g_boss_panic_effect = InitTexture(L"asset\\texture\\boss_1_1\\boss_panic_effect.png");				   // ボスのパニックエフェクト
		g_boss_shock_wave_effect = InitTexture(L"asset\\texture\\boss_1_1\\boss_shock_wave_effect.png");	   // ボスのショックウェーブエフェクト
		g_mini_golem_break_effect = InitTexture(L"asset\\texture\\boss_1_1\\mini_golem_break_effect.png");	   // ボスのミニゴーレムブレイクエフェクト

		// デバッグ用
		g_debug_color = InitTexture(L"asset\\texture\\sample_texture\\img_sensor.png"); // sensorのテクスチャ
		g_debug_boss_body_color = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_blue.png");
		g_debug_attack_color = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_red.png");
		g_debug_core = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_blue.png");

		InitializeBossDebug(); // デバッグ用の初期化
	}
	Box2dWorld &box2d_world = Box2dWorld::GetInstance();
	b2World *world = box2d_world.GetBox2dWorldPointer();

	// ボディが存在する場合は削除
	if (GetBossBody() != nullptr)
	{
		world->DestroyBody(GetBossBody());
		SetBossBody(nullptr);
	}

	// ボディのサイズを設定
	SetBossDrawSize(bodysize);

	// 実際のボスのサイズ
	bodysize.x = bodysize.x * 0.5;
	bodysize.y = bodysize.y * 0.4;

	SetBossRealSize(bodysize);

	// bossのサイズ
	b2Vec2 size; // サイズのスケールを設定
	size.x = bodysize.x / BOX2D_SCALE_MANAGEMENT;
	size.y = bodysize.y / BOX2D_SCALE_MANAGEMENT;

	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(position.x, position.y);
	body.angle = 0.0f;
	body.fixedRotation = true; // 回転を固定する
	body.userData.pointer = (uintptr_t)this;

	// ワールドに追加
	b2Body *m_boss_body = world->CreateBody(&body);

	SetBossBody(m_boss_body);
	outside_boss_body = m_boss_body; // 外部からの参照用

	b2PolygonShape body_shape;
	body_shape.SetAsBox(size.x * 0.5, size.y * 0.5);

	b2FixtureDef body_fixture;
	body_fixture.shape = &body_shape;
	body_fixture.friction = 0.3f;	 // 摩擦
	body_fixture.restitution = 0.1f; // 反発係数
	body_fixture.density = 0.1f;
	body_fixture.isSensor = false; // センサーかどうか
	body_fixture.filter = createFilterExclude("Boss_filter", {});

	b2Fixture *m_body_fixture = m_body->CreateFixture(&body_fixture);

	ObjectData *boss_body_data = new ObjectData{collider_boss};
	m_body_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(boss_body_data);

	// センサーのサイズを設定
	SetBossSensorSize(b2Vec2(bodysize.x * 2.0, bodysize.y));

	b2Vec2 sensor_size;
	sensor_size.x = bodysize.x / BOX2D_SCALE_MANAGEMENT * 2.0;
	sensor_size.y = bodysize.y / BOX2D_SCALE_MANAGEMENT;

	b2FixtureDef sensor_fixture;
	b2PolygonShape sensor_shape;

	b2Vec2 sensor_vertices[4] = {b2Vec2(0.0f, 0.0f)};

	sensor_shape.SetAsBox(sensor_size.x * 0.5, sensor_size.y * 0.5);

	sensor_fixture.shape = &sensor_shape;
	sensor_fixture.friction = 0.0f;	   // 摩擦
	sensor_fixture.restitution = 0.1f; // 反発係数
	sensor_fixture.density = 0.1f;
	sensor_fixture.isSensor = true; // センサーかどうか

	b2Fixture *m_sensor_fixture = m_body->CreateFixture(&sensor_fixture);

	ObjectData *boss_sensor_data = new ObjectData{collider_boss_senosr};
	m_sensor_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(boss_sensor_data);

	boss_field_level = 1;
}

void Boss_1_1::Update()
{
	if (m_body != nullptr)
	{
		//---------------------------------------------------------------------------------------------------------------------------
		// プレイヤーの位置を取得
		float player_x = PlayerPosition::GetPlayerPosition().x;
		if (player_x < m_body->GetPosition().x) // 左にいる
		{
			left_flag = true;
		}
		else
		{
			left_flag = false;
		}

		//-------------------------------------------------------------------------------------------

		// デバッグ用の更新
		UpdateBossDebug();

		// 衝撃波の更新
		ShockWaveUpdate();

		// ミニゴーレムの更新
		MiniGolemUpdate();
		// ミニゴーレムの破壊
		DestroyMiniGolemBody();
		// ジャンプの更新
		JumpUpdate();

		// ボスのコアの更新
		BossCoreUpdate();

		// ボスの死亡処理
		BossDead();

		// クールタイムの管理
		UpdateCoolTime();

		// エフェクトの管理
		UpdateEffectSheetCnt();

		// ボスの近くにプレイヤーがいるかどうかの判定
		if (GetPlayerNearbylocked() != 0)
		{
			SetPlayerNearbylocked(GetPlayerNearbylocked() - 1);
		}

		//-------------------------------------------------------------------------------------------
		// デバッグ用　ボタン押下でレベルアップ

		// アンカーポイントのボディを削除するデバッグ用
		if (Keyboard_IsKeyDown(KK_Y) && debug_flag == 0)
		{
			debug_flag = 60;
			boss_field_level++;
		}
		if (debug_flag != 0)
		{
			debug_flag--;
		}

		//-------------------------------------------------------------------------------------------

		switch (now_boss_state)
		{
		case wait_state:

			sheet_cnt = 0;

			break;
		case panic_state:

			if (50 < sheet_cnt)
			{
				CreateBossCore(b2Vec2(2.0f * BOSS_SIZE_SCALE, 2.0f * BOSS_SIZE_SCALE));
			}

			sheet_cnt += 0.5;

			if (Max_Panic_Stun_Frame <= sheet_cnt)
			{
				sheet_cnt = 0;
				CoreDeleteFlag = true;
			}

			break;

		case down_state:

			sheet_cnt += 0.5;

			if (Max_Down_Frame <= sheet_cnt)
			{
				sheet_cnt = 0;
				now_boss_state = wait_state;
			}
			break;
		case walk_state:
			if (static_cast<int>(sheet_cnt) % 10 == 0)
			{

				if (left_flag)
				{
					m_body->SetLinearVelocity(b2Vec2(-walk_power.x, walk_power.y));
				}
				else
				{
					m_body->SetLinearVelocity(b2Vec2(walk_power.x, walk_power.y));
				}

				if ((static_cast<int>(sheet_cnt) % 20 == 0))
				{
					m_body->SetLinearVelocity(b2Vec2_zero);
				}

				// カメラシェイクスタート
				CameraShake::StartCameraShake(1, 0, 10);
			}
			sheet_cnt += 0.5;

			if (Max_Walk_Sheet <= sheet_cnt)
			{
				sheet_cnt = 0;
				now_boss_state = wait_state;
			}

			break;

		case jump_state:
			sheet_cnt += 0.6;

			if (Max_Jump_Sheet <= sheet_cnt)
			{
				sheet_cnt = 0;

				// ジャンプの後に止まるかどうかを X軸の速度だけYはそのまま
				b2Body *body = GetBossBody();
				b2Vec2 velocity = body->GetLinearVelocity();
				body->SetLinearVelocity(b2Vec2(0.0f, velocity.y));

				// カメラシェイクスタート
				CameraShake::StartCameraShake(120, 00, 20);
				HitStop::SetHitStopFlag(5);

				now_boss_state = wait_state;
			}

			break;
		case shock_wave_state:
			sheet_cnt += 0.5;

			if (static_cast<int>(sheet_cnt) == Shock_Wave_Start_Frame)
			{
				CreateShockWave(b2Vec2(5.0f * BOSS_SIZE_SCALE, 6.0f * BOSS_SIZE_SCALE), left_flag);
				Shock_Wave_Fly_flag = true;

				// エフェクトスタート
				shock_wave_effect_sheet_cnt = 1;
			}

			if (Max_Shock_Wave_Sheet <= sheet_cnt)
			{
				sheet_cnt = 0;
				now_boss_state = wait_state;
			}

			break;
		case create_mini_golem_state:
			sheet_cnt += 0.5;

			if (static_cast<int>(sheet_cnt) == Create_Mini_Golem_Start_Frame)
			{
				CreateMiniGolem(b2Vec2(3.0f * BOSS_SIZE_SCALE, 2.0f * BOSS_SIZE_SCALE), left_flag); // 画像の都合で大きさを変えるため　生成時の位置はそのまま　実際の大きさは参照用
			}
			if (Max_Create_Mini_Golem_Sheet <= sheet_cnt)
			{
				Mini_golem_Create_flag = true; // ミニゴーレムのクリエイトフラグの管理
				sheet_cnt = 0;
				now_boss_state = wait_state;
			}

			break;
		case charge_attack_state:

			// シート1枚目はチャージを行う
			if (sheet_cnt < 100)
			{
				sheet_cnt += 0.75;
			}
			else // シート2枚目はチャージを行う速度
			{
				sheet_cnt += 0.5;
			}

			if (static_cast<int>(sheet_cnt) == Charge_Attack_Start_Frame)
			{
				CreateChargeAttack(b2Vec2(4.0f * BOSS_SIZE_SCALE, 4.0f * BOSS_SIZE_SCALE), left_flag);
				// エフェクトスタート
				charge_attack_effect_sheet_cnt = 1;
			}
			if (static_cast<int>(sheet_cnt) == Charge_Attack_End_Frame)
			{
				DeleteAttackBody();
			}

			// アニメーションの終了
			if (Max_Charge_Attack_Sheet <= sheet_cnt)
			{
				sheet_cnt = 0;
				now_boss_state = wait_state;
			}
			break;

		default:
			break;
		}
	}
}

void Boss_1_1::UpdateCoolTime(void)
{
	Now_Shock_Wave_CoolTime++;		 // 衝撃波のインクルメント
	Now_Jump_CoolTime++;			 // ジャンプのインクルメント
	Now_Create_MiniGolem_CoolTime++; // ミニゴーレムのインクルメント
	Now_Charge_Attack_CoolTime++;	 // チャージアタックのインクルメント
	Now_Max_Walk_CoolTime++;		 // 歩行アニメーション

	// ボスの近くにプレイヤーがいるかどうかの判定
	if (now_boss_state == wait_state)
	{
		if (Player_is_Nearby == true)
		{
			// 近くにいる場合

			// チャージアタック
			if (Now_Charge_Attack_CoolTime > Max_Charge_Attack_CoolTime)
			{
				now_boss_state = charge_attack_state;
				Now_Charge_Attack_CoolTime = 0;
				return;
			}

			// ジャンプ
			if (Now_Jump_CoolTime > Max_Jump_CoolTime)
			{
				now_boss_state = jump_state;
				Now_Jump_CoolTime = 0;
				return;
			}
		}
		else
		{
			// 遠くにいる場合

			// 衝撃波
			if (Now_Shock_Wave_CoolTime > Max_Shock_Wave_CoolTime)
			{
				now_boss_state = shock_wave_state;
				Now_Shock_Wave_CoolTime = 0;
				return;
			}

			// ミニゴーレム生成
			if (Now_Create_MiniGolem_CoolTime > Max_Create_MiniGolem_CoolTime)
			{
				now_boss_state = create_mini_golem_state;
				Now_Create_MiniGolem_CoolTime = 0;
				return;
			}

			// 何もしていない場合は歩行アニメーション
			if (wait_state == now_boss_state)
			{
				now_boss_state = walk_state;
			}
		}
	}
}

void Boss_1_1::BossDamaged(void)
{
	// ボスのHPを減らす処理
	SetBossHP(GetBossHP() - 1);
}

void Boss_1_1::BossDead(void)
{
	// ボスのHPが0以下になったらシーンを変更
	if (boss_hp <= 0)
	{
		SceneManager &sceneManager = SceneManager::GetInstance();
		sceneManager.ChangeScene(SCENE_RESULT);
	}
}

void Boss_1_1::BossCoreUpdate()
{
	if (CoreDeleteFlag == true)
	{
		DestroyBossCore(); // ボスのコアを破壊

		sheet_cnt = 0; // シートカウントをリセット
		CoreDeleteFlag = false;
	}
}

void Boss_1_1::CreateBossCore(b2Vec2 size)
{
	if (GetAnchorPointBody() == nullptr)
	{
		// サイズを設定
		SetAnchorPointSize(size);

		b2Vec2 anchorpoint_size;

		anchorpoint_size.x = size.x / BOX2D_SCALE_MANAGEMENT;
		anchorpoint_size.y = size.y / BOX2D_SCALE_MANAGEMENT;

		int left = -1;
		if (left_flag == true) // 右だったら加算する
		{
			left = 1;
		}

		// ボスの大きさを把握
		b2Vec2 boss_size;
		boss_size.x = reality_boss_size.x / BOX2D_SCALE_MANAGEMENT;
		boss_size.y = reality_boss_size.y / BOX2D_SCALE_MANAGEMENT;

		b2Vec2 position = m_body->GetPosition();

		b2BodyDef anchor_point_body;
		anchor_point_body.type = b2_dynamicBody;																   // 動的なオブジェクトにする
		anchor_point_body.position.Set(position.x + (boss_size.x * left * 0.2), position.y + (boss_size.y * 0.2)); // ポジションを設定
		anchor_point_body.angle = 0;																			   // 角度の初期化
		anchor_point_body.userData.pointer = (uintptr_t)this;													   // userDataのポインタを設定
		anchor_point_body.fixedRotation = true;																	   // 回転を固定する

		Box2dWorld &box2d_world = Box2dWorld::GetInstance(); // ワールドのインスタンスを取得する
		b2World *world = box2d_world.GetBox2dWorldPointer(); // ワールドのポインタを取得する

		b2Body *m_anchor_point_body = world->CreateBody(&anchor_point_body); // Bodyをワールドに追加

		SetAnchorPointBody(m_anchor_point_body);

		b2PolygonShape shape;												  // shapeには四角形を設定する
		shape.SetAsBox(anchorpoint_size.x * 0.5f, anchorpoint_size.y * 0.5f); // 四角形を設定する

		b2FixtureDef fixture;
		fixture.shape = &shape;		// シェイプをフィクスチャに設定する
		fixture.density = 1.0f;		// 密度
		fixture.friction = 0.05f;	// 摩擦
		fixture.restitution = 0.0f; // 反発係数
		fixture.isSensor = false;	// センサーかどうか

		b2Fixture *ground_fixture = m_anchor_point_body->CreateFixture(&fixture); // Bodyにフィクスチャを設定する

		// カスタムデータを作成して設定
		// 名前に値を設定
		// 名前にユーザーデータを設定
		ObjectData *data = new ObjectData{collider_anchor_point};
		ground_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(data);

		data->need_anchor_level = 1;

		data->object_name = Boss_core;

		// プレイヤーとジョイントする
		b2WeldJointDef jointDef;
		jointDef.bodyA = m_body;			   // ボスのボディ
		jointDef.bodyB = GetAnchorPointBody(); // ボスのアンカーポイントのボディ

		// ボス側
		jointDef.localAnchorA.Set(boss_size.x * 0.2 * left, boss_size.y * 0.2);
		// アンカーポイント側
		jointDef.localAnchorB.Set(0.0f, 0.0f);

		jointDef.collideConnected = true; // ジョイント同士の衝突を無効にする

		world->CreateJoint(&jointDef); // ワールドにジョイントを追加

		// エフェクトスタート
		panic_effect_sheet_cnt = 1;

		// カメラシェイクスタート
		CameraShake::StartCameraShake(40, 20, 40);
		HitStop::SetHitStopFlag(10);

		b2Vec2 vec = m_body->GetLinearVelocity();

		m_body->SetLinearVelocity(b2Vec2(0.0f, vec.y));
	}
}

void Boss_1_1::DestroyBossCore(void)
{
	// アンカーポイントボディを削除
	if (GetAnchorPointBody() != nullptr)
	{
		Box2dWorld &box2d_world = Box2dWorld::GetInstance(); // ワールドのインスタンスを取得する
		b2World *world = box2d_world.GetBox2dWorldPointer(); // ワールドのポインタを取得する

		world->DestroyBody(GetAnchorPointBody());

		// テクスチャをリセット
		panic_effect_sheet_cnt = 0;

		//最初の行動はチャージ攻撃にしたい
		Now_Charge_Attack_CoolTime += 1500;

		if (CorePullingFlag == true)
		{
			//ダウン状態に移行
			now_boss_state = down_state;
		}
		else
		{
			//ウェイト状態に移行
			now_boss_state = wait_state;
		}

		//フラグをリセット
		CorePullingFlag = false;

		//チャージ攻撃
		Now_Charge_Attack_CoolTime = 1500;

		// nullに設定
		SetAnchorPointBody(nullptr);
	}
}

void Boss_1_1::JumpUpdate()
{
	if (now_boss_state == jump_state)
	{
		if (Jump_Start_Frame <= sheet_cnt && sheet_cnt <= Jump_End_Frame)
		{
			b2Body *boss_body = GetBossBody();

			int minus_flag = 1;

			if (left_flag)
			{
				minus_flag = -1;
			}

			boss_body->SetLinearVelocity(b2Vec2(minus_flag * JumpPower.x, JumpPower.y));
		}
	}
}

void Boss_1_1::CreateChargeAttack(b2Vec2 attack_size, bool left)
{
	if (GetAttackBody() == nullptr)
	{

		// ボディのサイズを設定
		SetAttackDrawSize(attack_size);

		b2Vec2 size; // サイズのスケールを設定
		size.x = attack_size.x / BOX2D_SCALE_MANAGEMENT;
		size.y = attack_size.y / BOX2D_SCALE_MANAGEMENT;

		b2BodyDef body;
		body.type = b2_dynamicBody;

		b2Vec2 boss_pos = m_body->GetPosition();
		b2Vec2 boss_size = b2Vec2(GetBossRealSize().x / BOX2D_SCALE_MANAGEMENT, GetBossRealSize().y / BOX2D_SCALE_MANAGEMENT);

		if (left)
		{
			body.position.Set(boss_pos.x - (boss_size.x / 3) - (size.x / 2), boss_pos.y + boss_size.y / 2 - size.y / 2);
		}
		else
		{
			body.position.Set(boss_pos.x + (boss_size.x / 3) + (size.x / 2), boss_pos.y + boss_size.y / 2 - size.y / 2);
		}
		body.angle = 0.0f;
		body.fixedRotation = true; // 回転を固定する
		body.userData.pointer = (uintptr_t)this;

		Box2dWorld &box2d_world = Box2dWorld::GetInstance();
		b2World *world = box2d_world.GetBox2dWorldPointer();

		// ワールドに追加
		b2Body *m_attack_body = world->CreateBody(&body);

		SetAttackBody(m_attack_body);

		// 攻撃のフィクスチャ
		b2FixtureDef fixture;

		// クラスから b2Shape を継承した場合の設定
		b2PolygonShape shape; // クラスのメンバー変数として継承
		shape.SetAsBox(size.x * 0.5, size.y * 0.5);

		fixture.shape = &shape;		// 設定
		fixture.density = 0.1f;		// 密度
		fixture.friction = 0.0f;	// 摩擦
		fixture.restitution = 0.0f; // 反発係数
		fixture.isSensor = true;	// センサーかどうか

		b2Fixture *m_fixture = m_attack_body->CreateFixture(&fixture);

		ObjectData *boss_attack_data = new ObjectData{collider_chage_attack};
		m_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(boss_attack_data);

		// プレイヤーとジョイントする
		b2WeldJointDef jointDef;
		jointDef.bodyA = m_body;		  // ボスのボディ
		jointDef.bodyB = GetAttackBody(); // ボスのアンカーポイントのボディ

		if (left) // 左かどうか
		{
			// ボス側
			jointDef.localAnchorA.Set((-boss_size.x / 3), boss_size.y / 2 - size.y / 2);
			// アンカーポイント側
			jointDef.localAnchorB.Set((size.x * 0.5), 0.0f);
		}
		else // 右
		{
			// ボス側
			jointDef.localAnchorA.Set((boss_size.x / 3), boss_size.y / 2 - size.y / 2);
			// アンカーポイント側
			jointDef.localAnchorB.Set((-size.x * 0.5), 0.0f);
		}
		jointDef.collideConnected = true; // ジョイント同士の衝突を無効にする

		world->CreateJoint(&jointDef); // ワールドにジョイントを追加

		// アンカーを追加
		boss_field_level++;

		// カメラシェイクスタート
		CameraShake::StartCameraShake(40, 0, 60);
		HitStop::SetHitStopFlag(15);
	}
}

void Boss_1_1::CreateShockWave(b2Vec2 attack_size, bool left)
{
	if (GetAttackBody() == nullptr)
	{

		// ボディのサイズを設定
		SetAttackDrawSize(attack_size);

		b2Vec2 size; // サイズのスケールを設定
		size.x = attack_size.x / BOX2D_SCALE_MANAGEMENT;
		size.y = attack_size.y / BOX2D_SCALE_MANAGEMENT;

		b2BodyDef body;
		body.type = b2_dynamicBody;
		body.gravityScale = (0.0f); // 重力の影響を受けない

		b2Vec2 boss_pos = m_body->GetPosition();
		b2Vec2 boss_size = b2Vec2(GetBossRealSize().x / BOX2D_SCALE_MANAGEMENT, GetBossRealSize().y / BOX2D_SCALE_MANAGEMENT);

		if (left)
		{
			body.position.Set(boss_pos.x - (boss_size.x / 3) - (size.x / 2), boss_pos.y + boss_size.y / 2 - size.y / 2);
			ShockWaveLeftFlag = true;
		}
		else
		{
			body.position.Set(boss_pos.x + (boss_size.x / 3) + (size.x / 2), boss_pos.y + boss_size.y / 2 - size.y / 2);
			ShockWaveLeftFlag = false;
		}
		body.angle = 0.0f;
		body.fixedRotation = true; // 回転を固定する
		body.userData.pointer = (uintptr_t)this;

		Box2dWorld &box2d_world = Box2dWorld::GetInstance();
		b2World *world = box2d_world.GetBox2dWorldPointer();

		// ワールドに追加
		b2Body *m_attack_body = world->CreateBody(&body);

		SetAttackBody(m_attack_body);

		// 攻撃のフィクスチャ
		b2FixtureDef fixture;

		// クラスから b2Shape を継承した場合の設定
		b2PolygonShape shape; // クラスのメンバー変数として継承
		shape.SetAsBox(size.x * 0.5, size.y * 0.5);

		fixture.shape = &shape;		// 設定
		fixture.density = 0.0f;		// 密度
		fixture.friction = 0.0f;	// 摩擦
		fixture.restitution = 0.0f; // 反発係数
		fixture.isSensor = true;	// センサーかどうか

		b2Fixture *m_fixture = m_attack_body->CreateFixture(&fixture);

		ObjectData *boss_attack_data = new ObjectData{collider_shock_wave};
		m_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(boss_attack_data);

		// カメラシェイクスタート
		CameraShake::StartCameraShake(0, 20, 40);
		HitStop::SetHitStopFlag(5);
	}
}

void Boss_1_1::ShockWaveUpdate(void)
{
	if (Shock_Wave_Fly_flag == true)
	{
		if (GetAttackBody() != nullptr)
		{
			float minus_flag = 1;
			if (ShockWaveLeftFlag == true)
			{
				minus_flag = -1;
			}

			GetAttackBody()->SetLinearVelocity(b2Vec2(minus_flag * Shock_Wave_Speed, 0.0f));
		}
		Now_Shock_Wave_time_Frame++;

		// 衝撃波の終了
		if (Shock_Wave_time_Frame < Now_Shock_Wave_time_Frame)
		{
			DeleteAttackBody();
			Shock_Wave_Fly_flag = false;   // リセットする
			Now_Shock_Wave_time_Frame = 0; // リセットする
		}
	}
}

void Boss_1_1::CreateMiniGolem(b2Vec2 mini_golem_size, bool left)
{
	for (int i = 0; i < 2; i++)
	{
		if (GetMiniGolemBody(i) == nullptr && Mini_golem_Create_flag == true)
		{

			// ボディのサイズを設定
			SetMiniGolemDrawSize(mini_golem_size);

			b2Vec2 size; // サイズのスケールを設定
			size.x = mini_golem_size.x / BOX2D_SCALE_MANAGEMENT;
			size.y = mini_golem_size.y / BOX2D_SCALE_MANAGEMENT;

			b2BodyDef body;
			body.type = b2_dynamicBody;
			b2Vec2 boss_pos = m_body->GetPosition();
			b2Vec2 boss_size = b2Vec2(GetBossDrawSize().x / BOX2D_SCALE_MANAGEMENT, GetBossDrawSize().y / BOX2D_SCALE_MANAGEMENT);

			if (left)
			{
				body.position.Set(boss_pos.x, boss_pos.y);
			}
			else
			{
				body.position.Set(boss_pos.x, boss_pos.y);
			}
			body.angle = 0.0f;
			body.fixedRotation = false; // 回転を固定する
			body.userData.pointer = (uintptr_t)this;

			Box2dWorld &box2d_world = Box2dWorld::GetInstance();
			b2World *world = box2d_world.GetBox2dWorldPointer();

			// ワールドに追加
			b2Body *m_mini_golem_body = world->CreateBody(&body);

			SetMiniGolemBody(m_mini_golem_body, i);

			// ミニゴーレムのフィクスチャ
			b2FixtureDef fixture;

			// クラスから b2Shape を継承した場合の設定
			b2CircleShape shape;		 // クラスのメンバー変数として継承
			shape.m_radius = size.y / 2; // 画像の都合で大きさを変えるため　生成時の位置はそのまま

			fixture.shape = &shape;		// 設定
			fixture.density = 1.0f;		// 密度
			fixture.friction = 0.5f;	// 摩擦
			fixture.restitution = 0.3f; // 反発係数
			fixture.isSensor = false;	// センサーかどうか
			fixture.filter = createFilterExclude("MiniGolem_filter", {"Boss_filter"});

			b2Fixture *m_fixture = m_mini_golem_body->CreateFixture(&fixture);

			ObjectData *mini_golem_data = new ObjectData{collider_mini_golem};
			m_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(mini_golem_data);

			Mini_golem_Create_flag = false;

			return;
		}
	}
}

void Boss_1_1::MiniGolemUpdate(void)
{
	for (int i = 0; i < 2; i++)
	{
		if (GetMiniGolemBody(i) != nullptr)
		{
			// ボディが存在している
			b2Body *mini_golem_body = GetMiniGolemBody(i);

			// プレイヤーの位置を取得
			b2Vec2 player_pos = PlayerPosition::GetPlayerPosition();

			// プレイヤーと左か右かに移動する
			if (player_pos.x < mini_golem_body->GetPosition().x) // プレイヤーの左
			{
				if (mini_golem_body->GetAngularVelocity() > -3) // 最大回転速度を超えない
				{
					mini_golem_body->ApplyTorque(-0.1, true);
				}
			}
			else
			{
				if (mini_golem_body->GetAngularVelocity() < 3) // 最大回転速度を超えない
				{
					mini_golem_body->ApplyTorque(0.1, true);
				}
			}
		}
	}
}

void Boss_1_1::DestroyMiniGolemBody(void)
{
	if (destroy_mini_golem_flag == true)
	{
		Box2dWorld &box2d_world = Box2dWorld::GetInstance();
		b2World *world = box2d_world.GetBox2dWorldPointer();

		b2Body *m_mini_golem_body = destroy_mini_golem_body;

		// ソウルを落とす
		ItemManager &item_manager = ItemManager::GetInstance();
		item_manager.AddSpirit(m_mini_golem_body->GetPosition(), {1.0f, 2.0f}, 0.0f, ENEMY_GOLEM_SOULGAGE);

		// 生成エフェクト用の管理
		mini_golem_delete_effect_position = m_mini_golem_body->GetPosition();
		mini_golem_break_effect_cnt = 1;

		world->DestroyBody(m_mini_golem_body);

		for (int i = 0; i < 2; i++)
		{
			if (GetMiniGolemBody(i) == destroy_mini_golem_body)
			{
				SetMiniGolemBody(nullptr, i);
				destroy_mini_golem_flag = false;

				// カメラシェイクスタート
				CameraShake::StartCameraShake(0, 20, 10);
				HitStop::SetHitStopFlag(5);
			}
		}
	}
}

void Boss_1_1::DeleteAttackBody()
{
	if (GetAttackBody() != nullptr)
	{
		Box2dWorld &box2d_world = Box2dWorld::GetInstance();
		b2World *world = box2d_world.GetBox2dWorldPointer();

		b2Body *m_destroy_body = GetAttackBody();

		world->DestroyBody(m_destroy_body);

		SetAttackBody(nullptr);
	}
}

void Boss_1_1::Draw()
{
	if (m_body != nullptr)
	{
		// デバッグ用の描画
		DrawBossDebug();
		/*debugDraw();*/

		float scale = SCREEN_SCALE;

		// スクリーンの中心 (16m x 9m の仮想座標で、中心は x = 8, y = 4.5 と仮定)
		b2Vec2 screen_center;
		screen_center.x = SCREEN_CENTER_X;
		screen_center.y = SCREEN_CENTER_Y;

		// コライダーの位置を取得（プレイヤーの位置）
		b2Vec2 boss_pos = GetBossBody()->GetPosition();
		b2Vec2 real_boss_size;
		real_boss_size.x = GetBossRealSize().x / BOX2D_SCALE_MANAGEMENT;
		real_boss_size.y = GetBossRealSize().y / BOX2D_SCALE_MANAGEMENT;

		// プレイヤー位置を基準にスクリーン座標に変換する
		// 取得したbodyのポジションに基づいてBox2dスケールの変換を行う
		float draw_x = ((boss_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
		float draw_y = ((boss_pos.y - PlayerPosition::GetPlayerPosition().y - (real_boss_size.y * 0.7)) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

		switch (now_boss_state)
		{
		case wait_state:

			GetDeviceContext()->PSSetShaderResources(0, 1, &g_boss_walk_sheet1_Texture);

			DrawDividedSpriteBoss(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossDrawSize().x * scale, GetBossDrawSize().y * scale), 6, 6, 1, boss_alpha, left_flag);

			break;

		case panic_state:

			GetDeviceContext()->PSSetShaderResources(0, 1, &g_boss_panic_sheet_Texture);

			DrawDividedSpriteBoss(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossDrawSize().x * scale, GetBossDrawSize().y * scale), 16, 17, sheet_cnt, boss_alpha, left_flag);

			break;


		case down_state:

			GetDeviceContext()->PSSetShaderResources(0, 1, &g_boss_down_sheet);

			DrawDividedSpriteBoss(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossDrawSize().x * scale, GetBossDrawSize().y * scale), 8, 8, sheet_cnt, boss_alpha, left_flag);

			break;
		case jump_state:
			// シェーダーリソースを設定
			if (sheet_cnt < Max_Jump_Sheet / 2)
			{
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_boss_jump_sheet1_Texture);
				DrawDividedSpriteBoss(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossDrawSize().x * scale, GetBossDrawSize().y * scale), 6, 6, sheet_cnt, boss_alpha, left_flag);
			}
			else
			{
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_boss_jump_sheet2_Texture);
				DrawDividedSpriteBoss(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossDrawSize().x * scale, GetBossDrawSize().y * scale), 6, 6, sheet_cnt - Max_Jump_Sheet / 2, boss_alpha, left_flag);
			}
			break;
		case shock_wave_state:
			// シェーダーリソースを設定
			if (sheet_cnt < Max_Shock_Wave_Sheet / 2)
			{
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_boss_shock_wave_sheet1_Texture);

				DrawDividedSpriteBoss(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossDrawSize().x * scale, GetBossDrawSize().y * scale), 7, 7, sheet_cnt, boss_alpha, left_flag);
			}
			else
			{
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_boss_shock_wave_sheet2_Texture);
				DrawDividedSpriteBoss(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossDrawSize().x * scale, GetBossDrawSize().y * scale), 7, 7, sheet_cnt - Max_Shock_Wave_Sheet / 2, boss_alpha, left_flag);
			}
			break;

		case create_mini_golem_state:
			if (sheet_cnt < Max_Create_Mini_Golem_Sheet / 2)
			{
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_mini_boss_create_sheet1_Texture);

				DrawDividedSpriteBoss(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossDrawSize().x * scale, GetBossDrawSize().y * scale), 7, 7, sheet_cnt, boss_alpha, left_flag);
			}
			else
			{
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_mini_boss_create_sheet2_Texture);
				DrawDividedSpriteBoss(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossDrawSize().x * scale, GetBossDrawSize().y * scale), 7, 7, sheet_cnt - Max_Create_Mini_Golem_Sheet / 2, boss_alpha, left_flag);
			}
			break;

		case charge_attack_state:
			if (sheet_cnt < Max_Charge_Attack_Sheet / 2)
			{
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_boss_charge_attack_sheet1_Texture);

				DrawDividedSpriteBoss(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossDrawSize().x * scale, GetBossDrawSize().y * scale), 10, 10, sheet_cnt, boss_alpha, left_flag);
			}
			else
			{
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_boss_charge_attack_sheet2_Texture);
				DrawDividedSpriteBoss(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossDrawSize().x * scale, GetBossDrawSize().y * scale), 10, 10, sheet_cnt - Max_Charge_Attack_Sheet / 2, boss_alpha, left_flag);
			}
			break;

		case walk_state: // 歩行アニメーション

			if (sheet_cnt < Max_Walk_Sheet / 2)
			{
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_boss_walk_sheet1_Texture);

				DrawDividedSpriteBoss(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossDrawSize().x * scale, GetBossDrawSize().y * scale), 6, 6, sheet_cnt, boss_alpha, left_flag);
			}
			else
			{
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_boss_walk_sheet2_Texture);
				DrawDividedSpriteBoss(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossDrawSize().x * scale, GetBossDrawSize().y * scale), 6, 6, sheet_cnt - Max_Walk_Sheet / 2, boss_alpha, left_flag);
			}
			break;

		default:
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_debug_attack_color);
			DrawDividedSpriteBoss(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossDrawSize().x * scale, GetBossDrawSize().y * scale), 6, 6, sheet_cnt - Max_Walk_Sheet / 2, boss_alpha, left_flag);
			break;
		}

		// コアの描画
		if (GetAnchorPointBody() != nullptr)
		{
			// // コアの描画を削除

			// // シェーダーリソースを設定
			// // GetDeviceContext()->PSSetShaderResources(0, 1, &g_debug_attack_color);

			// // コライダーの位置を取得（プレイヤーの位置）
			// b2Vec2 anchorpoint_pos = GetAnchorPointBody()->GetPosition();

			// // プレイヤー位置を基準にスケール変換して描画位置を計算
			// // 取得したbodyのポジションに基づいてBox2dスケールの変換を行う
			// float anchor_point_draw_x = ((anchorpoint_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
			// float anchor_point_draw_y = ((anchorpoint_pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

			// // DrawSprite(XMFLOAT2(anchor_point_draw_x, anchor_point_draw_y), 0.0f, XMFLOAT2(GetAnchorPointSize().x * scale, GetAnchorPointSize().y * scale));
		}

		//----------------------------------------------------------------------------------------
		// ミニゴーレムの描画
		for (int i = 0; i < 2; i++)
		{
			if (GetMiniGolemBody(i) != nullptr)
			{
				// シェーダーリソースを設定
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_mini_boss_Texture);

				// コライダーの位置を取得（プレイヤーの位置）
				b2Vec2 mini_golem_pos = GetMiniGolemBody(i)->GetPosition();

				// プレイヤー位置を基準にスクリーン座標に変換する
				// 取得したbodyのポジションに基づいてBox2dスケールの変換を行う
				float mini_golem_draw_x = ((mini_golem_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
				float mini_golem_draw_y = ((mini_golem_pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

				DrawSprite(XMFLOAT2(mini_golem_draw_x, mini_golem_draw_y), GetMiniGolemBody(i)->GetAngle(), XMFLOAT2(GetMiniGolemDrawSize().x * scale, GetMiniGolemDrawSize().y * scale));
			}
		}
	}
	EffectDraw();
}

void Boss_1_1::debugDraw()
{
	///------------------------------------------------------------------------
	// ボディの四角形を描画

	float scale = SCREEN_SCALE;

	// スクリーンの中心 (16m x 9m の仮想座標で、中心は x = 8, y = 4.5 と仮定)
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;

	// シェーダーリソースを設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_debug_color);

	// コライダーの位置を取得（プレイヤーの位置）
	b2Vec2 boss_pos = GetBossBody()->GetPosition();

	// プレイヤー位置を基準にスクリーン座標に変換する
	// 取得したbodyのポジションに基づいてBox2dスケールの変換を行う
	float draw_x = ((boss_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
	float draw_y = ((boss_pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

	DrawSprite(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossDrawSize().x * scale, GetBossDrawSize().y * scale));

	DrawSprite(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossSensorSize().x * scale, GetBossSensorSize().y * scale));

	//----------------------------------------------------------------------------
	// 実際のボスのサイズを描画

	if (GetBossBody() != nullptr)
	{
		// シェーダーリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_debug_boss_body_color);

		// コライダーの位置を取得（プレイヤーの位置）
		b2Vec2 boss_pos = GetBossBody()->GetPosition();

		// プレイヤー位置を基準にスクリーン座標に変換する
		// 取得したbodyのポジションに基づいてBox2dスケールの変換を行う
		float boss_draw_x = ((boss_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
		float boss_draw_y = ((boss_pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

		DrawSprite(XMFLOAT2(boss_draw_x, boss_draw_y), 0.0f, XMFLOAT2(GetBossRealSize().x * scale, GetBossRealSize().y * scale));
	}
	//---------------------------------------------------------------------------
	// 攻撃の描画
	if (GetAttackBody() != nullptr)
	{
		// シェーダーリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_debug_attack_color);

		// コライダーの位置を取得（プレイヤーの位置）
		b2Vec2 attack_pos = GetAttackBody()->GetPosition();

		// プレイヤー位置を基準にスクリーン座標に変換する
		// 取得したbodyのポジションに基づいてBox2dスケールの変換を行う
		float attack_draw_x = ((attack_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
		float attack_draw_y = ((attack_pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

		DrawDividedSpriteBoss(XMFLOAT2(attack_draw_x, attack_draw_y), 0.0f, XMFLOAT2(GetAttackDrawSize().x * scale, GetAttackDrawSize().y * scale), 6, 4, 1, 1.0, left_flag);
	}
}

void Boss_1_1::UpdateEffectSheetCnt()
{
	// チャージアタックのエフェクト
	if (charge_attack_effect_sheet_cnt != 0)
	{
		charge_attack_effect_sheet_cnt += 0.5;

		if (Max_charge_attack_effect_sheet_cnt < charge_attack_effect_sheet_cnt)
		{
			charge_attack_effect_sheet_cnt = 0;
		}
	}

	// チャージ
	if (charge_effect_sheet_cnt != 0)
	{
		charge_effect_sheet_cnt += 0.5;

		if (Max_charge_effect_sheet_cnt < charge_effect_sheet_cnt)
		{
			charge_effect_sheet_cnt = 0;
		}
	}

	// 衝撃波
	if (shock_wave_effect_sheet_cnt != 0)
	{
		shock_wave_effect_sheet_cnt += 0.5;

		if (Max_shock_wave_effect_sheet_cnt < shock_wave_effect_sheet_cnt)
		{
			shock_wave_effect_sheet_cnt = 0;
		}
	}

	// ミニゴーレムの破壊
	if (mini_golem_break_effect_cnt != 0)
	{
		mini_golem_break_effect_cnt++;

		if (Max_mini_golem_break_effect < mini_golem_break_effect_cnt)
		{
			mini_golem_break_effect_cnt = 0;
		}
	}
}

void Boss_1_1::EffectDraw()
{
	float scale = SCREEN_SCALE;

	// スクリーンの中心 (16m x 9m の仮想座標で、中心は x = 8, y = 4.5 と仮定)
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;

	//---------------------------------------------------------------------------
	// チャージアタック
	if (GetAttackBody() != nullptr)
	{
		if (now_boss_state == charge_attack_state)
		{
			if (charge_attack_effect_sheet_cnt != 0)
			{
				// シェーダーリソースを設定
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_boss_charge_attack_effect);

				// コライダーの位置を取得（プレイヤーの位置）
				b2Vec2 attack_pos;
				attack_pos.x = GetAttackBody()->GetPosition().x;
				attack_pos.y = GetAttackBody()->GetPosition().y + (GetAttackDrawSize().y / BOX2D_SCALE_MANAGEMENT / 2);

				// プレイヤー位置を基準にスクリーン座標に変換する
				// 取得したbodyのポジションに基づいてBox2dスケールの変換を行う
				float attack_draw_x = ((attack_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
				float attack_draw_y = ((attack_pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

				DrawDividedSpriteBoss(XMFLOAT2(attack_draw_x, attack_draw_y), 0.0f, XMFLOAT2(GetAttackDrawSize().x * scale * 7, GetAttackDrawSize().y * scale * 7), 5, 6, charge_attack_effect_sheet_cnt, effect_alpha, left_flag);
			}
		}
		else // ショックウェーブ
		{
			if (shock_wave_effect_sheet_cnt != 0)
			{
				// シェーダーリソースを設定
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_boss_shock_wave_effect);

				// コライダーの位置を取得（プレイヤーの位置）
				b2Vec2 attack_pos = GetAttackBody()->GetPosition();

				// プレイヤー位置を基準にスクリーン座標に変換する
				// 取得したbodyのポジションに基づいてBox2dスケールの変換を行う
				float attack_draw_x = ((attack_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
				float attack_draw_y = ((attack_pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

				// 左か右かを判定してスプライトを反転させる
				bool left = 1;
				if (ShockWaveLeftFlag)
				{
					left = 0;
				}

				DrawDividedSpriteBoss(XMFLOAT2(attack_draw_x, attack_draw_y), 0.0f, XMFLOAT2(GetAttackDrawSize().x * scale * 1.3, GetAttackDrawSize().y * scale * 1.3), 6, 4, shock_wave_effect_sheet_cnt, effect_alpha, left);
			}
		}
	}

	// ミニゴーレムの破壊
	if (mini_golem_break_effect_cnt != 0)
	{
		// シェーダーリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_mini_golem_break_effect);

		// コライダーの位置を取得（プレイヤーの位置）
		b2Vec2 break_pos = mini_golem_delete_effect_position;

		// プレイヤー位置を基準にスクリーン座標に変換する
		// 取得したbodyのポジションに基づいてBox2dスケールの変換を行う
		float break_draw_x = ((break_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
		float break_draw_y = ((break_pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

		DrawDividedSpriteBoss(XMFLOAT2(break_draw_x, break_draw_y), 0.0f, XMFLOAT2(GetMiniGolemDrawSize().x * scale * 1.3 * 1.5, GetMiniGolemDrawSize().y * scale * 1.7 * 1.5), 4, 2, mini_golem_break_effect_cnt / 4, effect_alpha, 1);
	}
}

void Boss_1_1::Finalize()
{
	Box2dWorld &box2d_world = Box2dWorld::GetInstance();
	b2World *world = box2d_world.GetBox2dWorldPointer();

	if (GetBossBody() != nullptr)
	{
		world->DestroyBody(GetBossBody());
		SetBossBody(nullptr);
	}

	if (GetAttackBody() != nullptr)
	{
		world->DestroyBody(GetAttackBody());
		SetAttackBody(nullptr);
	}
}

b2Body *Boss_1_1::GetOutSideBody(void)
{
	return outside_boss_body;
}