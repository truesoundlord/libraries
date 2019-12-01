gfxContainer: 
		@echo "...installation au niveau du système d'exploitation !!"
		@echo "Compilation des fichiers source..."
		gcc -c -g -fPIC -O3 gfxContainer.c `pkg-config linkedlist --cflags` `pkg-config linkedlist --libs` -o libGFXContainer.o
		gcc -c -g -fPIC -O3 gfxFunctions.c `pkg-config linkedlist --cflags` `pkg-config linkedlist --libs` -o gfxfcts.o
		gcc -c -g -fPIC -O3 gfxErrorHandling.c `pkg-config linkedlist --cflags` `pkg-config linkedlist --libs` -o gfxerror.o		
		@echo "Compilation de la librairie partagée..."
		gcc -shared -Wl,-soname,libGFXContainer.so libGFXContainer.o gfxfcts.o gfxerror.o -lSDL2 -lSDL2_ttf -lpthread -lm -o libGFXContainer.so.1.0.1
		@echo "Invocation des outils systèmes..."
		@cp -v libGFXContainer.so.1.0.1 /usr/lib64
		@chmod 0755 /usr/lib64/libGFXContainer.so.1.0.1
		@cd /usr/lib64
		@ldconfig
		@ldconfig -p | grep GFX
		@echo "TERMINE"
		
nettoyage:
	@echo "Removing the files with trailing ~"
	@rm *~

backup: 
	@echo "Backing up..."
	@tar -cf backup.tar *.c *.h makefile
	@gzip -9 backup.tar
	
	


