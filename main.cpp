#include <stdio.h>
#include <iostream>

// Container
#include <vector>
#include <queue>
#include <set>
#include <algorithm>            // For min

//#include "Process.h"
//#include "QueueFactory.h"
#include "Scheduler.h"

void create_processes(std::vector<Process*> &vec);


/*---------------- Process Strategies ----------------*/

double FCFS_avg(std::vector<Process*> p_vec){
    int total_time = 0;
    int current_time = 0; 

    // Create scheduler
    Scheduler S(p_vec, FCFS);
    Process* p;

    while((p = S.next_job(current_time)) != nullptr){

        current_time += p->get_e_time();
        total_time += current_time;

        p->set_completed();
    }
    std::cout << "FCFS average: " << (double)total_time / S.get_job_counter() << std::endl;

    return (double)total_time / S.get_job_counter();
}


/*
double SJF_avg(std::vector<Process*> p_vec){
    int total_time = 0;
    int current_time = 0; 
    
    // Create scheduler
    Scheduler S(p_vec, SJF);
    Process* p;

    while((p = S.next_job(current_time)) != nullptr){

        current_time += p->get_e_time();
        total_time += current_time;

        p->set_completed();
    }

    std::cout << "SJF average: " << (double)total_time / S.get_job_counter() << std::endl;

    return (double)total_time / S.get_job_counter();
}
*/

double SJF_avg(std::vector<Process*> p_vec){

    int total_time = 0;
    int current_time = 0; 
    
    // Create scheduler
    Scheduler S(p_vec, SJF);
    Process* p;

    while((p = S.next_job(current_time)) != nullptr){

        p->give_p_time(1);
        current_time+=1;

        if(p->is_completed())
            total_time += ( current_time - p->get_r_time() );    // Add runtime of p which is time since started till now

    }

    std::cout << "SJF average: " << (double)total_time / S.get_job_counter() << std::endl;

    return (double)total_time / S.get_job_counter();

}



double EDF_avg(std::vector<Process*> p_vec){
    int total_time = 0;
    int current_time = 0; 
    
    // Create scheduler
    Scheduler S(p_vec, EDF);
    Process* p;

    while((p = S.next_job(current_time)) != nullptr){

        current_time += p->get_e_time();
        total_time += current_time;

        p->set_completed();
    }

    std::cout << "EDF average: " << (double)total_time / S.get_job_counter() << std::endl;

    return (double)total_time / S.get_job_counter();

}




double LLF_avg(std::vector<Process*> p_vec){
    int total_time = 0;
    int current_time = 0; 
    
    // Create scheduler
    Scheduler S(p_vec, LLF);
    Process* p;

    while((p = S.next_job(current_time)) != nullptr){

        current_time += p->get_e_time();
        total_time += current_time;

        p->set_completed();
    }

    std::cout << "LLF average: " << (double)total_time / S.get_job_counter() << std::endl;

    return (double)total_time / S.get_job_counter();
}


// Round Robin
double RR_avg(std::vector<Process*> p_vec, int q, bool random_order){
    int total_time = 0;
    int current_time = 0; 

    // Create scheduler
    Scheduler S(p_vec, RR);
    Process* p;

    // Permutate if set
    std::string optional_output = "";
    if(random_order){
        S.permutate_queue_random(); 
        optional_output = " permutated";
    }
        


    while((p = S.next_job(current_time)) != nullptr){

        int r_time = p->give_p_time(q);             // Give p to cpu, return time that the process doesn't need
        current_time += (q - r_time);               

        if(p->is_completed())                       // Update time if process id done
            total_time += current_time;

    }

    std::cout << "RR average" << optional_output << ": " << (double)total_time / S.get_job_counter() << std::endl;

    return (double)total_time / S.get_job_counter();

}




/*---------------- Main & initalization ----------------*/

void create_processes(std::vector<Process*> &vec){
    vec.push_back(new Process(1, 8, 0, 10) );
    vec.push_back(new Process(2, 5, 0, 9) );
    vec.push_back(new Process(3, 4, 0, 9) );
}


void create_processes_with_r_times(std::vector<Process*> &vec){
    vec.push_back(new Process(1, 22, 0, 0) );
    vec.push_back(new Process(2, 2, 0, 0) );
    vec.push_back(new Process(3, 3, 4, 0) );
    vec.push_back(new Process(4, 5, 4, 0) );
    vec.push_back(new Process(5, 8, 4, 0) );
}




int main(){
    // Create processes
    std::vector<Process*> p_vector;
    std::vector<Process*> p_vector2;

    // Call Scheduler functions
    create_processes(p_vector);                                        // Reset processes
    FCFS_avg(p_vector);

    create_processes(p_vector);                                        // Reset processes
    SJF_avg(p_vector);

    printf("\n/3 With ready times: ");
    create_processes_with_r_times(p_vector2);                          // Reset processes
    SJF_avg(p_vector2);
    printf("\n");


    create_processes(p_vector);                                        // Reset processes
    EDF_avg(p_vector);

    create_processes(p_vector);                                        // Reset processes
    LLF_avg(p_vector);

    // Random robin normal order
    create_processes(p_vector);                                        // Reset processes
    RR_avg(p_vector, 3, false);

    // Round Robin permutated
    create_processes(p_vector);                                        // Reset processes
    RR_avg(p_vector, 3, true);
}