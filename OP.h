#pragma once

#include "Video.h"

class OP
{
public:
	OP();
	~OP();


	static OP& GetInstance() {
		static OP instance;
		return instance;
	}






	void Initialize();
	void Update();
	void Draw();
	void Finalize();


private:

	Video video;
	bool m_pause;


};

