find_package(fmt 8 QUIET)

include(FetchContent)

if(NOT fmt_FOUND)
	
	FetchContent_Declare(
		fmt
		GIT_REPOSITORY https://github.com/fmtlib/fmt.git
		GIT_TAG        8.1.1
	)
    FetchContent_MakeAvailable(fmt)
endif()