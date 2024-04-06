#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstdarg>
#include "simulator.hpp"
#include <sstream>
#include <cctype>
#include "memory.hpp"
#include "memory.cpp"
#include "cores.cpp"
#include "cores.hpp"
#include "registers.hpp"
#include "registers.cpp"

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::find_if_not;
using std::ifstream;
using std::invalid_argument;
using std::isspace;
using std::istringstream;
using std::map;
using std::ofstream;
using std::pair;
using std::replace;
using std::stoi;
using std::streambuf;
using std::string;
using std::vector;

using vs = vector<string>;
using vss = vector<vector<string>>;
using vi = vector<int>;

RISCV::Inst stringToInst(const string &s) // converting strings inputs to instructions
{
    static const map<string, RISCV::Inst> instMap = {
        {"jal", RISCV::jal},
        {"jalr", RISCV::jalr},
        {"beq", RISCV::beq},
        {"bne", RISCV::bne},
        {"blt", RISCV::blt},
        {"bge", RISCV::bge},
        {"bltu", RISCV::bltu},
        {"begu", RISCV::begu},
        {"lb", RISCV::lb},
        {"lw", RISCV::lw},
        {"sb", RISCV::sb},
        {"sw", RISCV::sw},
        {"addi", RISCV::addi},
        {"add", RISCV::add},
        {"sub", RISCV::sub},
        {"ecall", RISCV::ecall},
        {"lbu", RISCV::lbu},
        {"lwu", RISCV::lwu},
        {"j", RISCV::j},
        {"la", RISCV::la},
        {"li", RISCV::li},
        {"mul", RISCV::mul},
        {"muli", RISCV::muli},
    };

    auto it = instMap.find(s);
    if (it != instMap.end())
    {
        return it->second;
    }
    else
    {
        // handling unknown instruction
        return RISCV::lwu;
    }
}

// Function to convert a string to the corresponding RISCV::reg value
RISCV::reg stringToReg(const string &s)
{
    static const map<string, RISCV::reg> regMap = {
        {"x0", RISCV::x0},
        {"x1", RISCV::x1},
        {"x2", RISCV::x2},
        {"x3", RISCV::x3},
        {"x4", RISCV::x4},
        {"x5", RISCV::x5},
        {"x6", RISCV::x6},
        {"x7", RISCV::x7},
        {"x8", RISCV::x8},
        {"x9", RISCV::x9},
        {"x10", RISCV::x10},
        {"x11", RISCV::x11},
        {"x12", RISCV::x12},
        {"x13", RISCV::x13},
        {"x14", RISCV::x14},
        {"x15", RISCV::x15},
        {"x16", RISCV::x16},
        {"x17", RISCV::x17},
        {"x18", RISCV::x18},
        {"x19", RISCV::x19},
        {"x20", RISCV::x20},
        {"x21", RISCV::x21},
        {"x22", RISCV::x22},
        {"x23", RISCV::x23},
        {"x24", RISCV::x24},
        {"x25", RISCV::x25},
        {"x26", RISCV::x26},
        {"x27", RISCV::x27},
        {"x28", RISCV::x28},
        {"x29", RISCV::x29},
        {"x30", RISCV::x30},
        {"x31", RISCV::x31},
        {"zero", RISCV::x0},
        {"ra", RISCV::x1},
        {"sp", RISCV::x2},
        {"gp", RISCV::x3},
        {"tp", RISCV::x4},
        {"to", RISCV::x5},
        {"t1", RISCV::x6},
        {"t2", RISCV::x7},
        {"s0", RISCV::x8},
        {"s1", RISCV::x9},
        {"a0", RISCV::x10},
        {"a1", RISCV::x11},
        {"a2", RISCV::x12},
        {"a3", RISCV::x13},
        {"a4", RISCV::x14},
        {"a5", RISCV::x15},
        {"a6", RISCV::x16},
        {"a7", RISCV::x17},
        {"s2", RISCV::x18},
        {"s3", RISCV::x19},
        {"s4", RISCV::x20},
        {"s5", RISCV::x21},
        {"s6", RISCV::x22},
        {"s7", RISCV::x23},
        {"s8", RISCV::x24},
        {"s9", RISCV::x25},
        {"s10", RISCV::x26},
        {"s11", RISCV::x27},
        {"t3", RISCV::x28},
        {"t4", RISCV::x29},
        {"t5", RISCV::x30},
        {"t6", RISCV::x31},
    };

    auto it = regMap.find(s);
    if (it != regMap.end())
    {
        return it->second;
    }
    else
    {
        // Handle unknown register
        return RISCV::x31;
    }
}

vector<string> split_string(const string &line)
{
    istringstream iss(line);
    vector<string> words;

    string word;
    while (iss >> word)
    {
        // Remove leading and trailing whitespaces
        word.erase(0, word.find_first_not_of(" \t\n\r\f\v"));
        word.erase(word.find_last_not_of(" \t\n\r\f\v") + 1);

        // Split the word by comma if present
        size_t pos;
        while ((pos = word.find(',')) != string::npos)
        {
            if (pos > 0)
            {
                words.push_back(word.substr(0, pos));
            }
            word.erase(0, pos + 1);
        }
        if (!word.empty())
        {
            words.push_back(word);
        }
    }

    return words;
}

// First Pass for Collecting Labels and Addresses
void collectLabels(vs &lines, map<string, int> &labelMap, map<string, int> &dataSizes, pair<int, int> &p)
{
    int address = 0;
    bool inDataSection = false;
    for (string &line : lines)
    {
        if (line.empty())
        {
            address++;
            continue;
        }
        vs words = split_string(line);
        if (!words.empty() && words[0] == ".data") // identifying data section
        {
            inDataSection = true;
            p.first = address;
            address++;
            continue;
        }
        if (!words.empty() && words[0] == ".text") // identifying .text section
        {
            inDataSection = false;
            p.second = address;
            address++;
            continue;
        }
        if (inDataSection) // adding labels to labelmap
        {
            if (!words.empty() && words[0].back() == ':')
            {
                string label = words[0].substr(0, words[0].size() - 1);
                labelMap[label] = address;
                dataSizes[label] = words.size() - 2;
                // cout<<dataSizes[label]<<endl;
                address++;
            }

            // else if (!words.empty() && words[0] == ".text")
            // {
            //     // End of .data
            //     inDataSection = false;
            //     p.second = address;
            //     address++;
            // }
        }
        else
        {
            if (!words.empty() && words[0].back() == ':')
            {
                string label = words[0].substr(0, words[0].size() - 1);
                labelMap[label] = address;
                address++;
            }
            else
            {
                // Increment address for non-label lines
                address++;
            }
        }
    }
}

// Second Pass: Generate Machine Code
int generateMachineCode(vs &lines, map<string, int> &labelMap, memory &m, pair<int, int> &p, int core)
{
    int address = 0;
    int address_str = 0;
    int address_memory = 0;
    for (string &line : lines)
    {
        vs words = split_string(line);
        int encode[4] = {0, 0, 0, 0};
        if (line.empty() || line[0] == '#') // for empty lines and for comments in RiscV simulator
        {
            m.write_instruction(address, encode, core);
            address++;
            continue;
        }
        if (address >= p.first && address < p.second) // running from .data to .text in assembly code
        {
            if (words[0] == ".data")
            {
                m.write_instruction(address, encode, core);
                address++;
                continue;
            }
            if (!words.empty() && words[0].back() == ':')
            {
                if (words[1] == ".word") // identifying .word
                {
                    string label = words[0].substr(0, words[0].size() - 1);
                    encode[1] = address_memory;
                    encode[2] = address_memory + words.size() - 3;
                    encode[0] = -2;
                    for (int j = 2; j < words.size(); j++)
                    {
                        m.write_memory(address_memory, stoi(words[j]), core);
                        address_memory++;
                    }
                    m.write_instruction(address, encode, core);
                    address++;
                    continue;
                }
                else if (words[1] == ".string") // for storing strings in data section
                {
                    string str;
                    for (size_t j = 2; j < words.size(); ++j)
                    {
                        str += words[j];
                        if (j != words.size() - 1)
                            str += " ";
                    }
                    m.write_str(words[0].substr(0, words[0].size() - 1), str, address_str, 1);
                    encode[0] = -1;
                    encode[1] = address_str;
                    address_str++;
                    m.write_instruction(address, encode, core);
                    address++;
                    continue;
                }
            }
        }
        if (address >= p.second) // running from .text section
        {

            // Used to store instructions in numerical form.
            if (words[0] == ".text")
            {
                m.write_instruction(address, encode, core);
                address++;
                continue;
            }
            if (!words.empty() && words[0].back() == ':')
            {
                string label = words[0].substr(0, words[0].size() - 1);
                encode[0] = labelMap[label];
                encode[1] = -101;
                m.write_instruction(address, encode, core);
                address++;
                continue;
            }

            for (int i = 0; i < (int)words.size(); i++) // wrote all cases to store in instruction set
            {
                if (i == 0)
                {
                    // Handle instruction
                    switch (stringToInst(words[i]))
                    {
                    case RISCV::la:
                        encode[0] = stringToInst(words[i]);
                        encode[1] = stringToReg(words[i + 1]);
                        encode[2] = labelMap[words[i + 2]];
                        i += 3; // Skip label and operands
                        break;
                    case RISCV::li:
                        encode[0] = stringToInst(words[i]);
                        encode[1] = stringToReg(words[i + 1]);
                        encode[2] = stoi(words[i + 2]);
                        i += 3; // Skip label and operands
                        break;
                    case RISCV::ecall:
                        encode[0] = stringToInst(words[i]);
                        encode[1] = 111111;
                        i += 3;
                        break;
                    case RISCV::beq:
                    case RISCV::bne:
                    case RISCV::blt:
                    case RISCV::bge:
                    case RISCV::bltu:
                    case RISCV::begu:
                        // Label cases
                        encode[0] = stringToInst(words[i]);
                        encode[1] = stringToReg(words[i + 1]);
                        encode[2] = stringToReg(words[i + 2]);
                        if (labelMap.find(words[i + 3]) != labelMap.end())
                        {
                            encode[3] = labelMap[words[i + 3]] + 1;
                        }
                        else
                        {
                            throw invalid_argument("Label not found: " + words[i + 3]);
                        }
                        i += 3; // Skip label and operands
                        break;
                    case RISCV::j:
                        encode[0] = stringToInst(words[i]);

                        if (labelMap.find(words[i + 1]) != labelMap.end())
                        {
                            encode[1] = labelMap[words[i + 1]] + 1;
                        }
                        else
                        {
                            throw invalid_argument("Label not found: " + words[i + 1]);
                        }
                        // cout<<encode[0]<<" "<<encode[1]<<endl;
                        i += 3;
                        break;
                    case RISCV::jal:
                        encode[0] = stringToInst(words[i]);
                        encode[1] = stringToReg(words[i + 1]);
                        if (labelMap.find(words[i + 2]) != labelMap.end())
                        {
                            encode[2] = labelMap[words[i + 2]] + 1;
                        }
                        else
                        {
                            throw invalid_argument("Label not found: " + words[i + 2]);
                        }
                        i += 3; // Skip label and operands
                        break;
                    case RISCV::jalr:
                        // Label cases
                        encode[0] = stringToInst(words[i]);
                        encode[1] = stringToReg(words[i + 1]);
                        encode[2] = stringToReg(words[i + 2]);
                        encode[3] = stoi(words[i + 3]);
                        i += 3; // Skip label and operands
                        break;
                    case RISCV::lw:
                    case RISCV::sw:
                    case RISCV::lwu:
                    case RISCV::lbu:
                    case RISCV::lb:
                    case RISCV::sb:
                        encode[0] = stringToInst(words[i]);
                        encode[1] = stringToReg(words[i + 1]);
                        encode[2] = stoi(words[2].substr(0, words[2].find('(')));
                        encode[3] = stringToReg(words[2].substr(words[2].find('(') + 1, words[2].find(')') - words[2].find('(') - 1));
                        i = i + 3;
                        break;
                    case RISCV::addi:
                        encode[0] = stringToInst(words[i]);
                        encode[1] = stringToReg(words[i + 1]);
                        encode[2] = stringToReg(words[i + 2]);
                        encode[3] = stoi(words[i + 3]);
                        i += 3;
                        break;
                    case RISCV::muli:
                        encode[0] = stringToInst(words[i]);
                        encode[1] = stringToReg(words[i + 1]);
                        encode[2] = stringToReg(words[i + 2]);
                        encode[3] = stoi(words[i + 3]);
                        i += 3;
                        break;
                    default:
                        // Handle non-branch instructions
                        encode[0] = stringToInst(words[i]);
                        break;
                    }
                }
                else
                {
                    // Handle registers
                    RISCV::reg r = stringToReg(words[i]);
                    encode[i] = r;
                }
            }
        }

        m.write_instruction(address, encode, core);
        address++;
    }
    return address;
}

void printRegisterTable(registers r, int core)
{
    std::cout << "Register Table : " << core << std::endl;
    std::cout << "================================================================================" << std::endl;
    for (int i = 0; i < 32; ++i)
    {
        if (i < 10)
            std::cout << "x" << i << "  : " << r.read(i) << std::endl;
        else
            std::cout << "x" << i << " : " << r.read(i) << std::endl;
    }
    std::cout << "================================================================================" << std::endl;
}
void segregateLines(const std::string &inputFilename, const std::string &outputFilename1, const std::string &outputFilename2, const std::string &outputFilename3)
{
    std::ifstream inputFile(inputFilename);
    if (!inputFile.is_open())
    {
        std::cerr << "Error opening input file: " << inputFilename << std::endl;
        return;
    }
    std::ofstream outputFile1(outputFilename1);
    if (!outputFile1.is_open())
    {
        std::cerr << "Error creating output file: " << outputFilename1 << std::endl;
        inputFile.close();
        return;
    }
    std::ofstream outputFile2(outputFilename2);
    if (!outputFile2.is_open())
    {
        std::cerr << "Error creating output file: " << outputFilename2 << std::endl;
        inputFile.close();
        outputFile1.close();
        return;
    }
    std::ofstream outputFile3(outputFilename3);
    if (!outputFile3.is_open())
    {
        std::cerr << "Error creating output file: " << outputFilename2 << std::endl;
        inputFile.close();
        outputFile1.close();
        return;
    }
    std::string line;
    while (std::getline(inputFile, line))
    {
        if (!line.empty())
        {
            int startNum = std::stoi(line.substr(0, line.find_first_of("")));
            std::ofstream *outFile = nullptr;

            if (startNum == 1)
                outFile = &outputFile1;
            else if (startNum == 2)
                outFile = &outputFile2;
            else if (startNum == 3)
                outFile = &outputFile3;
            if (outFile != nullptr)
                *outFile << line << std::endl;
            else
                std::cerr << "Invalid startNum: " << startNum << std::endl;
        }
    }
    inputFile.close();
    outputFile1.close();
    outputFile2.close();
    outputFile3.close();
}
int main()
{
    ofstream outputFile1("..\\data_files\\output\\terminal1.txt");
    ofstream outputFile2("..\\data_files\\output\\terminal2.txt");
    ofstream outputFile3("..\\data_files\\output\\console.txt");

    if (!outputFile1.is_open() || !outputFile2.is_open() || !outputFile3.is_open())
    {
        cerr << "Error: Unable to open the output files." << endl;
        return 1;
    }

    map<string, int> labelMap_1, labelMap_2;
    map<string, int> dataSizes_1, dataSizes_2;
    pair<int, int> p1, p2;
    registers r1, r2;
    memory m;

    bool dataforwardin_on;
    cout << "Should data forwarding be allowed??\nIf no enter ** 0 ** \nElse enter ** 1 **" << endl;
    cin >> dataforwardin_on;
    map<string, int> latency_map;
    int is_Latency;
    cout << "Do you want to add latencies to instructions : \nEnter ** 1 ** for yes\nEnter ** 0 ** for no\n";
    cin >> is_Latency;
    if (is_Latency)
    {
        int num_instructions;
        cout << "Enter the number of instructions you want to add latencies for: ";
        cin >> num_instructions;

        for (int i = 0; i < num_instructions; ++i)
        {
            string instruction;
            int latency;
            cout << "Enter instruction type (use only add,sub,mul,addi,muli) : ";
            cin >> instruction;
            cout << "Enter latency for " << instruction << " instruction: ";
            cin >> latency;
            latency_map[instruction] = latency;
        }
    }
    if (!latency_map.empty())
    {
        cout << "Instruction latencies:\n";
        for (const auto &pair : latency_map)
        {
            cout << pair.first << ": " << pair.second << endl;
        }
    }
    else
    {
        cout << "No latencies added.\n";
    }

    // for first file
    const string file_path = "..\\data_files\\input\\test.s";
    ifstream instructions_prog_1(file_path);
    if (!instructions_prog_1.is_open())
    {
        cout << "Error in opening the file 1" << endl;
        return 0;
    }

    vs lines_prog_1;
    string line_prog_1;
    while (getline(instructions_prog_1, line_prog_1))
    {
        auto it = std::find_if_not(line_prog_1.begin(), line_prog_1.end(), [](unsigned char c)
                                   { return std::isspace(c); });
        std::string trimmedInput(it, line_prog_1.end());
        lines_prog_1.push_back(trimmedInput);
    }

    instructions_prog_1.close();
    collectLabels(lines_prog_1, labelMap_1, dataSizes_1, p1);
    int no_inst_1 = generateMachineCode(lines_prog_1, labelMap_1, m, p1, 1);

    // for second file
    const string file_path_2 = "..\\data_files\\input\\SELECTION_SORT.s";
    ifstream instructions_prog_2(file_path_2);
    if (!instructions_prog_2.is_open())
    {
        cout << "Error in opening the file 2" << endl;
        return 0;
    }

    vs lines_prog_2;
    string line_prog_2;
    while (getline(instructions_prog_2, line_prog_2))
    {
        auto it2 = std::find_if_not(line_prog_2.begin(), line_prog_2.end(), [](unsigned char c)
                                    { return std::isspace(c); });
        std::string trimmedInput(it2, line_prog_2.end());
        if (trimmedInput.find(".string") == string::npos)
        {
        }
        else
            replace(trimmedInput.begin(), trimmedInput.end(), ',', ' ');
        lines_prog_2.push_back(trimmedInput);
    }
    instructions_prog_2.close();

    collectLabels(lines_prog_2, labelMap_2, dataSizes_2, p2);
    int no_inst_2 = generateMachineCode(lines_prog_2, labelMap_2, m, p2, 2);

    streambuf *coutbuf3 = cout.rdbuf();
    cout.rdbuf(outputFile3.rdbuf());

    ALU alui(latency_map, p1, p2, no_inst_1, no_inst_2, m, r1, r2, 1, 2, dataforwardin_on);

    cout.rdbuf(coutbuf3);

    streambuf *coutbuf1 = cout.rdbuf();
    cout.rdbuf(outputFile1.rdbuf());

    printRegisterTable(r1, 1);

    std::cout << "================================================================================\n"
              << "Label map code 1\n"
              << "================================================================================" << std::endl;
    for (const auto &pair : labelMap_1)
    {
        std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
    } // labels
    std::cout << "================================================================================\n"
              << "Address list code 1\n"
              << "================================================================================" << std::endl;
    for (int i = 0; i < 50; ++i)
    {
        if (i < 10)
            std::cout << "Address " << i << "  : " << m.read_memory(i, 1) << std::endl;
        else
            std::cout << "Address " << i << " : " << m.read_memory(i, 1) << std::endl;
    }
    std::cout << "================================================================================\n"
              << "String Map : code 1\n"
              << "================================================================================" << std::endl;
    for (const auto &pair : m.strmap_1)
    {
        std::cout << "Key: " << pair.first.first << ", Value: " << pair.first.second << ", Address: " << pair.second << std::endl;
    }
    std::cout << "================================================================================" << std::endl;

    cout.rdbuf(coutbuf1);

    streambuf *coutbuf2 = cout.rdbuf();
    cout.rdbuf(outputFile2.rdbuf());

    printRegisterTable(r2, 2);

    std::cout << "================================================================================\n"
              << "Label map code 2\n"
              << "================================================================================" << std::endl;

    for (const auto &pair : labelMap_2)
    {
        std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
    }
    std::cout << "================================================================================\n"
              << "Address list code 2\n"
              << "================================================================================" << std::endl;
    for (int i = 0; i < 50; ++i)
    {
        if (i < 10)
            std::cout << "Address " << i << "  : " << m.read_memory(i, 2) << std::endl;
        else
            std::cout << "Address " << i << " : " << m.read_memory(i, 2) << std::endl;
    }
    std::cout << "================================================================================\n"
              << "String Map : code 2\n"
              << "================================================================================" << std::endl;
    for (const auto &pair : m.strmap_2)
    {
        std::cout << "Key: " << pair.first.first << ", Value: " << pair.first.second << ", Address: " << pair.second << std::endl;
    }
    std::cout << "================================================================================" << std::endl;

    // for (int i = 0; i < 30; i++)
    // {
    //     cout << m.instructions_1[i][0] << " " << m.instructions_1[i][1] << " " << m.instructions_1[i][2] << " " << m.instructions_1[i][3] << " " << endl;
    // }

    cout.rdbuf(coutbuf2);

    outputFile1.close();
    outputFile2.close();
    outputFile3.close();
    segregateLines("..\\data_files\\output\\console.txt", "..\\data_files\\output\\console1.txt", "..\\data_files\\output\\console2.txt", "..\\data_files\\output\\OUTPUTS.txt");
    return 0;
}