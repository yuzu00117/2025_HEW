//----------------------------------------------------------------------------------------------------
// #name　UI_Block.h
// #description センサーに接触したらプレイヤーの右上にUIを表示する
// #make 2025/02/09
// #update 2025/02/09
// #comment 追加・修正予定  UIの種類をがんがん足したい
//         
//          
//----------------------------------------------------------------------------------------------------

#ifndef UI_BLOCK_H
#define UI_BLOCK_H

#include"include/box2d/box2d.h"
#include"Video.h"


//触れたら何を表示するもの
enum Ui_Block_Type
{
	NULL_UI_TYPE,//なにもないやつ
	ARROW,
	BUTTON_A,
	BUTTON_B,

	VIDEO_BUTTON_A,
	VIDEO_BUTTON_LEFT_STICK,
	VIDEO_BUTTON_RIGHT_STICK,
	VIDEO_BUTTON_ZR,

};



class UI_block
{
public:
	UI_block(b2Vec2 Position, b2Vec2 block_size,b2Vec2 Sensor_size,b2Vec2 Sensor_Position, Ui_Block_Type type,float texture_angle);
	~UI_block();


	void Initialize();
	void Update();
	void Draw();
	void Finalize();


	// ID を取得する
	int GetID() const {
		return id;
	}

	void SetID(int ID)
	{
		id = ID;
	}

	//描画用にサイズを持たせておく
	b2Vec2 GetSize() const
	{
		return m_size;
	}

	void SetSize(b2Vec2 size) {
		m_size = size;
	}


	//描画用にサイズを持たせておく
	b2Vec2 GetSensorSize() const
	{
		return m_sensor_size;
	}

	void SetSensorSize(b2Vec2 size) {
		m_sensor_size = size;
	}

	//bodyの習得
	b2Body* GetBody()
	{
		return m_body;
	}

	void SetBody(b2Body* body)
	{
		m_body = body;
	}



	bool GetFlag(void)
	{
		return m_flag;
	}


	void SetFlag(bool flag)
	{
		m_flag = flag;
	}


	bool	GetIfVideo() { return m_is_video; }

	//今の動画の状態を取得（そもそもUIが動画じゃない場合は動画終わったっていう状態が返ってくる）
	Video_State GetVideoState() { 
		if (m_is_video) { return m_video.GetState(); } 
		return Video_Finished;
	}
	//今の動画の状態をセット（そもそもUIが動画じゃない場合はセットされない）
	void	SetVideoState(Video_State state)
	{
		if (m_is_video) { m_video.SetState(state); }
	}

private:

	b2Body* m_body;

	b2Vec2 m_size;

	b2Vec2 m_sensor_size;

	int id;

	//管理に使う関数
	bool m_flag = false;

	Ui_Block_Type m_ui_type;

	float angle;

	Video m_video;	//動画インスタンス
	bool	m_is_video;	//このUIは動画かどうか

};


#endif //!UI_BLOCK_H
