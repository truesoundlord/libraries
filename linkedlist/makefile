linkedlist.o:	linkedlist.c linkedlist.h

sharedlibrary:	linkedlist.c linkedlist.h
		gcc -c -g -fPIC -O linkedlist.c -o liblinkedlist.o
		gcc -shared -Wl,-soname,liblinkedlist.so liblinkedlist.o -o liblinkedlist.so.1.0.1
		cp -v liblinkedlist.so.1.0.1 /usr/lib64
		chmod 0755 /usr/lib64/liblinkedlist.so.1.0.1
		cd /usr/lib64
		ldconfig
		ldconfig -p | grep linkedlist
								
