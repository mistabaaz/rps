
echo "building exe file"
gcc -g -Iinclude -c src/rps.c
gcc -g -Iinclude -c src/darray.c
gcc -g -Iinclude -c src/ui.c
gcc -g -Iinclude -c src/art.c
gcc -g rps.o darray.o ui.o art.o -o rps.exe
echo "removing object files"
rm rps.o darray.o ui.o art.o
echo "build successfull"
