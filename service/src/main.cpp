#include <Windows.h>
#include <google/protobuf/service.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <shlwapi.h>

#include <boost/algorithm/string/predicate.hpp>
#include <iostream>
#include <memory>
#include <string>

#include "exception.h"
#include "file.grpc.pb.h"
#include "file.pb.h"
#include "file/file.h"
#include "string_algorithms.h"

file::api::FileInfo::FileType ConvertToGrpc(FileType fileType) {
  switch (fileType) {
    case FileType::Dir:
      return file::api::FileInfo::Dir;
    case FileType::RegularFile:
      return file::api::FileInfo::RegularFile;
  }

  return {};
}

class FileExecutorImpl : public file::api::FileExecutor::Service {
 public:
  grpc::Status Execute(grpc::ServerContext* context,
                       const file::api::FilePath* request,
                       file::api::RetVal* response) final {
    try {
      const auto filePath = FromUtf8ToUtf16(request->filepath());
      LaunchFile(filePath);
      response->set_success(true);
    } catch (const std::exception& e) {
      std::cout << e.what() << std::endl;
      response->set_success(false);
      response->set_message(e.what());
    }

    return grpc::Status::OK;
  }

  grpc::Status GetFiles(grpc::ServerContext* context,
                        const file::api::FilePath* request,
                        file::api::FileList* response) override {
    try {
      const auto filePath = FromUtf8ToUtf16(request->filepath());
      for (auto&& info : GetFilesFromDirectory(filePath)) {
        auto fileInfo = response->add_fileinfo();
        fileInfo->set_name(FromUtf16ToUtf8(std::move(info.name)));
        fileInfo->set_type(ConvertToGrpc(info.type));
      }

      response->set_success(true);
    } catch (const std::exception& e) {
      std::cout << e.what() << std::endl;
      response->set_success(false);
      response->set_message(e.what());
    }

    return grpc::Status::OK;
  }
};

int main(int argc, char** argv) {
  const std::string server_address("0.0.0.0:50051");

  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

  FileExecutorImpl service;
  builder.RegisterService(&service);

  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();

  return 0;
}
