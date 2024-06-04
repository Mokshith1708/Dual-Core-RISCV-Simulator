## Dual-Core-RISCV-Simulator

- Developed a  simulator on the lines of Ripes. The simulator will have two cores and will be able to simulate a multi-core environment.
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
