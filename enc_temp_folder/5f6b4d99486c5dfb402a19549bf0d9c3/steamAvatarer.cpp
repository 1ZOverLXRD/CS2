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


// 回调函数，用于处理curl响应数据
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    try {
        s->append(static_cast<char*>(contents), size * nmemb);
    }
    catch (const std::bad_alloc& e) {
        spdlog::critical("内存分配失败: {}", e.what());
        return 0;
    }
    return size * nmemb;
}

static size_t WriteCallback_binary(void* contents, size_t size, size_t nmemb, std::vector<char>* buffer) {
    try {
        buffer->insert(buffer->end(), static_cast<char*>(contents), static_cast<char*>(contents) + size * nmemb);
    }
    catch (const std::bad_alloc& e) {
        spdlog::critical("内存分配失败: {}", e.what());
        return 0;
    }
    return size * nmemb;
}

// 测试代理是否可用的函数
static bool testInternet(const std::string& proxyHost = PROXYHOST, int proxyPort = PROXYPORT, int TIMEOUT = 3) {
    spdlog::trace("正在测试代理是否可用: {}:{}", proxyHost, proxyPort);

    CURL* curl = curl_easy_init();
    if (!curl) {
        spdlog::critical("初始化cURL失败！");
        return false;
    }

    std::string readBuffer;
    std::string proxy = proxyHost + ":" + std::to_string(proxyPort);

    curl_easy_setopt(curl, CURLOPT_URL, "https://steamcommunity.com/profiles/76561199543053266/");
    curl_easy_setopt(curl, CURLOPT_PROXY, proxy.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, TIMEOUT);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, TIMEOUT);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        spdlog::critical("cURL请求失败: {}", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        return false;
    }

    long httpCode = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
    curl_easy_cleanup(curl);

    if (httpCode >= 200 && httpCode < 300) {
        spdlog::trace("代理测试成功，状态码: {}", httpCode);
        return true;
    }
    else {
        spdlog::critical("代理测试失败，状态码: {}", httpCode);
        return false;
    }
}

// 读取指定网址源文件的方法
static bool readWebsiteSource(const std::string& url, std::string& source, const std::string& proxyHost = PROXYHOST, int proxyPort = PROXYPORT, int TIMEOUT = 3) {
    spdlog::trace("正在从解析 {}", url);

    CURL* curl = curl_easy_init();
    if (!curl) {
        spdlog::critical("初始化cURL失败！");
        return false;
    }

    source.clear();
    std::string proxy = proxyHost + ":" + std::to_string(proxyPort);

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_PROXY, proxy.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &source);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, TIMEOUT);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, TIMEOUT);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        spdlog::critical("读取网站源文件失败: {}", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        return false;
    }

    long httpCode = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
    curl_easy_cleanup(curl);

    if (httpCode >= 200 && httpCode < 300) {
        spdlog::trace("成功读取网站源文件，状态码: {}", httpCode);
        return true;
    }
    else {
        spdlog::critical("读取网站源文件失败，状态码: {}", httpCode);
        return false;
    }
}

// 解析源文件中的图片URL
std::string parseSource(const std::string& source, ull steamID64) {
    try {
        std::regex regex("<link rel=\"image_src\" href=\"(.*?)\">");
        std::smatch match;
        if (std::regex_search(source, match, regex)) {
            std::string url = match[1].str();
            spdlog::trace("正则匹配成功: {}", url);
            return url;
        }
        else {
            spdlog::critical("正则匹配失败，可能是ID: {}", steamID64);
            return "null";
        }
    }
    catch (const std::exception& e) {
        spdlog::critical("正则匹配异常: {}", e.what());
        return "null";
    }
}

// 下载图片并保存到本地
bool downloadImage(const std::string& url, const std::string& fileName, const std::string& proxyAddress = PROXYHOST, int proxyPort = PROXYPORT) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        spdlog::critical("初始化Curl失败！");
        return false;
    }

    std::string proxy = proxyAddress + ":" + std::to_string(proxyPort);
    curl_easy_setopt(curl, CURLOPT_PROXY, proxy.c_str());
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    std::vector<char> buffer;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback_binary);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        spdlog::critical("下载失败: {}", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        return false;
    }

    std::filesystem::path imageDir("Image");
    if (!std::filesystem::exists(imageDir) && !std::filesystem::create_directory(imageDir)) {
        spdlog::critical("创建目录失败！");
        curl_easy_cleanup(curl);
        return false;
    }

    std::filesystem::path filePath = imageDir / fileName;
    std::ofstream outFile(filePath, std::ios::binary);
    if (outFile) {
        outFile.write(buffer.data(), buffer.size());
        spdlog::trace("文件已保存: {}", filePath.string());
    }
    else {
        spdlog::critical("保存文件失败: {}", filePath.string());
        curl_easy_cleanup(curl);
        return false;
    }

    curl_easy_cleanup(curl);
    return true;
}
namespace steamAvatarer {
    bool getImage(ull steamID64,std::string& fileName) {
        std::string str_steamID = std::to_string(steamID64);
        std::filesystem::path filePath = fs::path("Image") / (str_steamID + ".jpg");

        if (!fs::exists(filePath)) {
            if (!proxyAvaliable && !testInternet()) return false;
            proxyAvaliable = true;

            std::string url = "https://steamcommunity.com/profiles/" + str_steamID;
            std::string source;
            if (!readWebsiteSource(url, source)) return false;

            std::string imageUrl = parseSource(source, steamID64);
            if (imageUrl == "null") return false;

            return downloadImage(imageUrl, filePath.filename().string());
        }

        fileName = filePath.string();
        return true;
    }

}