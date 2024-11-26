#ifndef ANCHOR_POINT
#define ANCHOR_POINT
#include"field.h"
#include<vector>


class AnchorPoint:public Field
{
public:
	AnchorPoint(b2Vec2 position, b2Vec2 body_size, float angle, bool bFixed, bool is_sensor, FieldTexture texture);
	~AnchorPoint();



	/**
	 * @brief  センサーに入ったらVectorにアンカーポイントのBodyを渡してプッシュする
	 * @param body センサーに入ったアンカーポイントの body
	 */
	static void InsideSensor(b2Body* new_anchor_point_body);


	/**
	 * @brief  センサーからでたアンカーポイントのBodyとVectorないのアンカーポイントのボディを比較して
	 　対応しているものをVectorから消す
	 * @param body センサーからでたアンカーポイントの body
	 */
	static void OutsideSensor(b2Body* delete_anchor_point_body);

	/**
	 * @brief センサー内のアンカーポイントの中から　
			  スティックの操作でアンカーポイントを選択する
	 * @param  
	 */
	static void SelectAnchorPoint(float stick_x,float stick_y);


	static void Initialize();
	static void Update();
	static void Draw();

	/**
 * @brief 　ターゲットとしたアンカーポイントのボディを取得する関数
 * @return  ターゲットとしたアンカーポイントのボディ
 */
	static b2Body* GetTargetAnchorPointBody();

private:
	
};



#endif // !ANCHOR_POINT

