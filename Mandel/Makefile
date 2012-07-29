
SRC_FILES = $(notdir $(wildcard src/*.cpp))
HPP_FILES =  $(wildcard src/*.hpp)
FILE_BASES = $(basename $(SRC_FILES))
OBJ_FILES = $(addsuffix .o,$(addprefix obj/,$(notdir $(FILE_BASES))))
UNAME = uname
CXX = g++ -g  -O3 -Iobj/gmp/include/ -Iobj/png/include/ -Ilibs/json/
LINK = $(CXX) -Wl,-no_pie -lpthread
LINK_AFTER = -lpthread

ifeq ($(UNAME), Linux)
LINK = $(LINK) -static -shared-libgcc
endif

# Final target
mandel: obj/libgmp.a obj/libpng.a obj/main.o
	$(LINK) ${HPP_OBJ_FILES} obj/main.o obj/libgmp.a obj/libpng.a -lz -o mandel $(LINK_AFTER)

reduce: obj/libgmp.a obj/libpng.a obj/reduce.o
	$(LINK) ${HPP_OBJ_FILES} obj/reduce.o obj/libgmp.a obj/libpng.a -lz -o reduce 

2bw: obj/libgmp.a obj/libpng.a obj/blackwhite.o
	$(LINK) ${HPP_OBJ_FILES} obj/blackwhite.o obj/libgmp.a obj/libpng.a -lz -o 2bw 

mapper/pp: obj/libgmp.a obj/libpng.a obj/postprocess.o obj/libjson.la 
	$(LINK) ${HPP_OBJ_FILES} obj/postprocess.o obj/libgmp.a obj/libpng.a obj/libjson.la -lz -o mapper/pp

mapper/info: obj/libgmp.a obj/libpng.a obj/info.o 
	$(LINK) ${HPP_OBJ_FILES} obj/info.o obj/libgmp.a obj/libpng.a -lz -o mapper/info


pp: mapper/pp

# 1. Make dirs
libs obj bin:
	mkdir $@

# 2. Download libs
obj/gmp.tbz: | obj
	curl ftp://ftp.gmplib.org/pub/gmp-5.0.5/gmp-5.0.5.tar.bz2 > obj/gmp.tbz

obj/png.tgz: | obj
	curl ftp://ftp.simplesystems.org/pub/libpng/png/src/libpng-1.5.12.tar.gz > obj/png.tgz

# 3. Unpack libs
libs/gmp/configure: obj/gmp.tbz
	[ -d libs/gmp ] || mkdir -p libs/gmp
	tar mxf obj/gmp.tbz -C libs/gmp --strip-components 1


libs/png/configure: obj/png.tgz
	[ -d libs/png ] || mkdir -p libs/png
	tar mxf obj/png.tgz -C libs/png --strip-components 1


# 4. Configure libs

LIBGMP_BUILDDIR = $(abspath obj/gmp)
libs/gmp/Makefile: libs/gmp/configure
	cd libs/gmp && ./configure --enable-static=YES --enable-shared=NO --enable-cxx --prefix=$(LIBGMP_BUILDDIR)

LIBPNG_BUILDDIR = $(abspath obj/png)
libs/png/Makefile: libs/png/configure
	cd libs/png && ./configure --enable-static=YES --enable-shared=NO --prefix=$(LIBPNG_BUILDDIR)

# 5. Build libs
obj/libgmp.a: libs/gmp/Makefile
	cd libs/gmp && make install
	cp obj/gmp/lib/libgmp.a obj/libgmp.a

obj/libpng.a: libs/png/Makefile
	cd libs/png && make install
	cp obj/png/lib/libpng.a obj/libpng.a

obj/libjson.la: libs/json/Makefile
	cd libs/json && make
	cp libs/json/bin/libJSONpp.la obj/libjson.la

${OBJ_FILES} : ${HPP_FILES}

# 6. Compile source
${OBJ_FILES} : obj/%.o : src/%.cpp
	$(CXX) -c $< -o $@


# Run tests if needed?
check: check_gmp

check_gmp:
	cd libs/gmp && make check

# Cleanup rules
clean:
	- rm mandel
	- rm obj/*.o

distclean: clean
	- rm -rf obj
	- rm -rf libs

