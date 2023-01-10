#include <stdio.h>
#include <iostream>

// Container
#include <vector>
#include <queue>
#include <set>
#include <algorithm>            // For min

#include "Process.h"
#include "QueueFactory.h"



/*---------------- Process Strategies ----------------*/

// First-come First-serve
double FCFS_avg(std::vector<Process> p_vec){ 
    int total_time = 0; 
    int current_time = 0;

    for(auto& p : p_vec){
        current_time += p.get_e_time();
        total_time += current_time; 
    }

    return total_time / double(p_vec.size());
}


// Shortest job first
double SJF_default_avg(std::vector<Process> p_vec){
    int total_time = 0;
    int current_time = 0;
    

    // Create queue
    std::queue<Process> p_queue;
    Queue_Creator qc(&p_queue, p_vec, SJF);
    qc.update_queue(0, false);

    while(!p_queue.empty()){
        Process p = p_queue.front();
        p_queue.pop();

        current_time += p.get_e_time();
        total_time += current_time; 
    }

    return total_time / double(p_vec.size());

}


/*

// Shortest job first but with ready times
double SJF_r_times_avg(std::vector<Process> p_vec){
    int total_time = 0;
    int current_time = 0;
    int done_processes = 0;
    
    // Create queue
    std::queue<Process> p_queue;
    Queue_Creator qc(&p_queue, p_vec, SJF);

    qc.update_queue(current_time, true);             
    current_time++;
    Process p = p_queue.front();                 // Load first process

    while(done_processes < p_vec.size()){
        qc.update_queue(current_time, true);     // Load new processes (if available)
        p = p_queue.front();                     // Always take process from front

        if(p.is_completed()){
            p_queue.pop();                      // remove process from queue
            total_time += current_time;
            done_processes++;
        }

        p.give_p_time(1);                       // Give 1 intervall time
        current_time++;
    }

    return total_time / double(p_vec.size());
}

*/

// Earliest deadline first
double EDF_avg(std::vector<Process> p_vec){
    int total_time = 0;
    int current_time = 0; 

    // Create queue
    std::queue<Process> p_queue;
    Queue_Creator qc(&p_queue, p_vec, EDF);
    qc.update_queue(0, false);

    while(!p_queue.empty()){
        Process p = p_queue.front();
        p_queue.pop();

        current_time += p.get_e_time();
        total_time += current_time; 
    }
     
    return total_time / double(p_vec.size());

}

// TODO: deadline 


// Least Laxity First - (deadline - ready_time - execution time)
double LLF_avg(std::vector<Process> p_vec){
    int total_time = 0;
    int current_time = 0; 

    // Create queue
    std::queue<Process> p_queue;
    Queue_Creator qc(&p_queue, p_vec, LLF);
    qc.update_queue(0, false);

    while(!p_queue.empty()){
        Process p = p_queue.front();
        p_queue.pop();

        current_time += p.get_e_time();
        total_time += current_time; 
    }

    return total_time / double(p_vec.size());
}


// Round Robin
double RR_avg(std::vector<Process> p_vec, int q){

    int total_time = 0;
    int current_time = 0; 

    // Create queue
    std::queue<Process> p_queue;
    Queue_Creator qc(&p_queue, p_vec, RR);
    qc.update_queue(0, false);

    while(!p_queue.empty()){

        Process p = p_queue.front();
        int r_time = p.give_p_time(q);  // Give p cpu time and return the rest time that the process didn't need
        current_time += (q - r_time);   // update current time (intervall - that the process didn't need)
        p_queue.pop();                  // Give cpu to other process

        // Check if process is complted
        if(p.is_completed())
            total_time += current_time; 
        else  // Process needs more cpu time, put at end of queue
            p_queue.push(p);
    }

    return total_time / double(p_vec.size());
}



/*---------------- Main & initalization ----------------*/

std::vector<Process> create_p_vector(){
    // Process (index, execution_time, ready_time, deadline)
    std::vector<Process> p_vector;
    p_vector.push_back( Process(1, 8, 0, 10) );
    p_vector.push_back( Process(2, 5, 0, 9) );
    p_vector.push_back( Process(3, 4, 0, 9) );
    
    return p_vector;
}


// Aufg 2
// Change the index to change the order
std::vector<Process> create_p_vector_changed(){
    // Process (index, execution_time, ready_time, deadline)
    std::vector<Process> p_vector;
    p_vector.push_back( Process(3, 8, 0, 10) );
    p_vector.push_back( Process(1, 5, 0, 9) );
    p_vector.push_back( Process(2, 4, 0, 9) );
    
    return p_vector;
}



int main(){

    // Create vector with processes 
    std::vector<Process> p_vector = create_p_vector();


    //Aufg 2
    // std::vector<Process> p_vector = create_p_vector_changed();

    // Call functions
    std::cout << "FCFS average: " << FCFS_avg(p_vector) << std::endl;
    std::cout << "SJF average: " << SJF_default_avg(p_vector) << std::endl;
    std::cout << "EDF average: " << EDF_avg(p_vector) << std::endl; 
    std::cout << "LLF average: " << LLF_avg(p_vector) << std::endl; 
    std::cout << "RR average: " << RR_avg(p_vector, 3) << std::endl; 
    
}