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

    int first_activity = 40; // First ISR activity for SYSCALL takes 40ms, all values in the device table are >= 40ms
    int delay_for_isr = 0; // change this to add a delay to the ISR

    /******************************************************************/

    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/
        if (activity == "CPU") {
            int execution_time = duration_intr / (CPU_SPEED / 100); //to account for CPU speed (will be tested for 100, 200, 300)
            execution += to_string(current_time) + ", " + to_string(execution_time) + ", CPU execution\n";
            current_time += execution_time;
        } else if (activity == "SYSCALL") {
            pair<string, int> output = intr_boilerplate(current_time, duration_intr, save_context_time, vectors);
            execution += output.first;
            current_time = output.second;

            int total_isr_time = delays[duration_intr];
            
            execution += to_string(current_time) + ", " + to_string(first_activity) + ", SYSCALL: run the device driver\n";
            current_time += first_activity;

            int second_activity = total_isr_time - first_activity;
            execution += to_string(current_time) + ", " + to_string(second_activity) + ", transfer data from device to memory\n";
            current_time += second_activity;

            if (delay_for_isr > 0) {
                execution += to_string(current_time) + ", " + to_string(delay_for_isr) + ", SYSCALL DELAY: check for errors\n";
                current_time += delay_for_isr;
            }

            execution += to_string(current_time) + ", " + to_string(iret) + ", IRET\n";
            current_time += iret;
        }
        else if (activity == "END_IO"){
            pair<string, int> output = intr_boilerplate(current_time, duration_intr, save_context_time, vectors);
            execution += output.first;
            current_time = output.second;

            int total_isr_time = delays[duration_intr];
            execution += to_string(current_time) + ", " + to_string(total_isr_time) + ", ENDIO: run the ISR (device driver)\n";
            current_time += total_isr_time;

            execution += to_string(current_time) + ", " + to_string(iret) + ", IRET\n";
            current_time += iret;
        }
        else{
            execution += to_string(current_time) + ", 0 " + activity + "\n"; 
        }


        /************************************************************************/

    }

    input_file.close();

    write_output(execution);

    return 0;
}
