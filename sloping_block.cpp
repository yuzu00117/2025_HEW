//-----------------------------------------------------------------------------------------------------
// #name sloping_block.cpp
// #description 傾斜のついたブロックのヘッダー
// #make 2024/12/04　永野義也
// #update 2024/12/04
// #comment 追加・修正予定
//          ・ないとおもうー
//
//----------------------------------------------------------------------------------------------------
#include"include/box2d/box2d.h"
#include"world_box2d.h"
#include"collider_type.h"
#include"sprite.h"
#include"texture.h"
#include"player_position.h"
#include"collider_type.h"
#include"player_position.h"
#include"sloping_block.h"

//テクスチャの入れ物
//グローバル変数
static ID3D11ShaderResourceView* g_sloping_block_right_down_Texture = NULL;//テクスチャ 右下
static ID3D11ShaderResourceView* g_sloping_block_left_down_Texture = NULL;//テクスチャ 　左した
static ID3D11ShaderResourceView* g_sloping_block_right_upper_Texture = NULL;//テクスチャ 　右上
static ID3D11ShaderResourceView* g_sloping_block_left_upper_Texture = NULL;//テクスチャ 　左上






sloping_block::sloping_block(b2Vec2 position, b2Vec2 size, SlopingBlockAspect aspect)
{

	//向きをセットしておく
	SetBlockAspect(aspect);

	//描画ようのサイズの設定
	SetSlopingBlockSize(size);




	//ワールドのインスタンスを持ってくる
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();


	//----------------------------------------------------------------------------//
	//一つ目のボディをつくる

	//サイズを設定する
	b2Vec2 sloping_block_size;
	sloping_block_size.x = size.x / BOX2D_SCALE_MANAGEMENT;
	sloping_block_size.y = size.y / BOX2D_SCALE_MANAGEMENT;


	b2BodyDef sloping_block_body;//斜面のブロック
	sloping_block_body.type = b2_staticBody;
	sloping_block_body.position.Set(position.x, position.y);
	sloping_block_body.fixedRotation = false;

	b2Body* m_sloping_block_body = world->CreateBody(&sloping_block_body);

	SetObjectSlopingBlockBody(m_sloping_block_body);



	b2PolygonShape sloping_block_shape;
	// 三角形のシェイプを定義
	b2PolygonShape triangleShape;
	b2Vec2 vertices[3] = {b2Vec2(0.0f,0.0f)};

	switch (aspect)
	{
	case right_down:
		vertices[0].Set(+sloping_block_size.x / 2, -sloping_block_size.y / 2); 
		vertices[1].Set(-sloping_block_size.x / 2, +sloping_block_size.y / 2); 
		vertices[2].Set(sloping_block_size.x / 2, sloping_block_size.y / 2);   
		break;
	case right_upper:
		vertices[0].Set(+sloping_block_size.x / 2, -sloping_block_size.y / 2);   
		vertices[1].Set(sloping_block_size.x / 2, sloping_block_size.y / 2);  
		vertices[2].Set(-sloping_block_size.x / 2, -sloping_block_size.y / 2);  
		break;
	case left_down:
		vertices[0].Set(-sloping_block_size.x / 2, sloping_block_size.y / 2); 
		vertices[1].Set(-sloping_block_size.x/2, -sloping_block_size.y/2); 
		vertices[2].Set(sloping_block_size.x, sloping_block_size.y); 
		break;
	case left_upper:
		vertices[0].Set(-sloping_block_size.x / 2, -sloping_block_size.y / 2); 
		vertices[1].Set(sloping_block_size.x / 2, -sloping_block_size.y / 2);
		vertices[2].Set(-sloping_block_size.x / 2, sloping_block_size.y / 2);
		break;
	default:
		break;
	}
	
	triangleShape.Set(vertices, 3); // 頂点を指定して三角形を設定

	b2FixtureDef sloping_block_fixture;

	sloping_block_fixture.shape = &triangleShape;
	sloping_block_fixture.density = 3.0f;
	sloping_block_fixture.friction = 0.5f;//摩擦
	sloping_block_fixture.restitution = 0.0f;//反発係数
	sloping_block_fixture.isSensor = false;//センサーかどうか、trueならあたり判定は消える

	b2Fixture* object_sloping_block_fixture = m_sloping_block_body->CreateFixture(&sloping_block_fixture);

	// カスタムデータを作成して設定
	ObjectData* object_sloping_block_data = new ObjectData{ collider_ground };//一旦地面判定
	object_sloping_block_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_sloping_block_data);


	int ID = object_sloping_block_data->GenerateID();//IDを取得
	object_sloping_block_data->id = ID;//フィクスチャに登録
	SetID(ID);//クラスに登録

};

sloping_block::~sloping_block()
{
}


void sloping_block::Initialize()
{
	//テクスチャの初期化
	g_sloping_block_left_down_Texture=	InitTexture(L"asset\\texture\\sample_texture\\sample_Sloping_block_left_down.png");//左下
	g_sloping_block_left_upper_Texture = InitTexture(L"asset\\texture\\sample_texture\\sample_Sloping_block_left_upper.png");//左上
	g_sloping_block_right_down_Texture = InitTexture(L"asset\\texture\\sample_texture\\sample_Sloping_block_right_down.png");//右下
	g_sloping_block_right_upper_Texture = InitTexture(L"asset\\texture\\sample_texture\\sample_Sloping_block_right_upper.png");//右上

}

void sloping_block::Update()
{
}

void sloping_block::Draw()
{
	// スケールをかけないとオブジェクトのサイズの表示が小さいから使う
	float scale = SCREEN_SCALE;

	// スクリーン中央位置 (プロトタイプでは乗算だったけど　今回から加算にして）
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;

	///ここから調整してね

	b2Vec2 sloping_block_pos = GetObjectSlopingBlockBody()->GetPosition();

	// プレイヤー位置を考慮してスクロール補正を加える
	//取得したbodyのポジションに対してBox2dスケールの補正を加える
	float draw_x = ((sloping_block_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
	float draw_y = ((sloping_block_pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


	switch (GetBlockAspect())//向きをコンストラクタでセットしてるのでそれを受け取って描画
	{
	case right_down:
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_sloping_block_right_down_Texture);
		break;
	case right_upper:
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_sloping_block_right_upper_Texture);
		break;
	case left_down:
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_sloping_block_left_down_Texture);
		break;
	case left_upper:
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_sloping_block_left_upper_Texture);
		break;
	default:
		break;
	}

	//draw
	DrawSprite(
		{ draw_x,
		  draw_y },
		GetObjectSlopingBlockBody()->GetAngle(),
		{ GetSlopingBlockSize().x * scale,GetSlopingBlockSize().y * scale }///サイズを取得するすべがない　フィクスチャのポインターに追加しようかな？ってレベル
	);
}

void sloping_block::Finalize()
{
}