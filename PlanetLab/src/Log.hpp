#pragma once

#include "spdlog/spdlog.h"

namespace PlanetLab
{

class Log
{
public:
	static void init(int argc, char** argv);

	inline static std::shared_ptr<spdlog::logger>& getCeresLogger() { return s_CeresLogger; }
	inline static std::shared_ptr<spdlog::logger>& gettPlanetLabLogger() { return s_PlanetLabLogger; }

	Log(const Log&) = delete;
	Log& operator=(const Log&) = delete;

private:
	Log() = default;
	~Log() = default;

private:
	static std::shared_ptr<spdlog::logger> s_CeresLogger;
	static std::shared_ptr<spdlog::logger> s_PlanetLabLogger;

};

#define PLANETLAB_ERROR(...)    ::PlanetLab::Log::gettPlanetLabLogger()->error(__VA_ARGS__)
#define PLANETLAB_WARNING(...)  ::PlanetLab::Log::gettPlanetLabLogger()->warn(__VA_ARGS__)
#define PLANETLAB_INFO(...)     ::PlanetLab::Log::gettPlanetLabLogger()->info(__VA_ARGS__)
#define PLANETLAB_TRACE(...)    ::PlanetLab::Log::gettPlanetLabLogger()->trace(__VA_ARGS__)

#define CERES_ERROR(...)        ::PlanetLab::Log::getCeresLogger()->error(__VA_ARGS__)
#define CERES_WARNING(...)      ::PlanetLab::Log::getCeresLogger()->warn(__VA_ARGS__)
#define CERES_INFO(...)         ::PlanetLab::Log::getCeresLogger()->info(__VA_ARGS__)
#define CERES_TRACE(...)        ::PlanetLab::Log::getCeresLogger()->trace(__VA_ARGS__)


}


