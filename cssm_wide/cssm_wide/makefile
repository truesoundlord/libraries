LDFLAGS=-g

cssm_wide.o: 	cssm_wide.c cssm_wide.h setElementEx.o getElementEx.o getPIDByName.c
setElementEx.o: setElementEx.c
getElementEx.o: getElementEx.c
getPIDByName.o: getPIDByName.c

#cssmtc:		cssm_wide.c cssm_wide.h 
#		gcc cssm_wide.c -lpthread -lrt -c

sharedlibrary: 	cssm_wide.c cssm_wide.h setElementEx.c getElementEx.c
		@echo "Librairie partagée compilée..."
		@echo "...installation au niveau du système d'exploitation !!"
		gcc -c -fPIC -O3 cssm_wide.c -o libcssmwide.o
		@echo "...compilation des add-ons..."
		gcc -c -fPIC -O3 setElementEx.c -o setElementEx.o
		gcc -c -fPIC -O3 getElementEx.c -o getElementEx.o
		gcc -c -fPIC -O3 getPIDByName.c -o getPIDByName.o
		@echo "...compilation de la librairie..."
		gcc -shared -Wl,-soname,libcssmwide.so libcssmwide.o setElementEx.o getElementEx.o getPIDByName.o -lpthread -lm -o libcssmwide.so.1.0.1
		@cp -v libcssmwide.so.1.0.1 /usr/lib64
		@chmod 0755 /usr/lib64/libcssmwide.so.1.0.1
		@cd /usr/lib64
		@echo "...linking..."
		@ldconfig
		@ldconfig -p | grep cssm
		@echo "TERMINE"
		
#nocssmtc:	cssm_wide.c cssm_wide.h
#		gcc -c cssm_wide.c  -lpthread -o cssm_wide_nocssmtc.o
#		@mkdir -p /exemples/Librairie/cssm_wide/nocssmtc
#		@mv cssm_wide_nocssmtc.o nocssmtc
#		@echo "done"

#clean:
#	@echo "Removing all .o files"
#	@rm *.o

nettoyage:
	@echo "Removing the files with trailing ~"
	@rm *~

backup: 
	@echo "Backing up..."
	@tar -cf backup.tar *.c *.h makefile
	@gzip -9 backup.tar
	
	


