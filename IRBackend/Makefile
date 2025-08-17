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
II = -I ./lib/include -I ./IRBackend/include -I ./General

all: dir IRBackend

dir:
	mkdir -p ./IRBackend/obj

IRBackend: ./IRBackend/obj/main.o ./IRBackend/obj/ReadFile.o ./IRBackend/obj/SizeFile.o ./IRBackend/obj/CreateNasm.o ./IRBackend/obj/CreateElf.o ./IRBackend/obj/ReadIR.o ./IRBackend/obj/CreateList.o ./IRBackend/obj/Recovery.o
	g++ ./IRBackend/obj/main.o ./IRBackend/obj/ReadFile.o ./IRBackend/obj/SizeFile.o ./IRBackend/obj/CreateNasm.o ./IRBackend/obj/CreateElf.o ./IRBackend/obj/ReadIR.o ./IRBackend/obj/CreateList.o ./IRBackend/obj/Recovery.o ${FLAGS} -o ir

./IRBackend/obj/main.o: ./IRBackend/main.cpp
	$(CC) $(II) $(FLAGS) -c ./IRBackend/main.cpp -o ./IRBackend/obj/main.o

./IRBackend/obj/ReadFile.o: ./lib/src/ReadFile.cpp
	$(CC) $(II) $(FLAGS) -c ./lib/src/ReadFile.cpp -o ./IRBackend/obj/ReadFile.o

./IRBackend/obj/SizeFile.o: ./lib/src/SizeFile.cpp
	$(CC) $(II) $(FLAGS) -c ./lib/src/SizeFile.cpp -o ./IRBackend/obj/SizeFile.o

./IRBackend/obj/CreateNasm.o: ./IRBackend/src/CreateNasm.cpp
	$(CC) $(II) $(FLAGS) -c ./IRBackend/src/CreateNasm.cpp -o ./IRBackend/obj/CreateNasm.o

./IRBackend/obj/CreateElf.o: ./IRBackend/src/CreateElf.cpp
	$(CC) $(II) $(FLAGS) -c ./IRBackend/src/CreateElf.cpp -o ./IRBackend/obj/CreateElf.o

./IRBackend/obj/ReadIR.o: ./IRBackend/src/ReadIR.cpp
	$(CC) $(II) $(FLAGS) -c ./IRBackend/src/ReadIR.cpp -o ./IRBackend/obj/ReadIR.o

./IRBackend/obj/Recovery.o: ./IRBackend/src/Recovery.cpp
	$(CC) $(II) $(FLAGS) -c ./IRBackend/src/Recovery.cpp -o ./IRBackend/obj/Recovery.o
	
./IRBackend/obj/CreateList.o: ./IRBackend/src/CreateList.cpp
	$(CC) $(II) $(FLAGS) -c ./IRBackend/src/CreateList.cpp -o ./IRBackend/obj/CreateList.o
clean:
	rm -rf ./IRBackend/obj/*.o ir