@echo off
echo "building exe file"
gcc -Iinclude -c src/rps.c
gcc -Iinclude -c src/darray.c
gcc -Iinclude -c src/ui.c
gcc -Iinclude -c src/art.c
gcc rps.o darray.o ui.o art.o -o rps.exe 
echo "removing object files"
del rps.o darray.o ui.o art.o
echo "build successfull"
pause