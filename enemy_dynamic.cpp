//-----------------------------------------------------------------------------------------------------
// #name enemyDynamic.h
// #description 動的エネミー(プレイヤー追従)のcppファイル
// #make 2024/11/20
// #update 2024/11/22
// #comment 追加・修正予定
//          ・ステータス調整
//			・必要に応じた移動方法ノ変更(地走バージョンノ作成など)
//           
//----------------------------------------------------------------------------------------------------

#include"include/box2d/box2d.h"
#include"enemy_dynamic.h"
#include"world_box2d.h"
#include"texture.h"
#include"Xinput_controller.h"
#include"sprite.h"
#include"keyboard.h"
#include<Windows.h>
#include"player_position.h"

//グローバル変数
static ID3D11ShaderResourceView* g_enemy_dynamic_texture = NULL;

void EnemyDynamic::Initialize()
{
	//テクスチャ読み込み
	g_enemy_dynamic_texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_yellow.png");
}

void EnemyDynamic::Update()
{
	//プレイヤー追従(簡易)
	//プレイヤーのポジション取得
	b2Vec2 player_position;
	player_position.x = PlayerPosition::GetPlayerPosition().x;
	player_position.y = PlayerPosition::GetPlayerPosition().y;

	//移動方向
	b2Vec2 enemy_vector;
	enemy_vector.x = player_position.x - GetEnemyBody()->GetPosition().x;
	enemy_vector.y = player_position.y - GetEnemyBody()->GetPosition().y;

	//移動量
	b2Vec2 enemy_move;
	enemy_move.x = (enemy_vector.x * m_speed);
	enemy_move.y = (enemy_vector.y * m_speed);

	GetEnemyBody()->ApplyForceToCenter(b2Vec2(enemy_move.x, enemy_move.y), true);
}

void EnemyDynamic::Draw()
{
	//殆どfieldと同様

	// スケールをかけないとオブジェクトのサイズの表示が小さいから使う
	float scale = SCREEN_SCALE;

	// スクリーン中央位置 (プロトタイプでは乗算だったけど　今回から加算にして）
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;

	b2Vec2 position;
	position.x = GetEnemyBody()->GetPosition().x;
	position.y = GetEnemyBody()->GetPosition().y;

	// プレイヤー位置を考慮してスクロール補正を加える
	//取得したbodyのポジションに対してBox2dスケールの補正を加える
	float draw_x = ((position.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
	float draw_y = ((position.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;



	//貼るテクスチャを指定
	GetDeviceContext()->PSSetShaderResources(1, 1, &g_enemy_dynamic_texture);


	//draw
	DrawSprite(
		{ draw_x,
		  draw_y },
		GetEnemyBody()->GetAngle(),
		{ GetEnemySize().x * scale , GetEnemySize().y * scale }
	);
}

void EnemyDynamic::Finalize()
{
	//殆どfieldと同様

	if (GetEnemyBody()) {
		// ボディを削除
		Box2dWorld& box2d_world = Box2dWorld::GetInstance();
		b2World* world = box2d_world.GetBox2dWorldPointer();
		world->DestroyBody(GetEnemyBody());
		SetEnemyBody(nullptr);
	}
	if (g_enemy_dynamic_texture)
	{
		UnInitTexture(g_enemy_dynamic_texture);
	}
}