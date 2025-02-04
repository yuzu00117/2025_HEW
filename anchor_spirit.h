//-----------------------------------------------------------------------------------------------------
// #name anchor_spirit.h
// #description アンカーのソウルゲージの値
// #make 2024/11/20			王泳心
// #update 2024/11/20
// #comment 追加・修正予定
//
//           ・アンカー追加されたら、ソウルゲージの値が一定値になったときの処理を追加する
// 
//----------------------------------------------------------------------------------------------------

#pragma once


#ifndef ANCHOR_SPIRIT_H
#define ANCHOR_SPIRIT_H

#define		MAX_ANCHOR_SPIRIT (400.0)

class AnchorSpirit
{
public:

	static void	Initialize();

	//今のソウルゲージの値をもらう
	static float	GetAnchorSpiritValue();

	//今のソウルゲージの値をマイナス、もしくはプラス（マイナスしたい場合は負数を渡してね！）
	static void	EditAnchorSpiritValue(float value);

	static int GetAnchorLevel()
	{
		int anchor_level;//アンカーのレベル
		int spirit = GetAnchorSpiritValue();

		if (spirit < 100) {
			anchor_level = 1;
		}
		else if (spirit < 300)
		{
			anchor_level = 2;
		}
		else
		{
			anchor_level = 3;
		}

		return anchor_level;
	}

private:
	AnchorSpirit(){}
	~AnchorSpirit(){}

	//ソールゲージ（後でアンカーに移動するかも）
	static float	m_spirit;
};


#endif // !ANCHOR_SPIRIT_H

