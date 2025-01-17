// □□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
// 
// [ Easing.cpp ]
// 
// 製作者: 永野義也
// 日付:  2024/06/26
// 
// [●] = シーンジャンプ
// [◆] = 関数毎にジャンプ
// 
// □□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□



// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
// プリプロセッサ命令 [●]

#pragma once

#ifndef _Easing_H
#define _Easing_H

// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
// 構造体 [●]


/**
 * @brief イージング
 */
class Ease
{

	/**
	* @brief イージングの種類
	*/
	enum class Type
	{
		None,
		Back,
		Bounce,
		Cubic,
		Circle,
		Elastic,
		Quint
	};

	/**
	 * @brief イージングのインアウト
	 */
	enum class IO
	{
		In,
		Out,
		InOut,
		OutIn
	};

public:

	/**
	 * @brief 自由なイージングを指定する
	 * @param x - 値（0~1）
	 * @param io - インかアウトか
	 * @param ease - イージングタイプ
	 * @return イーズがかかった値
	 */
	static double Easing(const double& x, const Ease::IO& io, const Ease::Type& ease);

	static double InBounce(const double&);// 弾性イーズイン
	static double InCubic(const double&);// 矩形イーズイン
	static double InCirc(const double&);// 丸形イーズイン
	static double InElastic(const double&);// 慣性イーズイン
	static double InQuint(const double&);// クイントイーズイン
	static double InBack(const double&);// バックイーズイン

	static double OutBounce(const double&);// 弾性イーズアウト
	static double OutCubic(const double&);// 矩形イーズアウト
	static double OutCirc(const double&);// 丸形イーズアウト
	static double OutElastic(const double&);// 慣性イーズアウト
	static double OutQuint(const double&);// クイントイーズアウト
	static double OutBack(const double&);// バックイーズアウト

	static double InOutBounce(const double&);
	static double InOutCubic(const double&);// 矩形イーズインイーズアウト
	static double InOutCirc(const double&);// 丸形イーズインイーズアウト
	static double InOutElastic(const double&);// 慣性イーズインイーズアウト
	static double InOutQuint(const double&);// クイントイーズインイーズアウト
	static double InOutBack(const double&);// バックイーズインイーズアウト

	static double OutInQuint(const double&);// クイントイーズアウトイーズイン

	static double easeInOutElastic(const double& x); //イーズインアウトエラスティック[◆]

	static double easeOutElastic(const double& x); //イーズアウトエラスティック[◆]

	static double easeOutBack(const double& x);// イーズアウトバック [◆]

	static double easeInElastic(const double& x);// イーズインエラスティック [◆]

};

#endif // !_Easing_H