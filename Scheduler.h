#pragma once
    #include "Process.h"
    #include <vector>
    #include <queue>
    #include <algorithm>                // For min_element, shuffle
    #include <random>                   



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

            this->update_p_queue();                  // Set up queue
        }

        /* Returns the next available job / process assigned by the Scheduler
        In all cases but Round Robin next_job searches for new processes and sorts them inside the queue
        */
        Process* next_job(int time){
            this->current_time = time;                      // Set current_time, if all processes are ready from the begining this value can be set to 0 each time

            // ----- Check if last job needs more time ----- //
            if(current_job != nullptr){
                if (!(*current_job).is_completed()){

                    /* In case of Round Robin the process doen't have to be sorted
                    but just put at the end of the procecss queue*/
                    if(this->method_id == RR)
                        p_queue.push(current_job);
                    else{
                        /* Push the not yet ready process back to the ready_processes 
                        then sort the queue again
                        */
                        ready_processes.push_back(current_job); 
                        update_p_queue();
                    }
                        
                }else 
                    job_counter++;
            }


            // ----- Check for ready processes ----- //
            if(update_ready_processes())
                update_p_queue();


            // ----- Return process ----- //
            if(p_queue.empty())                                 // Set to nullptr if none available
                current_job = nullptr;
            else    
                current_job = p_queue.front(), p_queue.pop();   // Set current job to front of queue and remove job from queu

            return current_job;                                 // Return next scheduled process
        }


        // Look for new ready processes and add them to the ready queue
        void update_p_queue(){
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


        // Puts process queue in random order
        void permutate_queue_random(){
            std::vector<Process*> shuffled_vec;

            // Move elements in vector
            while(!p_queue.empty()){
                shuffled_vec.push_back(p_queue.front());
                p_queue.pop();
            }

            // Shuffle vector
            auto rng = std::mt19937(std::random_device()());
            std::shuffle(shuffled_vec.begin(), shuffled_vec.end(), rng);

            // Put elements back in queue
            for(auto ele: shuffled_vec)
                this->p_queue.push(ele);
        }

        // Checks if any processes are left to process
        bool processes_left(){
            return this->p_vector.size();
        }


        int get_job_counter(){ return this->job_counter;}


    private:
        // Check for any ready processes inside p_vector and add them to ready_processes, returns true if ready_processes is not empty
        bool update_ready_processes(){

            for(auto pro = p_vector.begin(); pro != p_vector.end(); ){

                if(!(*pro)->is_scheduled() && (*pro)->get_r_time() <= this->current_time){
                    // Add the ready process
                    ready_processes.push_back(*pro);
                    (*pro)->set_scheduled(); 

                    pro = p_vector.erase(pro); 
                }else{
                    pro++;
                }
                
            }

            return ready_processes.size();
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

