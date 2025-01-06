//-----------------------------------------------------------------------------------------------------
// #name tool.h
// #description Floatの計算をかんたんにするため
// #make 2024/11/02　　永野義也
// #update 2024/11/02
// #comment 追加・修正予定
//          ・floatを簡単に 
// 　　　　 ・中身は理解してる　でもそんなむずいことしてない　使いやすくしてるだけ
//          
//----------------------------------------------------------------------------------------------------

#include"main.h"
#include"include/box2d/box2d.h"
#include"tool.h"




// ==================================================
// Float2に変換 [◆]
// ==================================================
XMFLOAT2 toFloat2(const XMFLOAT3& f) {
    return XMFLOAT2(f.x, f.y);
}

XMFLOAT2 toFloat2(const XMFLOAT4& f) {
    return XMFLOAT2(f.x, f.y);
}

XMFLOAT2 toFloat2(const XMINT2& f) {
    return XMFLOAT2(static_cast<float>(f.x), static_cast<float>(f.y));
}

XMFLOAT2 toFloat2(const b2Vec2& f) {
    return XMFLOAT2(f.x, f.y);
}

// ==================================================
// Float3に変換 [◆]
// ==================================================
XMFLOAT3 toFloat3(const XMFLOAT2& f) {
    return XMFLOAT3(f.x, f.y, 0.0f);
}

XMFLOAT3 toFloat3(const XMFLOAT4& f) {
    return XMFLOAT3(f.x, f.y, f.z);
}

XMFLOAT3 toFloat3(const XMINT2& f) {
    return XMFLOAT3(static_cast<float>(f.x), static_cast<float>(f.y), 0.0f);
}

XMFLOAT3 toFloat3(const b2Vec2& f) {
    return XMFLOAT3(f.x, f.y, 0.0f);
}

// ==================================================
// Float4に変換 [◆]
// ==================================================
XMFLOAT4 toFloat4(const XMFLOAT2& f) {
    return XMFLOAT4(f.x, f.y, 0.0f, 1.0f);
}

XMFLOAT4 toFloat4(const XMFLOAT3& f) {
    return XMFLOAT4(f.x, f.y, f.z, 1.0f);
}

XMFLOAT4 toFloat4(const XMINT2& f) {
    return XMFLOAT4(static_cast<float>(f.x), static_cast<float>(f.y), 0.0f, 1.0f);
}

XMFLOAT4 toFloat4(const b2Vec2& f) {
    return XMFLOAT4(f.x, f.y, 0.0f, 1.0f);
}


/**
 * @brief 絶対値を返す関数
 * @param value 入れたい値
 * @return 絶対値
 */
float ReturnAbsoluteValue(float value)
{
    if (0 < value)//０より大きいため受け取った値をそのまま返す
    {
        return value;
    }
    else 
    {
        return -value;
    }
}




// ランダムな整数を生成する関数
int GetRandomInt(int min, int max) {
    static std::random_device rd;  // ランダムデバイス（シード生成）
    static std::mt19937 gen(rd()); // メルセンヌ・ツイスタ乱数生成器
    std::uniform_int_distribution<> dist(min, max); // 範囲指定

    return dist(gen); // ランダムな整数を生成して返す
}

// 度数をラジアンに変換する関数
double DegreesToRadians(double degrees) {
    return degrees * M_PI / 180.0;
}


vector<string>
Split(const string& str, const char& word)
{
    istringstream stream(str);

    string field;
    vector<string> res;
    res.reserve(1024);


    while (getline(stream, field, word))
        res.push_back(field);

    return res;
}