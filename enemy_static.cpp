//-----------------------------------------------------------------------------------------------------
// #name enemy.h
// #description 静的エネミーのcppファイル
// #make 2024/11/19
// #update 2024/11/22
// #comment 追加・修正予定
//          ・
//           
//----------------------------------------------------------------------------------------------------

#include"include/box2d/box2d.h"
#include"enemy_static.h"
#include"world_box2d.h"
#include"texture.h"
#include"Xinput_controller.h"
#include"sprite.h"
#include"keyboard.h"
#include<Windows.h>
#include"player_position.h"

//グローバル変数
static ID3D11ShaderResourceView* g_enemy_static_texture = NULL;

void EnemyStatic::Initialize()
{
	//テクスチャ読み込み
	g_enemy_static_texture= InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_block.png");
}

void EnemyStatic::Update()
{
	
}

void EnemyStatic::Draw()
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
	GetDeviceContext()->PSSetShaderResources(1, 1, &g_enemy_static_texture);


	//draw
	DrawSprite(
		{ draw_x,
		  draw_y },
		GetEnemyBody()->GetAngle(),
		{ GetEnemySize().x * scale , GetEnemySize().y * scale }
	);
}

void EnemyStatic::Finalize()
{
	//殆どfieldと同様

	if (GetEnemyBody()) {
		// ボディを削除
		Box2dWorld& box2d_world = Box2dWorld::GetInstance();
		b2World* world = box2d_world.GetBox2dWorldPointer();
		world->DestroyBody(GetEnemyBody());
		SetEnemyBody(nullptr);
	}
	if (g_enemy_static_texture)
	{
		UnInitTexture(g_enemy_static_texture);
	}
}