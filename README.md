## Dual-Core-RISCV-Simulator

- Developing a  simulator on the lines of Ripes. The simulator will have two cores and will be able to simulate a multi-core environment.
- This is a Dual Core-RISCV-Simulator wchich consists of 4 parts. 
- The first part a 5 stage processor was created.
- In the second Part pipelining is introduced with data forwarding and stall management.
- In the third Part of the project Cache with LRU and Random replacement policy was added to the simulator.
- In the fourth Part of the project Graphical User Interface and 1 bit predictor were added to the simulator.

### How to run

- For non pipe-lined version

  ```bash
  cd Non_pipelined
  g++ simulator.cpp
  ./a
  cd ..
  ```

- For pipe-lined version without cache

  ```bash
  cd pipelined_withoutcache
  g++ simulator.cpp
  ./a
  cd ..
  ```

- For pipe-lined version with cache

  ```bash
  cd pipelined_withcache
  g++ simulator.cpp
  ./a
  cd ..
  ```

- For pipe-lined version with cache and GUI

    ```bash
  python app.py
  Note: Click on the link to open the Webpage.
  - In the GUI before checking the register or memory or output values Update core 1 , update core 2 and compile simulator should be clicked.
  - Also we need to wait until this message is displayed:
   " Simulator compiled and executed successfully. "

  ```
 
- After the Simulator is compiled we can check the register, memory and output values by clicking on the respective buttons in the GUI.
- At the end of execution, the simulator outputs the number of stalls, cache miss rate, and the IPC and CPI in `OUTPUTS.txt` file in `data_files/output` folder.
- Registers and memory values were printed in `terminal1.txt` and `terminal2.txt` for core1 and core 2 respectively in `data_files/output` folder.
- ecall outputs and all 5 arrays in pipelined will be printed in `console1.txt` and `console2.txt` for core1 and core2 respectively in `data_files/output` folder.
- for parameters in cache can change in parameters.txt file in folder pipelined_withcache.

### implemented functions

  ```assembly=
  add
  sub
  mul
  addi
  jal
  jalr
  beq
  bne
  blt
  bge
  j
  lw
  sw
  la
  li
  ecall
  ```
# - Phase 4
### (Date: 11/5/2024)
- 1 bit branch predictor is added.
- Code is cleaned up.
- Some changes in the GUI were made.
- All things were checked and updated for the last time.
### (Date: 10/5/2024)
- There was an error with the memory values of core-2 , it was always showing values of memory-1. So it was rectified.
- Work for predictor is started.

### (Date: 09/5/2024)
- Made a small correction in Cache.
- Started to work to create GUI.
- Flask was used to create GUI.
- A new folder called templates was added which contains the code for all the four pages of GUI.
- App.cpp was created which is the final file to render everthing into a webpage.

# - Phase 3
  - We changed the way we implemented the Pipelined stage.
  - Mistakes in phase 2 were rectified.
  - The simulator now includes a cache that is shared between both cores
  - LRU and random policies were used for cache and it is user based.
  - Parameters such as cache size, block size, associativity, access latency of the cache, and main memory access time are provided in a separate file as input.
  
### Meeting-19(Date: 10/4/24)
- Made some minor changes in cache.
- Added file in which inputs can be given.
- Readme is updated.

### Meeting-18(Date: 9/4/24)
- In the previous code offset was not implemented properly.
- It was coorected in this code.
- Random replacement policy is implemented.
  
### Meeting-17(Date: 8/4/24)
- Started to implement the cache
- The two replacement policies we used were LRU and random.
- Made changes for memory class to combine cache.
 - LRU is implemented

### Meeting-16(Date: 7/4/24)
- We removed the idea of using temporary registers for managing stalls.
- We decided to push dummy instructions in place of stalls to make it match the actual pipeline.
- Made changes for dataforwarding
- Added code for latency

### Meeting-15(Date: 2/4/24)
- There was a problem with the stalls from phase-2.
- So we cahnged the approach of how to deal with the stalls.

### Meeting-14(Date: 1/4/24)
-Discussed about the implementation of cache and made some cahnges for pipeline of phase-2.


# Phase 2
### - Phase 2 lag

  - We implemented Clock cycles.But we were not getting the count exactly same as in the ripes simulator. Maybe our approach towards stalls should be changed. It is giving some near value but not the exact value.
  - In next phase we would first like to change the detection of stalls process.
  - few constraints
    - we didn't write implementaion for back slash n(\n) through ecall.
    - there must me always a space straight mext after a label
      - ex:
    ```code=assembly
            str1: .string "initial array : " #this is correct.
            #after "str1:" space must me added.
    ```
    - for now we cant explicitly give the address of a array or a string.
    - for now we kept same console for both cores.
- we allocated a memory of 8 kb.Out of which 4 kb is for instruction set and 4 kb is for memory blocks combined for both cores equally.

### Meeting-13(Date: 10/3/24)

- Enabled user choice Data forwarding.
- Added latencies for `add`,`mul`,`addi`,`sub`,`muli`.
- Added few more cases in possible hazard cases.
- Fixed several bugs.
- Seperated consoles for both cores.

### Meeting-12(Date: 9/3/24)

- Added RAW hazard detection.
- Discussed possible hazard cases.
- Pipelining with data forwarding is done. Made some changes in pipeline , to make instructionfetch predict what pc to take next

### Meeting-11(Date: 8/3/24)

- Discussed how to implement data forwarding and started implementing.
  
### Meeting-10(Date: 3/3/24)

- Created seperate folder for Pipelined
- Discussed how to start doing pipelining.
- seperated execute function in Non-Pipelined into 5 steps as required in pipeline.
- Implemented pipelining by adding all the five stages seperately. We did it using Backward execution of pieline stages.
- We created seperate vectors for both cores for all 5 stages.
   
# Phase 1
  
### Meeting-9(Date: 25/2/24)

- added li and la instructions
- made some changes in lw and sw , added comment lines , empty lines skipping.
- Mking the ouput look prettier by creating new output files and appending the ouput to it.
- We appended outputs of program into new files terminal1.txt ,terminal2.txt and console.
  
### Meeting-8(Data: 24/2/24)

- Basically did most of the core class work.
- implemented instruction execution for : jal,jalr,beq,bne,blt,bge,lw,sw,addi,add,sub,j
- Fixed somr bugs in execution of instructions and storing strings
- implemented dual core funtionality(running both files at same time)
- Memory issues fixed

### Meeting-7 (Date: 22/2/24)

- added .data and .text cases
- added all cases in .data
- implemented layout to run functions and able to update registers

### Meeting-6 (Date: 20/2/24)

- added .data and .text parts in test file and checked functionality
- added sb lw sw lwu lbu lb in generate machine code function
- handeled generate machine code with more instructions properly
- wrote executeinstruction function in sim.cpp
- added label cases instructions

### Meeting-5 (Date: 19/2/24)

- reading and writing instructions to memory is partially done.
- added a bit of code for label extraction
- fixed few bugs in simulator.cpp
- added test file

### Meeting-4 (Date: 18/2/24)

- Added memory.cpp functions and instruction processing in simulator.cpp
- So we implemented the idea discussed on 17/2 and tried to encode instructions into numbers.

### Meeting-3 (Date: 17/2/24)

- Our major plan was to numerically encode all the instructions and place them in the memory.
- Then we took a decision to first partition the memroy and given half to each core.
- Also in the partitioned memory again we again made divided it and gave some part to store the numerically encoded strings.
- Also we thought to make the instruction_memory as 2D array with each row having 4 columns which are basically used to store all the details about instructions as decimal numbers.
- So we wrote another 2 functions in memory.cpp for reading instructions and writing instructions.
- We used enum to give numbers to all the registers and instructions.
- Updated insttostring and regtostring and divided instructions in simulator.hpp for numerical encoding

### Meeting-2 (Date: 16/2/24)

- wrote code for structure of memory and registers.
- Here we just created one memory which is basically an integer array
- We wrote the methods for read memory and write memory as memory array was private
- In the same way we did for registers

### Meeting-1 (Date: 15/2/24)

- Discussed the structure of our simulator and finalized the programming language.
- **Language:** C++
- Created folders and files in accordance with the decided structure.
- We planned to have different files/classes. We made classes for
  - Cores
  - memory
  - Simulator
  - registers
  - test.s file for input (This is loacted in data_files).
- For each class we wrote a hpp file and cpp file.
- hpp file mainly consisted the basic structure without implementation
- cpp file contained the implementations of the methods declared in hpp file.

### Note: Majority of the work was done together. So sometimes we only worked from one laptop and comitted from one git account.
