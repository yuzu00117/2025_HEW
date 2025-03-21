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
#include"1-1_boss.h"
#include"anchor_spirit.h"



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
static ID3D11ShaderResourceView* g_Ground_Grass_only_Texture = NULL;//はっぱだけ

static ID3D11ShaderResourceView* g_under_Ground_Texture = NULL;//地面のテクスチャ

static ID3D11ShaderResourceView* g_AnchorPoint_Texture = NULL;//アンカーポイントのテクスチャ
static ID3D11ShaderResourceView* g_right_ground_Texture = NULL;//地面右側のテクスチャ
static ID3D11ShaderResourceView* g_left_ground_Texture = NULL;//地面左側のテクスチャ

static ID3D11ShaderResourceView* g_under_right_ground_Texture = NULL;//地面右側のテクスチャ
static ID3D11ShaderResourceView* g_under_left_ground_Texture = NULL;//地面左側のテクスチャ

static ID3D11ShaderResourceView* g_under_right_ground_down_Texture = NULL;//地面右側下側のテクスチャ
static ID3D11ShaderResourceView* g_under_left_ground_down_Texture = NULL;//地面左側下側のテクスチャ

static ID3D11ShaderResourceView* g_sloop_left_side_texture = NULL;//地面スロープの右側
static ID3D11ShaderResourceView* g_sloop_right_side_texture = NULL;//地面スロープの左側



static ID3D11ShaderResourceView* g_invisibility_wall_Texture = NULL;//不可視の壁


//遺跡のステージブロック
static ID3D11ShaderResourceView* g_Iseki_Top_Texture = NULL;		//遺跡のうえ
static ID3D11ShaderResourceView* g_Iseki_Top_Left_Texture = NULL;	//遺跡のうえ左側
static ID3D11ShaderResourceView* g_Iseki_Top_Right_Texture = NULL;	//遺跡のうえ右側

static ID3D11ShaderResourceView* g_Iseki_Texture = NULL;		//遺跡の中
static ID3D11ShaderResourceView* g_Iseki_Left_Texture = NULL;	//遺跡の中左側
static ID3D11ShaderResourceView* g_Iseki_Right_Texture = NULL;	//遺跡の中右側

static ID3D11ShaderResourceView* g_Big_Wood_Texture = NULL;	//大きい木のテクスチャ

static ID3D11ShaderResourceView* g_Iseki_boss_wall_object_Texture = NULL;	//ボスの壁のテクスチャ

//チュートリアルリアルステージのテクスチャ
static ID3D11ShaderResourceView* g_sand_Texture = NULL;	//砂のマップ
static ID3D11ShaderResourceView* g_sand_up_Texture = NULL;	//砂のマップ上


//背景ブロック
static ID3D11ShaderResourceView* g_background_block_Texture = NULL;	//背景地中
static ID3D11ShaderResourceView* g_background_block_down_Texture = NULL;	//背景地上



Field::Field()
{
}



Field::~Field()
{
	Finalize();
}



//初期化
void Field::Initialize()
{
	
	//テクスチャの初期化
	
	if (g_AnchorPoint_Texture == NULL)
	{
		g_AnchorPoint_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_red.png");//アンカーポイントのテクスチャ

		g_Ground_Texture = InitTexture(L"asset\\texture\\stage_block\\1-1_block_grass_02.png");//グラウンドのテクスチャ
		g_under_Ground_Texture = InitTexture(L"asset\\texture\\stage_block\\1-1_block_soil_02.png");//グラウンドのテクスチャ
		g_right_ground_Texture = InitTexture(L"asset\\texture\\stage_block\\1-1_block_right_02.png");//草のテクスチャ　右側
		g_left_ground_Texture = InitTexture(L"asset\\texture\\stage_block\\1-1_block_left_02.png");//草のテクスチャ　左側



		g_invisibility_wall_Texture = InitTexture(L"asset\\texture\\sample_texture\\invisibility_wall.png");//透明な壁のテクスチャ

		g_under_right_ground_Texture = InitTexture(L"asset\\texture\\stage_block\\1-1_block_tuti_right.png");//右側のテクスチャ
		g_under_left_ground_Texture = InitTexture(L"asset\\texture\\stage_block\\1-1_block_tuti_Left.png");//右側のテクスチャ

		g_under_right_ground_down_Texture = InitTexture(L"asset\\texture\\stage_block\\1-1_block_tuti_right.png");//右側のテクスチャ
		g_under_left_ground_down_Texture = InitTexture(L"asset\\texture\\stage_block\\1-1_block_tuti_Left.png");//右側のテクスチャ

		g_sloop_left_side_texture = InitTexture(L"asset\\texture\\stage_block\\1-1_block_connection_Down_02.png");//右側のテクスチャ
		g_sloop_right_side_texture = InitTexture(L"asset\\texture\\stage_block\\1-1_block_connection_slope02.png");//右側のテクスチャ

		//----------------------------------------------------------------------------------------
		//遺跡のテクスチャ

		g_Iseki_Top_Texture = InitTexture(L"asset\\texture\\stage_block\\iseki_block_top.png");				//遺跡の上のテクスチャ
		g_Iseki_Top_Left_Texture = InitTexture(L"asset\\texture\\stage_block\\iseki_block_left.png");		//遺跡の上のテクスチャ  左
		g_Iseki_Top_Right_Texture = InitTexture(L"asset\\texture\\stage_block\\iseki_block_right.png");		//遺跡の上のテクスチャ  右

		g_Iseki_Texture = InitTexture(L"asset\\texture\\stage_block\\iseki_block.png");						//遺跡の中のテクスチャ
		g_Iseki_Left_Texture = InitTexture(L"asset\\texture\\stage_block\\iseki_block_naka_left.png");		//遺跡の中のテクスチャ  左
		g_Iseki_Right_Texture = InitTexture(L"asset\\texture\\stage_block\\iseki_block_naka_right.png");	//遺跡の中のテクスチャ  右


		g_Iseki_boss_wall_object_Texture = InitTexture(L"asset\\texture\\stage_block\\boss_wall_texture.png");	//ボスを倒すための壁

		//----------------------------------------------------------------------------------------
		//砂

		g_sand_Texture = InitTexture(L"asset\\texture\\stage_block\\sand_down.png");//砂のした
		g_sand_up_Texture = InitTexture(L"asset\\texture\\stage_block\\sand_top.png");//砂の上

		g_background_block_Texture= InitTexture(L"asset\\texture\\stage_block\\bg_background_block_up.png");
		g_background_block_down_Texture= InitTexture(L"asset\\texture\\stage_block\\bg_background_block.png");

		
		g_Ground_Grass_only_Texture = InitTexture(L"asset\\texture\\stage_block\\grass_only.png");
	}


	//----------------------------------------------------------------------------------------
	// 
	// 
	// 
	// 
	// 
	// 
	g_Big_Wood_Texture= InitTexture(L"asset\\texture\\wood_texture\\BigWood.png");
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
		Field::LoadCSV("asset/mapchip_stage_1_1.csv");
		break;
	case STAGE_ISEKI:
		// csvからマップチップを読み込む
		Field::LoadCSV("asset/mapchip_iseki.csv");
		break;
	case STAGE_BOSS:
		// csvからマップチップを読み込む
		Field::LoadCSV("asset/mapchip_boss_room.csv");
		break;
	case STAGE_TEST: //テスト用のマップチップを作成しました
		// csvからマップチップを読み込む
		Field::LoadCSV("asset/mapchip_test.csv");
		break;

	default:

		break;
	}

	
	//読み込んだデータをfield_mapに格納
	std::vector<std::vector<int>> field_map = m_field_data;


	//宝石やコインを今回も生成するどうかを決める
	// （生成するなら：respawning -> false）
	//	(生成しないなら：respawning -> true)
	//	respawningは生成処理してる時、引数としてItemManagerに渡せば、本当に生成するかどうかを制御できる
	bool respawning = false;
	Game& game_scene = Game::GetInstance();
	auto state = game_scene.GetCurrentGameState();
	if (state == GAME_STATE_RESPAWN_INITIAL ||
		state == GAME_STATE_RESPAWN_SAVE_POINT||
		state == GAME_STATE_PAUSE_RESPAWN_SAVE_POINT)
	{
		respawning = true;
	}

	switch (sceneManager.GetStageName())
	{
	case STAGE_NULL:
		break;
	case STAGE_TUTORIAL:
		for (int y = 0; y < m_field_height; ++y)
		{
			for (int x = 0; x < m_field_width; ++x)
			{
				if (field_map[y][x] == 1) {//動かない物
					//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
					objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, g_sand_up_Texture, false);
				}
				if (field_map[y][x] == 2) {//動かない物
					//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
					objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, g_Ground_Texture, false);
				}
				if (field_map[y][x] == 3) {//動かない物
					//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
					objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, g_Ground_Texture, false);
				}
				if (field_map[y][x] == 4) {//動かない物
					//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
					objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, g_sand_Texture, false);
				}
				if (field_map[y][x] == 5) {//動かない物
					//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
					objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, g_Ground_Texture, false);
				}
				if (field_map[y][x] == 6) {//動かない物
					//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
					objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, g_Ground_Texture, false);
				}


				//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
				//不可視の壁
				if (field_map[y][x] == 7) {//動かない物
					//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る

					objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, g_invisibility_wall_Texture, false);

				}


				//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
				//プレイヤー
				if (field_map[y][x] == 8) {//プレイヤーの表示

					Player& player = Player::GetInstance();

					b2Vec2 size = player.GetSensorSize();

					player.Finalize();

					const ItemSavePoint* SavePoint = player.GetRegisteredSavePoint();
					if (SavePoint != nullptr)
					{
						player.Initialize(SavePoint->GetRespawnPosition(), b2Vec2(1, 2), size);
						//リスポンした時の効果音
						app_atomex_start(Player_Jewelry_Colect_Sound);
					}
					else
					{
						player.Initialize(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1, 2), size);
					}
				}
				//-------------------------------------------------------------------------------------------


				//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
				//エネミー
				if (field_map[y][x] == 9) {//動かないエネミーの追加
					objectManager.AddEnemyStatic(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f),0.0);
				}
				//-------------------------------------------------------------------------------------------

				//--------------------------------------------------------------------
				//木のオブジェクト
				if (field_map[y][x] == 10) {//小さな木のオブジェクト 必要アンカーレベル１
					objectManager.AddWood(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(2.0f, 10.0f), b2Vec2(2.0f, 2.0f), 1);
				}


				if (field_map[y][x] == 11) {//中くらい木のオブジェクト 必要アンカーレベル2
					objectManager.AddWood(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(2.0f, 12.0f), b2Vec2(2.0f, 1.0f), 1);
				}

				//--------------------------------------------------------------------------------------
				//触れたら死ぬ
				if (field_map[y][x] == 12) {//中くらい木のオブジェクト 必要アンカーレベル2
					objectManager.AddContactBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), DEAD_BLOCK_TYPE,b2Vec2_zero);
				}
				//---------------------------------------------------------------------------------------


				//------------------------------------------------------------------------------------------
				//触れたらステージセレクトに行く
				//-------------------------------------------------------------------------------------------
				if (field_map[y][x] == 13) {//中くらい木のオブジェクト 必要アンカーレベル2
					objectManager.AddContactBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(2.0f, 10.0f), GO_STAGE_SELECT, b2Vec2_zero);
				}
				//-------------------------------------------------------------------------------------------------


				//-----------------------------------------------------------------------------------------------
				//エネミー  動く
				if (field_map[y][x] == 14) {//動くエネミーの追加
					objectManager.AddEnemyDynamic(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0);
				}
				//-----------------------------------------------------------------------------------------------

				//看板のテクスチャ 動く
				if (field_map[y][x] == 20) {
					objectManager.AddUiBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(10.0f, 8.0f), b2Vec2(20.0f, 8.0f),b2Vec2_zero,MOVE_SIGNBOARD,0.0f);
				}

				//看板のテクスチャ ジャンプ
				if (field_map[y][x] == 21) {
					objectManager.AddUiBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(10.0f, 8.0f), b2Vec2(20.0f, 8.0f), b2Vec2_zero, JUMP_SIGNBOARD, 0.0f);
				}

				//看板のテクスチャ アンカー
				if (field_map[y][x] == 22) {
					objectManager.AddUiBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(10.0f, 8.0f), b2Vec2(20.0f, 8.0f), b2Vec2_zero, ANCHOR_SIGNBOARD, 0.0f);
				}


				//看板の矢印
				if (field_map[y][x] == 23) {
					objectManager.AddUiBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(3.0f, 3.0f), b2Vec2(1.0f, 1.0f), b2Vec2_zero, ARROW_RIGHT_SIGNBOARD, 0.f);
				}

				//看板のテクスチャ アンカー
				if (field_map[y][x] == 24) {
					objectManager.AddUiBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(10.0f, 8.0f), b2Vec2(20.0f, 8.0f), b2Vec2_zero, SOUL_SIGNBOARD, 0.0f);
				}


			

			}
		}
		break;
	case STAGE_1_1:
		// csvからマップチップを読み込む
	//マップの数値に応じたオブジェクトを描画するため、対応したオブジェクトのインスタンスを生成
		for (int y = 0; y < m_field_height; ++y)
		{
			for (int x = 0; x < m_field_width; ++x)
			{
				if (field_map[y][x] == 1) {//動かない物
					//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
					objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, g_Ground_Texture, false);
				}
				if (field_map[y][x] == 2) {//動かない物
					//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
					objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, g_left_ground_Texture, false);
				}
				if (field_map[y][x] == 3) {//動かない物
					//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
					objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, g_right_ground_Texture, false);
				}
				if (field_map[y][x] == 4) {//動かない物
					//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
					objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, g_under_Ground_Texture, false);
				}
				if (field_map[y][x] == 5) {//動かない物
					//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
					objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, g_under_left_ground_Texture, false);
				}
				if (field_map[y][x] == 6) {//動かない物
					//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
					objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, g_under_right_ground_Texture, false);
				}
				if (field_map[y][x] == 7) {//動かない物
					//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
					objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, g_sloop_left_side_texture, false);
				}
				if (field_map[y][x] == 8) {//動かない物
					//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
					objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, g_sloop_right_side_texture, false);
				}
				if (field_map[y][x] == 9) {//動かない物
					//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
					objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, g_invisibility_wall_Texture, false);
				}
				if (field_map[y][x] == 10) {//動かない物
					//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
					objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, (y-0.5) / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 0.25f), 0.0f, true, true, g_Ground_Grass_only_Texture, false);
					objectManager.AddTextureBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 0.0, g_background_block_Texture);
				}
				if (field_map[y][x] == 11) {//動かない物
					//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
					objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, g_Ground_Texture, true);
				}
				if (field_map[y][x] == 12) {//動かない物
					//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
					objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, g_under_Ground_Texture, true);
				}
				if (field_map[y][x] == 13) {//動かない物

					Player& player = Player::GetInstance();

					b2Vec2 size = player.GetSensorSize();

					player.Finalize();

					const ItemSavePoint* SavePoint = player.GetRegisteredSavePoint();
					if (SavePoint != nullptr)
					{
						player.Initialize(SavePoint->GetRespawnPosition(), b2Vec2(1, 2), size);
						//リスポンした時の効果音
						app_atomex_start(Player_Jewelry_Colect_Sound);
					}
					else
					{
						player.Initialize(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1, 2), size);
					}
				}

				//------------------------------------------------------------------------------------------------------------------------------------------
				// エネミー
				if (field_map[y][x] == 14) {//エネミーの
					objectManager.AddEnemyDynamic(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f);
				}
				if (field_map[y][x] == 15) {//エネミーの
					objectManager.AddEnemyDynamic(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(2.0f, 2.0f), 0.0f);
				}
				if (field_map[y][x] == 16) {//エネミーの
					objectManager.AddEnemyFloating(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f);
				}
			
				if (field_map[y][x] == 17) {//動かないエネミー
					objectManager.AddEnemyStatic(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f);
				}

				if (field_map[y][x] == 18) {//浮遊エネミー
					objectManager.AddEnemyFloating(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(4.0f, 4.0f), 0.0f);
				}

				//------------------------------------------------------------------------------------------------------------------------------------------
				//木のオブジェクト

				if (field_map[y][x] == 20) {//木のオブジェクト
					objectManager.AddWood(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(2.0f, 10.f), b2Vec2(2.0f, 1.0f),1);
				}

				if (field_map[y][x] == 21) {//木のオブジェクト
					objectManager.AddWood(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(2.0f, 6.f), b2Vec2(2.0f, 1.0f), 1);
				}

				if (field_map[y][x] == 22) {//木のオブジェクト
					objectManager.AddWood(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(2.0f, 9.f), b2Vec2(2.0f, 1.0f), 2);
				}

				///段々と足場ブロック積んでるの下の二つ
				if (field_map[y][x] == 23) {//木のオブジェクト
					objectManager.AddWood(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(2.0f, 6.f), b2Vec2(2.0f, 1.0f), 1);
				}

				//段々と足場ブロック積んでるとこの一番うえ
				if (field_map[y][x] == 24) {//木のオブジェクト
					objectManager.AddWood(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(2.0f, 8.f), b2Vec2(2.0f, 1.0f), 2);
				}

				//数字がとぶ 
				if (field_map[y][x] == 25) {//木のオブジェクト
					objectManager.AddWood(b2Vec2(x / BOX2D_SCALE_MANAGEMENT-(0.5/BOX2D_SCALE_MANAGEMENT), y / BOX2D_SCALE_MANAGEMENT), b2Vec2(2.0f, 10.f), b2Vec2(2.0f, 1.0f), 2);
				}

				if (field_map[y][x] == 26) {//木のオブジェクト
					objectManager.AddWood(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(2.0f, 12.f), b2Vec2(2.0f, 1.0f), 2);
				}


				if (field_map[y][x] == 27) {//木のオブジェクト
					objectManager.AddWood(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(2.0f, 11.f), b2Vec2(2.0f, 1.0f), 1);
				}


			

				//-------------------------------------------------------------------------------------------
			
			
				//-------------------------------------------------------------------------------------------
				//転がす岩
				if (field_map[y][x] == 30) {//岩
					objectManager.AddRock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), 2,1,true);//左側
				}
				if (field_map[y][x] == 31) {//岩
					objectManager.AddRock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), 1.5, 1,true);//左側
				}
				//------------------------------------------------------------------------------------------------------------------------------------------
				//傾斜
				if (field_map[y][x] == 35) {//右下斜面
					objectManager.AddSloping_block(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), right_down);
				}
				if (field_map[y][x] == 36) {//左下斜面
					objectManager.AddSloping_block(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), left_down);
				}

				//--------------------------------------------------------------------------------------------------------------------------------------------
				//静的動的ブロック
				if (field_map[y][x] == 39) {
					objectManager.AddStatic_to_Dynamic_block(b2Vec2(x / BOX2D_SCALE_MANAGEMENT - 0.5 / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT+0.5/BOX2D_SCALE_MANAGEMENT), b2Vec2(4.f, 4.f), Box_collider,2,false);
				}

				if (field_map[y][x] == 40) {
					objectManager.AddStatic_to_Dynamic_block(b2Vec2(x / BOX2D_SCALE_MANAGEMENT , y / BOX2D_SCALE_MANAGEMENT), b2Vec2(3.f, 3.f), Box_collider, 1,false);
				}	
				
				if (field_map[y][x] == 41) {
					objectManager.AddStatic_to_Dynamic_block(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(3.f, 3.f), Box_collider, 1,true);
				}

				//静的動的ブロック
				if (field_map[y][x] == 42) {
					objectManager.AddStatic_to_Dynamic_block(b2Vec2(x / BOX2D_SCALE_MANAGEMENT - 0.5 / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT + 0.5 / BOX2D_SCALE_MANAGEMENT), b2Vec2(6.f, 6.f), Box_collider, 3, false);
				}

				if (field_map[y][x] == 43) {
					objectManager.AddStatic_to_Dynamic_block(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(5.f, 5.f), Box_collider, 1, false);
				}

				if (field_map[y][x] == 44) {
					objectManager.AddStatic_to_Dynamic_block(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(13.f, 5.f), Box_collider, 1, true);
				}
			
				//---------------------------------------------------------------------------------------------------------------------------------------------------
				//コインや宝石
				if (field_map[y][x] == 45) {//コイン
				itemManager.AddCoin(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 0, respawning);
				}
				if (field_map[y][x] == 46) {//青宝石
					itemManager.AddJewel(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, BLUE, respawning);
				}
				if (field_map[y][x] == 47) {//赤宝石
					itemManager.AddJewel(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, RED, respawning);
				}
				if (field_map[y][x] == 48) {//黄色宝石
					itemManager.AddJewel(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, YELLOW, respawning);
				}


				if (field_map[y][x] == 51) {//中間地点
					itemManager.AddSavePoint(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(5.0f, 2.5f), 0.0f, respawning);
				}

				//----------------------------------------------------------------------------------------------------------------------------------------------------
				//間欠泉
				if (field_map[y][x] == 56) {//間欠泉
					objectManager.AddGeyser(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, (y+0.3) / BOX2D_SCALE_MANAGEMENT ), b2Vec2(5.f, 3.f), b2Vec2(2.f,5.f),1,1,1);
				}

				if (field_map[y][x] == 57) {//間欠泉
					objectManager.AddGeyser(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(7.f, 5.f), b2Vec2(3.f, 20.f), 1, 1, 3);
				}


				//-----------------------------------------------------------------------------------------------------------------------------------------------------
				//動かす地面
				if (field_map[y][x] == 60) {//動かす地面
					objectManager.AddMovable_Ground(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(30.f, 9.f), b2Vec2(1.f, 1.f),3);
				}


				//----------------------------------------------------------------------------------------
				//触れたら死ぬ
				if (field_map[y][x] == 65) {
					objectManager.AddContactBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(50.0f, 2.0f), DEAD_BLOCK_TYPE, b2Vec2_zero);
				}

				//回復
				if (field_map[y][x] == 66) {
					itemManager.AddHealing(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(2.0f, 2.0f), 0.0f, respawning);
				}
			//------------------------------------------------------------------------------------------
			//触れたら遺跡ステージに行く
			//-------------------------------------------------------------------------------------------
				if (field_map[y][x] == 70) {
					objectManager.AddContactBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(2.0f, 10.0f), GO_STAGE_ISEKI, b2Vec2_zero);
				}

				//セーブポイント
				if (field_map[y][x] == 71) {
					itemManager.AddSavePoint(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(5.0f, 2.5f),0.0f, respawning);
				}


				//	地上の背景
				if (field_map[y][x] == 72) {
					objectManager.AddTextureBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 0.0, g_background_block_Texture);
				}
				if (field_map[y][x] == 73) {
					objectManager.AddTextureBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 0.0, g_background_block_Texture);
					objectManager.AddTextureBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, (y+1) / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 0.0, g_background_block_Texture);
				}

				//　地下の背景
				if (field_map[y][x] == 74) {
					objectManager.AddTextureBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 0.0, g_background_block_down_Texture);
				}
				if (field_map[y][x] == 75) {
					objectManager.AddTextureBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 0.0, g_background_block_down_Texture);
					objectManager.AddTextureBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, (y+1) / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 0.0, g_background_block_down_Texture);
				}







				//--------------------------------------------------------------------------------------
				//壊れるブロック
				//----------------------------------------------------------------------------------------
				if (field_map[y][x] == 80) {//オブジェクトと接触したら壊れる
					objectManager.AddBreakBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 5,5,0.0, g_Ground_Texture);
					objectManager.AddTextureBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 0.0, g_background_block_down_Texture);
				}

				if (field_map[y][x] == 81) {
					objectManager.AddBreakBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 5, 5, 0.0, g_under_Ground_Texture);
					objectManager.AddTextureBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 0.0, g_background_block_down_Texture);
				}

				if (field_map[y][x] == 82) {//左下斜面
					objectManager.AddSloping_block(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), left_down);
				}
				if (field_map[y][x] == 82) {//
					objectManager.AddBreakBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 5, 5, 0.0, g_under_Ground_Texture);
				}



				//--------------------------------------------------------------------------------------
				//足場ブロック
				if (field_map[y][x] == 95) {
					objectManager.AddOne_way_platformList(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(0.0f, -0.5f), b2Vec2(1.0f, 0.2f) ,false);
					objectManager.AddTextureBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 0.0, g_background_block_down_Texture);
				}


				//--------------------------------------------------------------------------------------
				//でかい木
				if (field_map[y][x] == 96) {
					objectManager.AddTextureBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(30.0f,30.f), 0.0,g_Big_Wood_Texture);
				}

				//----------------------------------------------------------
				//スポナー
				//----------------------------------------------------------
				if (field_map[y][x] == 97) {//エネミーのスポナー
					objectManager.AddSpawnerEnemy(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f);
				}
				if (field_map[y][x] == 98) {//壊れるブロックのスポナー
					objectManager.AddSpawnerBlockDamage(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, 1);
				}

				//回復アイテム
				if (field_map[y][x] == 100) {
					itemManager.AddHealing(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 0, respawning);
				}


				//右に矢印
				if (field_map[y][x] == 101) {
					objectManager.AddUiBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, (y + 0.5) / BOX2D_SCALE_MANAGEMENT), b2Vec2(4.0f, 4.0f), b2Vec2(1.0f, 1.0f), b2Vec2_zero, ARROW_RIGHT_SIGNBOARD, 0.f);
				}


				//右下に矢印
				if (field_map[y][x] == 102) {
					objectManager.AddUiBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, (y + 0.5) / BOX2D_SCALE_MANAGEMENT), b2Vec2(4.0f, 4.0f), b2Vec2(1.0f, 1.0f), b2Vec2_zero, ARROW_RIGHT_DOWN_SIGNBOARD, 0.f);
				}

				//上に矢印
				if (field_map[y][x] == 103) {
					objectManager.AddUiBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, (y + 0.5) / BOX2D_SCALE_MANAGEMENT), b2Vec2(4.0f, 4.0f), b2Vec2(1.0f, 1.0f), b2Vec2_zero, ARROW_UP_SIGNBOARD, 0.f);
				}

				//木
				if (field_map[y][x] == 104) {
					objectManager.AddUiBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, (y + 0.5) / BOX2D_SCALE_MANAGEMENT), b2Vec2(4.0f, 4.0f), b2Vec2(1.0f, 1.0f), b2Vec2_zero, WOOD_SIGNBOARD, 0.f);
				}

				//岩
				if (field_map[y][x] == 105) {
					objectManager.AddUiBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, (y + 0.5) / BOX2D_SCALE_MANAGEMENT), b2Vec2(4.0f, 4.0f), b2Vec2(1.0f, 1.0f), b2Vec2_zero, ROCK_SIGNBOARD, 0.f);
				}
				
				//落ちるブロックの
				if (field_map[y][x] == 106) {
					objectManager.AddUiBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, (y + 0.5) / BOX2D_SCALE_MANAGEMENT), b2Vec2(4.0f, 4.0f), b2Vec2(1.0f, 1.0f), b2Vec2_zero, FALL_SIGNBOARD, 0.f);
				}

				if (field_map[y][x] == 107) {
					objectManager.AddUiBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, (y + 0.5) / BOX2D_SCALE_MANAGEMENT), b2Vec2(4.0f, 4.0f), b2Vec2(1.0f, 1.0f), b2Vec2_zero, ARROW_LEFT_SIGNBOARD, 0.f);
				}


				//ハート
				if (field_map[y][x] == 108) {
					itemManager.AddHealing(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f,respawning);
				}
			}
		}	
		break;

		case STAGE_ISEKI:
			for (int y = 0; y < m_field_height; ++y)
			{
				for (int x = 0; x < m_field_width; ++x)
				{
					if (field_map[y][x] == 1) {//動かない物
						//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
						objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, g_Iseki_Top_Texture, false);
					}
					if (field_map[y][x] == 2) {//動かない物
						//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
						objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, g_Iseki_Top_Left_Texture, false);
					}
					if (field_map[y][x] == 3) {//動かない物
						//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
						objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, g_Iseki_Top_Right_Texture, false);
					}
					if (field_map[y][x] == 4) {//動かない物
						//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
						objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, g_Iseki_Texture, false);
					}
					if (field_map[y][x] == 5) {//動かない物
						//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
						objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, g_Iseki_Left_Texture, false);
					}
					if (field_map[y][x] == 6) {//動かない物
						//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
						objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, g_Iseki_Right_Texture, false);
					}


					if (field_map[y][x] == 7) {//動かない物
						//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
						objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, g_Ground_Texture, false);
					}
			
					if (field_map[y][x] == 8) {//動かない物
						//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
						objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, g_under_Ground_Texture, false);
					}

				

				

					//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
					//不可視の壁
					if (field_map[y][x] == 12) {//動かない物
						//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
						objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, g_invisibility_wall_Texture, false);
					}


					//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
					//プレイヤー
					if (field_map[y][x] == 13) {//プレイヤーの表示

						Player& player = Player::GetInstance();

						b2Vec2 size = player.GetSensorSize();

						player.Finalize();

						const ItemSavePoint* SavePoint = player.GetRegisteredSavePoint();
						if (SavePoint != nullptr)
						{
							player.Initialize(SavePoint->GetRespawnPosition(), b2Vec2(1, 2), size);
							//リスポンした時の効果音
							app_atomex_start(Player_Jewelry_Colect_Sound);
						}
						else
						{
							player.Initialize(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1, 2), size);
						}
					}
					//-------------------------------------------------------------------------------------------

					//------------------------------------------------------------------------------------------------------------------------------------------
					//傾斜

					if (field_map[y][x] == 14) {//右下斜面
						objectManager.AddSloping_block(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), right_down);
					}
					if (field_map[y][x] == 15) {//左下斜面
						objectManager.AddSloping_block(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), left_down);
					}

					if (field_map[y][x] == 16) {//右下斜面
						objectManager.AddSloping_block(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), right_up);
					}
					if (field_map[y][x] == 17) {//左下斜面
						objectManager.AddSloping_block(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), left_up);
					}
					//------------------------------------------------------------------------------------------------------------------------------


					//-----------------------------------------------------------------------------------------------------------------------------
					//コインや宝石
					if (field_map[y][x] == 20) {//コイン
						itemManager.AddCoin(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 0, respawning);
					}
					if (field_map[y][x] == 21) {//黄宝石
						itemManager.AddJewel(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, YELLOW, respawning);
					}

					//------------------------------------------------------------------------------------------------------------------------------------------
					//岩のオブジェクト

					//-------------------------------------------------------------------------------------------
					//転がす岩
					if (field_map[y][x] == 25) {//岩
						objectManager.AddRock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), 2, 1, true);//左側
					}
					if (field_map[y][x] == 26){//岩
						objectManager.AddRock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), 3, 3, true);//右側
					}


					//------------------------------------------------------------------------------------------------------------------------------------------
					// エネミー
					if (field_map[y][x] == 35) {//エネミーの
						objectManager.AddEnemyDynamic(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f);
					}
					if (field_map[y][x] == 36) {//エネミーの
						objectManager.AddEnemyDynamic(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(2.0f, 2.0f), 0.0f);
					}
					if (field_map[y][x] == 37) {//エネミーの
						objectManager.AddEnemyFloating(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f);
					}

					if (field_map[y][x] == 38) {//動かないエネミー
						objectManager.AddEnemyStatic(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f);
					}

					if (field_map[y][x] == 39) {//浮遊エネミー
						objectManager.AddEnemyFloating(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(4.0f, 4.0f), 0.0f);
					}

					//--------------------------------------------------------------------------------------------------------------------------------
					//ジャンプ台
					if (field_map[y][x] == 45) {
						objectManager.AddBossBoundBlock(b2Vec2(x  / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(3.f, 3.f), b2Vec2(0.0f,-0.8), boss_room_level_20, 0);
					}

					//大ジャンプ台
					if (field_map[y][x] == 46) {
						objectManager.AddBossBoundBlock(b2Vec2((x-0.5) / BOX2D_SCALE_MANAGEMENT, (y-0.5) / BOX2D_SCALE_MANAGEMENT), b2Vec2(4.f, 4.f), b2Vec2(0.0f, -0.9), boss_room_level_20, 0);
					}


					//--------------------------------------------------------------------------------------------------------------------------------------------------------
					//壊れるブロック
					if (field_map[y][x] == 50) {//オブジェクトと接触したら壊れる
						objectManager.AddBreakBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 5, 5, 0.0, g_Iseki_Texture);
					}

					if (field_map[y][x] == 51) {//オブジェクトと接触したら壊れる
						objectManager.AddBreakBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 5, 5, 0.0, g_Iseki_Texture);
					}

					if (field_map[y][x] == 51) {//左下斜面
						objectManager.AddSloping_block(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), left_down);
					}

					if (field_map[y][x] == 52) {//オブジェクトと接触したら壊れる
						objectManager.AddBreakBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 5, 5, 0.0, g_Iseki_Texture);
					}

					if (field_map[y][x] == 52) {//左下斜面
						objectManager.AddSloping_block(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), right_down);
					}

					if (field_map[y][x] == 53) {//オブジェクトと接触したら壊れる
						objectManager.AddBreakBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 5, 5, 0.0, g_Iseki_Top_Texture);
					}
					


					//------------------------------------------------------------------------------------------------------
					//柱オブジェクト
					if (field_map[y][x] == 60) {
						objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(2.0f,10.0f), 1, 6, boss_room_level_20,1);
					}

			
					//ボスの登場ムービーシーンに移行
					if (field_map[y][x] == 70) {
						objectManager.AddContactBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(2.0f, 10.0f), GO_BOSS_MOVIE, b2Vec2_zero);
					}



					if (field_map[y][x] == 79) {
						objectManager.AddUiBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, (y + 0.5) / BOX2D_SCALE_MANAGEMENT), b2Vec2(4.0f, 4.0f), b2Vec2(1.0f, 1.0f), b2Vec2_zero, ARROW_RIGHT_SIGNBOARD, 0.f);
					}

					//右下に矢印
					if (field_map[y][x] == 80) {
						objectManager.AddUiBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, (y + 0.5) / BOX2D_SCALE_MANAGEMENT), b2Vec2(4.0f, 4.0f), b2Vec2(1.0f, 1.0f), b2Vec2_zero, ARROW_RIGHT_DOWN_SIGNBOARD, 0.f);
					}

					//上に矢印
					if (field_map[y][x] == 81) {
						objectManager.AddUiBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, (y + 0.5) / BOX2D_SCALE_MANAGEMENT), b2Vec2(4.0f, 4.0f), b2Vec2(1.0f, 1.0f), b2Vec2_zero, ARROW_UP_SIGNBOARD, 0.f);
					}

					//木
					if (field_map[y][x] == 82) {
						objectManager.AddUiBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, (y + 0.5) / BOX2D_SCALE_MANAGEMENT), b2Vec2(4.0f, 4.0f), b2Vec2(1.0f, 1.0f), b2Vec2_zero, WOOD_SIGNBOARD, 0.f);
					}

					//岩
					if (field_map[y][x] == 83) {
						objectManager.AddUiBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, (y + 0.5) / BOX2D_SCALE_MANAGEMENT), b2Vec2(4.0f, 4.0f), b2Vec2(1.0f, 1.0f), b2Vec2_zero, ROCK_SIGNBOARD, 0.f);
					}

					//落ちるブロックの
					if (field_map[y][x] == 84) {
						objectManager.AddUiBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, (y + 0.5) / BOX2D_SCALE_MANAGEMENT), b2Vec2(4.0f, 4.0f), b2Vec2(1.0f, 1.0f), b2Vec2_zero, FALL_SIGNBOARD, 0.f);
					}

					if (field_map[y][x] == 85) {
						objectManager.AddUiBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, (y + 0.5) / BOX2D_SCALE_MANAGEMENT), b2Vec2(4.0f, 4.0f), b2Vec2(1.0f, 1.0f), b2Vec2_zero, ARROW_LEFT_SIGNBOARD, 0.f);
					}

					if (field_map[y][x] == 86) {
						objectManager.AddUiBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(10.0f, 8.0f), b2Vec2(30.0f, 8.0f), b2Vec2_zero, JUMP_SIGNBOARD, 0.0f);
					}

					if (field_map[y][x] == 87) {
						objectManager.AddUiBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, (y + 0.5) / BOX2D_SCALE_MANAGEMENT), b2Vec2(4.0f, 4.0f), b2Vec2(1.0f, 1.0f), b2Vec2_zero, BOSS_SIGNBOARD, 0.f);
					}

					//------------------------------------------------------------------------------------------------------
					//中間地
					if (field_map[y][x] == 99) {//中間地
						itemManager.AddSavePoint(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(5.0f, 2.5f), 0.0f, respawning);
					}


				}
			}
			break;
	case STAGE_BOSS:
		// csvからマップチップを読み込む
	

		for (int y = 0; y < m_field_height; ++y)
		{
			for (int x = 0; x < m_field_width; ++x)
			{
				if (field_map[y][x] == 1) {//動かない物
					//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
					objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, g_Iseki_Texture, false);
				}
				if (field_map[y][x] == 2) {//動かない物
					//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
					objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.1f), 0.0f, true, true, g_Ground_Texture, false);
				}
				if (field_map[y][x] == 3) {//動かない物
					//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
					objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, g_Ground_Texture, false);
				}
				if (field_map[y][x] == 4) {//動かない物
					//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
					objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, g_Ground_Texture, false);
				}
				if (field_map[y][x] == 5) {//動かない物
					//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
					objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, g_Ground_Texture, false);
				}
				if (field_map[y][x] == 6) {//動かない物
					//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
					objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, g_Ground_Texture, false);
				}
				if (field_map[y][x] == 7) {//動かない物
					//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
					objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, g_Ground_Texture, false);
				}
				if (field_map[y][x] == 8) {//動かない物
					//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
					objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, g_Ground_Texture, false);
				}
				if (field_map[y][x] == 9) {//動かない物
					//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
					objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, g_Ground_Texture, false);
				}
				if (field_map[y][x] == 10) {//動かない物
					//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
					objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, g_Ground_Texture, false);
				}
				if (field_map[y][x] == 11) {//動かない物
					itemManager.AddHealing(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, respawning);
				}




				//if (field_map[y][x] == 12) {//壊れるブロック
				//	objectManager.AddBreakBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3,3,0.0,g_under_Ground_Texture);
				//}

				if (field_map[y][x] == 12) {//壊れるブロック
					objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(3.0f, 25.0f), 0.0f, true, true, g_Iseki_boss_wall_object_Texture, false);
				}


				if (field_map[y][x] == 13) {//動かない物

					Player &player = Player::GetInstance();

					b2Vec2 size = player.GetSensorSize();

					player.Finalize();

					const ItemSavePoint* SavePoint = player.GetRegisteredSavePoint();
					if (SavePoint != nullptr)
					{
						player.Initialize(SavePoint->GetRespawnPosition(), b2Vec2(1, 2), size);
						//リスポンした時の効果音
						app_atomex_start(Player_Jewelry_Colect_Sound);
					}
					else
					{
						player.Initialize(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1, 2), size);
					}
				}

				
				//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

				if (field_map[y][x] == 14)
				{//ボスを生成
					Boss_1_1& boss = Boss_1_1::GetInstance();
					boss.Initialize(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(12.f, 16.f), true);
				}

				//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				if (field_map[y][x] == 15) {//ボスの地面ブロック破壊できる
					objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_1,1);
				}
				if (field_map[y][x] == 16) {//ボスの地面ブロック破壊できる
					objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_2,1);
				}
				if (field_map[y][x] == 17) {//ボスの地面ブロック破壊できる
					objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_3, 1);
				}
				if (field_map[y][x] == 18) {//ボスの地面ブロック破壊できる
					objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_4, 1);
				}
				if (field_map[y][x] == 19) {//ボスの地面ブロック破壊できる
					objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_5, 1);
				}
				if (field_map[y][x] == 20) {//ボスの地面ブロック破壊できる
					objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_6, 1);
				}
				if (field_map[y][x] == 21) {//ボスの地面ブロック破壊できる
					objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_7, 1);
				}
				if (field_map[y][x] == 22) {//ボスの地面ブロック破壊できる
					objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_8, 1);
				}
				if (field_map[y][x] == 23) {//ボスの地面ブロック破壊できる
					objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_9, 1);
				}
				if (field_map[y][x] == 24) {//ボスの地面ブロック破壊できる
					objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_10, 1);
				}
				if (field_map[y][x] == 25) {//ボスの地面ブロック破壊できる
					objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_11, 1);
				}
				if (field_map[y][x] == 26) {//ボスの地面ブロック破壊できる
					objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_12, 1);
				}
				if (field_map[y][x] == 27) {//ボスの地面ブロック破壊できる
					objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_13, 1);
				}
				if (field_map[y][x] == 25) {//ボスの地面ブロック破壊できる
					objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_14, 1);
				}
				if (field_map[y][x] == 26) {//ボスの地面ブロック破壊できる
					objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_15, 1);
				}
				if (field_map[y][x] == 27) {//ボスの地面ブロック破壊できる
					objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_16, 1);
				}
				if (field_map[y][x] == 28) {//ボスの地面ブロック破壊できる
					objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_17, 1);
				}

				if (field_map[y][x] == 29) {//ボスの地面ブロック破壊できる
					objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_18, 1);
				}
				if (field_map[y][x] == 30) {//ボスの地面ブロック破壊できる
					objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_19, 1);
				}
				if (field_map[y][x] == 31) {//ボスの地面ブロック破壊できる
					objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_20, 1);
				}
				
				//----------------------------------------------------------------------------------------------------------------------------------------------------
				//ボスのオブジェクトに使う柱
				b2Vec2 pillar_size=b2Vec2(1.5f,12.5f);

				if (field_map[y][x] == 32) {
					objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), pillar_size, 1, 6, boss_room_level_2,1);
				}
				//ボスのオブジェクトに使う柱
				if (field_map[y][x] == 33) {
					objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), pillar_size, 1, 6, boss_room_level_3,1);
				}
				//ボスのオブジェクトに使う柱
				if (field_map[y][x] == 34) {
					objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), pillar_size, 1, 6, boss_room_level_4,1);
				}
				//ボスのオブジェクトに使う柱
				if (field_map[y][x] == 35) {
					objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), pillar_size, 1, 6, boss_room_level_5,1);
				}
				//ボスのオブジェクトに使う柱
				if (field_map[y][x] == 36) {
					objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), pillar_size, 1, 6, boss_room_level_6,1);
				}
				//ボスのオブジェクトに使う柱
				if (field_map[y][x] == 37) { 
					objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), pillar_size, 1, 6, boss_room_level_7,1);
				}
				//ボスのオブジェクトに使う柱
				if (field_map[y][x] == 38) { 
					objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), pillar_size, 1, 6, boss_room_level_8,1);
				}
				//ボスのオブジェクトに使う柱
				if (field_map[y][x] == 39) { 
					objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), pillar_size, 1, 6, boss_room_level_9,1);
				}
				//ボスのオブジェクトに使う柱
				if (field_map[y][x] == 40) { 
					objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), pillar_size, 1, 6, boss_room_level_10,1);
				}
				//ボスのオブジェクトに使う柱
				if (field_map[y][x] == 41) { 
					objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), pillar_size, 1, 6, boss_room_level_11,1);
				}
				//ボスのオブジェクトに使う柱
				if (field_map[y][x] == 42) { 
					objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), pillar_size, 1, 6, boss_room_level_12,1);
				}
				//ボスのオブジェクトに使う柱
				if (field_map[y][x] == 43) { 
					objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), pillar_size, 1, 6, boss_room_level_13,1);
				}
				//ボスのオブジェクトに使う柱
				if (field_map[y][x] == 44) { 
					objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), pillar_size, 1, 6, boss_room_level_14,1);
				}
				//ボスのオブジェクトに使う柱
				if (field_map[y][x] == 45) { 
					objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), pillar_size, 1, 6, boss_room_level_15,1);
				}
				//ボスのオブジェクトに使う柱
				if (field_map[y][x] == 46) { 
					objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), pillar_size, 1, 6, boss_room_level_16,1);
				}
				//ボスのオブジェクトに使う柱
				if (field_map[y][x] == 47) { 
					objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), pillar_size, 1, 6, boss_room_level_17,1);
				}
				//ボスのオブジェクトに使う柱
				if (field_map[y][x] == 48) { 
					objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), pillar_size, 1, 6, boss_room_level_18,1);
				}
				//ボスのオブジェクトに使う柱
				if (field_map[y][x] == 49) { 
					objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), pillar_size, 1, 6, boss_room_level_19,1);
				}
				//ボスのオブジェクトに使う柱
				if (field_map[y][x] == 50) { 
					objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), pillar_size, 1, 6, boss_room_level_20,1);
				}
				//----------------------------------------------------------------------------------------------------------------------------------------------
				//ボス部屋のジャンプ台
				b2Vec2 jump_power = b2Vec2(0.0f, -0.8f);
				int jump_texture_type = 1;

				if (field_map[y][x] == 51) {
					objectManager.AddBossBoundBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(3.f, 3.f),jump_power,boss_room_level_2, jump_texture_type);
				}
				if (field_map[y][x] == 52) {
					objectManager.AddBossBoundBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(3.f, 3.f), jump_power, boss_room_level_3, jump_texture_type);
				}
				if (field_map[y][x] == 53) {
					objectManager.AddBossBoundBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(3.f, 3.f), jump_power, boss_room_level_4, jump_texture_type);
				}
				if (field_map[y][x] == 54) {
					objectManager.AddBossBoundBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(3.f, 3.f), jump_power, boss_room_level_5, jump_texture_type);
				}
				if (field_map[y][x] == 55) {
					objectManager.AddBossBoundBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(3.f, 3.f), jump_power, boss_room_level_6, jump_texture_type);
				}
				if (field_map[y][x] == 56) {
					objectManager.AddBossBoundBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(3.f, 3.f), jump_power, boss_room_level_7, jump_texture_type);
				}
				if (field_map[y][x] == 57) {
					objectManager.AddBossBoundBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(3.f, 3.f), jump_power, boss_room_level_8, jump_texture_type);
				}
				if (field_map[y][x] == 58) {
					objectManager.AddBossBoundBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(3.f, 3.f), jump_power, boss_room_level_9, jump_texture_type);
				}
				if (field_map[y][x] == 59) {
					objectManager.AddBossBoundBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(3.f, 3.f), jump_power, boss_room_level_10, jump_texture_type);
				}
				if (field_map[y][x] == 60) {
					objectManager.AddBossBoundBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(3.f, 3.f), jump_power, boss_room_level_11, jump_texture_type);
				}
				if (field_map[y][x] == 61) {
					objectManager.AddBossBoundBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(3.f, 3.f), jump_power, boss_room_level_12, jump_texture_type);
				}
				if (field_map[y][x] == 62) {
					objectManager.AddBossBoundBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(3.f, 3.f), jump_power, boss_room_level_13, jump_texture_type);
				}
				if (field_map[y][x] == 63) {
					objectManager.AddBossBoundBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(3.f, 3.f), jump_power, boss_room_level_14, jump_texture_type);
				}
				if (field_map[y][x] == 64) {
					objectManager.AddBossBoundBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(3.f, 3.f), jump_power, boss_room_level_15, jump_texture_type);
				}
				if (field_map[y][x] == 65) {
					objectManager.AddBossBoundBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(3.f, 3.f), jump_power, boss_room_level_16, jump_texture_type);
				}
				if (field_map[y][x] == 66) {
					objectManager.AddBossBoundBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(3.f, 3.f), jump_power, boss_room_level_17, jump_texture_type);
				}
				if (field_map[y][x] == 67) {
					objectManager.AddBossBoundBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(3.f, 3.f), jump_power, boss_room_level_18, jump_texture_type);
				}
				if (field_map[y][x] == 68) {
					objectManager.AddBossBoundBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(3.f, 3.f), jump_power, boss_room_level_19, jump_texture_type);
				}
				if (field_map[y][x] == 69) {
					objectManager.AddBossBoundBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(3.f, 3.f), jump_power, boss_room_level_20, jump_texture_type);
				}
				

				b2Vec2 boss_wall_size = { 17.f,25.f };
				if (field_map[y][x] == 70) {
					objectManager.AddBossWall(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), boss_wall_size, 6, 6, g_Iseki_boss_wall_object_Texture, false);
				}
				//ボスのオブジェクトに使う壁
				if (field_map[y][x] == 71) {
					objectManager.AddBossWall(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), boss_wall_size, 6, 6, g_Iseki_boss_wall_object_Texture,true);
				}



				//左向き
				if (field_map[y][x] == 80) {
					objectManager.AddBossCarryEnemySpawner(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.0f),boss_room_level_2, true);
				}
				if (field_map[y][x] == 81) {
					objectManager.AddBossCarryEnemySpawner(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.0f), boss_room_level_3, true);
				}
				if (field_map[y][x] == 82) {
					objectManager.AddBossCarryEnemySpawner(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.0f), boss_room_level_4, true);
				}
				if (field_map[y][x] == 83) {
					objectManager.AddBossCarryEnemySpawner(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.0f), boss_room_level_5, true);
				}
				if (field_map[y][x] == 84) {
					objectManager.AddBossCarryEnemySpawner(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.0f), boss_room_level_6, true);
				}
				if (field_map[y][x] == 85) {
					objectManager.AddBossCarryEnemySpawner(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.0f), boss_room_level_7, true);
				}
				if (field_map[y][x] == 86) {
					objectManager.AddBossCarryEnemySpawner(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.0f), boss_room_level_8, true);
				}
				if (field_map[y][x] == 87) {
					objectManager.AddBossCarryEnemySpawner(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.0f), boss_room_level_9, true);
				}
				if (field_map[y][x] == 88) {
					objectManager.AddBossCarryEnemySpawner(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.0f), boss_room_level_10, true);
				}
				if (field_map[y][x] == 89) {
					objectManager.AddBossCarryEnemySpawner(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.0f), boss_room_level_11, true);
				}
				if (field_map[y][x] == 90) {
					objectManager.AddBossCarryEnemySpawner(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.0f), boss_room_level_12, true);
				}
				if (field_map[y][x] == 91) {
					objectManager.AddBossCarryEnemySpawner(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.0f), boss_room_level_13, true);
				}



				//右向き
				if (field_map[y][x] == 100) {
					objectManager.AddBossCarryEnemySpawner(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.0f), boss_room_level_2, false);
				}
				if (field_map[y][x] == 101) {
					objectManager.AddBossCarryEnemySpawner(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.0f), boss_room_level_3, false);
				}
				if (field_map[y][x] == 102) {
					objectManager.AddBossCarryEnemySpawner(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.0f), boss_room_level_4, false);
				}
				if (field_map[y][x] == 103) {
					objectManager.AddBossCarryEnemySpawner(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.0f), boss_room_level_5, false);
				}
				if (field_map[y][x] == 104) {
					objectManager.AddBossCarryEnemySpawner(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.0f), boss_room_level_6, false);
				}
				if (field_map[y][x] == 105) {
					objectManager.AddBossCarryEnemySpawner(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.0f), boss_room_level_7, false);
				}
				if (field_map[y][x] == 106) {
					objectManager.AddBossCarryEnemySpawner(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.0f), boss_room_level_8, false);
				}
				if (field_map[y][x] == 107) {
					objectManager.AddBossCarryEnemySpawner(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.0f), boss_room_level_9, false);
				}
				if (field_map[y][x] == 108) {
					objectManager.AddBossCarryEnemySpawner(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.0f), boss_room_level_10, false);
				}
				if (field_map[y][x] == 109) {
					objectManager.AddBossCarryEnemySpawner(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.0f), boss_room_level_11, false);
				}
				if (field_map[y][x] == 110) {
					objectManager.AddBossCarryEnemySpawner(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.0f), boss_room_level_12, false);
				}
				if (field_map[y][x] == 111) {
					objectManager.AddBossCarryEnemySpawner(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.0f), boss_room_level_13, false);
				}


				//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				if (field_map[y][x] == 115) {//ボスの地面ブロック破壊できる
					objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_1, 0);
				}
				if (field_map[y][x] == 116) {//ボスの地面ブロック破壊できる
					objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_2, 0);
				}
				if (field_map[y][x] == 117) {//ボスの地面ブロック破壊できる
					objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_3, 0);
				}
				if (field_map[y][x] == 118) {//ボスの地面ブロック破壊できる
					objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_4, 0);
				}
				if (field_map[y][x] == 119) {//ボスの地面ブロック破壊できる
					objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_5, 0);
				}
				if (field_map[y][x] == 120) {//ボスの地面ブロック破壊できる
					objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_6, 0);
				}
				if (field_map[y][x] == 121) {//ボスの地面ブロック破壊できる
					objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_7, 0);
				}
				if (field_map[y][x] == 122) {//ボスの地面ブロック破壊できる
					objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_8, 0);
				}
				if (field_map[y][x] == 123) {//ボスの地面ブロック破壊できる
					objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_9, 0);
				}
				if (field_map[y][x] == 124) {//ボスの地面ブロック破壊できる
					objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_10, 0);
				}
				if (field_map[y][x] == 125) {//ボスの地面ブロック破壊できる
					objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_11, 0);
				}
				if (field_map[y][x] == 126) {//ボスの地面ブロック破壊できる
					objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_12, 0);
				}
				if (field_map[y][x] == 127) {//ボスの地面ブロック破壊できる
					objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_13, 0);
				}
				if (field_map[y][x] == 125) {//ボスの地面ブロック破壊できる
					objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_14, 0);
				}
				if (field_map[y][x] == 126) {//ボスの地面ブロック破壊できる
					objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_15, 0);
				}
				if (field_map[y][x] == 127) {//ボスの地面ブロック破壊できる
					objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_16, 0);
				}
				if (field_map[y][x] == 128) {//ボスの地面ブロック破壊できる
					objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_17, 0);
				}

				if (field_map[y][x] == 129) {//ボスの地面ブロック破壊できる
					objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_18, 0);
				}
				if (field_map[y][x] == 130) {//ボスの地面ブロック破壊できる
					objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_19, 0);
				}
				if (field_map[y][x] == 131) {//ボスの地面ブロック破壊できる
					objectManager.AddBossFieldBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 3, boss_room_level_20, 0);
				}

				//ボスのオブジェクトに使う柱
				b2Vec2 levl1_pillar_size = b2Vec2(3.0f, 14.5f);

				if (field_map[y][x] == 132) {
					objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), levl1_pillar_size, 1, 6, boss_room_level_2,2);
				}
				//ボスのオブジェクトに使う柱
				if (field_map[y][x] == 133) {
					objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), levl1_pillar_size, 1, 6, boss_room_level_3,2);
				}
				//ボスのオブジェクトに使う柱
				if (field_map[y][x] == 134) {
					objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), levl1_pillar_size, 1, 6, boss_room_level_4,2);
				}
				//ボスのオブジェクトに使う柱
				if (field_map[y][x] == 135) {
					objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), levl1_pillar_size, 1, 6, boss_room_level_5,2);
				}
				//ボスのオブジェクトに使う柱
				if (field_map[y][x] == 136) {
					objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), levl1_pillar_size, 1, 6, boss_room_level_6,2);
				}
				//ボスのオブジェクトに使う柱
				if (field_map[y][x] == 137) {
					objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), levl1_pillar_size, 1, 6, boss_room_level_7,2);
				}
				//ボスのオブジェクトに使う柱
				if (field_map[y][x] == 138) {
					objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), levl1_pillar_size, 1, 6, boss_room_level_8,2);
				}
				//ボスのオブジェクトに使う柱
				if (field_map[y][x] == 139) {
					objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), levl1_pillar_size, 1, 6, boss_room_level_9,2);
				}
				//ボスのオブジェクトに使う柱
				if (field_map[y][x] == 140) {
					objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), levl1_pillar_size, 1, 6, boss_room_level_10,2);
				}
				//ボスのオブジェクトに使う柱
				if (field_map[y][x] == 141) {
					objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), levl1_pillar_size, 1, 6, boss_room_level_11,2);
				}
				//ボスのオブジェクトに使う柱
				if (field_map[y][x] == 142) {
					objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), levl1_pillar_size, 1, 6, boss_room_level_12,2);
				}
				//ボスのオブジェクトに使う柱
				if (field_map[y][x] == 143) {
					objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), levl1_pillar_size, 1, 6, boss_room_level_13,2);
				}
				//ボスのオブジェクトに使う柱
				if (field_map[y][x] == 144) {
					objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), levl1_pillar_size, 1, 6, boss_room_level_14,2);
				}
				//ボスのオブジェクトに使う柱
				if (field_map[y][x] == 145) {
					objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), levl1_pillar_size, 1, 6, boss_room_level_15,2);
				}
				//ボスのオブジェクトに使う柱
				if (field_map[y][x] == 146) {
					objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), levl1_pillar_size, 1, 6, boss_room_level_16,2);
				}
				//ボスのオブジェクトに使う柱
				if (field_map[y][x] == 147) {
					objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), levl1_pillar_size, 1, 6, boss_room_level_17,2);
				}
				//ボスのオブジェクトに使う柱
				if (field_map[y][x] == 148) {
					objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), levl1_pillar_size, 1, 6, boss_room_level_18,2);
				}
				//ボスのオブジェクトに使う柱
				if (field_map[y][x] == 149) {
					objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), levl1_pillar_size, 1, 6, boss_room_level_19,2);
				}
				//ボスのオブジェクトに使う柱
				if (field_map[y][x] == 150) {
					objectManager.AddBossPillar(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), levl1_pillar_size, 1, 6, boss_room_level_20,2);
				}



			}
		}
		break;
	case STAGE_TEST:

		for (int y = 0; y < m_field_height; ++y)
		{
			for (int x = 0; x < m_field_width; ++x)
			{
				if (field_map[y][x] == 1) {//動かない物
					//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
					objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, g_Iseki_Texture, false);
				}
				if (field_map[y][x] == 2) {//動かない物  
					//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
					objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, g_Iseki_Left_Texture, false);
				}
				if (field_map[y][x] == 3) {//動かない物
					//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
					objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, g_Iseki_Right_Texture, false);
				}
				if (field_map[y][x] == 4) {//動かない物
					//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
					objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, g_Iseki_Top_Texture, false);
				}
				if (field_map[y][x] == 5) {//動かない物
					//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
					objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, g_Iseki_Top_Left_Texture, false);
				}
				if (field_map[y][x] == 6) {//動かない物
					//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
					objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, g_Iseki_Top_Right_Texture, false);
				}


				//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
				//不可視の壁
				if (field_map[y][x] == 7) {//動かない物
					//Sizeを BOX2D_SCALE_MANAGEMENTで割ってる影響で　座標の登録位置も割る
					objectManager.AddGround(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, g_invisibility_wall_Texture, false);
				}


				//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
				//プレイヤー
				if (field_map[y][x] == 8) {//プレイヤーの表示

					Player& player = Player::GetInstance();

					b2Vec2 size = player.GetSensorSize();

					player.Finalize();

					const ItemSavePoint* SavePoint = player.GetRegisteredSavePoint();
					if (SavePoint != nullptr)
					{
						player.Initialize(SavePoint->GetRespawnPosition(), b2Vec2(1, 2), size);
						//リスポンした時の効果音
						app_atomex_start(Player_Jewelry_Colect_Sound);
					}
					else
					{
						player.Initialize(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1, 2), size);
					}
				}
				//-------------------------------------------------------------------------------------------


				//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
				//エネミー
				if (field_map[y][x] == 9) {//動かないエネミーの追加
					objectManager.AddEnemyStatic(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0);
				}
				//-------------------------------------------------------------------------------------------

				//--------------------------------------------------------------------
				//木のオブジェクト
				if (field_map[y][x] == 10) {//小さな木のオブジェクト 必要アンカーレベル１
					objectManager.AddWood(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(2.0f, 10.0f), b2Vec2(2.0f, 2.0f), 1);
				}


				if (field_map[y][x] == 11) {//中くらい木のオブジェクト 必要アンカーレベル2
					objectManager.AddWood(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(2.0f, 12.0f), b2Vec2(2.0f, 1.0f), 2);
				}

				//--------------------------------------------------------------------------------------
				//触れたら死ぬ
				if (field_map[y][x] == 12) {//中くらい木のオブジェクト 必要アンカーレベル2
					objectManager.AddContactBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), DEAD_BLOCK_TYPE, b2Vec2_zero);
				}
				//---------------------------------------------------------------------------------------


				//------------------------------------------------------------------------------------------
				//触れたらステージセレクトに行く
				//-------------------------------------------------------------------------------------------
				if (field_map[y][x] == 13) {//中くらい木のオブジェクト 必要アンカーレベル2
					objectManager.AddContactBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(2.0f, 10.0f), GO_STAGE_SELECT, b2Vec2_zero);
				}
				//-------------------------------------------------------------------------------------------------


				//-----------------------------------------------------------------------------------------------
				//エネミー  動く
				if (field_map[y][x] == 14) {//動かないエネミーの追加
					objectManager.AddEnemyDynamic(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0);
				}
				//-----------------------------------------------------------------------------------------------

				if (field_map[y][x] == 15) {//壊れるオブジェク
					objectManager.AddBreakBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 4,4,0, g_under_Ground_Texture);
				}

				if (field_map[y][x] == 16) {//上から落ちるや
					objectManager.AddStatic_to_Dynamic_block(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(3.0f, 3.0f),Box_collider,1,false);
				}


				if (field_map[y][x] == 17) {//壊れるオブジェク
					objectManager.AddBreakBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 4, 4, 0, g_under_Ground_Texture);
				}
				
				if (field_map[y][x] == 17) {//壊れるオブジェク
					objectManager.AddSloping_block(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), left_down);
				}

				if (field_map[y][x] == 18) {//転がる岩
					objectManager.AddRock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT),2,2,false);
				}

				if (field_map[y][x] == 20) {//上から落ちるや
					objectManager.AddStatic_to_Dynamic_block(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(3.0f, 3.0f), Box_collider, 1, true);
				}

				if (field_map[y][x] == 21) {//上から落ちるや
					objectManager.AddTextureBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f,g_left_ground_Texture);
				}

				if (field_map[y][x] == 25) {//上から落ちるや
					itemManager.AddCoin(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.f, 1.f), 0, respawning);
				}


				if (field_map[y][x] == 26) {//上から落ちるや
					objectManager.AddUiBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(100.f, 100.f), b2Vec2(9.f,SCREEN_SCALE), b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), VIDEO_BUTTON_A, 0.0f);
				}

				if (field_map[y][x] == 27) {//上から落ちるや
					objectManager.AddUiBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(100.f, 100.f), b2Vec2(9.f, SCREEN_SCALE), b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), VIDEO_BUTTON_LEFT_STICK, 0.0f);
				}
				if (field_map[y][x] == 28) {//上から落ちるや
					objectManager.AddUiBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(100.f, 100.f), b2Vec2(9.f, SCREEN_SCALE), b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), VIDEO_BUTTON_RIGHT_STICK, 0.0f);
				}
				if (field_map[y][x] == 46) {//青宝石
					itemManager.AddJewel(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, BLUE, respawning);
				}
				if (field_map[y][x] == 47) {//赤宝石
					itemManager.AddJewel(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, RED, respawning);
				}
				if (field_map[y][x] == 48) {//黄色宝石
					itemManager.AddJewel(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, YELLOW, respawning);
				}


				if (field_map[y][x] == 70) {
					objectManager.AddContactBlock(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(2.0f, 10.0f), GO_BOSS_STAGE, b2Vec2_zero);
				}

				if (field_map[y][x] == 49) {//中間地
					itemManager.AddSavePoint(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(5.0f, 2.5f), 0.0f,respawning);
				}


			}
		}
		
		break;
	default:
		break;
	}

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

//	// スケールをかけないとオブジェクトのサイズの表示が小さいから使う
//	float scale = SCREEN_SCALE;
//
//	// スクリーン中央位置 (プロトタイプでは乗算だったけど　今回から加算にして）
//	b2Vec2 screen_center;
//	screen_center.x = SCREEN_CENTER_X;
//	screen_center.y = SCREEN_CENTER_Y;
//
//	//m_p_field_array の各位置に対応するフィールドオブジェクトを描画
//	for (int y = 0; y < m_field_height; ++y) {
//		for (int x = 0; x < m_field_width; ++x) {
//			if (m_p_field_array[y][x] != nullptr) {
//				b2Vec2 position;
//				position.x = m_p_field_array[y][x]->GetFieldBody()->GetPosition().x ;
//				position.y = m_p_field_array[y][x]->GetFieldBody()->GetPosition().y ;
//
//				// プレイヤー位置を考慮してスクロール補正を加える
//				//取得したbodyのポジションに対してBox2dスケールの補正を加える
//				float draw_x = ((position.x - PlayerPosition::GetPlayerPosition().x)*BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
//				float draw_y = ((position.y - PlayerPosition::GetPlayerPosition().y)*BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;
//	
//				//貼るテクスチャを指定
//				switch (m_p_field_array[y][x]->GetFieldTexture())
//				{
//				case anchor_point_texture:
//					GetDeviceContext()->PSSetShaderResources(0, 1, &g_AnchorPoint_Texture);
//					break;
//				case ground_texture:
//					GetDeviceContext()->PSSetShaderResources(0, 1, &g_Ground_Texture);
//					break;
//				case STAGE_BLOCK_TYPE_1:
//					GetDeviceContext()->PSSetShaderResources(0, 1, &g_Ground_Texture);
//					break;
//				case STAGE_BLOCK_TYPE_2:
//					GetDeviceContext()->PSSetShaderResources(0, 1, &g_Ground_Texture);
//					break;
//
//				case STAGE_BLOCK_GRASS:
//					GetDeviceContext()->PSSetShaderResources(0, 1, &g_Ground_Texture);
//					break;
//				case STAGE_BLOCK_GRASS_LEFT:
//					GetDeviceContext()->PSSetShaderResources(0, 1, &g_left_ground_Texture);
//					break;
//				case STAGE_BLOCK_GRASS_RIGHT:
//					GetDeviceContext()->PSSetShaderResources(0, 1, &g_right_ground_Texture);
//					break;
//				case STAGE_BLOCK_EARTH:
//					GetDeviceContext()->PSSetShaderResources(0, 1, &g_under_Ground_Texture);
//					break;
//				case STAGE_BLOCK_EARTH_UNDER_LEFT:
//					GetDeviceContext()->PSSetShaderResources(0, 1, &g_under_left_ground_Texture);
//					break;
//				case STAGE_BLOCK_EARTH_UNDER_RIGHT:
//					GetDeviceContext()->PSSetShaderResources(0, 1, &g_under_right_ground_Texture);
//					break;
//
//				case STAGE_BLOCK_INVISIBILITY:
//					GetDeviceContext()->PSSetShaderResources(0, 1, &g_invisibility_wall_Texture);
//				 break;
//
//				case STAGE_BLOCK_SLOOP_SIDE_LEFT:
//					GetDeviceContext()->PSSetShaderResources(0, 1, &g_sloop_left_side_texture);
//					break;
//
//				case STAGE_BLOCK_SLOOP_SIDE_RIGHT:
//					GetDeviceContext()->PSSetShaderResources(0, 1, &g_sloop_right_side_texture);
//					break;
//
//
////--------------------------------------------------------------------------------------------------------------------------------------------------------
//				case ISEKI_TOP_BLOCK:
//					GetDeviceContext()->PSSetShaderResources(0, 1, &g_Iseki_Top_Texture);
//					break;
//				case ISEKI_TOP_LEFT_BLOCK:
//					GetDeviceContext()->PSSetShaderResources(0, 1, &g_Iseki_Top_Left_Texture);
//					break;
//				case ISEKI_TOP__RIGHT_BLOCK:
//					GetDeviceContext()->PSSetShaderResources(0, 1, &g_Iseki_Top_Right_Texture);
//					break;
//
//				case ISEKI_BLOCK:
//					GetDeviceContext()->PSSetShaderResources(0, 1, &g_Iseki_Texture);
//					break;
//				case ISEKI_LEFT_BLOCK:
//					GetDeviceContext()->PSSetShaderResources(0, 1, &g_Iseki_Left_Texture);
//					break;
//				case ISEKI_RIGHT_BLOCK:
//					GetDeviceContext()->PSSetShaderResources(0, 1, &g_Iseki_Right_Texture);
//					break;
//			
//				case SAND_DOWN:
//					GetDeviceContext()->PSSetShaderResources(0, 1, &g_sand_Texture);
//					break;
//				case SAND_UP:
//					GetDeviceContext()->PSSetShaderResources(0, 1, &g_sand_up_Texture);
//					break;
//
//				case STAGE_GRASS_ONLY:
//					GetDeviceContext()->PSSetShaderResources(0, 1, &g_Ground_Grass_only_Texture);
//					break;
//
//
//
//				default:
//					GetDeviceContext()->PSSetShaderResources(0, 1, &g_Ground_Texture);
//					break;
//				}
//				
//				//draw
//				DrawSprite(
//					{ draw_x,
//					  draw_y },
//					m_p_field_array[y][x]->GetFieldBody()->GetAngle(),
//					{ m_p_field_array[y][x]->GetSize().x * scale , m_p_field_array[y][x]->GetSize().y * scale },3.0
//				);
//			}
//		}
//	}




	


	//アンカーポイントを描画
	AnchorPoint::Draw();
}



void Field::Finalize()
{
	//ワールドのインスタンスを持ってくる
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();

	//// m_p_field_array が nullptr の場合は処理をスキップ
	//if (m_p_field_array == nullptr) {
	//	return;
	//}

	//// 2次元配列のメモリ解放
	//for (int y = 0; y < m_field_height; ++y) {
	//	// m_p_field_array[y] が nullptr の場合はスキップ
	//	if (m_p_field_array[y] == nullptr) {
	//		continue;
	//	}

	//	for (int x = 0; x < m_field_width; ++x) {
	//		if (m_p_field_array[y][x] != nullptr) {
	//			world->DestroyBody(m_p_field_array[y][x]->GetFieldBody()); // フィールドを消す
	//			delete m_p_field_array[y][x];
	//			m_p_field_array[y][x] = nullptr;
	//		}
	//	}

	//	// 行のメモリを解放
	//	delete[] m_p_field_array[y];
	//	m_p_field_array[y] = nullptr;
	//}

	//// 配列全体を解放
	//delete[] m_p_field_array;
	//m_p_field_array = nullptr;


	if (g_Ground_Texture) UnInitTexture(g_Ground_Texture);
	if (g_under_Ground_Texture) UnInitTexture(g_under_Ground_Texture);

	if (g_AnchorPoint_Texture) UnInitTexture(g_AnchorPoint_Texture);
	if (g_right_ground_Texture) UnInitTexture(g_right_ground_Texture);
	if (g_left_ground_Texture) UnInitTexture(g_left_ground_Texture);

	if (g_under_right_ground_Texture) UnInitTexture(g_under_right_ground_Texture);
	if (g_under_left_ground_Texture) UnInitTexture(g_under_left_ground_Texture);

	if (g_under_right_ground_down_Texture) UnInitTexture(g_under_right_ground_down_Texture);
	if (g_under_left_ground_down_Texture) UnInitTexture(g_under_left_ground_down_Texture);

	if (g_sloop_left_side_texture) UnInitTexture(g_sloop_left_side_texture);
	if (g_sloop_right_side_texture) UnInitTexture(g_sloop_right_side_texture);

	if (g_invisibility_wall_Texture) UnInitTexture(g_invisibility_wall_Texture);

	// 遺跡のステージブロック
	if (g_Iseki_Top_Texture) UnInitTexture(g_Iseki_Top_Texture);
	if (g_Iseki_Top_Left_Texture) UnInitTexture(g_Iseki_Top_Left_Texture);
	if (g_Iseki_Top_Right_Texture) UnInitTexture(g_Iseki_Top_Right_Texture);

	if (g_Iseki_Texture) UnInitTexture(g_Iseki_Texture);
	if (g_Iseki_Left_Texture) UnInitTexture(g_Iseki_Left_Texture);
	if (g_Iseki_Right_Texture) UnInitTexture(g_Iseki_Right_Texture);

	if (g_Big_Wood_Texture) UnInitTexture(g_Big_Wood_Texture);

	if (g_Iseki_boss_wall_object_Texture) UnInitTexture(g_Iseki_boss_wall_object_Texture);

	if (g_sand_Texture) UnInitTexture(g_sand_Texture);
	if (g_sand_up_Texture) UnInitTexture(g_sand_up_Texture);

	if (g_background_block_down_Texture) UnInitTexture(g_background_block_down_Texture);
	if (g_background_block_Texture) UnInitTexture(g_background_block_Texture);


	if (g_Ground_Grass_only_Texture) UnInitTexture(g_Ground_Grass_only_Texture);
	
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


