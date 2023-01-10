
#define SJF 0
#define EDF 1
#define LLF 2
#define RR  3


class Process{
    public: 
        // Default constructor
        Process() : process_id(-1) {}

        Process(int id, int e_time, int r_time, int deadline){
            this->process_id = id;
            this->e_time = e_time;
            this->r_time = r_time; 
            this->deadline = deadline; 

            this->p_time = 0; 
            this->completed = false; 
        }

        // ------- Overload -------- //
        bool operator<(const Process& other) const{
            if(this->process_id < other.process_id)
                return true;
            
            return false; 
        }

         bool operator==(const std::vector<Process>::iterator& other) const {
            return this->process_id == (*other).get_id();
        }


        bool operator==(const Process& other) const { 
            if(this->process_id == other.process_id)
                return true;
           
            return false;
        }

        // -------- Overload ------- //

        // The value will be returned depending on the method
        int get_value(int method_id){

            switch(method_id){
                case SJF:
                    return get_e_time();
                case EDF:
                    return get_deadline();
                case LLF:
                    return get_laxity();
                case RR:
                default:
                    return Process::get_id();
            }
            return Process::get_id();
        }
        
        int get_id(){ return this->process_id; }

        // Return time it takes to execute this process
        int get_e_time(){ return this->e_time; }

        // Return the time at which the process becomes ready
        int get_r_time(){ return this->r_time; }

        int get_deadline(){ return this->deadline; }

        // Calculate and return laxity of Process
        int get_laxity(){ return deadline - r_time - e_time; }

        int is_completed(){ return completed; }


        // Give the process time to calculate for 'time' intervals, returns left time
        int give_p_time(int time){
            int new_p_time = p_time + time;

            if(new_p_time >= e_time){
                // More time than needed
                p_time = e_time; 
                this->completed = true;
                return new_p_time - e_time; 
            }

            // Set p_time
            this->p_time = new_p_time; 
            return 0; 
        }

    private:
        int process_id;
        int e_time;             // Execution time
        int r_time;             // Ready time
        int deadline;

        int p_time;             // Processed time
        bool completed;
};