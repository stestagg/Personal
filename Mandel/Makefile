
SRC_FILES = $(notdir $(wildcard src/*.cpp))
CXX_FILES = $(wildcard src/*.cxx)
HPP_FILES =  $(wildcard src/*.hpp)
FILE_BASES = $(basename $(SRC_FILES))
OBJ_FILES = $(addsuffix .o,$(addprefix obj/,$(notdir $(FILE_BASES))))
UNAME = $(shell uname)
CXX = g++ -O3 -g  -Iobj/gmp/include/ -Iobj/png/include/ -Ijson/
LINK = $(CXX) 
LINK_AFTER = -lpthread

ifeq ($(UNAME), Linux)
LINK += -static -shared-libgcc
endif

# Final target
all: bin/merge_g bin/mandel bin/scale_rgb bin/scale_g bin/2bw bin/pp bin/info bin/zoom_g bin/zoom_rgb bin/merge_rgb

bin/recolor: obj/libpng.a obj/recolor.o bin
	$(LINK) ${HPP_OBJ_FILES} obj/recolor.o obj/libgmp.a obj/libpng.a -lz -o bin/recolor $(LINK_AFTER)


bin/merge_g: obj/libgmp.a obj/libpng.a obj/merge_g.o bin
	$(LINK) ${HPP_OBJ_FILES} obj/merge_g.o obj/libgmp.a obj/libpng.a -lz -o bin/merge_g $(LINK_AFTER)

bin/merge_rgb: obj/libgmp.a obj/libpng.a obj/merge_rgb.o bin
	$(LINK) ${HPP_OBJ_FILES} obj/merge_rgb.o obj/libgmp.a obj/libpng.a -lz -o bin/merge_rgb $(LINK_AFTER)

bin/scale_rgb: obj/libgmp.a obj/libpng.a obj/scale_rgb.o bin
	$(LINK) ${HPP_OBJ_FILES} obj/scale_rgb.o obj/libgmp.a obj/libpng.a -lz -o bin/scale_rgb $(LINK_AFTER)

bin/scale_g: obj/libgmp.a obj/libpng.a obj/scale_g.o bin
	$(LINK) ${HPP_OBJ_FILES} obj/scale_g.o obj/libgmp.a obj/libpng.a -lz -o bin/scale_g $(LINK_AFTER)

bin/zoom_rgb: obj/libgmp.a obj/libpng.a obj/zoom_rgb.o bin
	$(LINK) ${HPP_OBJ_FILES} obj/zoom_rgb.o obj/libgmp.a obj/libpng.a -lz -o bin/zoom_rgb $(LINK_AFTER)

bin/zoom_g: obj/libgmp.a obj/libpng.a obj/zoom_g.o bin
	$(LINK) ${HPP_OBJ_FILES} obj/zoom_g.o obj/libgmp.a obj/libpng.a -lz -o bin/zoom_g $(LINK_AFTER)


bin/mandel: obj/libgmp.a obj/libpng.a obj/main.o bin
	$(LINK) ${HPP_OBJ_FILES} obj/main.o obj/libgmp.a obj/libpng.a -lz -o bin/mandel $(LINK_AFTER)

bin/2bw: obj/libgmp.a obj/libpng.a obj/blackwhite.o bin
	$(LINK) ${HPP_OBJ_FILES} obj/blackwhite.o obj/libgmp.a obj/libpng.a -lz -o bin/2bw 

bin/pp: obj/libgmp.a obj/libpng.a obj/postprocess.o obj/libjson.la bin 
	$(LINK) ${HPP_OBJ_FILES} obj/postprocess.o obj/libgmp.a obj/libpng.a obj/libjson.la -lz -o bin/pp

bin/info: obj/libgmp.a obj/libpng.a obj/info.o bin
	$(LINK) ${HPP_OBJ_FILES} obj/info.o obj/libgmp.a obj/libpng.a -lz -o bin/info


# 1. Make dirs
libs obj bin:
	mkdir $@

# 2. Download libs
obj/gmp.tbz: | obj
	curl https://gmplib.org/download/gmp/gmp-6.1.0.tar.bz2 > obj/gmp.tbz

obj/png.tgz: | obj
	curl ftp://ftp.simplesystems.org/pub/libpng/png/src/libpng16/libpng-1.6.22.tar.gz > obj/png.tgz

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

obj/libjson.la: json/Makefile
	cd json && make
	cp json/bin/libJSONpp.la obj/libjson.la

${OBJ_FILES} : ${HPP_FILES} ${CXX_FILES}

# 6. Compile source
${OBJ_FILES} : obj/%.o : src/%.cpp obj/libjson.la
	$(CXX) -c $< -o $@


# Run tests if needed?
check: check_gmp

check_gmp:
	cd libs/gmp && make check

# Cleanup rules
clean:
	- rm -rf bin
	- rm obj/*.o

distclean: clean
	- rm -rf obj
	- rm -rf libs

