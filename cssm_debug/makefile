CPPFLAGS=-D__LINUX__
cssmdebugtool.o: cssmdebugtool.c cssmdebugtool.h

sharedlibrary:	cssmdebugtool.c cssmdebugtool.h
		gcc -c -g -fPIC -O cssmdebugtool.c -D__LINUX__ -o libcssmdebug.o
		gcc -shared -Wl,-soname,libcssmdebug.so libcssmdebug.o -o libcssmdebug.so.1.0.1
		cp -v libcssmdebug.so.1.0.1 /usr/lib64
		chmod 0755 /usr/lib64/libcssmdebug.so.1.0.1
		cd /usr/lib64
		ldconfig
		ldconfig -p | grep cssmdebug
								
