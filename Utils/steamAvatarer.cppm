#include <spdlog/spdlog.h>
// 模块声明
export module steamAvatarer;
export import <windows.h>;
export import <cstdint>;
export import <curl/curl.h>;
export import <iostream>;
export import <string>;
export import <string_view>;
export import <vector>;
export import <regex>;
// 定义无符号长整型别名
export typedef unsigned long long ull;
// 导出的命名空间，包含获取图像的函数
export namespace steamAvatarer {
    bool getImage(ull steamID64,std::string&);
  
}