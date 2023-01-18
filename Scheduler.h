#pragma once
    #include "Process.h"
    #include <vector>
    #include <queue>
    #include <algorithm>                // For min_element



/* Chooses the next processes */
class Scheduler{

    public: 
        Scheduler(std::vector<Process*> p_vector, int method_id){
            // Initalize Scheduler with all processes and the corresponding method for the scheduler
            this->p_vector = p_vector;
            this->method_id = method_id;

            this->current_job = nullptr;
            this->current_time = 0;
            this->job_counter = 0; 
        }

        // Returns the next available job / process assigned by the Scheduler
        Process* next_job(){

            // Check if last job needs more time
            if(current_job != nullptr){
                if (!(*current_job).is_completed())
                    ready_processes.push_back(current_job);     // Process not done yet
                else 
                    job_counter++;
            }

            if(p_queue.empty())                                 // Set to nullptr if none available
                current_job = nullptr;
            else    
                current_job = p_queue.front(), p_queue.pop();   // Set current job to front of queue and remove job from queu

            return current_job;                                 // Return next scheduled process
        }


        // Look for new ready processes and add them to the ready queue
        void update_p_queue(int time){
            this->current_time = time;                                  // Update time
            update_ready_processes();

            // Move queue processes to ready_processes
            while(!p_queue.empty()){
                ready_processes.push_back(p_queue.front());
                p_queue.pop();
            }

            // Move processes from ready_processes to queue in correct order
            while(!ready_processes.empty()){

                std::vector<Process*>::iterator next_process = std::min_element(ready_processes.begin(), ready_processes.end(),
                    // Define order relation / as lamda function
                    [this](Process* p1, Process* p2){
                        return (*p1).get_value(this->method_id) < (*p2).get_value(this->method_id);
                    }
                );

                // Add proccess to queue
                p_queue.push(*next_process);

                // Delete element from ready_processe
                ready_processes.erase(next_process);
            }
        }


        int get_job_counter(){ return this->job_counter;}



    private:
        // Check for any ready processes inside p_vector and add them to ready_processes
        void update_ready_processes(){
           

            for(auto pro: this->p_vector){

                if(!pro->is_scheduled() && pro->get_r_time() <= current_time){
                    // Add the ready process
                    ready_processes.push_back(pro);
                    pro->set_scheduled(); 
                }
            }
        }


        // ------------- Variables -------------- //
        
        int current_time;                                   // Current time to determine the ready processes
        int method_id;                                      // Method used to schedule processes

        int job_counter;                                    // Number of process finished

        Process* current_job;                               // The Process that is currently running
        std::vector<Process*> p_vector;                     // Stores all processes
        std::vector<Process*> ready_processes;              // Vector of all processes* that need to be put inside the queue
        std::queue<Process*> p_queue;                       // Queue containing processes* in scheduled order


};

