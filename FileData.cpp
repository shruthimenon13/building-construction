#include <iostream>
using namespace std;

#include "common.h"
#include "FileData.h"


// The does a regex on the input string to extract command and arguments
void FileData::populate_data(string str)
{
    // We are looking for <DAY>: COMMAND(ARG1,ARG2) or <DAY>: COMMAND(ARG1)
    regex rx("([0-9]+): ?(.*)\\(([0-9]+)(,([0-9]+))?\\)");
    const char* ws = " \t\n\r\f\v";

    smatch match;
    trim(str,ws);

    if (regex_match(str, match, rx))
    {
      if(match.size() >= 6 ){
        time = stoi(match[1].str());
        command = match[2];
        arg1  = stoi(match[3].str());
        if(!match[5].str().empty()){
            arg2 = stoi(match[5].str());  // If there is no arg2 (in print case, we make arg2 = arg1)
        }else{
            arg2 = arg1;
        }
      }
    }
}



// This converts the data into a structure to insert into minheap tree
MinHeap_t FileData::get_minheap_struct(){
  MinHeap_t minheap;
  minheap.building_num = arg1;
  minheap.execution_time = 0;
  minheap.total_time = arg2;
  return minheap;
}

// This converts the data into a structure to insert into redblack
RBT_t FileData::get_RBT_struct(){
  RBT_t minheap;
  minheap.building_num = arg1;
  minheap.execution_time = 0;
  minheap.total_time = arg2;
  return minheap;
}

// This is a print for debug purpose 
void FileData::print() {
  printf("Input file: Time = %d, Command = %s, arg1 = %d, arg2=%d \n",time, command.c_str(),arg1, arg2);
}
