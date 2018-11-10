#include "Vtest.h"
#include "verilated.h"
#include "qubits.h"

//#include <signal.h>
//#include <execinfo>
//
//void signal_handler(int signal) {
//    if (signal == SIGABRT) {
//        std::cerr << "\nSIGABRT received\n";
//    } else {
//        std::cerr << "\nUnexpected signal " << signal << " received\n";
//    }
//    
//    const size_t N_FRAMES = 16;
//    void *frames[N_FRAMES];
//    size_t trace_size = 0;
//    
//    trace_size = backtrace(frames, N_FRAMES);
//    std::cerr << "Stack trace:\n";
//    backtrace_symbols_fd(frames, trace_size, STDERR_FILENO);
//    
//    exit(1);
//}

int main(int argc, char **argv) {
    //std::signal(SIGABRT, signal_handler);
    
    Verilated::commandArgs(argc, argv);
    Vtest *top = new Vtest();
    
    while (!Verilated::gotFinish()) {
        top->eval();
    }
    
    QuBit::clean_all();
    delete top;
    return 0;
}