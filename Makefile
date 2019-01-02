export EMCC_DEBUG=1

stage_two.js: stage_two.cpp
	em++ -I /home/jan/Downloads/boost/boost_1_67_0 --bind --std=c++11 stage_two.cpp -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -o stage_two.js -s EXTRA_EXPORTED_RUNTIME_METHODS="['addOnPostRun', 'calculateGraph']"
