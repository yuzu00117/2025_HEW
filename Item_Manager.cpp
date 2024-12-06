#include "Item_Manager.h"
#include "Item_SpeedUp.h"


void	ItemManager::Create(b2Vec2 position, b2Vec2 body_size, float angle, ItemType type, bool shape_polygon, float Alpha)
{
	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (m_p_item[i] == nullptr)
		{
			switch (type)
			{
			case ITEM_NONE:
				break;
			case ITEM_SPEED_UP:
				m_p_item[i] = new ItemSpeedUp(position, body_size, angle, shape_polygon, Alpha);
				break;
			}
			return;
		}
	}
}

void	ItemManager::Update(int ID)
{
	
	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (m_p_item[i]->GetID() == ID)
		{
			m_p_item[i]->Update();
			Destory(i);
		}
	}
}

void	ItemManager::Destory(int item_ID)
{
	delete m_p_item[item_ID];
	m_p_item[item_ID] = nullptr;
	b2World* world = Box2dWorld::GetInstance().GetBox2dWorldPointer();
	world->DestroyBody(m_p_item[item_ID]->GetBody());
}



void	ItemManager::Destory(b2Body* body)
{
	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (m_p_item[i]->GetBody() == body)
		{
			//m_p_item[i]->SetDestory(true);
			delete m_p_item[i];
			m_p_item[i] = nullptr;
			b2World* world = Box2dWorld::GetInstance().GetBox2dWorldPointer();
			world->DestroyBody(body);
		}
	}

}


void	ItemManager::Draw()
{
	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (m_p_item[i] != nullptr)
		{
			m_p_item[i]->Draw();
		}
	}
}
