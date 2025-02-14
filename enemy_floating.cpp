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


static ID3D11ShaderResourceView* g_EnemyFloating_Texture = NULL;	//動的エネミーのテクスチャ

EnemyFloating::EnemyFloating(b2Vec2 position, b2Vec2 body_size, float angle)
	:Enemy(ENEMY_FLOATING_LIFE, ENEMY_FLOATING_DAMAGE, ENEMY_FLOATING_SOULGAGE, ENEMY_FLOATING_SCORE, true, false)
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

	b2Fixture* enemy_floating_fixture = GetBody()->CreateFixture(&fixture);//Bodyをにフィクスチャを登録する

//プレイヤーを感知するセンサー
//--------------------------------------------
	b2PolygonShape sensor_shape;                         //shapeには色々な型がある　サークルとかもあるよ
	float one_square = 1.0f / BOX2D_SCALE_MANAGEMENT;	//　1マスのサイズ
	sensor_shape.SetAsBox( size.x * 0.5f + (one_square * 7), size.y * 0.5f + (one_square * 7) );//あたり判定を登録する4点　+((one_square * 7))fは幅が合計14マスが多くするため

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
}

void EnemyFloating::Initialize()
{
	if (g_EnemyFloating_Texture == NULL)
	{
		g_EnemyFloating_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_yellow.png");//動的エネミーのテクスチャ
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

	if (g_EnemyFloating_Texture != NULL)
	{
		UnInitTexture(g_EnemyFloating_Texture);
		g_EnemyFloating_Texture = NULL;
	}
}

void EnemyFloating::Update()
{
	if (GetUse() && m_sensed_player)
	{
		Move();
	}
	else if (!GetUse())
	{
		//ソウルを落とす
		ItemManager& item_manager = ItemManager::GetInstance();
		item_manager.AddSpirit(GetBody()->GetPosition(), { 1.0f,2.0f }, 0.0f, GetSoulgage(), false);

		//ワールドに登録したbodyの削除
		Box2dWorld& box2d_world = Box2dWorld::GetInstance();
		b2World* world = box2d_world.GetBox2dWorldPointer();
		world->DestroyBody(GetBody());

		//オブジェクトマネージャー内のエネミー削除
		ObjectManager& object_manager = ObjectManager::GetInstance();
		object_manager.DestroyEnemyFloating(GetID());
	}
}

void EnemyFloating::Draw()
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

	//貼るテクスチャを指定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_EnemyFloating_Texture);

	//draw
	DrawSprite(
		{ draw_x,
		  draw_y },
		GetBody()->GetAngle(),
		{ GetSize().x * scale , GetSize().y * scale }
	);
}

//移動
void EnemyFloating::Move()
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
