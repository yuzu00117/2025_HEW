#include"Item_SpeedUp.h"
#include"player.h"

void	ItemSpeedUp::Update()
{
	Player player = Player::GetInstance();
	player.SetSpeed(1.5f);
}