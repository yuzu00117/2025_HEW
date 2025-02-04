// #name teleport_block.cpp
// #description 接触したら指定した場所に飛ぶブロック
// #make 2024/01/02
// #update 2024/01/02
// #comment 追加・修正予定
//         
//          
//----------------------------------------------------------------------------------------------------

#include"contact_block.h"
#include"sprite.h"
#include"texture.h"
#include"include/box2d/box2d.h"
#include"world_box2d.h"
#include"player_position.h"
#include"create_filter.h"
#include"collider_type.h"
#include"game.h"


static ID3D11ShaderResourceView* g_Teleport_Block_Texture = NULL;//地面のテクスチャ


teleport_block::teleport_block(b2Vec2 Position, b2Vec2 teleport_block_size, b2Vec2 to_position)
{
	//サイズをセット
	SetTeleportBlockSize(teleport_block_size);

	b2Vec2 size;

	size.x = teleport_block_size.x / BOX2D_SCALE_MANAGEMENT;
	size.y = teleport_block_size.y / BOX2D_SCALE_MANAGEMENT;






	//ワールドのインスタンスを持ってくる
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();





	b2BodyDef body;//木の幹の部分
	body.type = b2_staticBody;
	body.position.Set(Position.x, Position.y);
	body.fixedRotation = true;

	b2Body* m_teleport_block_body = world->CreateBody(&body);

	SetTeleportBlockBody(m_teleport_block_body);


	//フィクスチャを付ける
	b2PolygonShape teleport_block_shape;

	teleport_block_shape.SetAsBox(size.x * 0.5, size.y * 0.5);

	b2FixtureDef teleport_block_fixture;

	teleport_block_fixture.shape = &teleport_block_shape;
	teleport_block_fixture.density = 3.0f;
	teleport_block_fixture.friction = 0.5f;//摩擦
	teleport_block_fixture.restitution = 0.0f;//反発係数
	teleport_block_fixture.isSensor = false;//センサーかどうか、trueならあたり判定は消える
	teleport_block_fixture.filter = createFilterExclude("object_filter", {});

	b2Fixture* object_teleport_fixture = m_body->CreateFixture(&teleport_block_fixture);

	// カスタムデータを作成して設定
	ObjectData* object_teleport_data = new ObjectData{ collider_teleport_block };//一旦壁判定
	object_teleport_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_teleport_data);





	int ID = object_teleport_data->GenerateID();
	object_teleport_data->id = ID;
	object_teleport_data->object_name = Object_teleport_block;
	SetID(ID);


	//テレポートブロックの所にテレポート先を入れる
	SetTeleportPoint(to_position);



};

teleport_block::~teleport_block()
{
}


void teleport_block::Initialize()
{
	//アンカーの錨の部分（日本語）
	g_Teleport_Block_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sensor.png");
	\

}

void teleport_block::Update()
{
	if (GetTeleportFlag() == true)
	{
		Game instance = Game::GetInstance();

		instance.Teleport_player(GetTeleportPoint());

		SetTeleportFlag(false);
	}

}



void teleport_block::Draw()
{

	///ここから調整してね




	// スケールをかけないとオブジェクトのサイズの表示が小さいから使う
	float scale = SCREEN_SCALE;

	// スクリーン中央位置 (プロトタイプでは乗算だったけど　今回から加算にして）
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;

	b2Vec2 Pos = GetTeleportBlockBody()->GetPosition();




	// プレイヤー位置を考慮してスクロール補正を加える
	//取得したbodyのポジションに対してBox2dスケールの補正を加える
	float draw_x = ((Pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
	float draw_y = ((Pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Teleport_Block_Texture);

	//draw
	DrawSprite(
		{ draw_x,
		  draw_y },
		GetTeleportBlockBody()->GetAngle(),
		{ GetTeleportBlockSize().x * scale,GetTeleportBlockSize().y * scale }///サイズを取得するすべがない　フィクスチャのポインターに追加しようかな？ってレベル
	);




}

void teleport_block::Finalize()
{
	//ワールドのインスタンスを持ってくる
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();

	if (GetTeleportBlockBody() != nullptr)
	{
		//ボディの削除
		world->DestroyBody(m_body);
	}

	//画像の解放

	if (g_Teleport_Block_Texture != NULL)
	{
		UnInitTexture(g_Teleport_Block_Texture);
		g_Teleport_Block_Texture = NULL;
	}

}