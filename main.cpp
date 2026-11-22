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
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <sstream>

enum class Operation {
    SUB_IMMEDIATE
};

enum class Register {
    X0 = 0, X1, X2, X3, X4, X5, X6, X7, X8, X9, X10, X11, X12, X13, X14, X15, X16, X17, X18, X19, X20, X21, X22, X23, X24, X25, X26, X27, X28, X29, X30,
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
    CPU() : instruction_pointer_{0}, stack_pointer_{1024}, program_{} {};

    void loadProgram(const std::filesystem::path& path) {
        // ifstream just for read, fstream for R/W
        std::ifstream file {path};

        // parse/interpret the assembly into a set of instructions which
        // our instruction pointer can point to
        program_ = source_to_instructions_(file);
    }

    void run() {
        while (instruction_pointer_ < program_.size()) {
            // fetch/"decode"
            const Instruction& instruction = program_[instruction_pointer_];

            // execute
            execute_(instruction);

            instruction_pointer_++;
        }
    }

private:
    int instruction_pointer_; // index of the next instruction in Program
    std::uint64_t stack_pointer_;
    std::array<std::uint64_t, 31> registers_; // 31 GP 64 bit registers X0-X30
    std::vector<Instruction> program_;

    std::vector<Instruction> source_to_instructions_(std::ifstream& file) {
        std::vector<Instruction> instructions;
        
        std::string line;
        int i = 0;
        while (std::getline(file, line)) {
            printf("line %d: %s\n", i++, line.c_str());

            std::istringstream parts {line};
            std::string mnemonic;
            parts >> mnemonic;

            if (mnemonic != "sub") {
                continue;
            }

            std::string destination;
            std::string source;
            std::string immediate;
            parts >> destination >> source >> immediate;
            
            // "sp," -> "sp"
            destination.pop_back(); 
            source.pop_back(); 

            if (destination == "sp" && source == "sp") {
                instructions.push_back({
                    Operation::SUB_IMMEDIATE,
                    Register::SP,
                    Register::SP,
                    std::stoi(immediate.substr(1)) // "#16" -> 16
                });
            }
        }

        return instructions;
    }

    void execute_(const Instruction& instruction) {
        switch (instruction.op) {
            case Operation::SUB_IMMEDIATE:
                // sub dst, src, #immediatevalue
                write_register_(instruction.destination, read_register_(instruction.source) - instruction.immediate);
        }
    }

    std::uint64_t read_register_(Register reg) {
        if (reg == Register::SP) {
            return stack_pointer_;
        }

        return 1;
    }

    void write_register_(Register reg, std::uint64_t value) {
        if (reg == Register::SP) {
            stack_pointer_ = value;
        }
        else if (reg >= Register::X0 && reg <= Register::X30) {
            registers_[static_cast<std::size_t>(reg)] = value;
        }
    }
};

class Memory {

};

int main() {
    CPU cpu;
    
    cpu.loadProgram("./program.txt");
    cpu.run();
}
