//-----------------------------------------------------------------------------------------------------
// #name game.h
// #description ゲーム
// #make 2024/11/04
// #update 2024/11/03
// #comment 追加・修正予定
//          ・Gameないの管理している　基本的にすべての関数がここにたどり着くことに
//           
//----------------------------------------------------------------------------------------------------

#include"tool.h"
#include"square_collider.h"
#include"include/box2d/box2d.h"
#include"collider_type.h"
#include"feild.h"


// 2次元配列の静的メンバの初期化
Field*** Field::m_p_field_array = nullptr;
int Field::m_field_height = 0;
int Field::m_field_width = 0;

//グローバル変数
static ID3D11ShaderResourceView* g_Texture = NULL;

Field::Field(XMFLOAT2 position, XMFLOAT2 size, float angle, bool bFixed,bool is_sensor,bool in_piece)
	: SquareCollider(position, size, angle, bFixed)
{
	// field専用の初期設定
	SquareCollider::SetColliderType(collider_ground);


	if (!bFixed) {
		b2FixtureDef fixture;
		fixture.shape = SquareCollider::GetColliderBody()->GetFixtureList()->GetShape();
		fixture.density = 1.0f;
		fixture.friction = 0.5f;  // プレイヤーに特化した摩擦
		fixture.restitution = 0.1f; // 反発係数の追加

		

		SquareCollider::GetColliderBody()->CreateFixture(&fixture);
	}
}

Field::~Field()
{
}

 void Field::Init(int field_width, int field_height)
{
	//テクスチャの初期化
	 g_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_green.png");

	 if (g_Texture == nullptr) {
		 
		 //テスト用　これうちたくないけどきもす
		 int i = 0;
	 }
	m_field_width = field_width;
	m_field_height = field_height;

	//2次元配列のメモリ確保

	m_p_field_array = new Field * *[field_height]; // 縦方向の配列を確保

	for (int y = 0; y < field_height; ++y) {
		m_p_field_array[y] = new Field * [field_width]; // 横方向の配列を各行ごとに確保

		for (int x = 0; x < field_width; ++x) {
			m_p_field_array[y][x] = nullptr; // 各要素を nullptr で初期化
		}
	}

	int field_map[20][30] =
	{
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,1,0,0,0,1,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	};

	for (int y = 0; y < field_height; ++y) {
		for (int x = 0; x < field_width; ++x) {
			if (field_map[y][x] == 1) {
				m_p_field_array[y][x] = new Field(XMFLOAT2(1 * x, 1 * y), XMFLOAT2(1.0f, 1.0f), 0.0f, true, true, true);
			}
		}
	}
	
}

void Field::Update()
{

}

void Field::Draw()
{

	//コライダーと位置情報の補正をするため
	float scale = 80.f;

	//現実で換算するとい　16m*9にする　つまり画面の真ん中は x.8 y.4.5

	for (int y = 0; y < m_field_height; ++y) {
		for (int x = 0; x < m_field_width; ++x) {
			if (m_p_field_array[y][x] != nullptr) {
				XMFLOAT2 position;
				position.x = m_p_field_array[y][x]->GetColliderBody()->GetPosition().x;
				position.y = m_p_field_array[y][x]->GetColliderBody()->GetPosition().y;

				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);
				DrawSprite(
					{ position.x * scale, position.y * scale },
					m_p_field_array[y][x]->GetColliderBody()->GetAngle(),
					{ m_p_field_array[y][x]->Get_m_size().x * 100 * scale, m_p_field_array[y][x]->Get_m_size().y * 100 * scale }
				);
			}
		}
	}


}

void Field::Uninit()
{
	// 2次元配列のメモリ解放
	for (int y = 0; y < m_field_height; ++y) {

		for (int x = 0; x < m_field_width; ++x) {

			if (m_p_field_array[y][x] != nullptr) {

				delete m_p_field_array [y][x];
				m_p_field_array [y][x] = nullptr;
			}
		}
		delete[] m_p_field_array [y];
	}
	delete[] m_p_field_array;
	m_p_field_array= nullptr;
}