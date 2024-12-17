all: built run processor

built:
	make -f ./Frontend/Makefile
	make -f ./Backend/Makefile

run:
	./frontend
	./backend

clean:
	make -f ./Frontend/Makefile clean
	make -f ./Backend/Makefile clean
	rm -rf ./asm.bin
	rm -rf ./asm.txt
	rm -rf ./backend.png
	rm -rf ./frontend.png
	rm -rf ./tree.txt
	rm -rf ./aaa.dot

processor:
	./Processor/asm ./asm.txt ./asm.bin
	./Processor/pro ./asm.bin