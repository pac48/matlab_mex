# rm ./proto/*.py
protoc -I=./proto/src --cpp_out=./proto ./proto/src/msgs.proto