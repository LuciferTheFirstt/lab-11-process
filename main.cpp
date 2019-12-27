#include <boost/program_options.hpp>
#include <iostream>
#include <string>
#include <process.hpp>
#include <async++.h>
#include <chrono>
namespace
{
	const size_t ERROR_IN_COMMAND_LINE = 1;
	const size_t SUCCESS = 0;
	const size_t ERROR_UNHANDLED_EXCEPTION = 2;

}

size_t timeout = 0;
std::string config;

void on_timeout(int timeout_)
{
	timeout = timeout_;
}


void on_config(std::string config_)
{
	config = config_;
}
int main(int argc, char** argv)
{
	try
	{
		namespace bp = boost::process;
		namespace po = boost::program_options;
		po::options_description desc("Options");
		desc.add_options()
			("help", " выводим вспомогательное сообщение")
			("config <Release|Debug>", po::value<std::string>()->notifier(on_config), "  указываем конфигурацию сборки (по умолчанию Debug)")
			("install", "  добавляем этап установки (в директорию _install)")
			("pack", " добавляем этап упаковки (в архив формата tar.gz)")
			("timeout <count>", po::value<int>()->notifier(on_timeout), " указываем время ожидания (в секундах)");
		po::variables_map vm;
		try
		{
			namespace bp = boost::process;
			namespace po = boost::program_options;
			po::options_description desc("Options");
			desc.add_options()
				("help", " выводим вспомогательное сообщение")
				("config <Release|Debug>", po::value<std::string>()->notifier(on_config), "  указываем конфигурацию сборки (по умолчанию Debug)")
				("install", "  добавляем этап установки (в директорию _install)")
				("pack", " добавляем этап упаковки (в архив формата tar.gz)")
				("timeout <count>", po::value<int>()->notifier(on_timeout), " указываем время ожидания (в секундах)");
			po::variables_map vm;
			try
			{
				po::store(po::parse_command_line(argc, argv, desc), vm);
				if (vm.count("help"))
				{
					std::cout << "sdfsdfs" << std::endl << desc << std::endl;
					return SUCCESS;
				}
				po::notify(vm);


				if (config == "")
					config = "Debug";

				std::string cmake_args_1 = "-H. -B_builds -DCMAKE_INSTALL_PREFIX=_install -DCMAKE_BUILD_TYPE=";
				cmake_args_1 += config;

				std::string cmake_args_2 = "--build _builds";

				bp::group g;

				int count = 0;

				auto task1 = async::spawn([&cmake_args_1, &g, &count] {
					bp::child c1("./cmake1", cmake_args_1, g);
					count++;
					c1.wait();
					count--;
					return c1.exit_code();                                                                                                                                                                                       });                                                                                                                                                                                                                                                                                                                                                                                                                         async::cancellation_token c;
				auto task_watchdog = async::spawn([&g, &count, &c] {
					if (timeout == 0) return;
					while (timeout > 0)
					{
						sleep(1);
						timeout--;
						async::interruption_point(c);
					}
					std::cout << "Reached timeout. Running " << count << " processes. Terminating";
					if (count > 0)
						g.terminate();

					});
				int result = task1.get();

				std::cout << "Ran command 1 with result code: " << result << std::endl;
				return result;
			}
			auto task2 = async::spawn([&cmake_args_2, &g, &count] {
				bp::child c2("./cmake1", cmake_args_2, g);
				count++;
				c2.wait();
				count--;
				return c2.exit_code();
				});
			result = task2.get();
			std::cout << "Ran command 2 with result code: " << result << std::endl;

			if (vm.count("install"))
			{
				if (result != 0)
				{
					std::cout << "Command 2 returned value: " << result << "\n Exiting.";
					return result;
				}
				std::string cmake_args_3 = "--build _builds --target install";
				auto task3 = async::spawn([&cmake_args_3, &g, &count] {                                                                                                                                                                       bp::child c3("./cmake1", cmake_args_3, g);
				count++;
				c3.wait();
				count--;                                                                                                                                                                                                           return c3.exit_code();                                                                                                                                                                                     });
				result = task3.get();
				std::cout << "Ran command 3 with result code: " << result << std::endl;

			}

			if (vm.count("pack"))
			{
				if (result != 0)
				{
					std::cout << "Previous command returned value: " << result << "\n Exiting.";
					return result;
				}
				std::string cmake_args_4 = "--build _builds --target package";
				auto task4 = async::spawn([&cmake_args_4, &g, &count] {
					bp::child c4("./cmake1", cmake_args_4, g);
					count++;
					c4.wait();
					count--;
					return c4.exit_code();
					});
				result = task4.get();
				std::cout << "Ran command with result code: " << result << std::endl;

			}
			// g.wait();
			c.cancel();
		}
		catch (po::error & e)
		{
			std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
			std::cerr << desc << std::endl;
			return ERROR_IN_COMMAND_LINE;
		}
		}
		catch (std::exception & e)
		{
			std::cerr << "Unhandled Exception reached the top of main: "
				<< e.what() << ", application will now exit" << std::endl;
			return ERROR_UNHANDLED_EXCEPTION;
		}
		return 0;
}
