cmake_minimum_required (VERSION 3.17 FATAL_ERROR)

project (AqcPqc
	LANGUAGES CXX
	DESCRIPTION "Experimental framework for integer factorization on variational quantum algorithms using FastVQA"
	VERSION 0.1.0
)

set(CMAKE_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin")
set(DISTRIBUTED OFF)
message(
	"DISTRIBUTED=${DISTRIBUTED}"
)

set(CMAKE_BUILD_TYPE Release)

find_package(FastVQA REQUIRED)
find_package (Eigen3 3.3 REQUIRED NO_MODULE)

#find_package(MPI)
#if(MPI_FOUND AND DISTRIBUTED)
#	set(DISTRIBUTED_OPTS_FILES ${PROJECT_SOURCE_DIR}/src/lattiq_distributed.cpp)
#else()
#	set(DISTRIBUTED_OPTS_FILES ${PROJECT_SOURCE_DIR}/src/lattiq.cpp)
#endif()

file(GLOB SRC_ORIG
	#"${DISTRIBUTED_OPTS_FILES}"
	${PROJECT_SOURCE_DIR}/src/aqc_pqc.cpp
	${PROJECT_SOURCE_DIR}/src/read_maxcut_dataset.cpp
	${PROJECT_SOURCE_DIR}/src/read_nb_dataset.cpp
	${PROJECT_SOURCE_DIR}/src/read_ftim_dataset.cpp
)

set (CMAKE_CXX_FLAGS "-I ~/.local/include -L ~/.local/lib -g3 -Wall -O2")       
add_subdirectory(src)
       
add_executable(${PROJECT_NAME} ${SRC_ORIG})
include_directories(
	${PROJECT_SOURCE_DIR}/src
)
target_link_libraries(${PROJECT_NAME} fastVQA::FastVQA Eigen3::Eigen QuEST) #gmpxx gmp m)
target_compile_definitions(${PROJECT_NAME}
    PRIVATE
    QuEST_PREC=4
)
