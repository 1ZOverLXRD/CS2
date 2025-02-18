#include "main.h"
#include "UI/Font/SourceHanSansCN-Light.h"
import <cstdlib>;
import <ctime>;
void initLog() {
	// ����һ����ɫ���������
	auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	console_sink->set_level(spdlog::level::trace);  // ������־�����ͼ���
	console_sink->set_pattern("%^[%L] %v%$");  // ���������ʽ����ɫ���Զ����ݵȼ����

	// ����һ���ļ����������
	auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("log.log", true); // �ļ�·����true��ʾ׷��д��
	file_sink->set_level(spdlog::level::trace);  // �����ļ���־����ͼ���
	file_sink->set_pattern("[%H:%M:%S] [%L] {%v}"); // �ļ���־�ĸ�ʽ

	// ������־��¼�����󶨵�console_sink��file_sink
	auto logger = std::make_shared<spdlog::logger>("logger", spdlog::sinks_init_list{ console_sink, file_sink });
	spdlog::set_default_logger(logger);
	spdlog::set_default_logger(logger);
	spdlog::set_level(spdlog::level::trace); // Set global log level to debug
	spdlog::set_pattern("[+]---[1ZOVERLXRD]---[%H:%M:%S]---[%^%L%$]---{%v}");
	std::cout << R"(
 __   ___________                _     __   ______________
/  | |___  /  _  |              | |    \ \ / /| ___ \  _  \
`| |    / /| | | |_   _____ _ __| |     \ V / | |_/ / | | |
 | |   / / | | | \ \ / / _ \ '__| |     /   \ |    /| | | |
_| |_./ /__\ \_/ /\ V /  __/ |  | |____/ /^\ \| |\ \| |/ /
\___/\_____/\___/  \_/ \___|_|  \_____/\/   \/\_| \_|___/

)" << std::endl;
}

int main() {
	srand(static_cast<unsigned int>(time(0)));
	initLog();
	loadConfig();//��������
	auto result = Offsets::initOffset();//��ʼ��ƫ��
	initBase();//��ʼ��dll�Ļ�ַ
	Gui.AttachAnotherWindow(ProcessUtils::GetProcessIdByName("cs2.exe"), Cheater::mainLoop,Cheater::logicThread,(void*)front_data_data, (int)front_data_size);
	//Gui.AttachAnotherWindow(ProcessUtils::GetProcessIdByName("cs2.exe"), Cheater::mainLoop,(void*)front_data_data, (int)front_data_size);

	return 0;
}