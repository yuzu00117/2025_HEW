//-----------------------------------------------------------------------------------------------------
// #name enemy_floating.cpp
// #description 浮遊敵
// #make 2025/1/17　王泳心
// #update 2025/1/17 
// #comment 追加・修正予定
// 
// 
//----------------------------------------------------------------------------------------------------
#include "enemy_floating.h"
#include "sprite.h"
#include "texture.h"
#include "collider_type.h"
#include "player_position.h"
#include "tool.h"
#include "create_filter.h"


static ID3D11ShaderResourceView* g_EnemyFloating_Texture = NULL;	//動的エネミーの移動中のテクスチャ
static ID3D11ShaderResourceView* g_EnemyFloating_Explode_Texture = NULL;	//動的エネミーの爆発のテクスチャ
static ID3D11ShaderResourceView* g_EnemyFloating_Die_Texture = NULL;	//動的エネミーの死ぬ間際のテクスチャ

EnemyFloating::EnemyFloating(b2Vec2 position, b2Vec2 body_size, float angle)
	:Enemy(ENEMY_FLOATING_LIFE, ENEMY_FLOATING_DAMAGE, ENEMY_FLOATING_SPIRIT_TYPE, ENEMY_FLOATING_SCORE, true, false)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;							//静的なオブジェクトにするならture
	body.position.Set(position.x, position.y);			//ポジションをセット
	body.angle = angle;									//角度の定義
	body.userData.pointer = (uintptr_t)this;			//userDataのポインタを定義 
	body.fixedRotation = true;							//回転を固定する、　これをオンにすると回転しない
	body.gravityScale = 0.0f;


	Box2dWorld& box2d_world = Box2dWorld::GetInstance();//ワールドのインスタンスを取得する
	b2World* world = box2d_world.GetBox2dWorldPointer();//ワールドのポインタを持ってくる

	SetBody(world->CreateBody(&body));//Bodyをワールドに固定


	SetSize(body_size);//表示用にサイズをセットしとく、表示のときにGetSizeを呼び出す



	b2Vec2 size;
	size.x = body_size.x / BOX2D_SCALE_MANAGEMENT;//サイズを１にすると　1m*1mになるため　サイズをさげて、物理演算の挙動を操作しやすくする
	size.y = body_size.y / BOX2D_SCALE_MANAGEMENT;



	b2PolygonShape shape;                         //shapeには色々な型がある　サークルとかもあるよ
	shape.SetAsBox(size.x * 0.5f, size.y * 0.5f);//あたり判定を登録する4点　*0.5するのは

	b2FixtureDef fixture;
	fixture.shape = &shape;    //シャープをフィクスチャに登録する
	fixture.density = 1.0f;    //密度
	fixture.friction = 0.05f;  //摩擦
	fixture.restitution = 0.0f;//反発係数
	fixture.isSensor = true;  //センサーかどうか、trueならあたり判定は消える
	fixture.filter = createFilterExclude("enemy_filter", {});

	b2Fixture* enemy_floating_fixture = GetBody()->CreateFixture(&fixture);//Bodyをにフィクスチャを登録する


//プレイヤーを感知するセンサー
//--------------------------------------------
	b2PolygonShape sensor_shape;                         //shapeには色々な型がある　サークルとかもあるよ
	float one_square = 1.0f / BOX2D_SCALE_MANAGEMENT;	//　1マスのサイズ
	sensor_shape.SetAsBox( size.x * 0.5f + (one_square * 15), size.y * 0.5f + (one_square * 15) );//あたり判定を登録する4点　+((one_square * 7))fは幅が合計14マスが多くするため

	b2FixtureDef sensor_fixture;
	sensor_fixture.shape = &sensor_shape;    //シャープをフィクスチャに登録する
	sensor_fixture.density = 1.0f;    //密度
	sensor_fixture.friction = 0.0f;  //摩擦
	sensor_fixture.restitution = 0.0f;//反発係数
	sensor_fixture.isSensor = true;  //センサーかどうか、trueならあたり判定は消える

	b2Fixture* enemy_sensor_fixture = GetBody()->CreateFixture(&sensor_fixture);//Bodyをにフィクスチャを登録する


	// カスタムデータを作成して設定
	// 動的エネミーに値を登録
	// 動的エネミーにユーザーデータを登録
	ObjectData* data = new ObjectData{ collider_enemy_floating };
	ObjectData* sensor_data = new ObjectData{ collider_enemy_floating_sensor };
	enemy_floating_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(data);
	enemy_sensor_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(sensor_data);
	data->object_name = Object_Enemy_Floating;
	sensor_data->object_name = Object_Enemy_Floating;
	int ID = data->GenerateID();
	data->id = ID;
	sensor_data->id = ID;
	SetID(ID);

	SetState(ENEMY_FLOATING_STATE_IDLE);
}

void EnemyFloating::Initialize()
{
	if (g_EnemyFloating_Texture == NULL)
	{
		g_EnemyFloating_Texture = InitTexture(L"asset\\texture\\enemy_texture\\enemy_floating_moving.png");//動的エネミーの移動のテクスチャ
		g_EnemyFloating_Explode_Texture = InitTexture(L"asset\\texture\\boss_1_1\\mini_golem_break_effect.png");//動的エネミーの爆発のテクスチャ
		g_EnemyFloating_Die_Texture = InitTexture(L"asset\\texture\\enemy_texture\\enemy_floating_dead.png");//動的エネミーが死ぬテクスチャ
	}
}

void EnemyFloating::Finalize()
{

	//ワールドのインスタンスを持ってくる
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();


	if (GetBody() != nullptr)
	{
		world->DestroyBody(GetBody());
	}

	if (g_EnemyFloating_Texture) UnInitTexture(g_EnemyFloating_Texture);
	if (g_EnemyFloating_Explode_Texture) UnInitTexture(g_EnemyFloating_Explode_Texture);
	if (g_EnemyFloating_Die_Texture) UnInitTexture(g_EnemyFloating_Die_Texture);

}

void EnemyFloating::Update()
{
	if (GetUse() && m_sensed_player || m_state == ENEMY_FLOATING_STATE_EXPLODE)
	{
		switch (GetState())
		{
		case ENEMY_FLOATING_STATE_IDLE:
			break;
		case ENEMY_FLOATING_STATE_MOVE:
			Move();
			break;
		case ENEMY_FLOATING_STATE_EXPLODE:
			if (m_anim_id >= 7)
			{
				//ソウルを落とす
				ItemManager& item_manager = ItemManager::GetInstance();
				item_manager.AddSpirit(GetBody()->GetPosition(), { 2.0f,3.0f }, 0.0f, GetSpiritType(), false);

				//ワールドに登録したbodyの削除
				Box2dWorld& box2d_world = Box2dWorld::GetInstance();
				b2World* world = box2d_world.GetBox2dWorldPointer();
				world->DestroyBody(GetBody());
				SetBody(nullptr);

				//オブジェクトマネージャー内のエネミー削除
				ObjectManager& object_manager = ObjectManager::GetInstance();
				object_manager.DestroyEnemyFloating(GetID());	
			}
			break;
		case ENEMY_FLOATING_STATE_DIE:
			break;
		default:
			break;
		}

	}
	else if (!GetUse())
	{

		//エネミーが「死んだ時のサウンド
		app_atomex_start(Enemy_Knock_Down2_Sound);

		//ソウルを落とす
		ItemManager& item_manager = ItemManager::GetInstance();
		item_manager.AddSpirit(GetBody()->GetPosition(), { 2.0f,3.0f }, 0.0f, GetSpiritType(), false);


		b2Vec2 now_positon = GetBody()->GetPosition();
		b2Vec2 now_size = GetSize();
		b2Vec2 now_vec = GetRandomVelocity(47.0f);
		while (now_vec.x < 1.2f && now_vec.x >-1.2f)
		{
			now_vec = GetRandomVelocity(47.0f);
		}

		item_manager.AddDamageValue(b2Vec2{ now_positon.x, now_positon.y - 0.2f }, b2Vec2{ 1.0f,1.0f }, 0.0f, DamageOwnerType_enemy, ENEMY_FLOATING_LIFE);

		//ワールドに登録したbodyの削除
		Box2dWorld& box2d_world = Box2dWorld::GetInstance();
		b2World* world = box2d_world.GetBox2dWorldPointer();
		world->DestroyBody(GetBody());
		SetBody(nullptr);

		//オブジェクトマネージャー内のエネミー削除
		ObjectManager& object_manager = ObjectManager::GetInstance();
		object_manager.DestroyEnemyFloating(GetID());

		
		object_manager.AddChangeEnemyFilterAndBody(now_positon, b2Vec2(now_size.x * 2, now_size.y * 2), b2Vec2_zero, g_EnemyFloating_Die_Texture, 4, 4, now_vec);

	}
}

void EnemyFloating::Draw()
{
	if (GetBody() != nullptr)
	{
		// スケールをかけないとオブジェクトのサイズの表示が小さいから使う
		float scale = SCREEN_SCALE;

		// スクリーン中央位置 (プロトタイプでは乗算だったけど　今回から加算にして）
		b2Vec2 screen_center;
		screen_center.x = SCREEN_CENTER_X;
		screen_center.y = SCREEN_CENTER_Y;

		b2Vec2 position = GetBody()->GetPosition();

		// プレイヤー位置を考慮してスクロール補正を加える
		//取得したbodyのポジションに対してBox2dスケールの補正を加える
		float draw_x = ((position.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
		float draw_y = ((position.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

		switch (GetState())
		{
		case ENEMY_FLOATING_STATE_IDLE:
			//貼るテクスチャを指定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_EnemyFloating_Texture);

			DrawSplittingSprite(
				{ draw_x,
				  draw_y },
				GetBody()->GetAngle(),
				{ GetSize().x * scale ,GetSize().y * scale },
				5, 5, m_anim_id, 3.0f
			);
			m_anim_id++;
			m_anim_id = (int)m_anim_id % 25;
			break;
		case ENEMY_FLOATING_STATE_MOVE:
			//貼るテクスチャを指定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_EnemyFloating_Texture);

			DrawSplittingSprite(
				{ draw_x,
				  draw_y },
				GetBody()->GetAngle(),
				{ GetSize().x * scale ,GetSize().y * scale },
				5, 5, m_anim_id, 3.0f
			);
			m_anim_id++;
			m_anim_id = (int)m_anim_id % 25;
			break;
		case ENEMY_FLOATING_STATE_EXPLODE:
			//貼るテクスチャを指定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_EnemyFloating_Explode_Texture);
			DrawSplittingSprite(
				{ draw_x,
				  draw_y },
				GetBody()->GetAngle(),
				{ GetSize().x * scale * 2.0f ,GetSize().y * scale * 2.0f },
				4, 2, m_anim_id, 3.0f
			);
			m_anim_id+=0.2f;
			break;
		case ENEMY_FLOATING_STATE_DIE:
			//貼るテクスチャを指定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_EnemyFloating_Die_Texture);

			DrawSplittingSprite(
				{ draw_x,
				  draw_y },
				GetBody()->GetAngle(),
				{ GetSize().x * scale ,GetSize().y * scale },
				5, 5, m_anim_id, 3.0f
			);
			m_anim_id++;
			m_anim_id = (int)m_anim_id % 25;
			break;
		default:
			break;
		}

	}

}

void EnemyFloating::SetState(ENEMY_FLOATING_STATE state)
{
	if (m_state == ENEMY_FLOATING_STATE_EXPLODE)
	{
		return;
	}
	switch (state)
	{
	case ENEMY_FLOATING_STATE_IDLE:
		break;
	case ENEMY_FLOATING_STATE_MOVE:
		break;
	case ENEMY_FLOATING_STATE_EXPLODE:
		app_atomex_start(Enemy_MiniGolem_Explosion_Sound);//浮遊敵の破裂音
		// フィルターを変更
		updateFixtureFilter("enemy_filter", { "Player_filter" });
		m_anim_id = 0.0f;
		break;
	case ENEMY_FLOATING_STATE_DIE:
		m_anim_id = 0.0f;
		break;
	}
	m_state = state;
}

void EnemyFloating::updateFixtureFilter(const std::string& category, const std::vector<std::string>& includeMasks)
{
	// ボディの最初のフィクスチャを取得
	b2Fixture* fixture = GetBody()->GetFixtureList();

	// フィクスチャが存在しない場合は早期リターン
	if (!fixture)
	{
		return;
	}

	// 新しいフィルターを作成
	b2Filter newFilter = createFilterExclude(category, includeMasks);

	// すべてのフィクスチャに対してフィルターを更新
	while (fixture)
	{


		fixture->SetFilterData(newFilter);
		fixture = fixture->GetNext(); // 次のフィクスチャに移動
	}
}


//移動
void EnemyFloating::Move()
{
	if (GetBody() != nullptr)
	{
		//プレイヤー追従(簡易)
		//プレイヤーのポジション取得
		b2Vec2 player_position = PlayerPosition::GetPlayerPosition();

		//  プレイヤーへ向かうベクトル
		b2Vec2 vec;
		vec.x = player_position.x - GetBody()->GetPosition().x;
		vec.y = player_position.y - GetBody()->GetPosition().y;



		GetBody()->SetLinearVelocity(b2Vec2(vec.x * m_speed, vec.y * m_speed));

	}

}
