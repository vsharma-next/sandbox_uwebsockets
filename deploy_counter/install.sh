git clone --recurse-submodules https://github.com/uNetworking/uWebSockets.git
cd ./uWebSockets && make -C uSockets && cd ..
mkdir simdjson
cd simdjson && wget https://raw.githubusercontent.com/simdjson/simdjson/master/singleheader/simdjson.h https://raw.githubusercontent.com/simdjson/simdjson/master/singleheader/simdjson.cpp && cd ..

# g++ -std=c++20 -I./uWebSockets/src -I./uWebSockets/uSockets/src ./simdjson/simdjson.cpp ./app.cpp ./uWebSockets/uSockets/*.o -lz -o ./app.exe
