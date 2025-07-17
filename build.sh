#/bin/sh

srcs=$(find src/ -type f | grep -E "[.]c(xx|pp)?$")

gcc -I./include $srcs
