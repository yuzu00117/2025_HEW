//-----------------------------------------------------------------------------------------------------
// #name collider_type.h
// #description コライダーの種類定義
// #make 2024/11/02　　永野義也
// #update 2024/11/02
// #comment 追加・修正予定
// 
// 　　　　　　・これじゃ足りないし、追加予定でもこれを使わなくてもいいわ、　なんかれんちゃんあたりこれ嫌いそう
// 　　　　　　・sexyな管理方法だと思うんやけどもっとうまいやり方ありそう
//   
//          
//----------------------------------------------------------------------------------------------------

#ifndef COLLIDER_TYPE
#define COLLIDER_TYPE



enum ColliderType
{
	collider_enemy,
	collider_player,
	collider_anchor,
	collider_ground,
};


#endif // !COLLIDER_TYPE


