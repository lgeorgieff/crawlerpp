bin_folder = ./bin
lib_folder = ./lib
odb_folder = ./odb
obj_folder = ./obj
test_folder = ./test

$(test_folder)/tests: tests.cpp $(bin_folder)/libcrawler_cpp.so
	g++ -Wall tests.cpp -L$(bin_folder) -lcrawler_cpp -lboost_system -Wl,-rpath,$(bin_folder) -o $(test_folder)/tests -lnetwork-uri -std=c++11

$(bin_folder)/libcrawler_cpp.so: $(obj_folder)/uri.o $(obj_folder)/exceptions.o $(obj_folder)/utils.o
	g++ -Wall -fPIC -shared $(obj_folder)/uri.o $(obj_folder)/utils.o  $(obj_folder)/exceptions.o -o $(bin_folder)/libcrawler_cpp.so -std=c++11

$(obj_folder)/uri.o: uri.cpp uri.h $(obj_folder)/exceptions.o $(obj_folder)/exceptions.o
	g++ -Wall -fPIC -c uri.cpp -o $(obj_folder)/uri.o -std=c++11

$(obj_folder)/utils.o: utils.cpp utils.h $(obj_folder)/exceptions.o
	g++ -Wall -fPIC -c utils.cpp -o $(obj_folder)/utils.o -std=c++11

$(obj_folder)/exceptions.o: exceptions.cpp exceptions.h
	g++ -Wall -fPIC -c exceptions.cpp -o $(obj_folder)/exceptions.o -std=c++11

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
	@(rm $(bin_folder)/* 2> /dev/null && echo "rm $(bin_folder)/*") || true
	@(rm $(odb_folder)/* 2> /dev/null && echo "rm $(odb_folder)/*") || true
	@(rm $(obj_folder)/* 2> /dev/null && echo "rm $(obj_folder)/*") || true
	@(rm $(test_folder)/* 2> /dev/null && echo "rm $(test_folder)/*") || true
