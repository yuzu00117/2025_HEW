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
#include"enemy_floating.h"
#include"object_manager.h"
#include"Item_Manager.h"
#include"1_1_boss_carry_object_enemy.h"
#include"scene.h"

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


	SceneManager& sceneManager = SceneManager::GetInstance();
	switch (sceneManager.GetStageName())
	{
	case STAGE_NULL:
		break;
	case STAGE_TUTORIAL:
		// csvからマップチップを読み込む
		Field::LoadCSV("asset/mapchip_tutorial.csv");
		break;
	case STAGE_1_1:
		// csvからマップチップを読み込む
		Field::LoadCSV("asset/mapchip_stage_1_1_test.csv");
		break;

	default:
		break;
	}

	
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
			if (field_map[y][x] == 1) {//動かない物
				//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
				m_p_field_array[y][x] = new Ground(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, ground_texture,false);
			}
	
			if (field_map[y][x] == 2) {//コイン
				itemManager.AddCoin(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f);
			}

			if (field_map[y][x] == 3) {//足場
				objectManager.AddOne_way_platformList(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(0.0f, 0.0f),b2Vec2(1.0f,0.1f));
			}
			if (field_map[y][x] == 4) {//足場 少し上に寄ったもの
				objectManager.AddOne_way_platformList(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(0.0f, -0.5f), b2Vec2(1.0f, 0.1f));
			}
			if (field_map[y][x] == 5) {//足場 少し下に寄ったもの
				objectManager.AddOne_way_platformList(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(0.0f, 0.5f), b2Vec2(1.0f, 0.1f));
			}


			if (field_map[y][x] == 6) {//動的なエネミー
				objectManager.AddEnemyDynamic(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f),0.0f);
			}


			if(field_map[y][x] == 7) {//小さな木のオブジェクト 必要アンカーレベル１
				objectManager.AddWood(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT),b2Vec2(2.0f,10.0f),b2Vec2(2.0f, 1.0f),1);
			}

			if (field_map[y][x] == 8) {//静的→動的のオブジェクト 必要アンカーレベル１ ブロックの高さが偶数だったためｙ軸に少しずれている
				objectManager.AddStatic_to_Dynamic_block(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT-0.5/BOX2D_SCALE_MANAGEMENT), b2Vec2(7.0f, 2.0f),Box_collider,2);
			}


			if (field_map[y][x] == 9) {//静的→動的のオブジェクト 必要アンカーレベル2 ブロックの幅が偶数だったためｘ軸に少しずれている
				objectManager.AddStatic_to_Dynamic_block(b2Vec2(x / BOX2D_SCALE_MANAGEMENT - 0.5 / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT ), b2Vec2(4.0f, 3.0f), Box_collider, 2);
			}
			if (field_map[y][x] == 10) {//アンカーレベル３の巨大な木
				objectManager.AddWood(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT ), b2Vec2(2.0f, 12.0f), b2Vec2(2.0f,2.0f), 3);
			}
			if (field_map[y][x] == 11) {//アンカーレベル３地面を動かせるやつ
				objectManager.AddMovable_Ground(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(20.f, 9.f), b2Vec2(1.0f, 1.0f), 3);
			}
			if (field_map[y][x] == 12) {//アンカーレベル３地面を動かせるやつ でそれのみ貫通するやつ
				m_p_field_array[y][x] = new Ground(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, ground_texture, true);
			}
			if (field_map[y][x] == 13) {//右下斜面
				objectManager.AddSloping_block(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), right_down);
			}
			if (field_map[y][x] == 14) {//転がすいわ
				objectManager.AddRock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), 3, 2);
			}

			if (field_map[y][x] == 15) {//間欠泉にいくようのテレポートブロック
				objectManager.AddTeleportBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(3.f,1.f), b2Vec2(25.f,-5.f));
			}

			if (field_map[y][x] == 16) {//間欠泉にいくようのテレポートブロック
				objectManager.AddTeleportBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(25.f, 1.f), b2Vec2(25.f, 4.f));
			}




			if (field_map[y][x] == 17) {//ボスの地面ブロック破壊できる
				objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f),3,boss_room_level_1);
			}
			if (field_map[y][x] == 18) {//ボスの地面ブロック破壊できる
				objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_2);
			}
			if (field_map[y][x] == 19) {//ボスの地面ブロック破壊できる
				objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_3);
			}
			if (field_map[y][x] == 20) {//ボスの地面ブロック破壊できる
				objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_4);
			}
			if (field_map[y][x] == 21) {//ボスの地面ブロック破壊できる
				objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_5);
			}
			if (field_map[y][x] == 22) {//ボスの地面ブロック破壊できる
				objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_6);
			}
			if (field_map[y][x] == 23) {//ボスの地面ブロック破壊できる
				objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_7);
			}
			if (field_map[y][x] == 24) {//ボスの地面ブロック破壊できる
				objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_8);
			}
			if (field_map[y][x] == 25) {//ボスの地面ブロック破壊できる
				objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_9);
			}



			//ボスのオブジェクトに使う柱
			if (field_map[y][x] == 26) {//ボスの地面ブロック破壊できる
				objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(3.f, 18.f),1,6, boss_room_level_1);
			}


			//ボスのオブジェクトに使う柱
			if (field_map[y][x] == 27) {//ボスの地面ブロック破壊できる
				objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(3.f, 18.f), 3, 18, boss_room_level_2);
			}

			//ボスのオブジェクトに使う柱
			if (field_map[y][x] == 28) {//ボスの地面ブロック破壊できる
				objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(3.f, 18.f), 6, 36, boss_room_level_3);
			}

			//ボスのオブジェクトに使う柱
			if (field_map[y][x] == 29) {//ボスの地面ブロック破壊できる
				objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(3.f, 18.f), 2, 12, boss_room_level_4);
			}

			//ボスのオブジェクトに使う柱
			if (field_map[y][x] == 30) {//ボスの地面ブロック破壊できる
				objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(3.f, 18.f), 3, 18, boss_room_level_5);
			}

			//ボスのオブジェクトに使う柱
			if (field_map[y][x] == 31) {//ボスの地面ブロック破壊できる
				objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 9.f), 1, 6, boss_room_level_6);
			}

			if (field_map[y][x] == 32) {//青宝石
				itemManager.AddJewel(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, BLUE);
			}

			if (field_map[y][x] == 33) {//赤宝石
				itemManager.AddJewel(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, RED);
			}

			if (field_map[y][x] == 34) {//黄色宝石
				itemManager.AddJewel(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, YELLOW);
			}


			//ステージの間欠泉
			if (field_map[y][x] == 35) {//ボスの地面ブロック破壊できる
				objectManager.AddGeyser(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(3.f, 3.f), b2Vec2(3.f,15.f),9,9,boss_room_level_18);
			}

			//ボスのステージの間欠泉
			if (field_map[y][x] == 36) {//ボスの地面ブロック破壊できる
				objectManager.AddGeyser(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(3.f, 1.f), b2Vec2(3.f, 6.f), 9, 9, boss_room_level_1);
			}

			//ボスのステージの間欠泉
			if (field_map[y][x] == 37) {//ボスの地面ブロック破壊できる
				objectManager.AddGeyser(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(3.f, 1.f), b2Vec2(3.f, 6.f), 9, 9, boss_room_level_2);
			}


			//ボスのステージのオブジェクトをもったエネミーのスポナー
			if (field_map[y][x] == 45) {//ボスの地面ブロック破壊できる
				objectManager.AddBossCarryEnemySpawner(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), boss_room_level_1,true);
			}

			if (field_map[y][x] == 46) {//ボスの地面ブロック破壊できる
				objectManager.AddBossCarryEnemySpawner(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), boss_room_level_2, true);
			}

			if (field_map[y][x] == 47) {//ボスの地面ブロック破壊できる
				objectManager.AddBossCarryEnemySpawner(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), boss_room_level_3, true);
			}

			if (field_map[y][x] == 48) {//ボスの地面ブロック破壊できる
				objectManager.AddBossCarryEnemySpawner(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), boss_room_level_4, false);
			}

			if (field_map[y][x] == 49) {//ボスの地面ブロック破壊できる
				objectManager.AddBossCarryEnemySpawner(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), boss_room_level_5, false);
			}

			if (field_map[y][x] == 50) {//中間地点
				itemManager.AddSavePoint(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f);
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

	if (g_Ground_Texture != NULL)
	{
		UnInitTexture(g_AnchorPoint_Texture);
		UnInitTexture(g_Ground_Texture);

		g_Ground_Texture = NULL;
		g_AnchorPoint_Texture = NULL;
	}

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

//マップ切り替え
void Field::LoadMap(StageType stage_type)
{
    // 現在のマップをクリア
    Field::Finalize();

    // マップタイプに応じて対応する CSV ファイルを読み込む
    switch (stage_type) {
        case StageType::Stage1:
            LoadCSV("asset/mapchip_stage_1_1.csv");
            break;
        // 他のステージを追加
        default:
            std::cerr << "Unknown map type" << std::endl;
            return;
    }

    // 新しいマップを初期化
    Initialize();
}