//----------------------------------------------------------------------------------------------------
// #name　texture_blokc.cpp
// #description 背景に描画するテクスチャ
// #make 2025/02/09
// #update 2025/02/09
// #comment 追加・修正予定  
//         
//          
//----------------------------------------------------------------------------------------------------
#include"texture_block.h"
#include"sprite.h"
#include"texture.h"
#include"include/box2d/box2d.h"
#include"world_box2d.h"
#include"player_position.h"
#include"create_filter.h"
#include"collider_type.h"
#include"game.h"
#include"player.h"







Texture_block::Texture_block(b2Vec2 Position, b2Vec2 block_size, float texture_angle, ID3D11ShaderResourceView* texture)
{
	//サイズをセット
	SetSize(block_size);


	b2Vec2 size;

	size.x = block_size.x / BOX2D_SCALE_MANAGEMENT;
	size.y = block_size.y / BOX2D_SCALE_MANAGEMENT;



	//ワールドのインスタンスを持ってくる
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();





	b2BodyDef body;//ボディ
	body.type = b2_staticBody;
	body.position.Set(Position.x, Position.y);
	body.fixedRotation = true;

	b2Body* m_block_body = world->CreateBody(&body);

	SetBody(m_block_body);


	//フィクスチャを付ける
	b2PolygonShape block_shape;

	block_shape.SetAsBox(size.x * 0.5, size.y * 0.5);

	b2FixtureDef block_fixture;

	block_fixture.shape = &block_shape;
	block_fixture.density = 3.0f;
	block_fixture.friction = 0.5f;//摩擦
	block_fixture.restitution = 0.0f;//反発係数
	block_fixture.isSensor = true;//センサーかどうか、trueならあたり判定は消える
	block_fixture.filter = createFilterExclude("texture_body_filter", { "Player_filter" ,"object_filter","enemy_filter","ground_filter" });

	b2Fixture* object_fixture = m_body->CreateFixture(&block_fixture);


	// ユニークポインターを使って ObjectData を作成
	m_objectData = std::make_unique<ObjectData>(collider_UI_block);
	object_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(m_objectData.get());


	int ID = object_data->GenerateID();
	object_data->id = ID;

	SetID(ID);


	
	angle = texture_angle * M_PI / 180.0;
	m_texture = texture;
};

Texture_block::~Texture_block()
{
	Finalize();
}


void Texture_block::Initialize()
{

}

void Texture_block::Update()
{

}



void Texture_block::Draw()
{

	if (GetBody() != nullptr)
	{

		///ここから調整してね

		// スケールをかけないとオブジェクトのサイズの表示が小さいから使う
		float scale = SCREEN_SCALE;

		// スクリーン中央位置 (プロトタイプでは乗算だったけど　今回から加算にして）
		b2Vec2 screen_center;
		screen_center.x = SCREEN_CENTER_X;
		screen_center.y = SCREEN_CENTER_Y;

		b2Vec2 Pos = GetBody()->GetPosition();




		// プレイヤー位置を考慮してスクロール補正を加える
		//取得したbodyのポジションに対してBox2dスケールの補正を加える
		float draw_x = ((Pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
		float draw_y = ((Pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;




		GetDeviceContext()->PSSetShaderResources(0, 1, &m_texture);
		//draw
		DrawSprite(
			{ draw_x,
			  draw_y },
			GetBody()->GetAngle(),
			{ GetSize().x * scale,GetSize().y * scale }
		);

	}




}

void Texture_block::Finalize()
{
	if (!m_body) return;

	for (b2Fixture* fixture = m_body->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext()) {
		if (!fixture) continue;

		// UserData 取得


		// 無効なポインタならスキップ
		if (!fixture->GetUserData().pointer) {
			continue;
		}


		// ObjectData を削除す
		fixture->GetUserData().pointer = 0;  // ポインタのクリア
	}

	// `b2Body` を削除
	Box2dWorld::GetInstance().GetBox2dWorldPointer()->DestroyBody(m_body);
	m_body = nullptr;




}