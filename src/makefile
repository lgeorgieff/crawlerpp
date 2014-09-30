bin_folder = ./bin
lib_folder = ./lib
odb_folder = ./odb
obj_folder = ./obj
test_folder = ./test
dynamic_lib_folders = $(bin_folder):/usr/local/lib/

$(test_folder)/tests: tests.cpp $(bin_folder)/libcrawler_pp.so uri.h $(odb_folder)/uri_odb_files
	g++ -Wall tests.cpp -L$(bin_folder) -lcrawler_pp -lboost_system -Wl,-rpath,$(dynamic_lib_folders) -o $(test_folder)/tests -lnetwork-uri -lodb-pgsql -lodb -std=c++11

$(bin_folder)/libcrawler_pp.so: $(obj_folder)/uri.o $(obj_folder)/exceptions.o $(obj_folder)/utils.o $(bin_folder)/uri.odb.o
	g++ -Wall -fPIC -shared $(obj_folder)/uri.o $(obj_folder)/utils.o  $(obj_folder)/exceptions.o $(obj_folder)/uri.odb.o -o $(bin_folder)/libcrawler_pp.so -std=c++11

$(obj_folder)/uri.o: uri.cpp uri.h $(obj_folder)/exceptions.o $(obj_folder)/utils.o
	g++ -Wall -fPIC -c uri.cpp -o $(obj_folder)/uri.o -std=c++11

$(bin_folder)/uri.odb.o: $(odb_folder)/uri_odb_files uri.pragma.h
	g++ -Wall -fPIC -c $(odb_folder)/uri.odb.cpp -o $(obj_folder)/uri.odb.o -std=c++11

$(odb_folder)/uri_odb_files: uri.h
	odb --database pgsql --generate-query --generate-schema --output-dir $(odb_folder) --std c++11 --odb-file-suffix ".odb" --hxx-suffix ".h" --cxx-suffix ".cpp" --ixx-suffix ".i" uri.h
	@(if [ ! -e $(odb_folder)/uri.h ]; then ln -s ../uri.h $(odb_folder)/uri.h; fi) && echo "ln -s ../uri.h $(odb_folder)/uri.h"

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


#TODO: refactor entry target (-l)
