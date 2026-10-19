report: main.o stats.o
    g++ main.o stats.o -o report

main.o: main.cpp stats.h
    g++ -Wall -c main.cpp -o main.o

stats.o: stats.cpp stats.h
    g++ -Wall -c stats.cpp -o stats.o

clean:
    rm -f *.o report
