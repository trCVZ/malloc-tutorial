gcc -Wall -Werror -Wpedantic -o main.out main.c

if [ $? -eq 0 ]; then
    ./main.out
else
    echo "Compilation failed"
fi