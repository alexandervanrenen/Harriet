# Harriet Script Language
# Copyright (c) 2012 Alexander van Renen (alexandervanrenen@gmail.com)
# See the file LICENSE.txt for copying permission.
######################################################################

# ===============================================
#   definitions
# ===============================================

all: sample

objDir:= obj/
srcDir:= src/

cf := -Werror -Wall -Wextra -Wuninitialized --std=c++0x -g0 -O3 -I./include
lf := -g0 -O3 --std=c++0x

comp?=  g++
comp:= $(comp)

build_dir=@mkdir -p $(dir $@)

# ===============================================
#   files
# ===============================================

src_files :=	Environment.o		\
		Expression.o		\
		ExpressionParser.o	\
		Function.o		\
		ScriptLanguage.o

obj_files := $(addprefix $(objDir),$(src_files))

# ===============================================
#   build sample
# ===============================================

sample: $(obj_files) sample.cpp
	$(comp) -o $@ sample.cpp $(obj_files) $(lf)


# ===============================================
#   build individual files
# ===============================================

$(objDir)%o: $(srcDir)%cpp $(srcDir)%hpp
	$(build_dir)
	$(comp) -c -o $@ $< $(cf)

# ====================================
#       clean up command
# ====================================

clean:
	rm $(objDir) -rf && find . -maxdepth 1 -name 'hyper_simple_client' -delete
