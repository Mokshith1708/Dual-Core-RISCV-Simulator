@echo off

g++ -o a.exe simulator.cpp

echo 0> input.txt
echo 0>> input.txt
.\a.exe < input.txt

del input.txt