#include"1-1_boss.h"
#include"include/box2d/box2d.h"
#include"texture.h"
#include"sprite.h"

// 使用するテクスチャファイルを格納
static ID3D11ShaderResourceView* g_mini_boss_Texture = NULL;//小さなゴーレムのテクスチャ
static ID3D11ShaderResourceView* g_boss_shock_wave_sheet1_Texture = NULL;//衝撃波のテクスチャ１
static ID3D11ShaderResourceView* g_boss_shock_wave_sheet2_Texture = NULL;//衝撃波のテクスチャ２
static ID3D11ShaderResourceView* g_boss_charge_attack_sheet1_Texture = NULL;//溜め攻撃のテクスチャ１
static ID3D11ShaderResourceView* g_boss_charge_attack_sheet2_Texture = NULL;//ため攻撃のテクスチャ２
static ID3D11ShaderResourceView* g_mini_boss_create_sheet1_Texture = NULL;//小さなゴーレムを生成する際のボス側１
static ID3D11ShaderResourceView* g_mini_boss_create_sheet2_Texture = NULL;//小さなゴーレムを生成する際のボス側２


Boss_1_1::Boss_1_1()
{
}

Boss_1_1::~Boss_1_1()
{
}

void Boss_1_1::Initialize()
{
	g_mini_boss_Texture =					InitTexture(L"asset\\texture\\boss_1_1\\mini_boss.png");//ミニゴーレムのインクルード
	g_boss_shock_wave_sheet1_Texture =		InitTexture(L"asset\\texture\\boss_1_1\\boss_shock_wave_sheet1.png");//衝撃波攻撃のインクルードシート１
	g_boss_shock_wave_sheet2_Texture =		InitTexture(L"asset\\texture\\boss_1_1\\boss_shock_wave_sheet2.png");//衝撃波攻撃のインクルードシート２
	g_boss_charge_attack_sheet1_Texture =	InitTexture(L"asset\\texture\\boss_1_1\\boss_charge_sheet1.png");//ため攻撃のシート１
	g_boss_charge_attack_sheet2_Texture =	InitTexture(L"asset\\texture\\boss_1_1\\boss_charge_sheet2_.png");//ため攻撃のシート２
	g_mini_boss_create_sheet1_Texture =		InitTexture(L"asset\\texture\\boss_1_1\\boss_create_mini_golem_sheet1.png");//ミニゴーレムの生成のボス側１
	g_mini_boss_create_sheet2_Texture =		InitTexture(L"asset\\texture\\boss_1_1\\boss_create_mini_golem_sheet2_.png");//ミニゴーレムの生成のボス側２
	

}

void Boss_1_1::Update()
{

}

void Boss_1_1::Draw()
{

}

void Boss_1_1::Finalize()
{

}