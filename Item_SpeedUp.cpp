#include"Item_SpeedUp.h"
#include"texture.h"
#include"player.h"

static ID3D11ShaderResourceView* g_Wood_Texture = NULL;//アンカーのテクスチャ


void	ItemSpeedUp::Update()
{
	Player player = Player::GetInstance();
	player.SetSpeed(1.5f);
}