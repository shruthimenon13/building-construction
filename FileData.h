#ifndef ___PARSER_H___
#define ___PARSER_H___

#include "common.h"

class FileData {

  public:

    int time;
    string command;
    int arg1;
    int arg2;

    void print();
    void populate_data(string line);
    MinHeap_t get_minheap_struct();
    RBT_t get_RBT_struct();

    //https://stackoverflow.com/questions/29179614/string-compare-failing-after-reading-line-from-txt-file
    inline std::string& rtrim(std::string& s, const char* t)
    {
        s.erase(s.find_last_not_of(t) + 1);
        return s;
    }

    // trim from beginning (left)
    inline std::string& ltrim(std::string& s, const char* t)
    {
        s.erase(0, s.find_first_not_of(t));
        return s;
    }

    // trim from both ends (left & right)
    inline std::string& trim(std::string& s, const char* t)
    {
        return ltrim(rtrim(s, t), t);
    }
};


#endif
