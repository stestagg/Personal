
SRC_FILES = $(notdir $(wildcard src/*.cpp))
FILE_BASES = $(basename $(SRC_FILES))
OBJ_FILES = $(addsuffix .o,$(addprefix obj/,$(notdir $(FILE_BASES))))
CXX = g++ -O3 -Iobj/gmp/include/ -Iobj/png/include/ -pg
LINK = $(CXX) 

# Final target
mandel: obj/libgmp.a obj/libpng.a ${OBJ_FILES}
	$(LINK) ${OBJ_FILES} obj/libgmp.a obj/libpng.a -lz -o mandel


# 1. Make dirs
libs obj bin:
	mkdir $@

# 2. Download libs
obj/gmp.tbz: | obj
	curl ftp://ftp.gmplib.org/pub/gmp-5.0.5/gmp-5.0.5.tar.bz2 > obj/gmp.tbz

obj/png.tgz: | obj
	curl ftp://ftp.simplesystems.org/pub/libpng/png/src/libpng-1.5.10.tar.gz > obj/png.tgz

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



#
