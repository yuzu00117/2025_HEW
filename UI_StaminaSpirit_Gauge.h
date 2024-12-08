#pragma once

#ifndef UI_SPIRIT_GAUGE_H
#define UI_SPIRIT_GAUGE_H

#include	"main.h"
#include	"anchor_spirit.h"
#include	"player_stamina.h"


class Game;
class StaminaSpiritGauge
{
	friend Game;
public:
	void	Initialize();
	void	Finalize();
	void	Draw();
private:
	StaminaSpiritGauge(){}
	~StaminaSpiritGauge(){}
	
	//体力＆ソウルゲージがMax状態の時のScale
	const	XMFLOAT2 m_scale = XMFLOAT2(200.0f, 33.0f);
	//体力＆ソウルゲージがMax状態の時のPosition
	const XMFLOAT2 m_position = XMFLOAT2(100.0f, 0.7f);

};

#endif // !UI_SPIRIT_GAUGE_H



