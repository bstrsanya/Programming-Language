FLAGS = -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations \
		-Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts \
		-Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal \
		-Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op \
		-Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self \
		-Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel \
		-Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types \
		-Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code \
		-Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers \
		-Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new \
		-fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging \
		-fno-omit-frame-pointer -pie -fPIE -Werror=vla \
		-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

all: language

language: main.o ReadFile.o SizeFile.o CtorDtor.o Parser.o GraphDump.o ParserNumbers.o 
	g++ main.o ReadFile.o SizeFile.o CtorDtor.o Parser.o GraphDump.o ParserNumbers.o ${FLAGS} -o language

main.o: main.cpp
	g++ -I./lib/include -I./include ${FLAGS} -c main.cpp

ReadFile.o: ./lib/src/ReadFile.cpp
	g++ -I./lib/include -I./include ${FLAGS} -c ./lib/src/ReadFile.cpp

SizeFile.o: ./lib/src/SizeFile.cpp
	g++ -I./lib/include -I./include ${FLAGS} -c ./lib/src/SizeFile.cpp

CtorDtor.o: ./src/CtorDtor.cpp
	g++ -I./include -I./lib/include ${FLAGS} -c ./src/CtorDtor.cpp

Parser.o: ./src/Parser.cpp
	g++ -I./include -I./lib/include ${FLAGS} -c ./src/Parser.cpp

GraphDump.o: ./src/GraphDump.cpp
	g++ -I./include -I./lib/include ${FLAGS} -c ./src/GraphDump.cpp

ParserNumbers.o: ./src/ParserNumbers.cpp
	g++ -I./include -I./lib/include ${FLAGS} -c ./src/ParserNumbers.cpp

clean:
	rm -rf *.o language