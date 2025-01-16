//-----------------------------------------------------------------------------------------------------
// #name boss_field_manager.cpp
// #description boss戦で発生するオブジェクトのファクトリー
// #make 2025/01/15 永野義也
// #update 2025/01/15
// #comment 追加・修正予定
//          ・オブジェクトを作るごとに生成する感じ
//----------------------------------------------------------------------------------------------------


#include"boss_field_manager.h"


//シングルトンのインスタンスを取得
BossFieldManager& BossFieldManager::GetInstance()
{
	static BossFieldManager instance;
	return instance;
}



void BossFieldManager::InitializeAll()
{


}

void BossFieldManager::UpdateAll()
{


}

void BossFieldManager::DrawAll()
{


}

void BossFieldManager::FinalizeAll()
{


}