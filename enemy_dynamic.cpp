//-----------------------------------------------------------------------------------------------------
// #name enemyDynamic.h
// #description 動的エネミー(プレイヤー追従)のcppファイル
// #make 2024/11/20
// #update 2024/12/13
// #comment 追加・修正予定
//          ・ステータス調整
//           
//----------------------------------------------------------------------------------------------------

#include"include/box2d/box2d.h"
#include"enemy_dynamic.h"
#include"world_box2d.h"
#include"texture.h"
#include"Xinput_controller.h"
#include"sprite.h"
#include"keyboard.h"
#include<Windows.h>
#include"player_position.h"
#include"player_stamina.h"
#include"contactlist.h"
#include"anchor_spirit.h"
#include"Item_Manager.h"

static ID3D11ShaderResourceView* g_EnemyDynamic_Texture = NULL;	//動的エネミーのテクスチャ

EnemyDynamic::EnemyDynamic(b2Vec2 position, b2Vec2 body_size, float angle)
	:Enemy(ENEMY_DYNAMIC_LIFE, ENEMY_DYNAMIC_DAMAGE, ENEMY_DYNAMIC_SOULGAGE, ENEMY_DYNAMIC_SCORE, true, false)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;							//静的なオブジェクトにするならture
	body.position.Set(position.x, position.y);			//ポジションをセット
	body.angle = angle;									//角度の定義
	body.userData.pointer = (uintptr_t)this;			//userDataのポインタを定義 
	body.fixedRotation = true;							//回転を固定する、　これをオンにすると回転しない


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
	fixture.isSensor = false;  //センサーかどうか、trueならあたり判定は消える

	b2Fixture* enemy_static_fixture = GetBody()->CreateFixture(&fixture);//Bodyをにフィクスチャを登録する

	// カスタムデータを作成して設定
	// 動的エネミーに値を登録
	// 動的エネミーにユーザーデータを登録
	ObjectData* data = new ObjectData{ collider_enemy_dynamic };
	enemy_static_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(data);
	data->object_name = Object_Enemy_Dynamic;
	int ID = data->GenerateID();
	data->id = ID;
	SetID(ID);
}

void EnemyDynamic::Initialize()
{
	g_EnemyDynamic_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_yellow.png");//動的エネミーのテクスチャ
}

void EnemyDynamic::Finalize()
{

	//ワールドのインスタンスを持ってくる
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();


	if (GetBody() != nullptr)
	{
		world->DestroyBody(GetBody());
	}
	UnInitTexture(g_EnemyDynamic_Texture);
}

void EnemyDynamic::Update()
{
	if (GetUse() && GetInScreen())
	{
		Move();
	}
	else if (!GetUse())
	{
		//ソウルを落とす
		ItemManager& item_manager = ItemManager::GetInstance();
		item_manager.AddSpirit(GetBody()->GetPosition(), { 1.0f,2.0f }, 0.0f, GetSoulgage());

		//ワールドに登録したbodyの削除
		Box2dWorld& box2d_world = Box2dWorld::GetInstance();
		b2World* world = box2d_world.GetBox2dWorldPointer();
		world->DestroyBody(GetBody());

		//オブジェクトマネージャー内のエネミー削除
		ObjectManager& object_manager = ObjectManager::GetInstance();
		object_manager.DestroyEnemyDynamic(GetID());
	}
}

void EnemyDynamic::Draw()
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
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_EnemyDynamic_Texture);

	//draw
	DrawSprite(
		{ draw_x,
		  draw_y },
		GetBody()->GetAngle(),
		{ GetSize().x * scale , GetSize().y * scale }
	);
}

//移動
void EnemyDynamic::Move()
{
	//プレイヤー追従(簡易)
	//プレイヤーのポジション取得
	b2Vec2 player_position;
	player_position.x = PlayerPosition::GetPlayerPosition().x;
	player_position.y = PlayerPosition::GetPlayerPosition().y;

	//移動方向
	b2Vec2 enemy_vector;
	enemy_vector.x = player_position.x - GetBody()->GetPosition().x;
	enemy_vector.y = player_position.y - GetBody()->GetPosition().y;

	//移動量
	b2Vec2 enemy_move;
	enemy_move.x = (enemy_vector.x * m_speed) / 5;
	enemy_move.y = (enemy_vector.y * m_speed) / 5;

	if (GetBody()->GetLinearVelocity() != b2Vec2(0.0, 0.0))
	{
		GetBody()->ApplyLinearImpulseToCenter(b2Vec2(enemy_move.x, 0.0), true);
	}
	else
	{
		GetBody()->ApplyLinearImpulseToCenter(b2Vec2(enemy_move.x, -0.02), true);
	}
}