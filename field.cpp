//-----------------------------------------------------------------------------------------------------
// #name field.cpp
// #description csvを用いてマップチップを作成し、描画するファイル
// #make 2024/11/04　永野義也
// #update 2024/12/13
// #comment 追加・修正予定
//          ・Fieldの設定をしてる  呼び出しの仕方としてｈスコープ解決演算子使ってやって （Field::Draw)
//			・マップを管理する基底クラスでグランドなどが継承している
//			・12/01 フィールドのマップチップをcsvを用いた描画に変更
//				・マップの変更はcsvファイルを編集してください
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
#include"object_manager.h"
#include"Item_Manager.h"




// 2次元配列の静的メンバの初期化
Field*** Field::m_p_field_array = nullptr;

// csvファイルから読み込んだマップデータの一時格納用
std::vector<std::vector<int>> Field::m_field_data;

// フィールドの幅と高さを保持する静的メンバ変数を初期化
// クラス全体で共有される変数として使用するため、ここで初期化
int Field::m_field_width = 0;
int Field::m_field_height = 0;


ObjectManager& objectManager = ObjectManager::GetInstance();
ItemManager& itemManager = ItemManager::GetInstance();

// 使用するテクスチャファイルを格納
static ID3D11ShaderResourceView* g_Ground_Texture = NULL;//地面のテクスチャ
static ID3D11ShaderResourceView* g_AnchorPoint_Texture = NULL;//アンカーポイントのテクスチャ

Field::Field()
{
}



Field::~Field()
{
}



//初期化
void Field::Initialize()
{
	
	//テクスチャの初期化
	g_Ground_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_green.png");//グラウンドのテクスチャ
	g_AnchorPoint_Texture= InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_red.png");//アンカーポイントのテクスチャ

	//APのイニシャライズ
	AnchorPoint::Initialize();



	// csvからマップチップを読み込む
	Field::LoadCSV("asset/mapchip_enemy.csv");
	//読み込んだデータをfield_mapに格納
	std::vector<std::vector<int>> field_map = m_field_data;

	//マップに基づいて2次元配列のメモリ確保
	m_p_field_array = new Field * *[m_field_height]; //縦方向の配列を確保





	for (int y = 0; y < m_field_height; ++y) {
		m_p_field_array[y] = new Field * [m_field_width]; //横方向の配列を各行ごとに確保

		for (int x = 0; x < m_field_width; ++x) {
			m_p_field_array[y][x] = nullptr; //各要素を nullptr で初期化
		}
	}

	//マップの数値に応じたオブジェクトを描画するため、対応したオブジェクトのインスタンスを生成
	for (int y = 0; y < m_field_height; ++y)
  {
		for (int x = 0; x < m_field_width; ++x)
    {
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
			if (field_map[y][x] == 5) {
				objectManager.AddEnemyStatic(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f);
			}
			if (field_map[y][x] == 6) {
				objectManager.AddEnemyDynamic(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f);
			}
			if (field_map[y][x] == 7) {
				objectManager.AddWood(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 8.0f),b2Vec2(1.0f,1.0f),1);
			}
			if (field_map[y][x] == 8) {
				objectManager.AddWood(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(2.0f, 17.0f), b2Vec2(3.0f, 1.0f), 3);
			}

			if (field_map[y][x] == 9) {
				objectManager.AddRock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), 3.0f,2);
			}


			
			if (field_map[y][x] == 10) {//足場ブロック
				objectManager.AddOne_way_platformList(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(0.0f, 0.0f), b2Vec2(3.0f, 0.2f));
			}



			if (field_map[y][x] == 11) {//右上　傾斜
				objectManager.AddSloping_block(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), right_upper);
			}


			if (field_map[y][x] == 12) {//右下　傾斜
				objectManager.AddSloping_block(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(5.0f, 5.0f), right_down);
			}

			if (field_map[y][x] == 13) {//左上　傾斜
				objectManager.AddSloping_block(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), left_upper);
			}

			if (field_map[y][x] == 14) {//左下　傾斜
				objectManager.AddSloping_block(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(17.0f,17.0f), left_down);
			}


			if (field_map[y][x] == 15) {//四角の静的動的オブジェクト
				objectManager.AddStatic_to_Dynamic_block(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(3.0f, 3.0f), Box_collider,1);
			}

			if (field_map[y][x] == 16) {//四角の静的動的オブジェクト
				objectManager.AddStatic_to_Dynamic_block(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(6.0f, 6.0f), Circle_collider, 1);
			}

			if (field_map[y][x] == 17) {//四角の静的動的オブジェクト
				objectManager.AddStatic_to_Dynamic_block(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(3.0f, 3.0f), Circle_collider, 1);
			}

			if (field_map[y][x] == 18) {//スピードアップアイテム
				itemManager.AddSpeedUp(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f);
			}
			if (field_map[y][x] == 19) {//引っ張れる床
				objectManager.AddMovable_Ground(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(3.0f, 5.0f), b2Vec2(1.0f, 1.0f), 3);
			}

		}
	}

	objectManager.InitializeAll();
	itemManager.InitializeAll();
}



void Field::Update()
{
	//アンカーポイントの更新
	AnchorPoint::Update();

	objectManager.UpdateAll();
	itemManager.UpdateAll();
}



void Field::Draw()
{
	// スケールをかけないとオブジェクトのサイズの表示が小さいから使う
	float scale = SCREEN_SCALE;

	// スクリーン中央位置 (プロトタイプでは乗算だったけど　今回から加算にして）
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;

	//m_p_field_array の各位置に対応するフィールドオブジェクトを描画
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


	objectManager.DrawAll();
	itemManager.DrawAll();

	//アンカーポイントを描画
	AnchorPoint::Draw();
}



void Field::Finalize()
{
	//ワールドのインスタンスを持ってくる
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();

	// 2次元配列のメモリ解放
	for (int y = 0; y < m_field_height; ++y) {

		for (int x = 0; x < m_field_width; ++x) {

			if (m_p_field_array[y][x] != nullptr) {
				
				world->DestroyBody(m_p_field_array[y][x]->GetFieldBody());//フィールドを消す
				delete m_p_field_array[y][x];
				m_p_field_array[y][x] = nullptr;
			}
		}
		delete[] m_p_field_array[y];
	}
	delete[] m_p_field_array;
	m_p_field_array = nullptr;

	//終了処理
	AnchorPoint::Finalize();
	objectManager.FinalizeAll();
	itemManager.FinalizeAll();

}



//csvファイルの読み込みを行う関数
//csvファイルを読み込んで、その内容を m_field_data に格納します。
bool Field::LoadCSV(const std::string &filename)
{
	std::ifstream file(filename);
	if (!file.is_open())
	{
		std::cerr << "Failed to open CSV file" << filename << std::endl;
		return false;
	}

	std::string line;
	m_field_data.clear(); //以前のデータをクリア

	//ファイルの各行を読み込む`
	while (std::getline(file, line))
	{
		std::vector<int> row;
		std::stringstream ss(line);
		std::string cell;

		//各セルを読み込み、カンマで区切られた数値を取得
		while (std::getline(ss, cell, ','))
		{
			try
			{
				row.push_back(std::stoi(cell)); // 数値に変換して追加
			}
			catch(const std::invalid_argument& e)
			{
				std::cerr << "Invalid data in CSV" << cell << std::endl;
				file.close();
				return false;
			}
		}
		m_field_data.push_back(row);
	}
	file.close();

	// 高さと幅をCSVのデータから取得
    m_field_height = m_field_data.size();  // 行数がフィールドの高さ
    m_field_width = (m_field_data.empty() ? 0 : m_field_data[0].size());  // 最初の行の列数がフィールドの幅
	return true;
}

//フィールドのオブジェクトを消す処理(消すオブジェクトのボディを取得)
void Field::DeleteFieldObject(b2Body* delete_object)
{
	for (int y = 0; y < m_field_height; ++y)
	{
		for (int x = 0; x < m_field_width; ++x)
		{
			if (m_p_field_array[y][x])
			{
				if (m_p_field_array[y][x]->GetFieldBody() == delete_object)
				{
					delete m_p_field_array[y][x];
					m_p_field_array[y][x] = nullptr;
					return;
				}
			}
		}
	}
}
