
#include "common.h"
#include "FileData.h"
#include "minheap.h"
#include "redblack_tree.h"

// Change to 1 to enable all the debug prints
#define ENA_DEBUG 0

// This is to keep track of the current work I am doing
typedef struct {
  int pending_file_task_exist;
  int job_in_progress;
  int days_worked_on_current_job;
}Status_t;



// ================
// Global variables
// ================
ifstream myfile;    // file handle of the input file
FileData filedata;  // Used for parsing the input file
MinHeap minheap;    // Minheap tree
Red_black_tree red_black_tree; // red black tree
Status_t status;   // Current status of job (ie, is there any pending job or tasks to do from input file)
FILE * output_file; // output file pointer

// =====================
// Functions prototypes
// =====================
void get_next_file_data(); // Gets the next line in the input file
void initialize_status();  // Initialzes the status structure
void do_filedata_task();   // Perform the job corresponding to the input file entry


int day = 0;
int global_counter = day; // This is the global counter. This will always match day since time is measured in days

int main(int argc, char** argv) {

  MinHeap_t job; // Current job

  // Confirm that the number of arguments matched expectations
  if(argc != 2){
      printf("ERROR: You have not entered arguments correctly. Please rerun with ./risingCity <input_filename> \n");
      exit(1);
  }

  // Open the input, output files
  string filename = argv[1];
  myfile.open(filename.c_str());
  output_file = fopen ("output_file.txt","w");

  // Check that we are able to open the files
  if (myfile.is_open() == 0){
    cout << "ERROR: Unable to open input file: " << filename << ". Please check the filename\n"; return 0;
  }
  if(output_file == NULL){
      cout << "ERROR: Unable to open output_file.txt for writing. Exiting\n"; return 0;
  }

  // Inform the file pointers to the trees so that they can direcly append to file while printing
  minheap.output_file = output_file;
  red_black_tree.output_file = output_file;


  // Initialize the current status register
  initialize_status();

  while(1) {

    global_counter = day;

    // Do the actual building job
    if(status.job_in_progress == 0){
        // If there is no job in progress, take the next job from the minheap
        if(minheap.heap_size >= 1){
                status.job_in_progress = 1;
                job = minheap.remove_root();

                red_black_tree.building_under_operation = job.building_num;

                // Make sure this is correct pointer
                // ie, RBT and Heap pointers are always correct
                Red_black_node *rbt_node_ptr = job.rbt_node_ptr;
                int rbt_building_num = (rbt_node_ptr -> building_info).building_num;
                if(rbt_building_num != job.building_num){
                  printf("RBT building number = %d not matching HEAP building num = %d, RBT pointer = %p \n", rbt_building_num, job.building_num,(void *)rbt_node_ptr);
                }

                // Set the dates worked as 0 since we are just starting now
                status.days_worked_on_current_job = 0;
                #if ENA_DEBUG
                  printf("Day %d : Starting to work on building %d. Executed_days so far = %d, \n",day, job.building_num, job.execution_time);
                #endif
        }
    }

    // ie, if we are doing a job, increment the time spent on job
     if(status.job_in_progress == 1){
          job.execution_time ++;
          status.days_worked_on_current_job ++;

          // Update execution time in Red Black tree
          // This can be done easily since the Minheap node has the pointer to RBT
          Red_black_node * rbt_node_ptr = job.rbt_node_ptr;
          (rbt_node_ptr -> building_info).execution_time ++;
      } // if job_in_progress

    // Do all tasks corresponding to the input file if there are pending entries
    if(status.pending_file_task_exist == 1 && filedata.time == day){
              #if ENA_DEBUG
                printf("Day %d: Doing the input file job \n", day);
              #endif
              do_filedata_task();
              get_next_file_data();
    }


    // If there is a job that is completed or run for 5 days, then delete the node and reset the job_in_progress
    if(status.job_in_progress == 1){

        if(job.execution_time == job.total_time){

            #if ENA_DEBUG
              printf("Day %d : Building %d is complete. Executed_days = %d, RBT pointer = %p !!! \n",day, job.building_num, job.execution_time, (void *)job.rbt_node_ptr);
            #endif

            red_black_tree.delete_node(job.rbt_node_ptr);
            fprintf(output_file,"(%d,%d)\n",job.building_num, day);
            status.job_in_progress = 0;

          }else if (status.days_worked_on_current_job == 5){

            // If we have worked on this for 5 days, then add it back. We have already incremented the executed time above
            #if ENA_DEBUG
              printf("Day %d : Building %d, global_counter=%d is not complete after 5 days. Adding back \n",day, global_counter, job.building_num);
            #endif

            minheap.insert_element(job);
            status.job_in_progress = 0;
          }
    }


    // Check if the entire program is complete.
    // This will be complete if there is no pending tasks to do from input file and there is nothing to do in the heap
    if((status.pending_file_task_exist == 0) && (minheap.heap_size == 0) && (status.job_in_progress == 0)){
          // NOTE: We are not adding this because the output file provided did not have this field
          //fprintf(output_file,"Completion date of City =  Day %d", day);
          break;
    }

    #if ENA_DEBUG
      cout << "======RBT START ======== \n" ;
      red_black_tree.print_entire_tree(red_black_tree.root,NULL,false);
      cout << "======RBT END ===== \n" ;
    #endif

    day++;
  } // while


  #if ENA_DEBUG
      minheap.print_heap();
  #endif


  // Close all input and output files
  printf("Program Complete: Please check output_file.txt for the outputs \n");
  myfile.close();
  fclose (output_file);
  return 0;
}



// ==============================================================================
// This function is actually going to execute the job mentioned in the input file
// ==============================================================================
void do_filedata_task(){
  MinHeap_t minheap_entry;
  RBT_t rbt_entry;

  if(filedata.command.compare("Insert") == 0){

      // If this is insert, we insert into the minheap & red black tree
      Red_black_node *rbt_node_ptr = NULL;

      // Insert into Red black tree
      rbt_entry = filedata.get_RBT_struct();
      rbt_node_ptr = red_black_tree.insert(filedata.arg1,rbt_entry);

      // Insert into the Heap
      // Note: The insert_element also updates the Red black tree node with the pointer to the minheap node
      minheap_entry = filedata.get_minheap_struct();
      minheap_entry.rbt_node_ptr = rbt_node_ptr;
      minheap.insert_element(minheap_entry);


  }else if((filedata.command.compare("Print") == 0) || (filedata.command.compare("PrintBuilding") == 0) || (filedata.command.compare("PrintBuliding") == 0) ){

      // This means this is a print
      // So ask the red black tree to print it.
      // Note: In the FileData.cpp, we already make arg2 = arg1 if arg2 is not present to handle one and two argument cases
      red_black_tree.Print(filedata.arg1, filedata.arg2);

  }else {
    printf("ERROR: Unrecogonized command. Expecting only Print, PrintBuilding, Insert commands. Command received =  %s \n",filedata.command.c_str());
    exit(0);
  }
}


// Gets the next entry in the input file
void get_next_file_data(){
  string line;
  if(getline (myfile,line)){
      status.pending_file_task_exist = 1;
      filedata.populate_data(line);
  }else{ status.pending_file_task_exist = 0;}
}

void initialize_status() {
  //cout << "Initialize status \n";
  get_next_file_data();
}
