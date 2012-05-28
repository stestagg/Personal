
obj/gmp.a : libs/gmp
	#todo: Make
	echo make


libs obj:
	mkdir $<


obj:
	mkdir obj


libs/gmp: libs obj/gmp.tbz
	mkdir libs/gmp
	tar xf obj/gmp.tbz -C libs/

obj/gmp.tbz: obj