#include <spdlog/spdlog.h>
#include <fstream>
#include <filesystem>
static bool proxyAvaliable = false;
// ģ������
module steamAvatarer;
// �����޷��ų����ͱ���
typedef unsigned long long ull;
static bool onGet = false;
// ������������Ͷ˿ڳ���
static const std::string PROXYHOST = "127.0.0.1";
static const int PROXYPORT = 7890;
namespace fs = std::filesystem;
//�ص����������ڴ���curl��Ӧ����
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t newLength = size * nmemb;
    try {
        s->append(static_cast<char*>(contents), newLength);
    }
    catch (const std::bad_alloc& e) {
        std::cerr << "�� WriteCallback ���ڴ����ʧ��: " << e.what() << std::endl;
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
        std::cerr << "�� WriteCallback_binary ���ڴ����ʧ��: " << e.what() << std::endl;
        return 0;
    }
    return newLength;
}

// ���Դ����Ƿ���õĺ���
static bool testInternet(std::string_view proxyHost = PROXYHOST, int proxyPort = PROXYPORT, int TIMEOUT = 3) {
    printf("[+][1ZOVERLXRD]��ʼʹ������ %s %d\n", proxyHost.data(), proxyPort);
    CURL* curl = curl_easy_init();
    if (!curl) {
        printf("��ʼ�� cURL �Խ��д������ʧ�ܡ�\n");
        return false;
    }

    std::string readBuffer;
    // ���������URL
    curl_easy_setopt(curl, CURLOPT_URL, "https://steamcommunity.com/profiles/76561199543053266/");
    // ���ô�����Ϣ
    std::string proxy = std::string(proxyHost) + ":" + std::to_string(proxyPort);
    curl_easy_setopt(curl, CURLOPT_PROXY, proxy.c_str());
    // ���ûص�����
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    // �������ӳ�ʱʱ��
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, TIMEOUT);
    // �������������ʱʱ��
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, TIMEOUT);

    std::cout << "[+][1ZOVERLXRD] ����ִ�� cURL �����Խ��д�����ԡ�" << std::endl;
    // ִ������
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "[+][1ZOVERLXRD] ��������� cURL ����ʧ��: " << curl_easy_strerror(res) << std::endl;
        curl_easy_cleanup(curl);
        return false;
    }

    // ��ȡ��Ӧ״̬��
    long httpCode = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

    // ����curl���
    curl_easy_cleanup(curl);

    // �ж�״̬���Ƿ�����
    if (httpCode >= 200 && httpCode < 300) {
        std::cout << "[+][1ZOVERLXRD] ������Գɹ���״̬��Ϊ: " << httpCode << std::endl;
        return true;
    }
    else {
        std::cerr << "[+][1ZOVERLXRD] �������ʧ�ܣ�״̬��Ϊ: " << httpCode << std::endl;
        return false;
    }
}

// ��ȡָ����ַԴ�ļ��ķ���
static bool readWebsiteSource(std::string_view url, std::string& source, std::string_view proxyHost = PROXYHOST, int proxyPort = PROXYPORT, int TIMEOUT = 3) {
    std::cout << "[+][1ZOVERLXRD] ��ʼ�� " << url << " ʹ�ô��� " << proxyHost << ":" << proxyPort << " ��ȡ��վԴ�ļ�" << std::endl;
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "��ʼ�� cURL �Զ�ȡ��վԴ�ļ�ʧ�ܡ�" << std::endl;
        return false;
    }

    source.clear();
    // ���������URL
    std::string urlStr(url);
    curl_easy_setopt(curl, CURLOPT_URL, urlStr.c_str());
    // ���ô�����Ϣ
    std::string proxy = std::string(proxyHost) + ":" + std::to_string(proxyPort);
    curl_easy_setopt(curl, CURLOPT_PROXY, proxy.c_str());
    // ���ûص�����
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &source);
    // �������ӳ�ʱʱ��
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, TIMEOUT);
    // �������������ʱʱ��
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, TIMEOUT);

    std::cout << "[+][1ZOVERLXRD] ����ִ�� cURL �����Զ�ȡ��վԴ�ļ���" << std::endl;
    // ִ������
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "��ȡ��վԴ�ļ�ʱ cURL ����ʧ��: " << curl_easy_strerror(res) << std::endl;
        curl_easy_cleanup(curl);
        return false;
    }

    // ��ȡ��Ӧ״̬��
    long httpCode = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

    // ����curl���
    curl_easy_cleanup(curl);

    // �ж�״̬���Ƿ�����
    if (httpCode >= 200 && httpCode < 300) {
        std::cout << "[+][1ZOVERLXRD] �ɹ���ȡ��վԴ�ļ���״̬��Ϊ: " << httpCode << std::endl;
        return true;
    }
    else {
        std::cerr << "[+][1ZOVERLXRD] ��ȡ��վԴ�ļ�ʧ�ܣ�״̬��Ϊ: " << httpCode << std::endl;
        return false;
    }
}

std::string parseSource(const std::string& source) {
    try {
        // ������ʽƥ�� <meta name="twitter:image" content="url" />
        std::regex regex("<link rel=\"image_src\" href=\"(.*?)\">");
        std::smatch match;
        if (std::regex_search(source, match, regex)) {
            // ��ȡƥ�������
            std::string url = match[1].str();
            std::cout << "[+][1ZOVERLXRD] ͼƬ��ַ: " << url << std::endl;
            return url;
        }
        else {
            std::cerr << "[+][1ZOVERLXRD] ����û��ƥ��ɹ�" << std::endl;
            return "null";
        }
    }
    catch (const std::exception& e) {
        std::cerr << "[+][1ZOVERLXRD] Error: " << e.what() << std::endl;
        return "null";
    }
}

// �޸ĺ�� downloadImage ���������� bool �ж������Ƿ�ɹ�
bool downloadImage(std::string_view url, std::string_view fileName, std::string_view proxyAddress = PROXYHOST, int proxyPort = PROXYPORT) {
    CURL* curl;
    CURLcode res;

    // ƴ�Ӵ����ַ�Ͷ˿�
    std::string proxy = std::string(proxyAddress) + ":" + std::to_string(proxyPort);

    // ��ʼ�� curl
    curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Curl ��ʼ��ʧ��" << std::endl;
        return false;
    }

    // ���ô���
    if (!proxy.empty()) {
        res = curl_easy_setopt(curl, CURLOPT_PROXY, proxy.c_str());
        if (res != CURLE_OK) {
            std::cerr << "���ô���ʧ��: " << curl_easy_strerror(res) << std::endl;
            curl_easy_cleanup(curl);
            return false;
        }
    }

    // ���� URL
    res = curl_easy_setopt(curl, CURLOPT_URL, url.data());
    if (res != CURLE_OK) {
        std::cerr << "���� URL ʧ��: " << curl_easy_strerror(res) << std::endl;
        curl_easy_cleanup(curl);
        return false;
    }

    // ����д��ص�
    std::vector<char> buffer;
    res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback_binary);
    if (res != CURLE_OK) {
        std::cerr << "���ûص�����ʧ��: " << curl_easy_strerror(res) << std::endl;
        curl_easy_cleanup(curl);
        return false;
    }
    res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    if (res != CURLE_OK) {
        std::cerr << "���ûص�����ʧ��: " << curl_easy_strerror(res) << std::endl;
        curl_easy_cleanup(curl);
        return false;
    }

    // ִ������
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "����ʧ��: " << curl_easy_strerror(res) << std::endl;
        curl_easy_cleanup(curl);
        return false;
    }
    else {
        // ���� Image Ŀ¼����������ڣ�
        std::filesystem::path imageDir("Image");
        if (!std::filesystem::exists(imageDir)) {
            if (!std::filesystem::create_directory(imageDir)) {
                std::cerr << "[+][1ZOVERLXRD] ���� Image Ŀ¼ʧ��" << std::endl;
                curl_easy_cleanup(curl);
                return false;
            }
        }

        std::filesystem::path filePath = imageDir / fileName;
        std::string file = filePath.string();

        // ���سɹ�������ͼƬ�������ļ�
        std::ofstream outFile(file, std::ios::binary);
        if (outFile) {
            outFile.write(buffer.data(), buffer.size());
            std::cout << "[+][1ZOVERLXRD] �ļ��ѱ��浽: " << file << std::endl;
        }
        else {
            std::cerr << "[+][1ZOVERLXRD] ���ļ�ʧ��: " << file << std::endl;
            curl_easy_cleanup(curl);
            return false;
        }
    }

    // ����
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