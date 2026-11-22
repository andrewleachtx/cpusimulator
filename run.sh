FLAGS=(-Wall -Wextra -Wpedantic)
clang++ -std=c++20 main.cpp -o cpusimulator $FLAGS
rc=$?

if ((rc == 0)); then
    ./cpusimulator
fi