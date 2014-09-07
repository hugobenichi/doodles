#!/bin/bash


gcc -c -g emb.c -I/usr/include/python3.2mu/ -I/usr/include/ -std=c99
gcc -o py_from_c emb.o -lpython3.2 -lm -L/usr/lib/python3.2/config

config_env_var="-Xlinker -E -Wl,-O1 -Wl,-Bsymbolic-functions -Wl,-Bsymbolic"

t="-Wl,-E -Wl,-O1 -Wl,-Bsymbolic-functions -Wl,-Bsymbolic"

# you can get this linker line with the programs:
# import distutils.sysconfig
# distutils.sysconfig.get_config_var('LINKFORSHARED')


#gcc -c pycode.c -I/usr/include/python3.2mu/ -std=c99 -fPIC
#gcc -shared -o libpycode.so pycode.o -Wl,-E -Wl,-O1 -Wl,-Bsymbolic-functions -Wl,-Bsymbolic -lpython3.2 -lm -L/usr/lib/python3.2/config
#gcc -shared -o libpycode.so pycode.o /usr/lib/python3.2/config/libpython3.2mu.a -lm

#gcc -o pycode_exec pycode.o -lpython3.2 -lm -L/usr/lib/python3.2/config
