#include "main.h"
#include "UI/Font/SourceHanSansCN-Light.h"
import <cstdlib>;
import <ctime>;
void initLog() {
	// 创建一个颜色输出接收器
	auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	console_sink->set_level(spdlog::level::trace);  // 设置日志输出最低级别
	console_sink->set_pattern("%^[%L] %v%$");  // 设置输出格式，颜色会自动根据等级变更

	// 创建一个文件输出接收器
	auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("log.log", true); // 文件路径，true表示追加写入
	file_sink->set_level(spdlog::level::trace);  // 设置文件日志的最低级别
	file_sink->set_pattern("[%H:%M:%S] [%L] {%v}"); // 文件日志的格式

	// 创建日志记录器，绑定到console_sink和file_sink
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
	loadConfig();//加载配置
	auto result = Offsets::initOffset();//初始化偏移
	initBase();//初始化dll的基址
	Gui.AttachAnotherWindow(ProcessUtils::GetProcessIdByName("cs2.exe"), Cheater::mainLoop,Cheater::logicThread,(void*)front_data_data, (int)front_data_size);
	//Gui.AttachAnotherWindow(ProcessUtils::GetProcessIdByName("cs2.exe"), Cheater::mainLoop,(void*)front_data_data, (int)front_data_size);

	return 0;
}