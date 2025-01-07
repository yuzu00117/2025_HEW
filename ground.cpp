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


/**
 * @brief  フィールドのコンストラクタ
 * @param position  登録する座標
 * @param body_size ボディの大きさ、関数中で大きさの調整をする
 * @param angle     角度を設定
 * @param bFixed
 * @param is_sensor
 * @param texture
 */
Ground::Ground(b2Vec2 position, b2Vec2 body_size, float angle, bool bFixed, bool is_sensor, FieldTexture texture,bool object_sensor)
{
	//テクスチャをセット
	SetFieldTexture(texture);


	b2BodyDef body;
	body.type = bFixed ? b2_staticBody : b2_dynamicBody;	//静的なオブジェクトにするならture
	body.position.Set(position.x, position.y);			//ポジションをセット
	body.angle = angle;									//角度の定義
	body.userData.pointer = (uintptr_t)this;			//userDataのポインタを定義 
	body.fixedRotation = true;							//回転を固定する、　これをオンにすると回転しない


	Box2dWorld& box2d_world = Box2dWorld::GetInstance();//ワールドのインスタンスを取得する
	b2World* world = box2d_world.GetBox2dWorldPointer();//ワールドのポインタを持ってくる

	SetFieldBody(world->CreateBody(&body));//Bodyをワールドに固定


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
	b2Fixture* ground_fixture = GetFieldBody()->CreateFixture(&fixture);//Bodyをにフィクスチャを登録する

	


	// カスタムデータを作成して設定
	// プレイヤーに値を登録
	// プレーヤーにユーザーデータを登録
	ObjectData* playerdata = new ObjectData{ collider_ground };




	ground_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(playerdata);
}

Ground::~Ground()
{
}