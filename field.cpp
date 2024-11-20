//-----------------------------------------------------------------------------------------------------
// #name field.cpp
// #description フィールド
// #make 2024/11/04
// #update 2024/11/03
// #comment 追加・修正予定
//          ・Fieldの設定をしてる  呼び出しの仕方としてｈスコープ解決演算子使ってやって （Field::Draw)
//           
//----------------------------------------------------------------------------------------------------

#include"tool.h"
#include"include/box2d/box2d.h"
#include"field.h"
#include"player_position.h"
#include"world_box2d.h"
#include"sprite.h"
#include"texture.h"


// 2次元配列の静的メンバの初期化
Field*** Field::m_p_field_array = nullptr;
int Field::m_field_height = 0;
int Field::m_field_width = 0;

//グローバル変数　テクスチャの入れ物
static ID3D11ShaderResourceView* g_Ground_Texture = NULL;



/**
 * @brief  フィールドのコンストラクタ
 * @param position  登録する座標 
 * @param body_size ボディの大きさ、関数中で大きさの調整をする
 * @param angle     角度を設定
 * @param bFixed    
 * @param is_sensor 
 * @param texture 
 */
Field::Field(b2Vec2 position, b2Vec2 body_size, float angle, bool bFixed, bool is_sensor,FieldTexture texture)
{
	b2BodyDef body;
	body.type = bFixed?b2_staticBody:b2_dynamicBody;	//静的なオブジェクトにするならture
	body.position.Set(position.x,position.y);			//ポジションをセット
	body.angle = angle;									//角度の定義
	body.userData.pointer = (uintptr_t)this;			//userDataのポインタを定義 
	body.fixedRotation = true;							//回転を固定する、　これをオンにすると回転しない


	Box2dWorld& box2d_world = Box2dWorld::GetInstance();//ワールドのインスタンスを取得する
	b2World* world = box2d_world.GetBox2dWorldPointer();//ワールドのポインタを持ってくる

	m_body = world->CreateBody(&body);//Bodyをワールドに固定


	SetSize(body_size);//表示用にサイズをセットしとく、表示のときにGetSizeを呼び出す

	

	b2Vec2 size;
	size.x = body_size.x/BOX2D_SCALE_MANAGEMENT;//サイズを１にすると　1m*1mになるため　サイズをさげて、物理演算の挙動を操作しやすくする
	size.y = body_size.y/BOX2D_SCALE_MANAGEMENT;

	

	b2PolygonShape shape;                         //shapeには色々な型がある　サークルとかもあるよ
	shape.SetAsBox(size.x * 0.5f, size.y * 0.5f );//あたり判定を登録する4点　*0.5するのは
	
	b2FixtureDef fixture;
	fixture.shape = &shape;    //シャープをフィクスチャに登録する
	fixture.density = 1.0f;    //密度
	fixture.friction = 0.05f;  //摩擦
	fixture.restitution = 0.0f;//反発係数
	fixture.isSensor = false;  //センサーかどうか、trueならあたり判定は消える

	m_body->CreateFixture(&fixture);//Bodyをにフィクスチャを登録する

}

Field::~Field()
{
}

void Field::Initialize(int field_width, int field_height)
{
	//テクスチャの初期化
	g_Ground_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_green.png");

	//Drawの方で使うためメンバ変数内に代入しとく
	m_field_height = field_height;
	m_field_width  = field_width;

	//2次元配列のメモリ確保
	m_p_field_array = new Field * *[field_height]; // 縦方向の配列を確保

	for (int y = 0; y < field_height; ++y) {
		m_p_field_array[y] = new Field * [field_width]; // 横方向の配列を各行ごとに確保

		for (int x = 0; x < field_width; ++x) {
			m_p_field_array[y][x] = nullptr; // 各要素を nullptr で初期化
		}
	}


	//マップを登録する二次元配列
	int field_map[20][90] =
	{
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	};

	for (int y = 0; y < field_height; ++y) {
		for (int x = 0; x < field_width; ++x) {
			if (field_map[y][x] == 1) {
				//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
				m_p_field_array[y][x] = new Field(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, ground_texture);
			}
			if (field_map[y][x] == 2) {
				m_p_field_array[y][x] = new Field(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, ground_texture);
			}
			if (field_map[y][x] == 3) {
				m_p_field_array[y][x] = new Field(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, false, true, ground_texture);
			}
			if (field_map[y][x] == 4) {
				m_p_field_array[y][x] = new Field(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, ground_texture);
			}
		}
	}
}



void Field::Update()
{

}

void Field::Draw()
{

	// スケールをかけないとオブジェクトのサイズの表示が小さいから使う
	float scale = SCREEN_SCALE;

	// スクリーン中央位置 (プロトタイプでは乗算だったけど　今回から加算にして）
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;



	for (int y = 0; y < m_field_height; ++y) {
		for (int x = 0; x < m_field_width; ++x) {
			if (m_p_field_array[y][x] != nullptr) {
				b2Vec2 position;
				position.x = m_p_field_array[y][x]->GetFieldBody()->GetPosition().x ;
				position.y = m_p_field_array[y][x]->GetFieldBody()->GetPosition().y ;

				// プレイヤー位置を考慮してスクロール補正を加える
				//取得したbodyのポジションに対してBox2dスケールの補正を加える
				float draw_x = ((position.x - PlayerPosition::GetPlayerPosition().x)*BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
				float draw_y = ((position.y - PlayerPosition::GetPlayerPosition().y)*BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


		
				//貼るテクスチャを指定
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Ground_Texture);
				
			
				//draw
				DrawSprite(
					{ draw_x,
					  draw_y },
					m_p_field_array[y][x]->GetFieldBody()->GetAngle(),
					{ m_p_field_array[y][x]->GetSize().x * scale , m_p_field_array[y][x]->GetSize().y * scale }
				);
			}
		}
	}
}

void Field::Finalize()
{
	// 2次元配列のメモリ解放
	for (int y = 0; y < m_field_height; ++y) {

		for (int x = 0; x < m_field_width; ++x) {

			if (m_p_field_array[y][x] != nullptr) {

				delete m_p_field_array[y][x];
				m_p_field_array[y][x] = nullptr;
			}
		}
		delete[] m_p_field_array[y];
	}
	delete[] m_p_field_array;
	m_p_field_array = nullptr;
}