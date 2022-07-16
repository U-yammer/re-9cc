CFLAGS=-std=c11 -g -static

# make
# make 9cc でも ok
9cc: 9cc.c

# 9ccを実行して，./test.shを実行するよ
test: 9cc
	./test.sh

clean:
	rm -f 9cc *.o *~ tmp*

.PHONY: test clean