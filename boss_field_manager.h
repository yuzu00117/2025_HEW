//-----------------------------------------------------------------------------------------------------
// #name boss_field_manager.h
// #description boss戦で発生するオブジェクトのファクトリー
// #make 2025/01/15 永野義也
// #update 2025/01/15
// #comment 追加・修正予定
//          ・オブジェクトを作るごとに生成する感じ
//----------------------------------------------------------------------------------------------------


#ifndef BOSS_FIELD_MANAGER_H
#define BOSS_FIELD_MANAGER_H


enum BossFieldObjectType
{
    NULL,

};

class BossFieldManager
{
public:

	//シングルトンのインスタンスを取得
	static BossFieldManager& GetInstance();

    // 全てのオブジェクトを初期化
    void InitializeAll();

    // 全てのオブジェクトを更新
    void UpdateAll();

    // 全てのオブジェクトを描画
    void DrawAll();

    // 全てのオブジェクトを破棄
    void FinalizeAll();

private:

};



#endif // !BOSS_FIELD_MANAGER_H

