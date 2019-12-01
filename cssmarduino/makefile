LDFLAGS=-g

libcssmarduino.o: 	libcssmarduino.c libcssmarduino.h 

sharedlibrary: 	libcssmarduino.c libcssmarduino.h 
		@echo "Librairie partagée compilée..."
		@echo "...installation au niveau du système d'exploitation !!"
		gcc -c -fPIC -O3 libcssmarduino.c -o libcssmwide.o
		@echo "...compilation de la librairie..."
		gcc -shared -Wl,-soname,libcssmarduino.so libcssmarduino.o -lpthread -lm -o libcssmarduino.so.0.0.1
		@cp -v libcssmarduino.so.0.0.1 /usr/lib64
		@chmod 0755 /usr/lib64/libcssmarduino.so.0.0.1
		@cd /usr/lib64
		@echo "...linking..."
		@ldconfig
		@ldconfig -p | grep cssm
		@echo "TERMINE"

nettoyage:
	@echo "Removing the files with trailing ~"
	@rm *~

backup: 
	@echo "Backing up..."
	@tar -cf backup.tar *.c *.h makefile
	@gzip -9 backup.tar
	
	


