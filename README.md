# Dual-Core-RISCV-Simulator
Developing a  simulator on the lines of Ripes. The simulator will have dual cores and will be able to simulate a multi-core environment
## Meeting-9(Date: 25/2/24)
- added li and la instructions
- made some changes in lw and sw , added comment lines , empty lines skipping. still some problem with .data segment is there
- 
## Meeting-8(Data: 24/2/24)
- implemented instruction execution for : jal,jalr,beq,bne,blt,bge,lw,sw,addi,add,sub,j
- Fixed somr bugs in execution of instructions and storing strings
- implemented dual core funtionality(running both files at same time)
- Memory issues fixed

## Meeting-7 (Date: 22/2/24)
- added .data and .text cases 
- added all cases in .data
- implemented layout to run functions and able to update registers

## Meeting-6 (Date: 20/2/24)
- added .data and .text parts in test file and checked functionality
- added sb lw sw lwu lbu lb in gen erate machine code function
- handeled generate machine code with more instructions properly
- wrote executeinstruction function in sim.cpp
- added label cases instructions

## Meeting-5 (Date: 19/2/24)
- reading and writing instructions to memory is partially done.
- added a bit for label
- fixed few bugs in simulator.cpp
- added test file 

## Meeting-4 (Date: 18/2/24)
- Added memory.cpp functions and instruction processing in simulator.cpp

## Meeting-3 (Date: 17/2/24)
- Updated insttostring and regtostring and divided instructions in simulator.hpp
- Updated memory allocations in memory.hpp

## Meeting-2 (Date: 16/2/24)
- wrote code for structure of memory and registers.

## Meeting-1 (Date: 15/2/24)
- Discussed the structure of our simulator and finalized the programming language.
- **Language:** C++
- Created folders and files in accordance with the decided structure.
