output: main.o application.o commands.o parser.o stringTools.o
				clang++ -DDEBUG=1 -DLINUX=1 -std=c++17 *.o -o main

build: main
				clang++ -DDEBUG=1 -DLINUX=1 -std=c++17 *.o -o main

main.o: main.cpp
				@echo recompiling main.cpp
				@clang++ -DDEBUG=1 -DLINUX=1 -c main.cpp -std=c++17

application.o: application.cpp application.h
				@echo recompiling application.cpp
				@clang++ -DDEBUG=1 -DLINUX=1 -c application.cpp -std=c++17

commands.o: commands.cpp commands.h
				@echo recompiling commands.cpp
				@clang++ -DDEBUG=1 -DLINUX=1 -c commands.cpp -std=c++17

parser.o: parser.cpp parser.h
				@echo recompiling parser.cpp
				@clang++ -DDEBUG=1 -DLINUX=1 -c parser.cpp -std=c++17

stringTools.o: stringTools.cpp stringTools.h
				@echo recompiling stringTools.cpp
				@clang++ -DDEBUG=1 -DLINUX=1 -c stringTools.cpp -std=c++17

clean:
				@rm *.o output
