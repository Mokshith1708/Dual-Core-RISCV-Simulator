@echo off
cd Pipelined_withoutcache
g++ -o a.exe simulator.cpp

echo 1> input.txt
echo 0>> input.txt
.\a.exe < input.txt

del input.txt