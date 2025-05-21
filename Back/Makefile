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
II = -I ./lib/include -I ./Back/include -I ./General

all: dir Back

dir:
	mkdir -p ./Back/obj

Back: ./Back/obj/main.o ./Back/obj/ReadFile.o ./Back/obj/SizeFile.o ./Back/obj/GraphDump.o ./Back/obj/Recovery.o ./Back/obj/CreateList.o
	g++ ./Back/obj/main.o ./Back/obj/ReadFile.o ./Back/obj/SizeFile.o ./Back/obj/GraphDump.o ./Back/obj/Recovery.o ./Back/obj/CreateList.o ${FLAGS} -o back

./Back/obj/main.o: ./Back/main.cpp
	$(CC) $(II) $(FLAGS) -c ./Back/main.cpp -o ./Back/obj/main.o

./Back/obj/Recovery.o: ./Back/src/Recovery.cpp
	$(CC) $(II) $(FLAGS) -c ./Back/src/Recovery.cpp -o ./Back/obj/Recovery.o

./Back/obj/ReadFile.o: ./lib/src/ReadFile.cpp
	$(CC) $(II) $(FLAGS) -c ./lib/src/ReadFile.cpp -o ./Back/obj/ReadFile.o

./Back/obj/SizeFile.o: ./lib/src/SizeFile.cpp
	$(CC) $(II) $(FLAGS) -c ./lib/src/SizeFile.cpp -o ./Back/obj/SizeFile.o

./Back/obj/GraphDump.o: ./General/GraphDump.cpp
	$(CC) $(II) $(FLAGS) -c ./General/GraphDump.cpp -o ./Back/obj/GraphDump.o

./Back/obj/CreateList.o: ./Back/src/CreateList.cpp
	$(CC) $(II) $(FLAGS) -c ./Back/src/CreateList.cpp -o ./Back/obj/CreateList.o

clean:
	rm -rf ./Back/obj/*.o back