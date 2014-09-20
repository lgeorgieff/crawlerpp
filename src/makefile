bin_folder = ./bin
lib_folder = ./lib
odb_folder = ./odb
obj_folder = ./obj
test_folder = ./test
header_folder = ./headers

$(test_folder)/tests: tests.cpp $(bin_folder)/libcrawler_cpp.so
	g++ -Wall tests.cpp -L$(bin_folder) -lcrawler_cpp -Wl,-rpath,$(bin_folder) -o $(test_folder)/tests -std=c++11

$(bin_folder)/libcrawler_cpp.so: $(obj_folder)/uri.o $(obj_folder)/exceptions.o $(obj_folder)/utils.o
	g++ -Wall -fPIC -shared $(obj_folder)/uri.o $(obj_folder)/utils.o  $(obj_folder)/exceptions.o -o $(bin_folder)/libcrawler_cpp.so -std=c++11

$(obj_folder)/uri.o: uri.cpp $(header_folder)/uri.h $(obj_folder)/exceptions.o $(obj_folder)/exceptions.o
	g++ -Wall -fPIC -c uri.cpp -o $(obj_folder)/uri.o -std=c++11 -I$(header_folder)

$(obj_folder)/utils.o: utils.cpp $(header_folder)/utils.h $(obj_folder)/exceptions.o
	g++ -Wall -fPIC -c utils.cpp -o $(obj_folder)/utils.o -std=c++11 -I$(header_folder)

$(obj_folder)/exceptions.o: exceptions.cpp $(header_folder)/exceptions.h
	g++ -Wall -fPIC -c exceptions.cpp -o $(obj_folder)/exceptions.o -std=c++11 -I$(header_folder)

# This target ist thought only for manually creating all sub directories
.PHONY: init
init:
	mkdir -p $(bin_folder)
	mkdir -p $(lib_folder)
	mkdir -p $(odb_folder)
	mkdir -p $(obj_folder)
	mkdir -p $(test_folder)

.PHONY: clean
clean:
	if [ -d $(bin_folder) ] && [ "$$(ls $(bin_folder)/*)" ]; then rm $(bin_folder)/*; fi
#	if [ -d $(lib_folder) ] && [ "$$(ls $(lib_folder)/*)" ]; then rm $(lib_folder)/*; fi
	if [ -d $(odb_folder) ] && [ "$$(ls $(odb_folder)/*)" ]; then rm -r $(odb_folder); fi
	if [ -d $(obj_folder) ] && [ "$$(ls $(obj_folder)/*)" ]; then rm -r $(obj_folder); fi
	if [ -d $(test_folder) ] && [ "$$(ls $(test_folder)/*)" ]; then rm -r $(test_folder); fi
