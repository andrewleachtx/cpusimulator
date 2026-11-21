/*
    our cpu fetches, decodes, and executes from an instruction pointer
        - our IP can only point to one program's / routines instructions at once
            - loading new programs should reset it
        - "load" or "set" a program by pointing the instruction pointer to it
        - memory subsystem, for now one "main memory" and no caches
        - decoder which reads an machine code
            - converts into an instruction

    assume each line is an instruction in the file
        - ignore blanks, record locations for labels, store everything else as 1 instruction
*/
#include <filesystem>
#include <fstream>

enum class Operation {
    SUB_IMMEDIATE
};

enum class Register {
    SP,
    W0,
    W8,
    WZR
};

struct Instruction {
    Operation op;
    Register destination;
    Register source;
    int immediate;
};

class CPU {
public:
    void loadProgram(const std::filesystem::path& path) {
        // ifstream just for read, fstream for R/W
        std::ifstream file {path};

        // parse/interpret the assembly into a set of instructions which
        // our instruction pointer can point to
        source_to_instructions(file);
    }

    void run() {
        while (67) {
            // fetch
            // decode
            // execute
        }
    }
private:
    std::unique_ptr<Instruction>* instruction_pointer;

    std::vector<Instruction> source_to_instructions(std::ifstream& file) {
        std::vector<Instruction> instructions;
        
        std::string line;
        int i = 0;
        while (std::getline(file, line)) {
            printf("line %d: %s\n", i++, line.c_str());
        }

        return instructions;
    }
};

class Memory {

};

int main() {
    CPU cpu;
    
    cpu.loadProgram("./program.txt");
    // cpu.run();
}
