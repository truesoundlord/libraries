LDFLAGS=-g

uep_wide.o: 	uep_wide.c uep_wide.h setElementEx.o getElementEx.o getPIDByName.c
setElementEx.o: setElementEx.c
getElementEx.o: getElementEx.c
getPIDByName.o: getPIDByName.c

sharedlibrary: 	uep_wide.c uep_wide.h setElementEx.c getElementEx.c
		@echo "Librairie partagée compilée..."
		@echo "...installation au niveau du système d'exploitation !!"
		gcc -c -O3 -fPIC uep_wide.c -g -o libuepwide.o
		@echo "...compilation des add-ons..."
		gcc -c -O3 -fPIC setElementEx.c -g -o setElementEx.o
		gcc -c -O3 -fPIC getElementEx.c -g -o getElementEx.o
		gcc -c -O3 -fPIC getPIDByName.c -g -o getPIDByName.o
		@echo "...compilation de la librairie..."
		gcc -shared -Wl,-soname,libuepwide.so libuepwide.o setElementEx.o getElementEx.o getPIDByName.o -lpthread -lm -o libuepwide.so.1.0.1
		@cp -v libuepwide.so.1.0.1 /usr/lib64
		@chmod 0755 /usr/lib64/libuepwide.so.1.0.1
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
	
	


