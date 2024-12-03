#include"object.h"
#include"include/box2d/box2d.h"
#include"world_box2d.h"
#include"field.h"
#include"collider_type.h"
#include"player_position.h"
#include"texture.h"
#include"sprite.h"
#include"object_map_type.h"


//テクスチャの入れ物
static ID3D11ShaderResourceView* g_Object_Texture = NULL;//地面のテクスチャ

static ID3D11ShaderResourceView* g_AnchorPoint_Texture = NULL;//アンカーポイントのテクスチャ


Object *objects[MAX_OBJECT_NUMBER];



Object::Object(b2Vec2 position, b2Vec2 body_size,bool is_sensor,object_map_type type)
{

	SetSize(body_size);//サイズをせっと

	


	//オブジェクトマップの入れ物にセットする
	for (int y = 0; y < OBJECT_MAX_HEIGHT; y++)
	{
		for (int x = 0; x < OBJECT_MAX_WIDTH; x++)
		{
			switch (type)
			{
			case type_wood:
				object_map[y][x] = object_map_name_Wood[y][x];
				break;
			case type_Rock:
				object_map[y][x] = object_map_name_Rock[y][x];
				break;
			default:
				break;
			}
		}
	}
	

	
	for (int y = 0;y< OBJECT_MAX_HEIGHT; y++)
	{
		for (int x = 0; x < OBJECT_MAX_WIDTH; x++)
		{
			if (object_map[y][x] != 0)//ボディが存在するかのチェック
			{
				//存在するならボディを作る
				b2BodyDef body;
				body.type = b2_dynamicBody;


				//真ん中を中心としたいため補正する
				body.position.Set(
					(position.x-4/BOX2D_SCALE_MANAGEMENT)+x/BOX2D_SCALE_MANAGEMENT,
					(position.y-4/BOX2D_SCALE_MANAGEMENT)+y/BOX2D_SCALE_MANAGEMENT
				);
				body.angle = 0.0;
				body.fixedRotation = false;//回転を固定にする
				body.userData.pointer = (uintptr_t)this;

				Box2dWorld& box2d_world = Box2dWorld::GetInstance();
				b2World* world = box2d_world.GetBox2dWorldPointer();

				b2Body *m_body=world->CreateBody(&body);

				SetObjectBody(m_body, x, y);//bodyの情報を登録


				b2Vec2 size;
				size.x = body_size.x / BOX2D_SCALE_MANAGEMENT;//サイズを１にすると　1m*1mになるため　サイズをさげて、物理演算の挙動を操作しやすくする
				size.y = body_size.y / BOX2D_SCALE_MANAGEMENT;


				b2PolygonShape shape;
				shape.SetAsBox(size.x * 0.5, size.y * 0.5f);

				b2FixtureDef fixture;
				fixture.shape = &shape;
				fixture.density = 10.0f;//密度
				fixture.friction = 0.05f;//摩擦
				fixture.restitution = 0.0f;//反発係数
				fixture.isSensor = false;//センサーかどうか、trueならあたり判定は消える

				b2Fixture* object_fixture = m_body->CreateFixture(&fixture);


			
				ObjectData* objectdata;

				 switch (object_map[y][x])
				 {
				 case 1://アンカーポイント
					 objectdata= new ObjectData{collider_anchor_point};//ここのオブジェクトデータを配列にしているから比較ができていない　ここいいの考えて
					 object_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(objectdata);

					 //テクスチャをセット
					 SetObjectTexture(anchorpoint_texture, x, y);
					 break;
				 case 2://アンカーポイントじゃないけどオブジェクトではあるよね
					 objectdata = new ObjectData{collider_object};
					 object_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(objectdata);

					 //テクスチャをセット
					 SetObjectTexture(sample_blue, x, y);
					 break;

				 case 3:
					 break;

				 default:
					 break;
				 }


				 //-----------------------------------------------------------------------------------
				 //溶接ジョイントを作成

				 if (0 <= x - 1)//オブジェクトのマップ上の一番左だと計算しない
				 {
					 if (object_map[y][x - 1] != 0)//今作成したボディの左にオブジェクトがあるかどうかの確認
					 {
						 //オブジェクトがあった溶接ジョイントする
						 b2WeldJointDef jointDef;
						 jointDef.bodyA = GetObjectBody(x, y);
						 jointDef.bodyB = GetObjectBody(x - 1, y);
						 jointDef.localAnchorA.Set(-size.x*0.5, 0.0f);
						 jointDef.localAnchorB.Set(size.x*0.5, 0.0f);
						 jointDef.collideConnected = false;					  //ジョイントした物体同士の接触を消す

						 world->CreateJoint(&jointDef);						  //ワールドにジョイントを追加
					 }
				 }

				 if (0 <= y - 1)
				 {
					 if (object_map[y - 1][x] != 0)
					 {
						 //オブジェクトがあった溶接ジョイントする
						 b2WeldJointDef jointDef;
						 jointDef.bodyA = GetObjectBody(x, y);
						 jointDef.bodyB = GetObjectBody(x, y-1);
						 jointDef.localAnchorA.Set(0.0f, -size.y*0.5);
						 jointDef.localAnchorB.Set(0.0f, size.y*0.5);
						 jointDef.collideConnected = false;					  //ジョイントした物体同士の接触を消す

						 world->CreateJoint(&jointDef);
					 }
				 }
				 
			}
		}

	}

}

Object::~Object()
{

}


void Object::Initialize()
{
	//画像のインクルード
	g_Object_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_green.png");//グランドのテクスチャ
	g_AnchorPoint_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_red.png");//アンカーポイントのテクスチャ



}

void Object::SetObject(int x, int y,b2Vec2 body_size, bool is_sensor, object_map_type type)
{
	for (int i = 0; i < MAX_OBJECT_NUMBER; i++)
	{
		if (objects[i] == nullptr)//値がnullだったら　いれる
		{
			objects[i]=new Object(b2Vec2( x/ BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), false, type);
			
			break;
		}
	}
}

void Object::Update()
{

}

void Object::Draw()
{
	// スケールをかけないとオブジェクトのサイズの表示が小さいから使う
	float scale = SCREEN_SCALE;

	// スクリーン中央位置 (プロトタイプでは乗算だったけど　今回から加算にして）
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;

	for (int i = 0; i < MAX_OBJECT_NUMBER; i++)//オブジェクト最大数だけ回す
	{
		if (objects[i] != nullptr) {//オブジェクトが存在するかの確認
			for (int y = 0; y < OBJECT_MAX_HEIGHT; ++y) {
				for (int x = 0; x < OBJECT_MAX_WIDTH; ++x) {
					if (objects[i]->object_map[y][x]!=0) {
						b2Vec2 position;
						position.x = objects[i]->GetObjectBody(x,y)->GetPosition().x;
						position.y = objects[i]->GetObjectBody(x,y)->GetPosition().y;

						// プレイヤー位置を考慮してスクロール補正を加える
						//取得したbodyのポジションに対してBox2dスケールの補正を加える
						float draw_x = ((position.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
						float draw_y = ((position.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;



						//貼るテクスチャを指定
						switch (objects[i]->GetObjectTexture(x,y))
						{
						case anchorpoint_texture:
							GetDeviceContext()->PSSetShaderResources(0, 1, &g_AnchorPoint_Texture);
							break;
						case sample_blue:
							GetDeviceContext()->PSSetShaderResources(0, 1, &g_Object_Texture);
							break;
						default:
							break;
						}



						//draw
						DrawSprite(
							{ draw_x,
							  draw_y },
							objects[i]->GetObjectBody(x,y)->GetAngle(),
							{ objects[i]->GetSize().x * scale , objects[i]->GetSize().y * scale}
						);
					}
				}
			}
		}
	}
}

void Object::Finalize()
{

}