# Harriet Script Language
# Copyright (c) 2012, 2013 Alexander van Renen (alexandervanrenen@gmail.com)
# See the file LICENSE.txt for copying permission.
######################################################################

all: calculator

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

$(objDir)%.o: %.cpp
	$(build_dir)
	$(CXX) -MD -c -o $@ $< $(cf)
	@cp $(objDir)$*.d $(objDir)$*.P; \
		sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
			-e '/^$$/ d' -e 's/$$/ :/' < $(objDir)$*.d >> $(objDir)$*.P; \
		rm -f $(objDir)$*.d

-include $(objDir)*.P
-include $(objDir)*/*.P

clean:
	rm $(objDir) -rf
	find . -name "tester" -type f -delete
	find . -name "calculator" -type f -delete
