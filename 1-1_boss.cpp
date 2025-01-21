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


// 使用するテクスチャファイルを格納
static ID3D11ShaderResourceView* g_mini_boss_Texture = NULL;//小さなゴーレムのテクスチャ
static ID3D11ShaderResourceView* g_boss_shock_wave_sheet1_Texture = NULL;//衝撃波のテクスチャ１
static ID3D11ShaderResourceView* g_boss_shock_wave_sheet2_Texture = NULL;//衝撃波のテクスチャ２
static ID3D11ShaderResourceView* g_boss_charge_attack_sheet1_Texture = NULL;//溜め攻撃のテクスチャ１
static ID3D11ShaderResourceView* g_boss_charge_attack_sheet2_Texture = NULL;//ため攻撃のテクスチャ２
static ID3D11ShaderResourceView* g_mini_boss_create_sheet1_Texture = NULL;//小さなゴーレムを生成する際のボス側１
static ID3D11ShaderResourceView* g_mini_boss_create_sheet2_Texture = NULL;//小さなゴーレムを生成する際のボス側２
static ID3D11ShaderResourceView* g_mini_boss_walk_sheet1_Texture = NULL;//ゴーレムの歩きモーション１
static ID3D11ShaderResourceView* g_mini_boss_walk_sheet2_Texture = NULL;//ゴーレムの歩きモーション２



//-------------------------------------------------------------------------------------------
//デバック用の画像
static ID3D11ShaderResourceView* g_debug_color = NULL;//デバック用

static ID3D11ShaderResourceView* g_debug_attack_color = NULL;//デバック用




Boss_1_1::Boss_1_1()
{



}

Boss_1_1::~Boss_1_1()
{
}

void Boss_1_1::Initialize(b2Vec2 position, b2Vec2 bodysize,bool left)
{
	g_mini_boss_Texture = InitTexture(L"asset\\texture\\boss_1_1\\mini_boss.png");//ミニゴーレムのインクルード
	g_boss_shock_wave_sheet1_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_shock_wave_sheet1.png");//衝撃波攻撃のインクルードシート１
	g_boss_shock_wave_sheet2_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_shock_wave_sheet2.png");//衝撃波攻撃のインクルードシート２
	g_boss_charge_attack_sheet1_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_charge_sheet1.png");//ため攻撃のシート１
	g_boss_charge_attack_sheet2_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_charge_sheet2_.png");//ため攻撃のシート２
	g_mini_boss_create_sheet1_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_create_mini_golem_sheet1.png");//ミニゴーレムの生成のボス側１
	g_mini_boss_create_sheet2_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_create_mini_golem_sheet2_.png");//ミニゴーレムの生成のボス側２
	g_mini_boss_walk_sheet1_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_walk_sheet1.png");//ゴーレムの歩きモーション１
	g_mini_boss_walk_sheet2_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_walk_sheet2.png");//ゴーレムの歩きモーション２


	//デバック用
	g_debug_color = InitTexture(L"asset\\texture\\sample_texture\\img_sensor.png");//sensorのテクスチャ
	g_debug_attack_color = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_red.png");


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


	b2PolygonShape body_shape;




	b2Vec2 vertices[4] = { b2Vec2(0.0f,0.0f) };

	if (left == true)
	{
		// 反時計回りで頂点を設定
		vertices[0].Set(-size.x / 3, size.y / 2);  // 左下
		vertices[1].Set(size.x / 2, size.y / 2);   // 右下
		vertices[2].Set(size.x / 2, 0.0);    // 右上
		vertices[3].Set(-size.x / 3, 0.0);   // 左上
	}
	else
	{
		// 反時計回りで頂点を設定
		vertices[0].Set(-size.x / 2, size.y / 2);  // 左下
		vertices[1].Set(size.x / 3, size.y / 2);   // 右下
		vertices[2].Set(size.x / 3, 0.0);    // 右上
		vertices[3].Set(-size.x / 2, 0.0);   // 左上
	}


	body_shape.Set(vertices, 4);

	b2FixtureDef body_fixture;
	body_fixture.shape = &body_shape;
	body_fixture.friction = 0.0f;//摩擦
	body_fixture.restitution = 0.1f;//反発係数
	body_fixture.density = 0.1f;
	body_fixture.isSensor = false;//センサーかどうか、trueならあたり判定は消える
	body_fixture.filter = createFilterExclude("enemy_filter", {});


	b2Fixture* m_body_fixture = m_body->CreateFixture(&body_fixture);


	now_boss_state = charge_attack_state;

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

		if (old_left_flag != left_flag)
		{
			Initialize(GetBossBody()->GetPosition(), b2Vec2(18.f, 27.f), left_flag);
		}

		old_left_flag = left_flag;
		//-------------------------------------------------------------------------------------------------------------------------------

		//衝撃波の更新処理
		ShockWaveUpdate();







		switch (now_boss_state)
		{
		case wait_state:
			break;
		case walk_state:
			sheet_cnt += 0.5;
			if (Max_Walk_Sheet <= sheet_cnt)
			{
				sheet_cnt = 0;
			}

			m_body->SetLinearVelocity(b2Vec2(-0.5f, 0.0f));


			break;
		case shock_wave_state:
			sheet_cnt += 0.5;
			
			if (static_cast<int>(sheet_cnt) == Shock_Wave_Start_Frame)
			{
				CreateShockWave(b2Vec2(1.0f, 4.0f), left_flag);
				Shock_Wave_Fly_flag = true;
			}
			

			
			


			if (Max_Shock_Wave_Sheet <= sheet_cnt)
			{
				sheet_cnt = 0;
				now_boss_state = create_mini_golem_state;
			}


			break;
		case create_mini_golem_state:
			sheet_cnt += 0.5;
			if (Max_Create_Mini_Golem_Sheet <= sheet_cnt)
			{
				sheet_cnt = 0;
				now_boss_state = charge_attack_state;
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
			}
			if (static_cast<int>(sheet_cnt) == Charge_Attack_End_Frame)
			{
				DeleteAttackBody();
			}
			
			//モーションが完了した
			if (Max_Charge_Attack_Sheet <= sheet_cnt)
			{
				sheet_cnt = 0;
				now_boss_state = shock_wave_state;
			}
			break;
		default:
			break;

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
		b2Vec2 boss_size = b2Vec2(GetBossDrawSize().x/BOX2D_SCALE_MANAGEMENT, GetBossDrawSize().y / BOX2D_SCALE_MANAGEMENT);

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


		ObjectData* boss_attack_data = new ObjectData{ collider_object };
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
		b2Vec2 boss_size = b2Vec2(GetBossDrawSize().x / BOX2D_SCALE_MANAGEMENT, GetBossDrawSize().y / BOX2D_SCALE_MANAGEMENT);

		if (left) {
			body.position.Set(boss_pos.x - (boss_size.x / 3) - (size.x / 2), boss_pos.y + boss_size.y / 2-size.y/2);
		}
		else
		{
			body.position.Set(boss_pos.x + (boss_size.x / 3) + (size.x / 2), boss_pos.y + boss_size.y / 2-size.y / 2);
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


	/*	ObjectData* boss_attack_data = new ObjectData{ collider_object };
		m_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(boss_attack_data);*/

		//プレイヤーとジョイントする
		b2WeldJointDef jointDef;
		jointDef.bodyA = m_body;//ボスのボディ
		jointDef.bodyB = GetAttackBody();//bossの攻撃のボディ

		

	}
}


void Boss_1_1::ShockWaveUpdate(void)
{
	if (Shock_Wave_Fly_flag==true)
	{
		if (GetAttackBody() != nullptr)
		{
			float minus_flag = 1;
			if (left_flag == true)
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
		float scale = SCREEN_SCALE;

		// スクリーン中央位置 (16m x 9m の解像度で、中央は x = 8, y = 4.5 と仮定)
		b2Vec2 screen_center;
		screen_center.x = SCREEN_CENTER_X;
		screen_center.y = SCREEN_CENTER_Y;



		// コライダーの位置の取得（プレイヤーの位置）
		b2Vec2 boss_pos = GetBossBody()->GetPosition();

		// プレイヤー位置を考慮してスクロール補正を加える
		//取得したbodyのポジションに対してBox2dスケールの補正を加える
		float draw_x = ((boss_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
		float draw_y = ((boss_pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;




	

		switch (now_boss_state)
		{
		case wait_state:
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
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_mini_boss_walk_sheet1_Texture);

				DrawDividedSpriteBoss(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossDrawSize().x * scale, GetBossDrawSize().y * scale), 6, 6, sheet_cnt, boss_alpha, left_flag);
			}
			else
			{
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_mini_boss_walk_sheet2_Texture);
				DrawDividedSpriteBoss(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossDrawSize().x * scale, GetBossDrawSize().y * scale), 6, 6, sheet_cnt - Max_Walk_Sheet / 2, boss_alpha, left_flag);
			}
			break;

		default:
			break;
		}






		debugDraw();
	}
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



	//---------------------------------------------------------------------------
	//攻撃判定のDraw

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

		DrawSprite(XMFLOAT2(attack_draw_x, attack_draw_y), 0.0f, XMFLOAT2(GetAttackDrawSize().x * scale, GetAttackDrawSize().y * scale));
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