
// alternative implementations

// Get index of next available shortest job
Process next_shortest_job(std::vector<Process> p_vector, std::set<Process>* processed_elements){

    Process P(0, 0);
    bool initalized = false;                        // Check if the Process is initalized

    for(auto& elem : p_vector){
        // iterate through vector

        if((*processed_elements).count(elem) > 0)   // Element already processed
            continue;                               

        if(!initalized){                             // Initalize element when encountered for the first time
            P = elem; 
            initalized = true;
            continue;
        }    

        if(elem.get_e_time() < P.get_e_time())
            P = elem; 
        
    }


    if(!initalized)
        throw std::runtime_error("No instance available");


    (*processed_elements).insert(P);
    return P; 
}







/*
Get the next job from the vector regarding the sorting method

Methods:
SJF - Next shortest job
EDF - Earliest deadline
LLF - Least laxity (deadline - ready_time - execution time)
RR  - In order 
*/
Process next_job(std::vector<Process> p_vector, std::set<Process>* processed_elements, int method){
    std::vector<Process>::iterator next_shortest_job = std::min_element(p_vector.begin(), p_vector.end(), 
        // Order relation - lambda function
        [&processed_elements, method](Process& p1, Process& p2){                                // p1 tested process, p2 comparison process
            // Check if in set

            if((*processed_elements).count(p1) == 0 && (*processed_elements).count(p2) > 0)       // Only p1 is not in the set
                return true; 
            else if((*processed_elements).count(p2) == 0 && (*processed_elements).count(p1) > 0)  // Only p2 is not in the set
                return false; 
            else{

                switch(method){
                    case SJF:                 // Next shortest job
                        return p1.get_e_time() < p2.get_e_time();                                     // Shortest execution time
                        break;

                    case EDF:                 // Next earliest deadline
                        return p1.get_deadline() < p2.get_deadline();                                 // Shortest deadline 
                        break;

                    case LLF:                 // Least Laxity 
                        return ( p1.get_deadline() - p1.get_r_time() - p1.get_e_time() ) < ( p2.get_deadline() - p2.get_r_time() - p2.get_e_time() );
                        break;

                    case RR:                  // Round Robin

                    default:
                        return p1 < p2; 
                }
            }
        }

    );
    (*processed_elements).insert(*next_shortest_job);
    return *next_shortest_job;
}

/*
Create a new process queue

Methods:
SJF
EDF
LLF
RR
*/
std::queue<Process> create_process_queue(std::vector<Process> p_vector, int method){
    std::queue<Process> process_queue; 
    std::set<Process> processed_elements;                               // Store all elements which have been processed already

    for(int i=0; i<p_vector.size(); i++){
        Process p = next_job(p_vector, &processed_elements, method);
        process_queue.push(p);
    }

    return process_queue;
}



     /*
        Create a new queue 

        Methods:
            SJF - Next shortest job
            EDF - Earliest deadline
            LLF - Least laxity (deadline - ready_time - execution time)
            RR  - In order 
        */

       /*
        std::queue<Process> create_queue(std::vector<Process> p_vector, int method_id){
            clear_queue();                                      // Empty the queue

            std::set<Process> processed_elements;               // All elements that are already in the queue

            // Insert elements
            for(int i=0; i<p_vector.size(); i++){
                Process p = next_process(p_vector, &processed_elements, method_id);
                process_queue.push(p);
            }
            return this->process_queue;
        }

        */


/*
        // Returns the next process to be added to the queue
        Process next_process2(std::vector<Process> p_vector, std::set<Process>* processed_elements, int method_id){
            std::vector<Process>::iterator process = std::min_element(p_vector.begin(), p_vector.end(), 
                // Order relation - lambda function
                [&processed_elements, method_id](Process& p1, Process& p2){          

                    if((*processed_elements).count(p1) == 0 && (*processed_elements).count(p2) > 0)       // Only p1 is not in the set
                        return true; 
                    else if((*processed_elements).count(p2) == 0 && (*processed_elements).count(p1) > 0)  // Only p2 is not in the set
                        return false; 
                    else{
                        return p1.get_value(method_id) < p2.get_value(method_id);
                    }
                }
            );

            (*processed_elements).insert(*process);
            return *process;
        }

*/