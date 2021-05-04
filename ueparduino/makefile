LDFLAGS=-g -luep_wide -I/librairies/uep_wide

libueparduino.o: 	libueparduino.c libueparduino.h 

sharedlibrary: 	libueparduino.c libueparduino.h 
		@echo "Librairie partagée compilée..."
		@echo "...installation au niveau du système d'exploitation !!"
		gcc -c -fPIC -O3 libueparduino.c -luep_wide -I/librairies/uep_wide -o libueparduino.o
		@echo "...compilation de la librairie..."
		gcc -shared -Wl,-soname,libueparduino.so libueparduino.o -lpthread -lm  -o libueparduino.so.0.0.1
		@cp -v libueparduino.so.0.0.1 /usr/lib64
		@chmod 0755 /usr/lib64/libueparduino.so.0.0.1
		@cd /usr/lib64
		@echo "...linking..."
		@ldconfig
		@ldconfig -p | grep uep
		@echo "TERMINE"

nettoyage:
	@echo "Removing the files with trailing ~"
	@rm *~

backup: 
	@echo "Backing up..."
	@tar -cf backup.tar *.c *.h makefile
	@gzip -9 backup.tar
	
	


