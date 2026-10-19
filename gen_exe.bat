@echo off
echo "building exe file"
gcc -c rps.c
gcc -c darray.c
gcc -c ui.c
gcc -c art.c
gcc rps.o darray.o ui.o art.o -o rps.exe 
echo "removing object files"
del rps.o darray.o ui.o art.o
echo "build successfull"
pause