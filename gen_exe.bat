@echo off
echo "building exe file"
gcc -g -c rps.c
gcc -g -c darray.c
gcc -g -c ui.c
gcc -g -c art.c
gcc -g rps.o darray.o ui.o art.o -o rps.exe 
echo "removing object files"
del rps.o darray.o ui.o art.o
echo "build successfull"
pause