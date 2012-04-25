## libJSONpp makefile.


CXXFLAGS = -O2 -DJSON_POSIX
CXX = g++ $(CXXFLAGS)
LD = ld
#-Wall

FILES = value parser_errors parser_UNI parser_UTF8 convert types

.PHONY : libJSONpp clean check static shared

libJSONpp:  shared
	make check

static : bin/libJSONpp.la

shared : bin/libJSONpp.so
	
bin/libJSONpp.so : obj/ bin/ $(addsuffix .o,$(addprefix obj/,${FILES}))
	$(CXX) -shared -fPIC -Wl,-soname,libJSONpp.so.1 -o bin/libJSONpp.so $(addsuffix .o,$(addprefix obj/,${FILES})) 

bin/libJSONpp.la : obj/ bin/ $(addsuffix .o,$(addprefix obj/,${FILES}))
	ar rcs bin/libJSONpp.la $(addsuffix .o,$(addprefix obj/,${FILES}))

$(addsuffix .o,$(addprefix obj/,${FILES})) : obj/%.o : libJSONpp/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

test : bin/libJSONpp.la 
	cp tests/test.json bin/
	cp tests/test.json.UTF32 bin/
	$(CXX) $(CXXFLAGS) tests/*.cpp bin/libJSONpp.la -o bin/test

check : test
	chmod +x runtest.sh
	./runtest.sh

obj/ : 
	mkdir obj

bin/ :
	mkdir bin

clean :
	-rm -r obj/
	-rm -r bin/

