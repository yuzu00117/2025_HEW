// #name　break_block.cpp
// #description オブジェクトと接触したら壊れるブロック
// #make 2025/02/06
// #update 2025/02/06
// #comment 追加・修正予定  豪快な感じがする
//         
//          
//----------------------------------------------------------------------------------------------------

#include"break_block.h"
#include"texture.h"
#include"sprite.h"
#include"world_box2d.h"
#include"collider_type.h"
#include"player_position.h"
#include"1-1_boss.h"
#include"create_filter.h"
#include"tool.h"
#include"break_effect.h"
#include"camera_shake.h"


//オブジェクトに触れたら壊れるオブジェクト
Break_Block::Break_Block(b2Vec2 Position, b2Vec2 block_size, int divisions_x, int divisions_y, float angle, ID3D11ShaderResourceView* g_Texture)
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
	body.angle = angle * M_PI / 180.0;

	b2Body* m_block_body = world->CreateBody(&body);

	SetBody(m_block_body);


	//フィクスチャを付ける
	b2PolygonShape block_shape;

	block_shape.SetAsBox(size.x * 0.5, size.y * 0.5);

	b2FixtureDef block_fixture;

	block_fixture.shape = &block_shape;
	block_fixture.density = 1.0f;
	block_fixture.friction = 0.5f;//摩擦
	block_fixture.restitution = 0.0f;//反発係数
	block_fixture.isSensor = true;//センサーかどうか、trueならあたり判定は消える
	block_fixture.filter = createFilterExclude("break_block_body_filter", {});

	b2Fixture* object_fixture = m_body->CreateFixture(&block_fixture);

	

	// ユニークポインターを使って ObjectData を作成
	m_objectData = std::make_unique<ObjectData>(collider_break_block);
	object_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(m_objectData.get());

	b2FixtureDef player_fixture;

	player_fixture.shape = &block_shape;
	player_fixture.density = 1.0f;
	player_fixture.friction = 0.5f;//摩擦
	player_fixture.restitution = 0.0f;//反発係数
	player_fixture.isSensor = false;//センサーかどうか、trueならあたり判定は消える
	player_fixture.filter = createFilterExclude("break_block_body_filter", {"object_filter"});

	b2Fixture* m_player_fixture = m_body->CreateFixture(&player_fixture);

	// ユニークポインターを使って ObjectData を作成
	m_player_objectData = std::make_unique<ObjectData>(collider_break_block);
	object_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(m_player_objectData.get());



	int ID = m_objectData->GenerateID();
	m_objectData->id = ID;
	m_player_objectData->id = ID;

	SetID(ID);


	Texture = g_Texture;
	

};

Break_Block::~Break_Block()
{
	Finalize();
}


void Break_Block::Initialize()
{


}

void Break_Block::Update()
{
	if (m_body != nullptr)
	{
		if (m_flag == true)
		{
			PillarFragmentsManager::GetInstance().Destroy_Splitting(m_body, Texture, GetSize());

			SetBody(nullptr);

			CameraShake::StartCameraShake(20, 20, 20);

			m_flag = false;
		}

	}
}



void Break_Block::Draw()
{

	///ここから調整してね

	if (m_body != nullptr)
	{
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


		GetDeviceContext()->PSSetShaderResources(0, 1, &Texture);

		//draw
		DrawSprite(
			{ draw_x,
			  draw_y },
			angle,
			{ GetSize().x * scale,GetSize().y * scale }
		);

	}



}

void Break_Block::Finalize()
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