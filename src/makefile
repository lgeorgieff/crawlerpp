bin_folder = ./bin
lib_folder = ./lib
odb_folder = ./odb
header_folder = ./headers


$(lib_folder)/uri.o: uri.cpp $(header_folder)/uri.h $(lib_folder)/exceptions.o
	g++ -Wall -fPIC -c uri.cpp -o $(lib_folder)/uri.o -std=c++11 -I$(header_folder)

$(lib_folder)/exceptions.o: exceptions.cpp $(header_folder)/exceptions.h
	g++ -Wall -fPIC -c exceptions.cpp -o $(lib_folder)/exceptions.o -std=c++11 -I$(header_folder)

.PHONY: clean
clean:
	rm $(bin_folder)/* $(lib_folder)/* $(odb_folder)/*