//-----------------------------------------------------------------------------------------------------
// #name enemy.h
// #description 静的エネミーのcppファイル
// #make 2024/11/19
// #update 2024/12/13
// #comment 追加・修正予定
//          ・
//           
//----------------------------------------------------------------------------------------------------

#include"include/box2d/box2d.h"
#include"enemy_static.h"
#include"world_box2d.h"
#include"texture.h"
#include"Xinput_controller.h"
#include"sprite.h"
#include"keyboard.h"
#include<Windows.h>
#include"player_position.h"
#include"contactlist.h"
#include"anchor_spirit.h"
#include"object_manager.h"
#include"Item_Manager.h"

static ID3D11ShaderResourceView* g_EnemyStatic_Texture = NULL;	//静的エネミーのテクスチャ

EnemyStatic::EnemyStatic(b2Vec2 position, b2Vec2 body_size, float angle)
	:Enemy(ENEMY_STATIC_LIFE, ENEMY_STATIC_DAMAGE, ENEMY_STATIC_SOULGAGE, ENEMY_STATIC_SCORE, true, false)
{
	b2BodyDef body;
	body.type = b2_staticBody;							//静的なオブジェクトにするならture
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
	ObjectData* data = new ObjectData{ collider_enemy_static };
	enemy_static_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(data);
	data->object_name = Object_Enemy_Static;
	int ID = data->GenerateID();
	data->id = ID;
	SetID(ID);
}

void EnemyStatic::Initialize()
{
	g_EnemyStatic_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_block.png");//静的エネミーのテクスチャ
}

void EnemyStatic::Finalize()
{

	//ワールドのインスタンスを持ってくる
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();


	if (GetBody() != nullptr)
	{
		world->DestroyBody(GetBody());
	}

	if (g_EnemyStatic_Texture != NULL)
	{
		UnInitTexture(g_EnemyStatic_Texture);
		g_EnemyStatic_Texture = NULL;
	}
}

void EnemyStatic::Update()
{
	if (GetUse() && GetInScreen())
	{

	}
	else if (!GetUse())
	{
		//ソウルを落とす
		ItemManager& item_manager = ItemManager::GetInstance();
		auto position = GetBody()->GetPosition();
		position.y -= GetSize().y / BOX2D_SCALE_MANAGEMENT / 2;
		item_manager.AddSpirit(position, {1.0f,2.0f}, 0.0f, GetSoulgage());

		//ワールドに登録したbodyの削除
		Box2dWorld& box2d_world = Box2dWorld::GetInstance();
		b2World* world = box2d_world.GetBox2dWorldPointer();
		world->DestroyBody(GetBody());

		//オブジェクトマネージャー内のエネミー削除
		ObjectManager& object_manager = ObjectManager::GetInstance();
		object_manager.DestroyEnemyStatic(GetID());
	}
}

void EnemyStatic::Draw()
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
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_EnemyStatic_Texture);

	//draw
	DrawSprite(
		{ draw_x,
		  draw_y },
		GetBody()->GetAngle(),
		{ GetSize().x * scale , GetSize().y * scale }
	);
}