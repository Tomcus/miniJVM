find_package(spdlog 1.9 QUIET)

include(FetchContent)

if(NOT spdlog_FOUND)
	
	FetchContent_Declare(
		spdlog
		GIT_REPOSITORY https://github.com/gabime/spdlog.git
		GIT_TAG        v1.9.2
	)
    FetchContent_MakeAvailable(spdlog)
endif()