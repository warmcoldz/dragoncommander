find_path(BOOST_ALGORITHM_INCLUDE_DIRS "boost/algorithm/algorithm.hpp")

add_executable(main
	main.cpp
	exception.h
	string_algorithms.h
	file/file.h
	file/file.cpp
	file.grpc.pb.cc
	file.pb.cc)

target_include_directories(main PRIVATE ${BOOST_ALGORITHM_INCLUDE_DIRS})
target_link_libraries(main PRIVATE gRPC::gpr gRPC::grpc gRPC::grpc++ gRPC::grpc++_alts gRPC::grpc_plugin_support gRPC::grpc++_reflection)
target_link_libraries(main PRIVATE c-ares::cares)
target_link_libraries(main PRIVATE Shlwapi)
