# 2.1 用：手写的一份 Makefile，也就是 1.4 那三条命令的「配方」。
#
# 冒号左边是「要做出来的东西」，右边是「靠什么做出来」：
# report 靠 main.o 和 stats.o，main.o 靠 main.cpp 和 stats.h……
# 底下缩进的那一行是做法（开头必须是 Tab，空格不行），
# 到了真要做的时候原样交给 shell 跑，make 还会把它打在屏幕上——
# 所以你每次都能看见它到底编了哪几个文件。
#
# 想让 report 从头来过：make clean，再 make。

report: main.o stats.o
	g++ main.o stats.o -o report

main.o: main.cpp stats.h
	g++ -Wall -c main.cpp -o main.o

stats.o: stats.cpp stats.h
	g++ -Wall -c stats.cpp -o stats.o

clean:
	rm -f *.o report
