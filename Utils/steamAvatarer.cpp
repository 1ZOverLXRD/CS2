#include <spdlog/spdlog.h>
#include <fstream>
#include <filesystem>
static bool proxyAvaliable = false;
// 模块声明
module steamAvatarer;
// 定义无符号长整型别名
typedef unsigned long long ull;
static bool onGet = false;
// 定义代理主机和端口常量
static const std::string PROXYHOST = "127.0.0.1";
static const int PROXYPORT = 7890;
namespace fs = std::filesystem;
//回调函数，用于处理curl响应数据
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t newLength = size * nmemb;
    try {
        s->append(static_cast<char*>(contents), newLength);
    }
    catch (const std::bad_alloc& e) {
        std::cerr << "在 WriteCallback 中内存分配失败: " << e.what() << std::endl;
        return 0;
    }
    return newLength;
}

static size_t WriteCallback_binary(void* contents, size_t size, size_t nmemb, std::vector<char>* buffer) {
    size_t newLength = size * nmemb;
    try {
        buffer->insert(buffer->end(), static_cast<char*>(contents), static_cast<char*>(contents) + newLength);
    }
    catch (const std::bad_alloc& e) {
        std::cerr << "在 WriteCallback_binary 中内存分配失败: " << e.what() << std::endl;
        return 0;
    }
    return newLength;
}

// 测试代理是否可用的函数
static bool testInternet(std::string_view proxyHost = PROXYHOST, int proxyPort = PROXYPORT, int TIMEOUT = 3) {
    printf("[+][1ZOVERLXRD]开始使用主机 %s %d\n", proxyHost.data(), proxyPort);
    CURL* curl = curl_easy_init();
    if (!curl) {
        printf("初始化 cURL 以进行代理测试失败。\n");
        return false;
    }

    std::string readBuffer;
    // 设置请求的URL
    curl_easy_setopt(curl, CURLOPT_URL, "https://steamcommunity.com/profiles/76561199543053266/");
    // 设置代理信息
    std::string proxy = std::string(proxyHost) + ":" + std::to_string(proxyPort);
    curl_easy_setopt(curl, CURLOPT_PROXY, proxy.c_str());
    // 设置回调函数
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    // 设置连接超时时间
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, TIMEOUT);
    // 设置整体操作超时时间
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, TIMEOUT);

    std::cout << "[+][1ZOVERLXRD] 正在执行 cURL 请求以进行代理测试。" << std::endl;
    // 执行请求
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "[+][1ZOVERLXRD] 代理测试中 cURL 请求失败: " << curl_easy_strerror(res) << std::endl;
        curl_easy_cleanup(curl);
        return false;
    }

    // 获取响应状态码
    long httpCode = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

    // 清理curl句柄
    curl_easy_cleanup(curl);

    // 判断状态码是否正常
    if (httpCode >= 200 && httpCode < 300) {
        std::cout << "[+][1ZOVERLXRD] 代理测试成功，状态码为: " << httpCode << std::endl;
        return true;
    }
    else {
        std::cerr << "[+][1ZOVERLXRD] 代理测试失败，状态码为: " << httpCode << std::endl;
        return false;
    }
}

// 读取指定网址源文件的方法
static bool readWebsiteSource(std::string_view url, std::string& source, std::string_view proxyHost = PROXYHOST, int proxyPort = PROXYPORT, int TIMEOUT = 3) {
    std::cout << "[+][1ZOVERLXRD] 开始从 " << url << " 使用代理 " << proxyHost << ":" << proxyPort << " 读取网站源文件" << std::endl;
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "初始化 cURL 以读取网站源文件失败。" << std::endl;
        return false;
    }

    source.clear();
    // 设置请求的URL
    std::string urlStr(url);
    curl_easy_setopt(curl, CURLOPT_URL, urlStr.c_str());
    // 设置代理信息
    std::string proxy = std::string(proxyHost) + ":" + std::to_string(proxyPort);
    curl_easy_setopt(curl, CURLOPT_PROXY, proxy.c_str());
    // 设置回调函数
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &source);
    // 设置连接超时时间
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, TIMEOUT);
    // 设置整体操作超时时间
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, TIMEOUT);

    std::cout << "[+][1ZOVERLXRD] 正在执行 cURL 请求以读取网站源文件。" << std::endl;
    // 执行请求
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "读取网站源文件时 cURL 请求失败: " << curl_easy_strerror(res) << std::endl;
        curl_easy_cleanup(curl);
        return false;
    }

    // 获取响应状态码
    long httpCode = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

    // 清理curl句柄
    curl_easy_cleanup(curl);

    // 判断状态码是否正常
    if (httpCode >= 200 && httpCode < 300) {
        std::cout << "[+][1ZOVERLXRD] 成功读取网站源文件，状态码为: " << httpCode << std::endl;
        return true;
    }
    else {
        std::cerr << "[+][1ZOVERLXRD] 读取网站源文件失败，状态码为: " << httpCode << std::endl;
        return false;
    }
}

std::string parseSource(const std::string& source) {
    try {
        // 正则表达式匹配 <meta name="twitter:image" content="url" />
        std::regex regex("<link rel=\"image_src\" href=\"(.*?)\">");
        std::smatch match;
        if (std::regex_search(source, match, regex)) {
            // 获取匹配的内容
            std::string url = match[1].str();
            std::cout << "[+][1ZOVERLXRD] 图片地址: " << url << std::endl;
            return url;
        }
        else {
            std::cerr << "[+][1ZOVERLXRD] 正则没有匹配成功" << std::endl;
            return "null";
        }
    }
    catch (const std::exception& e) {
        std::cerr << "[+][1ZOVERLXRD] Error: " << e.what() << std::endl;
        return "null";
    }
}

// 修改后的 downloadImage 函数，返回 bool 判断下载是否成功
bool downloadImage(std::string_view url, std::string_view fileName, std::string_view proxyAddress = PROXYHOST, int proxyPort = PROXYPORT) {
    CURL* curl;
    CURLcode res;

    // 拼接代理地址和端口
    std::string proxy = std::string(proxyAddress) + ":" + std::to_string(proxyPort);

    // 初始化 curl
    curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Curl 初始化失败" << std::endl;
        return false;
    }

    // 配置代理
    if (!proxy.empty()) {
        res = curl_easy_setopt(curl, CURLOPT_PROXY, proxy.c_str());
        if (res != CURLE_OK) {
            std::cerr << "设置代理失败: " << curl_easy_strerror(res) << std::endl;
            curl_easy_cleanup(curl);
            return false;
        }
    }

    // 配置 URL
    res = curl_easy_setopt(curl, CURLOPT_URL, url.data());
    if (res != CURLE_OK) {
        std::cerr << "设置 URL 失败: " << curl_easy_strerror(res) << std::endl;
        curl_easy_cleanup(curl);
        return false;
    }

    // 配置写入回调
    std::vector<char> buffer;
    res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback_binary);
    if (res != CURLE_OK) {
        std::cerr << "设置回调函数失败: " << curl_easy_strerror(res) << std::endl;
        curl_easy_cleanup(curl);
        return false;
    }
    res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    if (res != CURLE_OK) {
        std::cerr << "设置回调数据失败: " << curl_easy_strerror(res) << std::endl;
        curl_easy_cleanup(curl);
        return false;
    }

    // 执行请求
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "下载失败: " << curl_easy_strerror(res) << std::endl;
        curl_easy_cleanup(curl);
        return false;
    }
    else {
        // 创建 Image 目录（如果不存在）
        std::filesystem::path imageDir("Image");
        if (!std::filesystem::exists(imageDir)) {
            if (!std::filesystem::create_directory(imageDir)) {
                std::cerr << "[+][1ZOVERLXRD] 创建 Image 目录失败" << std::endl;
                curl_easy_cleanup(curl);
                return false;
            }
        }

        std::filesystem::path filePath = imageDir / fileName;
        std::string file = filePath.string();

        // 下载成功，保存图片到本地文件
        std::ofstream outFile(file, std::ios::binary);
        if (outFile) {
            outFile.write(buffer.data(), buffer.size());
            std::cout << "[+][1ZOVERLXRD] 文件已保存到: " << file << std::endl;
        }
        else {
            std::cerr << "[+][1ZOVERLXRD] 打开文件失败: " << file << std::endl;
            curl_easy_cleanup(curl);
            return false;
        }
    }

    // 清理
    curl_easy_cleanup(curl);

    return true;
}
namespace steamAvatarer {
    bool getImage(ull steamID64,std::string& fileName) {
        std::string str_steamID = std::to_string(steamID64);
        std::filesystem::path imageDir("Image");
        std::filesystem::path filePath = imageDir / (str_steamID+".jpg");
        if (!std::filesystem::exists(filePath.c_str())) {
            if (!proxyAvaliable) {
                if (!testInternet())return false;
                proxyAvaliable = true;
            }

            std::string url = "https://steamcommunity.com/profiles/" + str_steamID;
            std::string source;
            if (!readWebsiteSource(url, source))return false;
            std::string imageUrl;
            if ((imageUrl = parseSource(source)).compare("null") == 0)return false;
            return downloadImage(imageUrl, filePath.filename().string());
        }
        fileName = filePath.string();
        return true;
    }

}