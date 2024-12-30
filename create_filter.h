//-----------------------------------------------------------------------------------------------------
// #name create_filter.h
// #description フィルターを付ける
// #make 2024/12/04　永野義也
// #update 2024/12/04
// #comment 追加・修正予定
//        
//----------------------------------------------------------------------------------------------------

#ifndef CREATE_FILTER_H
#define CREATE_FILTER_H



#include"include/box2d/box2d.h"
#include <unordered_map>
#include <string>


// フィルターの宣言
extern std::unordered_map<std::string, uint16_t> filter_type;


/**
 * @brief 　フィルターを追加する
 * @param category それ自体のフィルター
 * @param excludeMasks 　設定したフィルターと除外したいフィルターを設定
 * @return 　b2filter型で、自分自身のフィルターと除外情報が入ってる
 */
b2Filter createFilterExclude(const std::string& category, const std::vector<std::string>& excludeMasks);

uint16_t GetFilter(const std::string& category);


#endif // !CREATE_FILTER_H