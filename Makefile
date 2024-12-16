all: built

built:
	make -f ./Frontend/Makefile
	make -f ./Backend/Makefile
run:
	./frontend
	./backend
clean:
	make -f ./Frontend/Makefile clean
	make -f ./Backend/Makefile clean
