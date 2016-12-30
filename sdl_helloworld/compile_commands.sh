
# 1) compiling
# "$ make install" puts header fildes into /usr/lib/include/SDL2
# so this include works
clang -c src/helloworld.c
# otherwise, direct include from build is:
clang -c -ISDL2-2.0.5/include/ src/helloworld.c

# 2) linking
# "$ make install" puts the main library into /usr/local/lib/libSDL2*
# so this linking works
clang -o out/helloworld -lSDL2 src/helloworld.c
