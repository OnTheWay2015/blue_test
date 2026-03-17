:: #define GOOGLE_PROTOBUF_VERSION 3010001
:: #define GOOGLE_PROTOBUF_VERSION 4000000

::protoc.exe --proto_path=./   --cpp_out=../protocol/win p1.proto
::protoc.exe --proto_path=./   --cpp_out=../protocol/win p2.proto
::protoc.exe --proto_path=./   --cpp_out=../protocol/win t1.proto
protoc.exe --proto_path=./   --cpp_out=../protocol/win server_base.proto

pause
