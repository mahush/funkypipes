cmake -S . -B build \
    -DCMAKE_C_COMPILER=/home/matthias/other_projects/LLVM-20.1.0-Linux-ARM64/bin/clang \
    -DCMAKE_CXX_COMPILER=/home/matthias/other_projects/LLVM-20.1.0-Linux-ARM64/bin/clang++ \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_CXX_FLAGS="-DENABLE_COVERAGE -O0 -fno-inline -Xclang -disable-O0-optnone -g -fprofile-instr-generate -fcoverage-mapping"\
    -DCMAKE_C_FLAGS="-fprofile-instr-generate -fcoverage-mapping"
