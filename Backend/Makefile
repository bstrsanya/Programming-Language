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
II = -I ./lib/include -I ./Backend/include -I ./General

all: backend

backend: ./Backend/obj/main.o ./Backend/obj/ReadFile.o ./Backend/obj/SizeFile.o ./Backend/obj/GraphDump.o ./Backend/obj/Recovery.o ./Backend/obj/CreateAsm.o
	g++ ./Backend/obj/main.o ./Backend/obj/ReadFile.o ./Backend/obj/SizeFile.o ./Backend/obj/GraphDump.o ./Backend/obj/Recovery.o ./Backend/obj/CreateAsm.o ${FLAGS} -o backend

./Backend/obj/main.o: ./Backend/main.cpp
	$(CC) $(II) $(FLAGS) -c ./Backend/main.cpp -o ./Backend/obj/main.o

./Backend/obj/Recovery.o: ./Backend/src/Recovery.cpp
	$(CC) $(II) $(FLAGS) -c ./Backend/src/Recovery.cpp -o ./Backend/obj/Recovery.o

./Backend/obj/CreateAsm.o: ./Backend/src/CreateAsm.cpp
	$(CC) $(II) $(FLAGS) -c ./Backend/src/CreateAsm.cpp -o ./Backend/obj/CreateAsm.o

./Backend/obj/ReadFile.o: ./lib/src/ReadFile.cpp
	$(CC) $(II) $(FLAGS) -c ./lib/src/ReadFile.cpp -o ./Backend/obj/ReadFile.o

./Backend/obj/SizeFile.o: ./lib/src/SizeFile.cpp
	$(CC) $(II) $(FLAGS) -c ./lib/src/SizeFile.cpp -o ./Backend/obj/SizeFile.o

./Backend/obj/GraphDump.o: ./General/GraphDump.cpp
	$(CC) $(II) $(FLAGS) -c ./General/GraphDump.cpp -o ./Backend/obj/GraphDump.o

clean:
	rm -rf ./Backend/obj/*.o backend