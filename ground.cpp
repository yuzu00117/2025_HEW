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
Ground::Ground(b2Vec2 position, b2Vec2 body_size, float angle, bool bFixed, bool is_sensor, FieldTexture texture)
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



	//上の面 (ground)
	//-----------------------------------------------------------------------
	b2Vec2 upper_size;
	upper_size.x = body_size.x / BOX2D_SCALE_MANAGEMENT;//サイズを１にすると　1m*1mになるため　サイズをさげて、物理演算の挙動を操作しやすくする
	upper_size.y = body_size.y / BOX2D_SCALE_MANAGEMENT;



	b2PolygonShape shape_upper;                         //shapeには色々な型がある　サークルとかもあるよ
	shape_upper.SetAsBox(upper_size.x * 0.35f, upper_size.y * 0.49f);//あたり判定を登録する4点　*0.5するのは

	b2FixtureDef fixture_upper;
	fixture_upper.shape = &shape_upper;    //シャープをフィクスチャに登録する
	fixture_upper.density = 1.0f;    //密度
	fixture_upper.friction = 0.5f;  //摩擦
	fixture_upper.restitution = 0.0f;//反発係数
	fixture_upper.isSensor = false;  //センサーかどうか、trueならあたり判定は消える

	b2Fixture* ground_upper_fixture = GetFieldBody()->CreateFixture(&fixture_upper);//Bodyをにフィクスチャを登録する

	// カスタムデータを作成して設定
	// 地面に値を登録
	// 地面にユーザーデータを登録
	ObjectData* ground_upper_data = new ObjectData{ collider_ground };
	ground_upper_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(ground_upper_data);




	//左右の面 (wall)
	//------------------------------------------------------------------

	b2Vec2 side_size;
	side_size.x = body_size.x / BOX2D_SCALE_MANAGEMENT;//サイズを１にすると　1m*1mになるため　サイズをさげて、物理演算の挙動を操作しやすくする
	side_size.y = body_size.y / BOX2D_SCALE_MANAGEMENT;


	b2PolygonShape	shape_side;
	shape_side.SetAsBox(side_size.x * 0.5f, side_size.y * 0.45f);

	b2FixtureDef fixture_side;
	fixture_side.shape = &shape_side;    //シャープをフィクスチャに登録する
	fixture_side.density = 0.0f;    //密度
	fixture_side.friction = 0.0f;  //摩擦
	fixture_side.restitution = 0.0f;//反発係数
	fixture_side.isSensor = false;  //センサーかどうか、trueならあたり判定は消える

	b2Fixture* ground_side_fixture = GetFieldBody()->CreateFixture(&fixture_side);//Bodyをにフィクスチャを登録する

	// カスタムデータを作成して設定
	// 地面に値を登録
	// 地面にユーザーデータを登録
	ObjectData* ground_side_data = new ObjectData{ collider_wall };
	ground_side_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(ground_side_data);
}

Ground::~Ground()
{
}