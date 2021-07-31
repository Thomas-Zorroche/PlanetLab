#include "Log.hpp"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace PlanetLab
{

std::shared_ptr<spdlog::logger> Log::s_CeresLogger;
std::shared_ptr<spdlog::logger> Log::s_PlanetLabLogger;

void Log::init(int argc, char** argv)
{
	spdlog::set_pattern("%^[%T] %n: %v%$");

	s_CeresLogger = spdlog::stdout_color_mt("CERES");
	s_PlanetLabLogger = spdlog::stdout_color_mt("PLANETLAB");
	
	bool logInitialized = false;
	spdlog::level::level_enum level = spdlog::level::info;
	for (size_t i = 1; i < (size_t)argc; i++)
	{
		if (strcmp(argv[i], "-trace") == 0)
		{
			level = spdlog::level::trace; logInitialized = true;
		}
		if (strcmp(argv[i], "-info") == 0)
		{
			level = spdlog::level::info;  logInitialized = true;
		}
		if (strcmp(argv[i], "-warn") == 0)
		{
			level = spdlog::level::warn;  logInitialized = true;
		}
		if (strcmp(argv[i], "-error") == 0)
		{
			level = spdlog::level::err;   logInitialized = true;
		}

		if (logInitialized)
		{
			s_PlanetLabLogger->set_level(level);
			s_CeresLogger->set_level(level);
			break;
		}
	}
	
	if (!logInitialized)
	{
		s_CeresLogger->set_level(spdlog::level::info);
		s_PlanetLabLogger->set_level(spdlog::level::info);
	}

	PLANETLAB_INFO("Initialize Log with {} level", to_string_view(s_CeresLogger->level()));
}


}