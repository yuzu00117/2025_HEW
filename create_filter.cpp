#include"create_filter.h"
#include"include/box2d/box2d.h"

//フィルター自体ビットフラグで付けるものなんだけど、文字列で管理したいので　変換ようね
//フィルターを付けるための
std::unordered_map<std::string, uint16_t> filter_type = {
    {"NULL_filter",             0x0000}, // 無効なカテゴリ
    {"Player_filter",           0x0001}, // 1 << 0
    {"wall_filter",             0x0002}, // 1 << 1
    {"enemy_filter",            0x0004}, // 1 << 2
    {"one-way_platform_filter", 0x0008}, // 1 << 3
    {"object_filter",           0x0010}, // 1 << 4
};

b2Filter createFilterExclude(const std::string& category, const std::vector<std::string>& excludeMasks) {
    b2Filter filter;

    // カテゴリを設定
    filter.categoryBits = filter_type[category];

    // マスクを初期化（すべてのカテゴリと衝突可能にする）
    filter.maskBits = 0xFFFF;

    // 除外するカテゴリを削除
    for (const std::string& excludeMask : excludeMasks) {
        filter.maskBits &= ~filter_type[excludeMask];
    }

    return filter;
}