compile:
	gcc -o deb-package/usr/bin/arg_counter source/hello.c
	gcc -fPIC --shared -o deb-package/usr/lib/hello_shared.so source/shared-lib.c

build:
	dpkg-deb --build deb-package