/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 *
 */

#include "interrupts.hpp"
using namespace std; 

int main(int argc, char** argv) {

    //vectors is a C++ std::vector of strings that contain the address of the ISR
    //delays  is a C++ std::vector of ints that contain the delays of each device
    //the index of these elemens is the device number, starting from 0
    auto [vectors, delays] = parse_args(argc, argv);
    std::ifstream input_file(argv[1]);

    std::string trace;      //!< string to store single line of trace file
    std::string execution;  //!< string to accumulate the execution output

    /******************ADD YOUR VARIABLES HERE*************************/
    int current_time = 0;
    int save_context_time = 10;
    int iret = 1;

    /******************************************************************/

    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/
        if (activity == "CPU") {
            execution += to_string(current_time) + ", " + to_string(duration_intr) + ", CPU execution\n";
            current_time += duration_intr;
        } else if (activity == "SYSCALL"){
            pair<string, int> output = intr_boilerplate(current_time, duration_intr, save_context_time, vectors);
            execution += output.first;
            current_time = output.second;

            execution += to_string(current_time) + ", " + to_string(iret) + ", IRET\n";
            current_time += iret;
        }
        else if (activity == "END_IO"){
            
        }
        else{
            execution += "Error: Unknown activity " + activity + "\n"; 
        }


        /************************************************************************/

    }

    input_file.close();

    write_output(execution);

    return 0;
}
