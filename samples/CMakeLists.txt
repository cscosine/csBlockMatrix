project(csBlockMatrixSample LANGUAGES CXX)

# prepare application
cs_add_executable(csBlockMatrixSample)

cs_find_package(csBlockMatrixSample PRIVATE csCore COMPONENTS core REQUIRED CONFIG)

cs_target_sources(csBlockMatrixSample
	PRIVATE
	eigenPlayground.cpp
)

cs_target_link_libraries(csBlockMatrixSample
	PUBLIC
		csBlockMatrix::csBlockMatrix
		csCore::core
)

cs_install_target(csBlockMatrixSample COMPONENT csBlockMatrixSample DEFAULT)
