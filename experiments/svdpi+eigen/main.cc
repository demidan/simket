#include "Vtest.h"
#include "verilated.h"
#include "qubits.h"

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    Vtest *top = new Vtest();
    
    while (!Verilated::gotFinish()) {
        top->eval();
    }
    
    QuBit::clean_all();
    delete top;
    return 0;
}