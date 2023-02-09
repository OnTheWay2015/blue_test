protoc --proto_path=./p1 --cpp_out=../protocol/p1 p1/p1.proto
protoc --proto_path=./p2 --cpp_out=../protocol/p2 p2/p2.proto
protoc --proto_path=./ --proto_path=./p2  --proto_path=./p1 --cpp_out=../protocol test.proto

 