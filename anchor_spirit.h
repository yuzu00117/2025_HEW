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

#define		MAX_ANCHOR_SPIRIT (300.0)

class AnchorSpirit
{
public:

	static void	Initialize();

	static	void Update();

	//今自動回復中か取得
	static	bool	GetIfAutoHeal() { return m_auto_heal; }
	//今自動回復中かセット
	static	void	SetIfAutoHeal(bool flag) { m_auto_heal = flag; }

	//今のソウルゲージの値をもらう
	static float	GetAnchorSpiritValue();

	//今のソウルゲージの値をマイナス、もしくはプラス（マイナスしたい場合は負数を渡してね！）
	static void	EditAnchorSpiritValue(float value);

	//今のソウルゲージの値を直接変更
	static void	SetAnchorSpiritValueDirectly(float value);


	static int GetAnchorLevel()
	{
		int anchor_level;//アンカーのレベル
		int spirit = GetAnchorSpiritValue();

		if (spirit < 101) {
			anchor_level = 1;
		}
		else if (spirit < 201)
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
	//ソールこれから減る値
	static float	m_damage;

	//自動回復のフラグ
	static bool		m_auto_heal;
};


#endif // !ANCHOR_SPIRIT_H

