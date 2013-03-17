# Harriet Script Language
# Copyright (c) 2012, 2013 Alexander van Renen (alexandervanrenen@gmail.com)
# See the file LICENSE.txt for copying permission.
######################################################################

all: calculator tester

objDir:= obj/
srcDir:= src/

-include config.local

CXX ?= g++
cf := -Werror -Wall -Wextra -Wuninitialized --std=c++0x -g0 -O3 -I./src -I./libs/gtest/include
lf := -g0 -O3 --std=c++0x -I./src -I./libs/gtest/include

build_dir = @mkdir -p $(dir $@)

-include src/LocalMakefile
obj_files := $(addprefix $(objDir),$(obj_files_src))

calculator: $(obj_files) obj/samples/calculator.o
	$(CXX) -o $@ obj/samples/calculator.o $(obj_files) $(lf)

tester: libs/gtest $(obj_files) obj/tests/tester.o
	$(CXX) -o $@ obj/tests/tester.o $(lf) $(obj_files) libs/gtest/libgtest.a -pthread

$(objDir)%.o: %.cpp
	$(build_dir)
	$(CXX) -MD -c -o $@ $< $(cf)
	@cp $(objDir)$*.d $(objDir)$*.P; \
		sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
			-e '/^$$/ d' -e 's/$$/ :/' < $(objDir)$*.d >> $(objDir)$*.P; \
		rm -f $(objDir)$*.d

-include $(objDir)*.P
-include $(objDir)*/*.P

libs/gtest:
	$(build_dir)
	cd libs/ ;\
	wget -O gtest-1.6.0.zip https://googletest.googlecode.com/files/gtest-1.6.0.zip ;\
	unzip -q gtest-1.6.0.zip ;\
	cd gtest-1.6.0 ;\
	mkdir -p build ;\
	cd build ;\
	cmake -G"Unix Makefiles" .. ;\
	make ;\
	ar -r libgtest.a libgtest_main.a
	mkdir -p libs/gtest/include
	mv libs/gtest-1.6.0/include/gtest/* libs/gtest/include
	mv libs/gtest-1.6.0/build/libgtest.a libs/gtest/
	rm libs/gtest-1.6.0.zip
	rm -rf libs/gtest-1.6.0

clean:
	rm $(objDir) -rf
	find . -name "tester" -type f -delete
	find . -name "calculator" -type f -delete
