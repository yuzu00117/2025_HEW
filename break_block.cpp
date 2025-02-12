#include"break_block.h"
#include"texture.h"
#include"sprite.h"
#include"world_box2d.h"
#include"collider_type.h"
#include"player_position.h"
#include"1-1_boss.h"
#include"create_filter.h"
#include"tool.h"



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
	block_fixture.isSensor = false;//センサーかどうか、trueならあたり判定は消える
	block_fixture.filter = createFilterExclude("object_filter", {});

	b2Fixture* object_fixture = m_body->CreateFixture(&block_fixture);

	// カスタムデータを作成して設定
	ObjectData* object_data = new ObjectData{ collider_break_block };
	object_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_data);




	int ID = object_data->GenerateID();
	object_data->id = ID;

	SetID(ID);


	Texture = g_Texture;
	

};

Break_Block::~Break_Block()
{
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




		}

	}
}



void Break_Block::Draw()
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


	GetDeviceContext()->PSSetShaderResources(0, 1, &Texture);

	//draw
	DrawSprite(
		{ draw_x,
		  draw_y },
		angle,
		{ GetSize().x * scale,GetSize().y * scale }
	);





}

void Break_Block::Finalize()
{
	//ワールドのインスタンスを持ってくる
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();

	if (GetBody() != nullptr)
	{
		//ボディの削除
		world->DestroyBody(m_body);
	}

	//画像の解放

	if (Texture != NULL)
	{
		UnInitTexture(Texture);
		Texture = NULL;
	
	}

}