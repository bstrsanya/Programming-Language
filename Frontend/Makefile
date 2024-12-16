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

CC = g++
II = -I ./lib/include -I ./Frontend/include -I ./General

all: frontend

frontend: ./Frontend/obj/main.o ./Frontend/obj/ReadFile.o ./Frontend/obj/SizeFile.o ./Frontend/obj/CtorDtor.o ./Frontend/obj/Parser.o ./Frontend/obj/GraphDump.o ./Frontend/obj/ParserNumbers.o 
	g++ ./Frontend/obj/main.o ./Frontend/obj/ReadFile.o ./Frontend/obj/SizeFile.o ./Frontend/obj/CtorDtor.o ./Frontend/obj/Parser.o ./Frontend/obj/GraphDump.o ./Frontend/obj/ParserNumbers.o ${FLAGS} -o frontend

./Frontend/obj/main.o: ./Frontend/main.cpp
	$(CC) $(II) $(FLAGS) -c ./Frontend/main.cpp -o ./Frontend/obj/main.o

./Frontend/obj/ReadFile.o: ./lib/src/ReadFile.cpp
	$(CC) $(II) $(FLAGS) -c ./lib/src/ReadFile.cpp -o ./Frontend/obj/ReadFile.o

./Frontend/obj/SizeFile.o: ./lib/src/SizeFile.cpp
	$(CC) $(II) $(FLAGS) -c ./lib/src/SizeFile.cpp -o ./Frontend/obj/SizeFile.o

./Frontend/obj/CtorDtor.o: ./Frontend/src/CtorDtor.cpp
	$(CC) $(II) $(FLAGS) -c ./Frontend/src/CtorDtor.cpp -o ./Frontend/obj/CtorDtor.o

./Frontend/obj/Parser.o: ./Frontend/src/Parser.cpp
	$(CC) $(II) $(FLAGS) -c ./Frontend/src/Parser.cpp -o ./Frontend/obj/Parser.o

./Frontend/obj/GraphDump.o: ./General/GraphDump.cpp
	$(CC) $(II) $(FLAGS) -c ./General/GraphDump.cpp -o ./Frontend/obj/GraphDump.o

./Frontend/obj/ParserNumbers.o: ./Frontend/src/ParserNumbers.cpp
	$(CC) $(II) $(FLAGS) -c ./Frontend/src/ParserNumbers.cpp -o ./Frontend/obj/ParserNumbers.o

clean:
	rm -rf ./Frontend/obj/*.o frontend