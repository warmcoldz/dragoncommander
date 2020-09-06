#include <google/protobuf/service.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <shlwapi.h>
#include <windows.h>

#include <boost/algorithm/string/predicate.hpp>
#include <iostream>
#include <memory>
#include <string>

#include "exception.h"
#include "file.grpc.pb.h"
#include "file.pb.h"
#include "file/file.h"
#include "string_algorithms.h"

using namespace std::string_literals;

namespace dragon_commander {
namespace {
file::api::FileInfo::FileType ConvertToGrpc(FileType fileType) {
  switch (fileType) {
    case FileType::Dir:
      return file::api::FileInfo::Dir;
    case FileType::RegularFile:
      return file::api::FileInfo::RegularFile;
  }

  return {};
}

}  // namespace

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

  grpc::Status Delete(::grpc::ServerContext* context,
                      const ::file::api::FilePath* request,
                      ::file::api::RetVal* response) override {
    try {
      const auto filePath = FromUtf8ToUtf16(request->filepath());
      RemoveFile(filePath);
      response->set_success(true);
    } catch (const std::exception& e) {
      std::cout << e.what() << std::endl;
      response->set_success(false);
      response->set_message(e.what());
    }

    return grpc::Status::OK;
  }

  grpc::Status CreateDir(::grpc::ServerContext* context,
                         const ::file::api::FilePath* request,
                         ::file::api::RetVal* response) override {
    try {
      const auto filePath = FromUtf8ToUtf16(request->filepath());
      MakeDir(filePath);
      response->set_success(true);
    } catch (const std::exception& e) {
      std::cout << e.what() << std::endl;
      response->set_success(false);
      response->set_message(e.what());
    }

    return grpc::Status::OK;
  }
};

}  // namespace dragon_commander

int main(int argc, char** argv) {
  const auto server_address = "0.0.0.0:50051"s;

  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

  dragon_commander::FileExecutorImpl service;
  builder.RegisterService(&service);

  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  server->Wait();
  return 0;
}
