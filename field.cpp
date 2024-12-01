//-----------------------------------------------------------------------------------------------------
// #name field.cpp
// #description フィールド

// #make 2024/11/04　永野義也
// #update 2024/11/29

// #comment 追加・修正予定
//          ・Fieldの設定をしてる  呼び出しの仕方としてｈスコープ解決演算子使ってやって （Field::Draw)
//			・マップを管理する基底クラスでグランドなどが継承している
//           
//----------------------------------------------------------------------------------------------------

#include"tool.h"
#include"include/box2d/box2d.h"
#include"field.h"
#include"player_position.h"
#include"world_box2d.h"
#include"sprite.h"
#include"texture.h"
#include"collider_type.h"
#include"ground.h"
#include"anchor_point.h"
#include"enemy_dynamic.h"
#include"enemy_static.h"


// 2次元配列の静的メンバの初期化
Field*** Field::m_p_field_array = nullptr;





int Field::m_field_height = 0;
int Field::m_field_width = 0;

//グローバル変数　テクスチャの入れ物


static ID3D11ShaderResourceView* g_Ground_Texture = NULL;//地面のテクスチャ

static ID3D11ShaderResourceView* g_AnchorPoint_Texture = NULL;//アンカーポイントのテクスチャ

static ID3D11ShaderResourceView* g_EnemyDynamic_Texture = NULL;	//動的エネミーのテクスチャ
static ID3D11ShaderResourceView* g_EnemyStatic_Texture = NULL;	//静的エネミーのテクスチャ




Field::Field()
{
}

Field::~Field()
{
}

void Field::Initialize(int field_width, int field_height)
{
	//テクスチャの初期化
	g_Ground_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_green.png");//グランドのテクスチャ

	g_AnchorPoint_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_red.png");//アンカーポイントのテクスチャ
	g_EnemyDynamic_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_yellow.png");//動的エネミーのテクスチャ
	g_EnemyStatic_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_block.png");//静的エネミーのテクスチャ


	//APのイニシャライズ
	AnchorPoint::Initialize();

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
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{4,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
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
				m_p_field_array[y][x] = new Ground(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, ground_texture);
			}
			if (field_map[y][x] == 2) {
				m_p_field_array[y][x] = new AnchorPoint(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, anchor_point_texture);
			}
			if (field_map[y][x] == 3) {
				m_p_field_array[y][x] = new AnchorPoint(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 5.0f), 0.0f, false, true, anchor_point_texture);
			}
			if (field_map[y][x] == 4) {
				m_p_field_array[y][x] = new Ground(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, ground_texture);
			}
			if (field_map[y][x] == 4) {
				m_p_field_array[y][x] = new Ground(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, ground_texture);
			}
			if (field_map[y][x] == 5) {
				m_p_field_array[y][x] = new EnemyStatic(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, false, true, enemy_static_texture);
			}
			if (field_map[y][x] == 6) {
				m_p_field_array[y][x] = new EnemyDynamic(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, enemy_dynamic_texture);
			}
		}
	}
}



void Field::Update()
{
	AnchorPoint::Update();
	Enemy::Update();
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
				switch (m_p_field_array[y][x]->GetFieldTexture())
				{
				case anchor_point_texture:

					GetDeviceContext()->PSSetShaderResources(0, 1, &g_AnchorPoint_Texture);
					break;
				case ground_texture:

					GetDeviceContext()->PSSetShaderResources(0, 1, &g_Ground_Texture);
					break;
				case enemy_dynamic_texture:

					GetDeviceContext()->PSSetShaderResources(0, 1, &g_EnemyDynamic_Texture);
					break;
				case enemy_static_texture:

					GetDeviceContext()->PSSetShaderResources(0, 1, &g_EnemyStatic_Texture);
					break;
				default:
					break;
				}
				
				
			
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

	AnchorPoint::Draw();
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

	Enemy::Finalize();
}