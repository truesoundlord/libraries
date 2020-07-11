LDFLAGS=-g

uep_wide.o: 	uep_wide.c uep_wide.h setElementEx.o getElementEx.o getPIDByName.c
setElementEx.o: setElementEx.c
getElementEx.o: getElementEx.c
getPIDByName.o: getPIDByName.c

sharedlibrary: 	uep_wide.c uep_wide.h setElementEx.c getElementEx.c
		@echo "Librairie partagée compilée..."
		@echo "...installation au niveau du système d'exploitation !!"
		gcc -c -fPIC -O3 uep_wide.c -o libuepwide.o
		@echo "...compilation des add-ons..."
		gcc -c -fPIC -O3 setElementEx.c -o setElementEx.o
		gcc -c -fPIC -O3 getElementEx.c -o getElementEx.o
		gcc -c -fPIC -O3 getPIDByName.c -o getPIDByName.o
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
	
	


