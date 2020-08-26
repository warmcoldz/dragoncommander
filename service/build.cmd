set PROTO_BIN="../vcpkg/buildtrees/protobuf/x64-windows-rel/protoc.exe"
set GRPC_CPP_PLUGIN="../vcpkg/buildtrees/grpc/x64-windows-rel/grpc_cpp_plugin.exe"
set SRC_DIR=../proto
set DST_DIR=src

%PROTO_BIN% -I=%SRC_DIR% --grpc_out=%DST_DIR% --plugin=protoc-gen-grpc=%GRPC_CPP_PLUGIN% %SRC_DIR%/file.proto
%PROTO_BIN% -I=%SRC_DIR% --cpp_out=%DST_DIR% %SRC_DIR%/file.proto

mkdir .build
pushd .build
cmake -G "Visual Studio 16 2019" -A x64 -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake ..
popd
