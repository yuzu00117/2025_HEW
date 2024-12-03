#ifndef OBJECT_MAP_TYPE_H
#define OBJECT_MAP_TYPE_H

#include"object.h"

int object_map_name_Wood[OBJECT_MAX_HEIGHT][OBJECT_MAX_WIDTH]
{
{0,0,0,0,0,0,0,},
{0,0,0,1,0,0,0,},
{0,0,0,2,0,0,0,},
{0,0,0,2,0,0,0,},
{0,0,0,2,0,0,0,},
{0,0,0,2,0,0,0,},
{0,0,0,0,0,0,0,},
};

int object_map_name_Rock[OBJECT_MAX_HEIGHT][OBJECT_MAX_WIDTH]
{
{0,0,0,0,0,0,0,},
{0,2,2,2,2,2,0,},
{0,2,2,2,2,2,0,},
{0,1,2,2,2,1,0,},
{0,2,2,2,2,2,0,},
{0,2,2,2,2,2,0,},
{0,0,0,0,0,0,0,},
};

#endif // !OBJECT_MAP_TYPE_H

