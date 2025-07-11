project(csBlockMatrixSample LANGUAGES CXX)

# prepare application
cs_add_executable(csBlockMatrixSample)

cs_target_sources(csBlockMatrixSample
	PRIVATE
	eigenPlayground.cpp
)
set_target_properties(csBlockMatrixSample PROPERTIES AUTOMOC ON)
set_target_properties(csBlockMatrixSample PROPERTIES AUTOUIC ON)
set_target_properties(csBlockMatrixSample PROPERTIES AUTORCC ON)

cs_target_link_libraries(csBlockMatrixSample
	PUBLIC
		csBlockMatrix::csBlockMatrix
)

cs_install_target(csBlockMatrixSample csBlockMatrixSampleExport DEFAULT)

cs_finalize_target(csBlockMatrixSample)
