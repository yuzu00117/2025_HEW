// #name@contact_block.h
// #description ÚG‚µ‚½‚ç‚È‚É‚©‚ÌƒAƒNƒVƒ‡ƒ“‚ğs‚¤@¡’Ç‰Á‚µ‚Ä‚¢‚é‚Ì‚ÍG‚ê‚½‚ç€–S‚·‚éˆ—
// #make 2025/02/06
// #update 2025/02/06
// #comment ’Ç‰ÁEC³—\’è  ÚG‚ÉŠÖ‚µ‚Ä‚È‚É‚©’Ç‰Á‚ª‚ ‚é‚Ì‚Å‚ ‚ê‚ÎEnumŒ^‚Éˆ—‚ğ‚½‚·
//         
//          
//----------------------------------------------------------------------------------------------------

#ifndef CONTACT_BLOCK_H
#define CONTACT_BLOCK_H

#include"include/box2d/box2d.h"


//G‚ê‚½‚ç‰½‚ğ‚·‚é‚Ì‚©‚Ìˆ—‚ğ
enum Contact_Block_Type
{
	NULL_TYPE,		//‚È‚É‚à‚È‚¢‚â‚Â
	DEAD_BLOCK_TYPE,//G‚ê‚½‚ç€‚Ê

};



class contact_block
{
public:
	contact_block(b2Vec2 Position, b2Vec2 block_size, Contact_Block_Type type, b2Vec2 num);
	~contact_block();


	void Initialize();
	void Update();
	void Draw();
	void Finalize();


	// ID ‚ğæ“¾‚·‚é
	int GetID() const {
		return id;
	}

	void SetID(int ID)
	{
		id = ID;
	}

	//•`‰æ—p‚ÉƒTƒCƒY‚ğ‚½‚¹‚Ä‚¨‚­
	b2Vec2 GetSize() const
	{
		return m_size;
	}

	void SetSize(b2Vec2 size) {
		m_size = size;
	}


	//body‚ÌK“¾
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

private:

	b2Body* m_body;

	b2Vec2 m_size;

	int id;

	//ŠÇ—‚Ég‚¤ŠÖ”
	bool m_flag = false;

	Contact_Block_Type m_contact_type;

};


#endif // !CONTACT_BLOCK_H
