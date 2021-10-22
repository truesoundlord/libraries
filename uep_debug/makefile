CPPFLAGS=-D__LINUX__
uepdebugtool.o: uepdebugtool.c uepdebugtool.h

sharedlibrary:	uepdebugtool.c uepdebugtool.h
		gcc -c -g -fPIC -O uepdebugtool.c -D__LINUX__ -o libuepdebug.o
		gcc -shared -Wl,-soname,libuepdebug.so libuepdebug.o -o libuepdebug.so.1.0.1
		cp -v libuepdebug.so.1.0.1 /usr/lib64
		chmod 0755 /usr/lib64/libuepdebug.so.1.0.1
		cd /usr/lib64
		ldconfig
		ldconfig -p | grep uepdebug
								
