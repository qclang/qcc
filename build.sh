#/bin/sh

srcs=$(find src/ -type f | grep -E "[.]c(xx|pp)?$")

g++ -std=c++17 -I./include $srcs -o bin/program
