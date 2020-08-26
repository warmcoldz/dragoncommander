pushd vcpkg
bootstrap-vcpkg.bat
vcpkg integrate install
vcpkg install grpc:x64-windows
vcpkg install boost:x64-windows
vcpkg install gtest:x64-windows
popd
