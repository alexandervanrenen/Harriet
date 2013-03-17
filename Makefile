# Harriet Script Language
# Copyright (c) 2012, 2013 Alexander van Renen (alexandervanrenen@gmail.com)
# See the file LICENSE.txt for copying permission.
######################################################################

all: calculator tester

objDir:= obj/
srcDir:= src/

-include config.local

CXX ?= g++
cf := -Werror -Wall -Wextra -Wuninitialized --std=c++0x -g0 -O3 -fPIC -I./src -I./libs/gtest/include
lf := -g0 -O3 --std=c++0x -I./src -I./libs/gtest/include

build_dir = @mkdir -p $(dir $@)

src_files :=	Environment.o		\
				Expression.o		\
				ExpressionParser.o	\
				Function.o			\
				ScriptLanguage.o
obj_files := $(addprefix $(objDir),$(src_files))

calculator: $(obj_files) samples/calculator.cpp
	$(CXX) -o $@ samples/calculator.cpp $(obj_files) $(lf)

tester: libs/gtest $(obj_files) tests/tester.cpp
	$(CXX) -o $@ tests/tester.cpp $(lf) $(obj_files) libs/gtest/libgtest.a -pthread

$(objDir)%o: $(srcDir)%cpp $(srcDir)%hpp
	$(build_dir)
	$(CXX) -c -o $@ $< $(cf)

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
