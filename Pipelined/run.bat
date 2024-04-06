@echo off

rem Compile the C++ program
g++ -o a.exe simulator.cpp

rem Run the compiled program with input from file
echo 0> input.txt
echo 0>> input.txt
.\a.exe < input.txt

rem Delete the temporary input file
del input.txt