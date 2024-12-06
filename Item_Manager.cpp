#include "Item_Manager.h"
#include "Item_SpeedUp.h"


// シングルトンのインスタンス取得
ItemManager& ItemManager::GetInstance() {
	static ItemManager instance;
	return instance;
}


void	ItemManager::AddSpeedUp(b2Vec2 position, b2Vec2 body_size, float angle, bool shape_polygon, float Alpha)
{
	// 既存の引数コンストラクタを利用して生成
	m_SpeedUp_List.emplace_back(std::make_unique<ItemSpeedUp>(position, body_size, angle, shape_polygon, Alpha));
}


Item* ItemManager::FindItem_SpeedUp_ByID(int ID)
{
	for (const auto& w : m_SpeedUp_List) {
		if (w->GetID() == ID) {
			return w.get();
		}
	}
	return nullptr; // 見つからない場合は nullptr を返す
}


//void	ItemManager::Update(b2Body* body)
//{
//	for (int i = 0; i < ITEM_MAX; i++)
//	{
//		if (m_p_item[i]->GetBody() == body)
//		{
//			m_p_item[i]->Update();
//			Destory(i);
//		}
//	}
//}

//void	ItemManager::Destory(int item_ID)
//{
//	delete m_p_item[item_ID];
//	m_p_item[item_ID] = nullptr;
//	b2World* world = Box2dWorld::GetInstance().GetBox2dWorldPointer();
//	world->DestroyBody(m_p_item[item_ID]->GetBody());
//}



//void	ItemManager::Destory(b2Body* body)
//{
//	for (int i = 0; i < ITEM_MAX; i++)
//	{
//		if (m_p_item[i]->GetBody() == body)
//		{
//			//m_p_item[i]->SetDestory(true);
//			delete m_p_item[i];
//			m_p_item[i] = nullptr;
//			b2World* world = Box2dWorld::GetInstance().GetBox2dWorldPointer();
//			world->DestroyBody(body);
//		}
//	}
//
//}



//void	ItemManager::Draw()
//{
//	for (int i = 0; i < ITEM_MAX; i++)
//	{
//		if (m_p_item != nullptr)
//		{
//			m_p_item[i]->Draw();
//		}
//	}
//}
