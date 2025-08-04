#/bin/sh

arg="$1"

if [ "$arg" == "tok" ] || [ -z $arg ]; then
	srcs="programs/tokenizer.cxx "
	srcs+=$(find src/tokenizer/ | grep -E "[.]c(xx|pp)$")
	g++ -std=c++17 -I./include $srcs -o bin/program
fi
