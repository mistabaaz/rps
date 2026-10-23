
echo "building exe file"
gcc -Iinclude -c src/rps.c
gcc -Iinclude -c src/darray.c
gcc -Iinclude -c src/ui.c
gcc -Iinclude -c src/art.c
gcc rps.o darray.o ui.o art.o -o rps
echo "removing object files"
rm rps.o darray.o ui.o art.o
echo "build successfull"
