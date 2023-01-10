#pragma once
    #include <algorithm>            // For min_element
    #include <vector>
    #include <set>
    #include <queue>



/*Create a new queue using create queue

Methods: SJF, EDF, LLF, RR
*/
class Queue_Creator{

    public: 
        Queue_Creator(std::queue<Process>* p_queu, std::vector<Process> p_vector, int method_id){
            this->process_queue = p_queu;
            this->p_vector = p_vector;
            this->method_id = method_id;
        }


        // Add all ready processes to ready vector
        void update_ready_processes(int current_time){
            ready_processes.clear();

            // Search for ready processes
            for(auto elem: this->p_vector){
    
                if(elem.get_r_time() <= current_time && !elem.is_completed()){                // check if process is ready
                    ready_processes.push_back(elem);                 // Put process to ready vector
                }
            
            }
        }

        // Update queue by adding all ready element to it 
        void update_queue(int current_time, bool reorder){
            std::set<Process> processed_elements; 

            update_ready_processes(current_time);              

            // ------- Put ready processes in queue --------- //

            // Get next ready process
            Process next_p = next_process(&processed_elements);
            for(auto element : ready_processes){
                if(next_p.get_id() < 0)
                    break;
                
                // Add element to end of queue 
                (*process_queue).push(next_p);
                next_p = next_process(&processed_elements); // Get next element
            }
        }


    private:
        // Get the next process
        Process next_process(std::set<Process>* processed_elements){
            Process default_p(-1, 0, 0, 0);         // default process
            
            if(ready_processes.size() == 0)
                return default_p; 

            std::vector<Process>::iterator process = std::min_element(ready_processes.begin(), ready_processes.end(),
                // Define order relation
                [&processed_elements, this](Process& p1, Process& p2){ 
                    if((*processed_elements).count(p1) == 0 && (*processed_elements).count(p2) > 0)   
                        return true;
                    else if((*processed_elements).count(p2) == 0 && (*processed_elements).count(p1) > 0)
                        return false;
                    else 
                        return p1.get_value(this->method_id) < p2.get_value(this->method_id);
                }
            );

            if(process != p_vector.end())
                (*processed_elements).insert(*process);

            return *process;
        }



        int method_id;                                      // The way of sorting the processes into the queue
        std::queue<Process>* process_queue;                 // Refrence to Queue containg all ready processes in order
        std::vector<Process> p_vector;                      // Vector of all processes
        std::vector<Process> ready_processes;               // Vector of all Processes that are ready

};

