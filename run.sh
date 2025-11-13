mkdir -p build/
cmake -B ./build -S .
cmake --build ./build
cd ./build
ctest -V
