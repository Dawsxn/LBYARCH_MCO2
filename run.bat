nasm -f win64 asm_coord.asm -o asm_coord.obj
gcc -c c_coord.c -o c_coord.obj -m64
gcc -c main.c -o main.obj -m64
gcc main.obj c_coord.obj asm_coord.obj -o coord_test.exe -m64
coord_test.exe
