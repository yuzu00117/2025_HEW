//-----------------------------------------------------------------------------------------------------
// #name ground.cpp
// #description 地面をつくる
// #make 2024/11/16
// #update 2024/12/03
// #comment 追加・修正予定
//          ・Filedのクラスを継承して作成　
//			・フィクスチャを複数設定して壁の判定と地面の判定を設定したい
//           
//----------------------------------------------------------------------------------------------------
#include"ground.h"
#include"include/box2d/box2d.h"
#include"world_box2d.h"
#include"create_filter.h"
#include"contactlist.h"
#include"player_position.h"
#include"create_filter.h"
#include"collider_type.h"


/**
 * @brief  フィールドのコンストラクタ
 * @param position  登録する座標
 * @param body_size ボディの大きさ、関数中で大きさの調整をする
 * @param angle     角度を設定
 * @param bFixed
 * @param is_sensor
 * @param texture
 */
Ground::Ground(b2Vec2 position, b2Vec2 body_size, float angle, bool bFixed, bool is_sensor, ID3D11ShaderResourceView* texture, bool object_sensor)
{
	//テクスチャをセット
	g_texture=texture;


	b2BodyDef body;
	body.type = bFixed ? b2_staticBody : b2_dynamicBody;	//静的なオブジェクトにするならture
	body.position.Set(position.x, position.y);			//ポジションをセット
	body.angle = angle;									//角度の定義
	body.userData.pointer = (uintptr_t)this;			//userDataのポインタを定義 
	body.fixedRotation = true;							//回転を固定する、　これをオンにすると回転しない


	Box2dWorld& box2d_world = Box2dWorld::GetInstance();//ワールドのインスタンスを取得する
	b2World* world = box2d_world.GetBox2dWorldPointer();//ワールドのポインタを持ってくる

	m_body = world->CreateBody(&body);

	SetBody(m_body);//Bodyをワールドに固定


	SetSize(body_size);//表示用にサイズをセットしとく、表示のときにGetSizeを呼び出す



	//ふぃくすちゃー
	//-----------------------------------------------------------------------
	b2Vec2 upper_size;
	upper_size.x = body_size.x / BOX2D_SCALE_MANAGEMENT;//サイズを１にすると　1m*1mになるため　サイズをさげて、物理演算の挙動を操作しやすくする
	upper_size.y = body_size.y / BOX2D_SCALE_MANAGEMENT;



	b2PolygonShape shape;
	shape.SetAsBox(upper_size.x * 0.5, upper_size.y * 0.5f);



	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;//密度
	fixture.friction = 0.3f;//摩擦
	fixture.restitution = 0.1f;//反発係数
	fixture.isSensor = false;//センサーかどうか、trueならあたり判定は消える

	if (object_sensor) {
		fixture.filter = createFilterExclude("one-way_platform_filter", { "object_filter" });
	}
	else
	{
		fixture.filter = createFilterExclude("ground_filter", {});
	}
	b2Fixture* ground_fixture = GetBody()->CreateFixture(&fixture);//Bodyをにフィクスチャを登録する

	


	
	// ユニークポインターを使って ObjectData を作成
	m_objectData = std::make_unique<ObjectData>(collider_ground);
	ground_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(m_objectData.get());


}
Ground::~Ground()
{
	Finalize();
}

void Ground::Finalize() {
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


void Ground::Initialize()
{

}

void Ground::Update()
{

}

void Ground::Draw()
{
	if (m_body != nullptr)
	{
	

		// コライダーと位置情報の補正をするため
		float scale = SCREEN_SCALE;

		b2Vec2 screen_center;
		screen_center.x = SCREEN_CENTER_X;
		screen_center.y = SCREEN_CENTER_Y;


		// コライダーの位置の取得（アイテムーの位置）
		b2Vec2 position;
		position.x = m_body->GetPosition().x;
		position.y = m_body->GetPosition().y;


		// プレイヤー位置を考慮してスクロール補正を加える
		//取得したbodyのポジションに対してBox2dスケールの補正を加える
		float draw_x = ((position.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
		float draw_y = ((position.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

		// シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_texture);
		//描画
		DrawSprite(
			{ draw_x,
			  draw_y },
			m_body->GetAngle(),
			{ GetSize().x * scale ,GetSize().y * scale  },
			3
		);

		
	}
}

