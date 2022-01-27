find_package(lyra 1.6 QUIET)

include(FetchContent)

if(NOT lyra_FOUND)
	
	FetchContent_Declare(
		lyra
		GIT_REPOSITORY https://github.com/bfgroup/Lyra.git
		GIT_TAG        1.6
	)
    FetchContent_MakeAvailable(lyra)
endif()