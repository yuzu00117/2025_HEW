//-----------------------------------------------------------------------------------------------------
// #name 1-1_boss.cpp
// #description bossをつくる　えぐい
// #make 2025/01/07　　永野義也
// #update 2025/01/21
// #comment 追加・修正予定
//          ・特になし
//----------------------------------------------------------------------------------------------------
#include"1-1_boss.h"
#include"include/box2d/box2d.h"
#include"texture.h"
#include"sprite.h"
#include"main.h"
#include"world_box2d.h"
#include"create_filter.h"
#include"player_position.h"
#include"display.h"
#include"collider_type.h"
#include"keyboard.h"
#include"1_1boss_state_debug.h"
#include"scene.h"
#include"anchor.h"
#include"camera_shake.h"
#include"hit_stop.h"


// 使用するテクスチャファイルを格納
static ID3D11ShaderResourceView* g_mini_boss_Texture = NULL;//小さなゴーレムのテクスチャ
static ID3D11ShaderResourceView* g_boss_shock_wave_sheet1_Texture = NULL;//衝撃波のテクスチャ１
static ID3D11ShaderResourceView* g_boss_shock_wave_sheet2_Texture = NULL;//衝撃波のテクスチャ２
static ID3D11ShaderResourceView* g_boss_charge_attack_sheet1_Texture = NULL;//溜め攻撃のテクスチャ１
static ID3D11ShaderResourceView* g_boss_charge_attack_sheet2_Texture = NULL;//ため攻撃のテクスチャ２

static ID3D11ShaderResourceView* g_boss_walk_sheet1_Texture = NULL;//ゴーレムの歩きモーション１
static ID3D11ShaderResourceView* g_boss_walk_sheet2_Texture = NULL;//ゴーレムの歩きモーション２
static ID3D11ShaderResourceView* g_boss_jump_sheet1_Texture = NULL;//ゴーレムのジャンプモーション１
static ID3D11ShaderResourceView* g_boss_jump_sheet2_Texture = NULL;//ゴーレムのジャンプモーション2
static ID3D11ShaderResourceView* g_boss_panic_sheet_Texture = NULL;//ゴーレムの混乱被弾モーション

static ID3D11ShaderResourceView* g_mini_boss_create_sheet1_Texture = NULL;//小さなゴーレムを生成する際のボス側１
static ID3D11ShaderResourceView* g_mini_boss_create_sheet2_Texture = NULL;//小さなゴーレムを生成する際のボス側２


//ボスまわりのエフェクトのテクスチャ
static ID3D11ShaderResourceView* g_boss_charge_effect = NULL;//ボスのため中のエフェクト
static ID3D11ShaderResourceView* g_boss_charge_attack_effect = NULL;//ボスのため攻撃の判定のエフェクト
static ID3D11ShaderResourceView* g_mini_golem_break_effect = NULL;//小さなゴーレムを破壊するときにでるエフェクト
static ID3D11ShaderResourceView* g_boss_panic_effect = NULL;//ボスが被弾したときのエフェクト
static ID3D11ShaderResourceView* g_boss_shock_wave_effect = NULL;//ボスの衝撃波攻撃




//-------------------------------------------------------------------------------------------
//デバック用の画像
static ID3D11ShaderResourceView* g_debug_color = NULL;//デバック用

static ID3D11ShaderResourceView* g_debug_boss_body_color = NULL;//デバック用

static ID3D11ShaderResourceView* g_debug_attack_color = NULL;//デバック用

static ID3D11ShaderResourceView* g_debug_core = NULL;//デバック用






//そとのCPPから参照できるため
b2Body* outside_boss_body;

Boss_1_1::Boss_1_1()
{



}

Boss_1_1::~Boss_1_1()
{
}



void Boss_1_1::Initialize(b2Vec2 position, b2Vec2 bodysize,bool left)
{

	if (g_mini_boss_create_sheet1_Texture == NULL)
	{
		g_mini_boss_Texture = InitTexture(L"asset\\texture\\boss_1_1\\mini_boss.png");//ミニゴーレムのインクルード
		g_boss_shock_wave_sheet1_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_shock_wave_sheet1.png");//衝撃波攻撃のインクルードシート１
		g_boss_shock_wave_sheet2_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_shock_wave_sheet2.png");//衝撃波攻撃のインクルードシート２
		g_boss_charge_attack_sheet1_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_charge_sheet1.png");//ため攻撃のシート１
		g_boss_charge_attack_sheet2_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_charge_sheet2_.png");//ため攻撃のシート２
		g_mini_boss_create_sheet1_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_create_mini_golem_sheet1.png");//ミニゴーレムの生成のボス側１
		g_mini_boss_create_sheet2_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_create_mini_golem_sheet2_.png");//ミニゴーレムの生成のボス側２
		g_boss_walk_sheet1_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_walk_sheet1.png");//ゴーレムの歩きモーション１
		g_boss_walk_sheet2_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_walk_sheet2.png");//ゴーレムの歩きモーション２
		g_boss_jump_sheet1_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_jump_new_sheet1.png");//ゴーレムのジャンプモーション１
		g_boss_jump_sheet2_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_jump_new_sheet2.png");//ゴーレムのジャンプモーション２
		g_boss_panic_sheet_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_panic_sheet1.png");//ゴーレムの怯みモーション１


		//エフェクト

		g_boss_charge_attack_effect = InitTexture(L"asset\\texture\\boss_1_1\\boss_charge_attack_effect.png");	//ボスのチャージ攻撃時のエフェクト
		g_boss_charge_effect = InitTexture(L"asset\\texture\\boss_1_1\\boss_charge_effect.png");				//ボスのチャージ攻撃時のエフェクト
		g_boss_panic_effect = InitTexture(L"asset\\texture\\boss_1_1\\boss_panic_effect.png");					//ボスのチャージ攻撃時のエフェクト
		g_boss_shock_wave_effect = InitTexture(L"asset\\texture\\boss_1_1\\boss_shock_wave_effect.png");		//ボスのチャージ攻撃時のエフェクト
		g_mini_golem_break_effect = InitTexture(L"asset\\texture\\boss_1_1\\mini_golem_break_effect.png");		//ボスのチャージ攻撃時のエフェクト
	
		//デバック用
		g_debug_color = InitTexture(L"asset\\texture\\sample_texture\\img_sensor.png");//sensorのテクスチャ
		g_debug_boss_body_color = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_blue.png");
		g_debug_attack_color = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_red.png");
		g_debug_core = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_blue.png");


		InitializeBossDebug();//デバック用のもの

	}

	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();


	//ボディがあったら消す
	if (GetBossBody() != nullptr)
	{
		world->DestroyBody(GetBossBody());
		SetBossBody(nullptr);
	}

	//ボディのサイズをセット
	SetBossDrawSize(bodysize);


	//実際のボスのサイズ
	bodysize.x=bodysize.x * 0.5;
	bodysize.y=bodysize.y * 0.4;

	SetBossRealSize(bodysize);

	//bossのサイズ
	b2Vec2 size; //サイズのスケールを調整
	size.x = bodysize.x / BOX2D_SCALE_MANAGEMENT;
	size.y = bodysize.y / BOX2D_SCALE_MANAGEMENT;




	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(position.x, position.y);
	body.angle = 0.0f;
	body.fixedRotation = true;//回転を固定にする
	body.userData.pointer = (uintptr_t)this;


	

	//ワールドに登録
	b2Body* m_boss_body = world->CreateBody(&body);

	SetBossBody(m_boss_body);
	outside_boss_body = m_boss_body;//外部からの参照よう「

	b2PolygonShape body_shape;







	body_shape.SetAsBox(size.x*0.5,size.y*0.5);


	b2FixtureDef body_fixture;
	body_fixture.shape = &body_shape;
	body_fixture.friction = 0.3f;//摩擦
	body_fixture.restitution = 0.1f;//反発係数
	body_fixture.density = 0.1f;
	body_fixture.isSensor = false;//センサーかどうか、trueならあたり判定は消える
	body_fixture.filter = createFilterExclude("Boss_filter",{});

	
	b2Fixture* m_body_fixture = m_body->CreateFixture(&body_fixture);

	ObjectData* boss_body_data = new ObjectData{ collider_boss };
	m_body_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(boss_body_data);


	//センサーのサイズを記録
	SetBossSensorSize(b2Vec2(bodysize.x*2.0,bodysize.y));

	b2Vec2 sensor_size;
	sensor_size.x = bodysize.x / BOX2D_SCALE_MANAGEMENT*2.0;
	sensor_size.y = bodysize.y / BOX2D_SCALE_MANAGEMENT;


	b2FixtureDef sensor_fixture;
	b2PolygonShape sensor_shape;

	b2Vec2 sensor_vertices[4] = { b2Vec2(0.0f,0.0f) };



	sensor_shape.SetAsBox(sensor_size.x * 0.5, sensor_size.y * 0.5);

	sensor_fixture.shape = &sensor_shape;
	sensor_fixture.friction = 0.0f;//摩擦
	sensor_fixture.restitution = 0.1f;//反発係数
	sensor_fixture.density = 0.1f;
	sensor_fixture.isSensor = true;//センサーかどうか、trueならあたり判定は消える



	b2Fixture* m_sensor_fixture = m_body->CreateFixture(&sensor_fixture);

	ObjectData* boss_sensor_data = new ObjectData{ collider_boss_senosr };
	m_sensor_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(boss_sensor_data);

	time_count_flag = true;
	elapsed_time = 0.0f;
}

void Boss_1_1::Update()
{
	if (m_body != nullptr)
	{

		//---------------------------------------------------------------------------------------------------------------------------
		//左右の振り向きを作る
		float player_x= PlayerPosition::GetPlayerPosition().x;
		if (player_x < m_body->GetPosition().x)//左にいる
		{
			left_flag = true;
		}
		else
		{
			left_flag = false;
		}

	
		//-------------------------------------------------------------------------------------------

		//デバック文字の更新
		UpdateBossDebug();

		//衝撃波の更新処理
		ShockWaveUpdate();

		//ミニゴーレムの更新処理
		MiniGolemUpdate();
		//ミニゴーレム
		DestroyMiniGolemBody();
		//ジャンプの更新処理
		JumpUpdate();

		//ボスのコアの更新処理
		BossCoreUpdate();

		//ボスが死んだか
		BossDead();

		

		//クールタイムの管理
		UpdateCoolTime();

		//エフェクトの管理
		UpdateEffectSheetCnt();


		//ボスの近距離か遠距離かの判定に使う
		if (GetPlayerNearbylocked() != 0)
		{
			SetPlayerNearbylocked(GetPlayerNearbylocked() - 1);
		}


		//-------------------------------------------------------------------------------------------
		// デバック用　本番環境ではけす
		
		//アンカーポイントのボディを削除するデバック用
		if (Keyboard_IsKeyDown(KK_Y)&&debug_flag==0)
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


			
			CreateBossCore(b2Vec2 (2.0f,2.0f));
		
			sheet_cnt += 0.5;

			if (Max_Panic_Stun_Frame <= sheet_cnt)
			{
				sheet_cnt = 0;
				CoreDeleteFlag = true;

			}

			break;
		case walk_state:
			if (static_cast<int>(sheet_cnt)%10 == 0)
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

				//カメラシェイクスタート
				CameraShake::StartCameraShake(3, 00, 10);
				
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

				//ジャンプのあと地すべりするから X軸の加速度を０にYはそのまま
				b2Body*body =GetBossBody();
				b2Vec2 velocity=body->GetLinearVelocity();
				body->SetLinearVelocity(b2Vec2(0.0f, velocity.y));


				//カメラシェイクスタート
				CameraShake::StartCameraShake(120, 00, 20);
				HitStop::SetHitStopFlag(5);

				now_boss_state = wait_state;
			}

			break;
		case shock_wave_state:
			sheet_cnt += 0.5;
			
			if (static_cast<int>(sheet_cnt) == Shock_Wave_Start_Frame)
			{
				CreateShockWave(b2Vec2(5.0f, 6.0f), left_flag);
				Shock_Wave_Fly_flag = true;

				//エフェクトスタート
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
				CreateMiniGolem(b2Vec2(3.0f,2.0f),left_flag);//画像が横に空白があるため　ｘを引き伸ばし　実際の半径で参照しているのはｙ軸
				
			
			}
			if (Max_Create_Mini_Golem_Sheet <= sheet_cnt)
			{
				Mini_golem_Create_flag = true;//ミニゴーレムのクリエイトフラグの管理
				sheet_cnt = 0;
				now_boss_state = wait_state;
			}


			break;
		case charge_attack_state:

			//シート１枚目からは進行が少し早い
			if (sheet_cnt < 100)
			{
				sheet_cnt += 0.75;
			}
			else // シート２枚目からは進行が少し遅い
			{
				sheet_cnt += 0.5;
			}

			if (static_cast<int>(sheet_cnt) == Charge_Attack_Start_Frame)
			{
				CreateChargeAttack(b2Vec2(4.0f, 4.0f), left_flag);
				//エフェクトスタート
				charge_attack_effect_sheet_cnt = 1;
			}
			if (static_cast<int>(sheet_cnt) == Charge_Attack_End_Frame)
			{
				DeleteAttackBody();
			}
			
			//モーションが完了した
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

	// 時間をカウント
	if (time_count_flag == true)
	{
		elapsed_time += 1.0f / 60.0f; // 1フレームで1/60秒
	}
}

void Boss_1_1::UpdateCoolTime(void)
{
	Now_Shock_Wave_CoolTime++;		//衝撃波攻撃のインクリメント
	Now_Jump_CoolTime++;			//ジャンプのインクリメント
	Now_Create_MiniGolem_CoolTime++;//ミニゴーレムのインクリメント
	Now_Charge_Attack_CoolTime++;	//チャージ攻撃のインクリメント
	Now_Max_Walk_CoolTime++;		//歩きモーション

	//近距離か遠距離か
	if (now_boss_state == wait_state)
	{
		if (Player_is_Nearby == true)
		{
			//近距離にいる

			//チャージ攻撃
			if (Now_Charge_Attack_CoolTime > Max_Charge_Attack_CoolTime)
			{
				now_boss_state = charge_attack_state;
				Now_Charge_Attack_CoolTime = 0;
				return;
			}

			//ジャンプ
			if (Now_Jump_CoolTime > Max_Jump_CoolTime)
			{
				now_boss_state = jump_state;
				Now_Jump_CoolTime = 0;
				return;
			}

		}
		else
		{
			//遠距離にいる

		

			//衝撃波攻撃
			if (Now_Shock_Wave_CoolTime > Max_Shock_Wave_CoolTime)
			{
				now_boss_state = shock_wave_state;
				Now_Shock_Wave_CoolTime = 0;
				return;
			}

			//小岩生成
			if (Now_Create_MiniGolem_CoolTime > Max_Create_MiniGolem_CoolTime)
			{
				now_boss_state = create_mini_golem_state;
				Now_Create_MiniGolem_CoolTime = 0;
				return;
			}


			//何もなかったら歩きモーション
			if (wait_state == now_boss_state)
			{
				now_boss_state=walk_state;
			}

	

		}
	}
}

void Boss_1_1::BossDamaged(void)
{
	//ボスのHPを減らすそれにより形態変更する
	SetBossHP(GetBossHP() - 1);

	
}

void Boss_1_1::BossDead(void)
{
	//ボスのHPが０以下になったらリザルトに飛ぶ
	if (boss_hp <= 0)
	{
		time_count_flag = false; //時間のカウントを止める

		SceneManager& sceneManager = SceneManager::GetInstance();
		sceneManager.ChangeScene(SCENE_RESULT);
	}
}

void Boss_1_1::BossCoreUpdate()
{
	if (CoreDeleteFlag == true)
	{
		DestroyBossCore();//ボスのコアを破壊

		sheet_cnt = 0;//シートカウントをリセット
		now_boss_state = charge_attack_state;
		CoreDeleteFlag = false;
		
	}
}


void Boss_1_1::CreateBossCore(b2Vec2 size)
{

	if (GetAnchorPointBody() == nullptr)
	{
		//サイズをセット
		SetAnchorPointSize(size);

		b2Vec2 anchorpoint_size;

		anchorpoint_size.x = size.x / BOX2D_SCALE_MANAGEMENT;
		anchorpoint_size.y = size.y / BOX2D_SCALE_MANAGEMENT;



		b2Vec2 position = m_body->GetPosition();

		b2BodyDef anchor_point_body;
		anchor_point_body.type = b2_dynamicBody;//静的なオブジェクトにするならtrue
		anchor_point_body.position.Set(position.x, position.y);			//ポジションをセット
		anchor_point_body.angle = 0;									//角度の定義
		anchor_point_body.userData.pointer = (uintptr_t)this;			//userDataのポインタを定義 
		anchor_point_body.fixedRotation = true;							//回転を固定する、　これをオンにすると回転しない


		Box2dWorld& box2d_world = Box2dWorld::GetInstance();//ワールドのインスタンスを取得する
		b2World* world = box2d_world.GetBox2dWorldPointer();//ワールドのポインタを持ってくる

		b2Body* m_anchor_point_body = world->CreateBody(&anchor_point_body);//Bodyをワールドに固定

		SetAnchorPointBody(m_anchor_point_body);


		b2PolygonShape shape;                         //shapeには色々な型がある　サークルとかもあるよ
		shape.SetAsBox(anchorpoint_size.x * 0.5f, anchorpoint_size.y * 0.5f);//あたり判定を登録する4点　*0.5するのは

		b2FixtureDef fixture;
		fixture.shape = &shape;    //シャープをフィクスチャに登録する
		fixture.density = 1.0f;    //密度
		fixture.friction = 0.05f;  //摩擦
		fixture.restitution = 0.0f;//反発係数
		fixture.isSensor = false;  //センサーかどうか、trueならあたり判定は消える

		b2Fixture* ground_fixture = m_anchor_point_body->CreateFixture(&fixture);//Bodyをにフィクスチャを登録する

		// カスタムデータを作成して設定
		// 地面に値を登録
		// 地面にユーザーデータを登録
		ObjectData* data = new ObjectData{ collider_anchor_point };
		ground_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(data);

		
		data->need_anchor_level = 1;

		data->object_name = Boss_core;


		
		//プレイヤーとジョイントする
		b2WeldJointDef jointDef;
		jointDef.bodyA = m_body;//ボスのボディ
		jointDef.bodyB = GetAnchorPointBody();//bossの攻撃のボディ

		//boss側
		jointDef.localAnchorA.Set(0.0f, 0.0f);
		//攻撃側
		jointDef.localAnchorB.Set(0.0f, 0.0f);

		jointDef.collideConnected = true;//ジョイントした物体同士の接触を消す

		world->CreateJoint(&jointDef); //ワールドにジョイントを追加


		//エフェクトスタート
		panic_effect_sheet_cnt = 1;

		//カメラシェイクスタート
		CameraShake::StartCameraShake(40, 20, 40);
		HitStop::SetHitStopFlag(10);


		b2Vec2 vec= m_body->GetLinearVelocity();

		m_body->SetLinearVelocity(b2Vec2(0.0f, vec.y));

		
	}
}

void Boss_1_1::DestroyBossCore(void)
{
	//アンカーポイントボディがあれば
	if (GetAnchorPointBody() != nullptr)
	{
		Box2dWorld& box2d_world = Box2dWorld::GetInstance();//ワールドのインスタンスを取得する
		b2World* world = box2d_world.GetBox2dWorldPointer();//ワールドのポインタを持ってくる



		world->DestroyBody(GetAnchorPointBody());

		//テクスチャを終了
		panic_effect_sheet_cnt = 0;

		//nullをセット
		SetAnchorPointBody(nullptr);
	}
}

void Boss_1_1::JumpUpdate()
{
	if (now_boss_state == jump_state)
	{
		if (Jump_Start_Frame <= sheet_cnt && sheet_cnt <= Jump_End_Frame)
		{
			b2Body* boss_body =GetBossBody();

			int minus_flag=1;

			if (left_flag)
			{
				minus_flag = -1;
			}

			boss_body->SetLinearVelocity(b2Vec2(minus_flag* JumpPower.x, JumpPower.y));
		}

	}
}




void Boss_1_1::CreateChargeAttack(b2Vec2 attack_size, bool left)
{
	if (GetAttackBody() == nullptr) {

		//ボディのサイズをセット
		SetAttackDrawSize(attack_size);

		b2Vec2 size; //サイズのスケールを調整
		size.x = attack_size.x / BOX2D_SCALE_MANAGEMENT;
		size.y = attack_size.y / BOX2D_SCALE_MANAGEMENT;

		b2BodyDef body;
		body.type = b2_dynamicBody;

		b2Vec2 boss_pos = m_body->GetPosition();
		b2Vec2 boss_size = b2Vec2(GetBossRealSize().x/BOX2D_SCALE_MANAGEMENT, GetBossRealSize().y / BOX2D_SCALE_MANAGEMENT);

		if (left) {
			body.position.Set(boss_pos.x - (boss_size.x / 3) - (size.x / 2), boss_pos.y+boss_size.y/2-size.y/2);
		}
		else
		{
			body.position.Set(boss_pos.x + (boss_size.x / 3 ) + (size.x / 2), boss_pos.y + boss_size.y / 2-size.y/2);
		}
		body.angle = 0.0f;
		body.fixedRotation = true;//回転を固定にする
		body.userData.pointer = (uintptr_t)this;


		Box2dWorld& box2d_world = Box2dWorld::GetInstance();
		b2World* world = box2d_world.GetBox2dWorldPointer();

		//ワールドに登録
		b2Body* m_attack_body = world->CreateBody(&body);

		
		SetAttackBody(m_attack_body);

		//通常攻撃のフィクスチャ
		b2FixtureDef fixture;

		// クラス内に b2Shape をメンバーとして保持する場合の例
		b2PolygonShape shape; // クラスのメンバー変数として保持
		shape.SetAsBox(size.x * 0.5, size.y * 0.5);

		fixture.shape = &shape;//形を設定
		fixture.density = 0.1f;//密度
		fixture.friction = 0.0f;//摩擦
		fixture.restitution = 0.0f;//反発係数
		fixture.isSensor = true;//センサーかどうか

		b2Fixture* m_fixture = m_attack_body->CreateFixture(&fixture);


		ObjectData* boss_attack_data = new ObjectData{ collider_chage_attack };
		m_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(boss_attack_data);

		//プレイヤーとジョイントする
		b2WeldJointDef jointDef;
		jointDef.bodyA = m_body;//ボスのボディ
		jointDef.bodyB = GetAttackBody();//bossの攻撃のボディ

		if (left)//右かどうか
		{
			//boss側
			jointDef.localAnchorA.Set((-boss_size.x /3),boss_size.y/2-size.y/2);
			//攻撃側
			jointDef.localAnchorB.Set((size.x * 0.5), 0.0f);
		}
		else//左側
		{
			//boss側
			jointDef.localAnchorA.Set((boss_size.x /3), boss_size.y / 2 - size.y / 2);
			//攻撃側
			jointDef.localAnchorB.Set((-size.x * 0.5), 0.0f);
		}
		jointDef.collideConnected = true;//ジョイントした物体同士の接触を消す

		world->CreateJoint(&jointDef); //ワールドにジョイントを追加


		//地面を破壊
		boss_field_level++;

		//カメラシェイクスタート
		CameraShake::StartCameraShake(40, 0, 60);
		HitStop::SetHitStopFlag(15);

		

	}

}


void Boss_1_1::CreateShockWave(b2Vec2 attack_size, bool left)
{
	if (GetAttackBody() == nullptr) {

		//ボディのサイズをセット
		SetAttackDrawSize(attack_size);

		b2Vec2 size; //サイズのスケールを調整
		size.x = attack_size.x / BOX2D_SCALE_MANAGEMENT;
		size.y = attack_size.y / BOX2D_SCALE_MANAGEMENT;

		b2BodyDef body;
		body.type = b2_dynamicBody;
		body.gravityScale=(0.0f);//重力の影響をうけない

		b2Vec2 boss_pos = m_body->GetPosition();
		b2Vec2 boss_size = b2Vec2(GetBossRealSize().x / BOX2D_SCALE_MANAGEMENT, GetBossRealSize().y / BOX2D_SCALE_MANAGEMENT);

		if (left) {
			body.position.Set(boss_pos.x - (boss_size.x / 3) - (size.x / 2), boss_pos.y + boss_size.y / 2-size.y/2);
			ShockWaveLeftFlag = true;
		}
		else
		{
			body.position.Set(boss_pos.x + (boss_size.x / 3) + (size.x / 2), boss_pos.y + boss_size.y / 2-size.y / 2);
			ShockWaveLeftFlag = false;
		}
		body.angle = 0.0f;
		body.fixedRotation = true;//回転を固定にする
		body.userData.pointer = (uintptr_t)this;
		


		Box2dWorld& box2d_world = Box2dWorld::GetInstance();
		b2World* world = box2d_world.GetBox2dWorldPointer();

		//ワールドに登録
		b2Body* m_attack_body = world->CreateBody(&body);

		SetAttackBody(m_attack_body);

		//通常攻撃のフィクスチャ
		b2FixtureDef fixture;

		// クラス内に b2Shape をメンバーとして保持する場合の例
		b2PolygonShape shape; // クラスのメンバー変数として保持
		shape.SetAsBox(size.x * 0.5, size.y * 0.5);

		fixture.shape = &shape;//形を設定
		fixture.density = 0.0f;//密度
		fixture.friction = 0.0f;//摩擦
		fixture.restitution = 0.0f;//反発係数
		fixture.isSensor = true;//センサーかどうか

		b2Fixture* m_fixture = m_attack_body->CreateFixture(&fixture);


		ObjectData* boss_attack_data = new ObjectData{ collider_shock_wave };
		m_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(boss_attack_data);

		//カメラシェイクスタート
		CameraShake::StartCameraShake(0, 20, 40);
		HitStop::SetHitStopFlag(5);
	}
}


void Boss_1_1::ShockWaveUpdate(void)
{
	if (Shock_Wave_Fly_flag==true)
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

		//衝撃波の終了
		if (Shock_Wave_time_Frame < Now_Shock_Wave_time_Frame)
		{
			DeleteAttackBody();
			Shock_Wave_Fly_flag = false;//リセット処理
			Now_Shock_Wave_time_Frame = 0;//リセット処理
		}
	}
}



void Boss_1_1::CreateMiniGolem(b2Vec2 mini_golem_size, bool left)
{
	for(int i=0;i<2;i++)
	{
		if (GetMiniGolemBody(i) == nullptr&&Mini_golem_Create_flag==true) {

			//ボディのサイズをセット
			SetMiniGolemDrawSize(mini_golem_size);

			b2Vec2 size; //サイズのスケールを調整
			size.x = mini_golem_size.x / BOX2D_SCALE_MANAGEMENT;
			size.y = mini_golem_size.y / BOX2D_SCALE_MANAGEMENT;

			b2BodyDef body;
			body.type = b2_dynamicBody;
			b2Vec2 boss_pos = m_body->GetPosition();
			b2Vec2 boss_size = b2Vec2(GetBossDrawSize().x / BOX2D_SCALE_MANAGEMENT, GetBossDrawSize().y / BOX2D_SCALE_MANAGEMENT);

			if (left) {
				body.position.Set(boss_pos.x, boss_pos.y );
			}
			else
			{
				body.position.Set(boss_pos.x, boss_pos.y );
			}
			body.angle = 0.0f;
			body.fixedRotation = false;//回転を固定にする
			body.userData.pointer = (uintptr_t)this;



			Box2dWorld& box2d_world = Box2dWorld::GetInstance();
			b2World* world = box2d_world.GetBox2dWorldPointer();

			//ワールドに登録
			b2Body* m_mini_golem_body = world->CreateBody(&body);

			SetMiniGolemBody(m_mini_golem_body,i);

			//ミニゴーレムのフィクスチャ
			b2FixtureDef fixture;

			// クラス内に b2Shape をメンバーとして保持する場合の例
			b2CircleShape shape; // クラスのメンバー変数として保持
			shape.m_radius = size.y / 2;//画像が横に空白があるため　ｙを参照

			fixture.shape = &shape;//形を設定
			fixture.density = 1.0f;//密度
			fixture.friction = 0.5f;//摩擦
			fixture.restitution = 0.3f;//反発係数
			fixture.isSensor = false;//センサーかどうか
			fixture.filter= createFilterExclude("MiniGolem_filter", {"Boss_filter"});

			b2Fixture* m_fixture = m_mini_golem_body->CreateFixture(&fixture);


			ObjectData* mini_golem_data = new ObjectData{ collider_mini_golem };
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
			//ボディを持ってくる
			b2Body* mini_golem_body = GetMiniGolemBody(i);

			//プレイヤーの位置情報の定義
			b2Vec2 player_pos = PlayerPosition::GetPlayerPosition();
			
			
			//プレイヤーと左右どちらに行くかの調整
			if (player_pos.x < mini_golem_body->GetPosition().x)//プレイヤーの方が左
			{
				if (mini_golem_body->GetAngularVelocity() > -3)//最大回転量を制限
				{
					mini_golem_body->ApplyTorque(-0.1, true);
				}
			
			}
			else
			{
				if (mini_golem_body->GetAngularVelocity() < 3)//最大回転量を制限
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
		Box2dWorld& box2d_world = Box2dWorld::GetInstance();
		b2World* world = box2d_world.GetBox2dWorldPointer();

		b2Body* m_mini_golem_body = destroy_mini_golem_body;


		//作成エフェクト用の管理
		mini_golem_delete_effect_position = m_mini_golem_body->GetPosition();
		mini_golem_break_effect_cnt = 1;

		world->DestroyBody(m_mini_golem_body);

		for (int i = 0; i < 2; i++)
		{
			if (GetMiniGolemBody(i) == destroy_mini_golem_body)
			{
				SetMiniGolemBody(nullptr, i);
				destroy_mini_golem_flag = false;


				//カメラシェイクスタート
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

		Box2dWorld& box2d_world = Box2dWorld::GetInstance();
		b2World* world = box2d_world.GetBox2dWorldPointer();

		b2Body* m_destroy_body = GetAttackBody();

		world->DestroyBody(m_destroy_body);

		SetAttackBody(nullptr);

	}
}

void Boss_1_1::Draw()
{
	if (m_body != nullptr)
	{

		//文字の表示
		DrawBossDebug();
	/*	debugDraw();*/

		float scale = SCREEN_SCALE;

		// スクリーン中央位置 (16m x 9m の解像度で、中央は x = 8, y = 4.5 と仮定)
		b2Vec2 screen_center;
		screen_center.x = SCREEN_CENTER_X;
		screen_center.y = SCREEN_CENTER_Y;



		// コライダーの位置の取得（プレイヤーの位置）
		b2Vec2 boss_pos = GetBossBody()->GetPosition();
		b2Vec2 real_boss_size;
		real_boss_size.x = GetBossRealSize().x / BOX2D_SCALE_MANAGEMENT;
		real_boss_size.y = GetBossRealSize().y / BOX2D_SCALE_MANAGEMENT;

	

		// プレイヤー位置を考慮してスクロール補正を加える
		//取得したbodyのポジションに対してBox2dスケールの補正を加える
		float draw_x = ((boss_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
		float draw_y = ((boss_pos.y - PlayerPosition::GetPlayerPosition().y-(real_boss_size.y*0.7)) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


		switch (now_boss_state)
		{
		case wait_state:

			GetDeviceContext()->PSSetShaderResources(0, 1, &g_boss_walk_sheet1_Texture);

			DrawDividedSpriteBoss(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossDrawSize().x * scale, GetBossDrawSize().y * scale), 6, 6, 1, boss_alpha, left_flag);
			
			break;

		case panic_state:

			GetDeviceContext()->PSSetShaderResources(0, 1, &g_boss_panic_sheet_Texture);

			DrawDividedSpriteBoss(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossDrawSize().x * scale, GetBossDrawSize().y * scale), 7, 7, sheet_cnt, boss_alpha, left_flag);

			break;
		case jump_state:
			// シェーダリソースを設定
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
			// シェーダリソースを設定
			if (sheet_cnt < Max_Shock_Wave_Sheet / 2) {
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
			if (sheet_cnt < Max_Create_Mini_Golem_Sheet / 2) {
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
			if (sheet_cnt < Max_Charge_Attack_Sheet / 2) {
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_boss_charge_attack_sheet1_Texture);

				DrawDividedSpriteBoss(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossDrawSize().x * scale, GetBossDrawSize().y * scale), 10, 10, sheet_cnt, boss_alpha, left_flag);
			}
			else
			{
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_boss_charge_attack_sheet2_Texture);
				DrawDividedSpriteBoss(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossDrawSize().x * scale, GetBossDrawSize().y * scale), 10, 10, sheet_cnt - Max_Charge_Attack_Sheet / 2, boss_alpha, left_flag);
			}
			break;

		case walk_state://歩きモーション

			if (sheet_cnt < Max_Walk_Sheet / 2) {
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

		
		//コアをの描画
		if (GetAnchorPointBody() != nullptr)
		{
			//シェーダリソースを設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_debug_attack_color);

			// コライダーの位置の取得（プレイヤーの位置）
			b2Vec2 anchorpoint_pos = GetAnchorPointBody()->GetPosition();

			// プレイヤー位置を考慮してスクロール補正を加える
			//取得したbodyのポジションに対してBox2dスケールの補正を加える
			float anchor_point_draw_x = ((anchorpoint_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
			float anchor_point_draw_y = ((anchorpoint_pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

			DrawSprite(XMFLOAT2(anchor_point_draw_x, anchor_point_draw_y), 0.0f, XMFLOAT2(GetAnchorPointSize().x * scale, GetAnchorPointSize().y * scale));

		}

		//----------------------------------------------------------------------------------------
		//ミニゴーレムのDraw
		for (int i = 0; i < 2; i++)
		{
			if (GetMiniGolemBody(i) != nullptr)
			{

				//シェーダリソースを設定
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_mini_boss_Texture);

				// コライダーの位置の取得（プレイヤーの位置）
				b2Vec2 mini_golem_pos = GetMiniGolemBody(i)->GetPosition();

				// プレイヤー位置を考慮してスクロール補正を加える
				//取得したbodyのポジションに対してBox2dスケールの補正を加える
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
	//ボディのあたり判定を表示

	float scale = SCREEN_SCALE;

	// スクリーン中央位置 (16m x 9m の解像度で、中央は x = 8, y = 4.5 と仮定)
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;

	// シェーダリソースを設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_debug_color);

	// コライダーの位置の取得（プレイヤーの位置）
	b2Vec2 boss_pos = GetBossBody()->GetPosition();

	// プレイヤー位置を考慮してスクロール補正を加える
	//取得したbodyのポジションに対してBox2dスケールの補正を加える
	float draw_x = ((boss_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
	float draw_y = ((boss_pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;



	
	DrawSprite(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossDrawSize().x * scale, GetBossDrawSize().y * scale));

	DrawSprite(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossSensorSize().x * scale, GetBossSensorSize().y * scale));

	//----------------------------------------------------------------------------
	//実際のボディのサイズを表示

	if (GetBossBody() != nullptr)
	{

		//シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_debug_boss_body_color);

		// コライダーの位置の取得（プレイヤーの位置）
		b2Vec2 boss_pos = GetBossBody()->GetPosition();

		// プレイヤー位置を考慮してスクロール補正を加える
		//取得したbodyのポジションに対してBox2dスケールの補正を加える
		float boss_draw_x = ((boss_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
		float boss_draw_y = ((boss_pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

		DrawSprite(XMFLOAT2(boss_draw_x, boss_draw_y), 0.0f, XMFLOAT2(GetBossRealSize().x * scale, GetBossRealSize().y * scale));
	}
	//---------------------------------------------------------------------------
	if (GetAttackBody() != nullptr)
	{
		

			//シェーダリソースを設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_debug_attack_color);

			// コライダーの位置の取得（プレイヤーの位置）
			b2Vec2 attack_pos = GetAttackBody()->GetPosition();

			// プレイヤー位置を考慮してスクロール補正を加える
			//取得したbodyのポジションに対してBox2dスケールの補正を加える
			float attack_draw_x = ((attack_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
			float attack_draw_y = ((attack_pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

			DrawDividedSpriteBoss(XMFLOAT2(attack_draw_x, attack_draw_y), 0.0f, XMFLOAT2(GetAttackDrawSize().x * scale, GetAttackDrawSize().y * scale), 6, 4, 1, 1.0, left_flag);
		
	}

	

	
	
}

void Boss_1_1::UpdateEffectSheetCnt()
{

	//ピヨピヨ
	if (panic_effect_sheet_cnt != 0)
	{
		panic_effect_sheet_cnt += 0.5;

		if (Max_panic_effect_sheet_cnt<panic_effect_sheet_cnt)
		{
			panic_effect_sheet_cnt = 0;
		}

		
	}

	//チャージ攻撃の攻撃時のエフェクト
	if (charge_attack_effect_sheet_cnt != 0)
	{
		charge_attack_effect_sheet_cnt +=0.5;

		if (Max_charge_attack_effect_sheet_cnt < charge_attack_effect_sheet_cnt)
		{
			charge_attack_effect_sheet_cnt = 0;
		}

		
	}

	//チャージ中
	if (charge_effect_sheet_cnt != 0)
	{
		charge_effect_sheet_cnt += 0.5;

		if (Max_charge_effect_sheet_cnt < charge_effect_sheet_cnt)
		{
			charge_effect_sheet_cnt = 0;
		}

		
	}

	//衝撃波攻撃
	if (shock_wave_effect_sheet_cnt != 0)
	{
		shock_wave_effect_sheet_cnt += 0.5;

		if (Max_shock_wave_effect_sheet_cnt < shock_wave_effect_sheet_cnt)
		{
			shock_wave_effect_sheet_cnt = 0;
		}

		
	}


	//miniゴーレムの破壊時
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

	// スクリーン中央位置 (16m x 9m の解像度で、中央は x = 8, y = 4.5 と仮定)
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;
	//---------------------------------------------------------------------------
	//チャージ攻撃
	if (GetAttackBody() != nullptr)
	{
		if (now_boss_state == charge_attack_state)
		{
			if (charge_attack_effect_sheet_cnt !=0)
			{
				//シェーダリソースを設定
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_boss_charge_attack_effect);

				// コライダーの位置の取得（プレイヤーの位置）
				b2Vec2 attack_pos;
				attack_pos.x = GetAttackBody()->GetPosition().x;
				attack_pos.y= GetAttackBody()->GetPosition().y+ (GetAttackDrawSize().y/BOX2D_SCALE_MANAGEMENT/2);

				// プレイヤー位置を考慮してスクロール補正を加える
				//取得したbodyのポジションに対してBox2dスケールの補正を加える
				float attack_draw_x = ((attack_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
				float attack_draw_y = ((attack_pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


				
				

				DrawDividedSpriteBoss(XMFLOAT2(attack_draw_x, attack_draw_y), 0.0f, XMFLOAT2(GetAttackDrawSize().x * scale*7 , GetAttackDrawSize().y * scale*7), 5, 6, charge_attack_effect_sheet_cnt, effect_alpha, left_flag);
			}
		
		}
		else//ショックウェーブ
		{
			if (shock_wave_effect_sheet_cnt!=0)
			{
				//シェーダリソースを設定
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_boss_shock_wave_effect);

				// コライダーの位置の取得（プレイヤーの位置）
				b2Vec2 attack_pos = GetAttackBody()->GetPosition();

				// プレイヤー位置を考慮してスクロール補正を加える
				//取得したbodyのポジションに対してBox2dスケールの補正を加える
				float attack_draw_x = ((attack_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
				float attack_draw_y = ((attack_pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


				//これ貰ったスプライトが向きが反対だったから修正
				bool left = 1;
				if (ShockWaveLeftFlag)
				{
					left = 0;
				}

				DrawDividedSpriteBoss(XMFLOAT2(attack_draw_x, attack_draw_y), 0.0f, XMFLOAT2(GetAttackDrawSize().x * scale * 1.3, GetAttackDrawSize().y * scale * 1.3), 6, 4, shock_wave_effect_sheet_cnt, effect_alpha, left);
			}
		}
	}

	//ミニゴーレムの破壊
	if (mini_golem_break_effect_cnt != 0)
	{
		//シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_mini_golem_break_effect);

		// コライダーの位置の取得（プレイヤーの位置）
		b2Vec2 break_pos = mini_golem_delete_effect_position;

		// プレイヤー位置を考慮してスクロール補正を加える
		//取得したbodyのポジションに対してBox2dスケールの補正を加える
		float break_draw_x = ((break_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
		float break_draw_y = ((break_pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


	

		DrawDividedSpriteBoss(XMFLOAT2(break_draw_x, break_draw_y), 0.0f, XMFLOAT2(GetMiniGolemDrawSize().x * scale * 1.3*1.5, GetMiniGolemDrawSize().y * scale * 1.7*1.5), 4, 2, mini_golem_break_effect_cnt/4, effect_alpha, 1);
	}

	//ピヨピヨの表示
	if (panic_effect_sheet_cnt != 0)
	{
		//シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_boss_panic_effect);

		b2Vec2 panic_pos = m_body->GetPosition();

		// プレイヤー位置を考慮してスクロール補正を加える
		//取得したbodyのポジションに対してBox2dスケールの補正を加える
		float panic_draw_x = ((panic_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
		float panic_draw_y = ((panic_pos.y - PlayerPosition::GetPlayerPosition().y-((reality_boss_size.y/2)/BOX2D_SCALE_MANAGEMENT)) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;




		DrawDividedSpriteBoss(XMFLOAT2(panic_draw_x, panic_draw_y), 0.0f, XMFLOAT2(panic_effect_size.x * scale , panic_effect_size.y * scale ), 10, 13, panic_effect_sheet_cnt / 4, effect_alpha, 1);
	}
}

void Boss_1_1::Finalize()
{

	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();


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

b2Body* Boss_1_1::GetOutSideBody(void)
{
	return outside_boss_body;
}