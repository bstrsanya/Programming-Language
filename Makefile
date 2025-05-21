all: built

built:
	make -f ./Frontend/Makefile
	make -f ./Backend/Makefile
	make -f ./Back/Makefile
	make -f ./IRBackend/Makefile

clean:
	make -f ./Frontend/Makefile clean
	make -f ./Backend/Makefile clean
	make -f ./Back/Makefile clean
	make -f ./IRBackend/Makefile clean
	rm -rf ./asm.bin
	rm -rf ./asm.txt
	rm -rf ./backend.png
	rm -rf ./frontend.png
	rm -rf ./tree.txt
	rm -rf ./aaa.dot
	rm -rf ./IR.txt