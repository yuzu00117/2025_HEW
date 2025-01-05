#include"1-1_boss.h"
#include"include/box2d/box2d.h"
#include"texture.h"
#include"sprite.h"

// �g�p����e�N�X�`���t�@�C�����i�[
static ID3D11ShaderResourceView* g_mini_boss_Texture = NULL;//�����ȃS�[�����̃e�N�X�`��
static ID3D11ShaderResourceView* g_boss_shock_wave_sheet1_Texture = NULL;//�Ռ��g�̃e�N�X�`���P
static ID3D11ShaderResourceView* g_boss_shock_wave_sheet2_Texture = NULL;//�Ռ��g�̃e�N�X�`���Q
static ID3D11ShaderResourceView* g_boss_charge_attack_sheet1_Texture = NULL;//���ߍU���̃e�N�X�`���P
static ID3D11ShaderResourceView* g_boss_charge_attack_sheet2_Texture = NULL;//���ߍU���̃e�N�X�`���Q
static ID3D11ShaderResourceView* g_mini_boss_create_sheet1_Texture = NULL;//�����ȃS�[�����𐶐�����ۂ̃{�X���P
static ID3D11ShaderResourceView* g_mini_boss_create_sheet2_Texture = NULL;//�����ȃS�[�����𐶐�����ۂ̃{�X���Q


Boss_1_1::Boss_1_1()
{
}

Boss_1_1::~Boss_1_1()
{
}

void Boss_1_1::Initialize()
{
	g_mini_boss_Texture =					InitTexture(L"asset\\texture\\boss_1_1\\mini_boss.png");//�~�j�S�[�����̃C���N���[�h
	g_boss_shock_wave_sheet1_Texture =		InitTexture(L"asset\\texture\\boss_1_1\\boss_shock_wave_sheet1.png");//�Ռ��g�U���̃C���N���[�h�V�[�g�P
	g_boss_shock_wave_sheet2_Texture =		InitTexture(L"asset\\texture\\boss_1_1\\boss_shock_wave_sheet2.png");//�Ռ��g�U���̃C���N���[�h�V�[�g�Q
	g_boss_charge_attack_sheet1_Texture =	InitTexture(L"asset\\texture\\boss_1_1\\boss_charge_sheet1.png");//���ߍU���̃V�[�g�P
	g_boss_charge_attack_sheet2_Texture =	InitTexture(L"asset\\texture\\boss_1_1\\boss_charge_sheet2_.png");//���ߍU���̃V�[�g�Q
	g_mini_boss_create_sheet1_Texture =		InitTexture(L"asset\\texture\\boss_1_1\\boss_create_mini_golem_sheet1.png");//�~�j�S�[�����̐����̃{�X���P
	g_mini_boss_create_sheet2_Texture =		InitTexture(L"asset\\texture\\boss_1_1\\boss_create_mini_golem_sheet2_.png");//�~�j�S�[�����̐����̃{�X���Q
	

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