#include <spdlog/spdlog.h>
// ģ������
export module steamAvatarer;
export import <windows.h>;
export import <cstdint>;
export import <curl/curl.h>;
export import <iostream>;
export import <string>;
export import <string_view>;
export import <vector>;
export import <regex>;
// �����޷��ų����ͱ���
export typedef unsigned long long ull;
// �����������ռ䣬������ȡͼ��ĺ���
export namespace steamAvatarer {
    bool getImage(ull steamID64,std::string&);
  
}