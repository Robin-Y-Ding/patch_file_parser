#pragma once
#ifndef FILEOP_H_INCLUDED
#define FILEOP_H_INCLUDED
#include <iostream>
#include <string>
#include <sstream>
#include <istream>
#include <fstream>
#include <ostream>
#include <vector>
#include <stdlib.h>
#include <cstdlib>
#include <cstdio>
#include "patch.h"
using std::vector;
using std::string;


class FileOp
{
public:
    FileOp() = default;
    FileOp(string fp) : c_patch_file_path(fp){}
    void read_file(void);
    void read_hunk(string hunk_file, string patch_file_name);	
    string o_patch_file_path;

private:
    string c_patch_file_path;
	patch pt;
};

void FileOp::read_hunk(string hunk_file, string patch_file_name)
{
    std::fstream rf;
    rf.open(hunk_file.c_str(),std::ios::in);
    if(!rf)
        std::cout << "Error: Cannot open the file. Please check the file path." << std::endl;
    string s;
    std::stringstream ss;
    hunk currenthk;
    pt.currenthk = currenthk;
    while(getline(rf,s))
    {
	if(s.find("@@")!= string::npos)
        {
		pt.whether_hunk_finish();
		pt.get_hunk_content(patch_file_name, s);
		getline(rf,s);
        }
	pt.get_hunk_code(s);
    }
	pt.hk_show_file_name(pt.currenthk);
	pt.hk_show_hunk_code(pt.currenthk);
	pt.hk_gen_line_num(pt.currenthk);
	pt.hk_show_changed_lines(pt.currenthk);
	pt.add_hunk_vec();

}

void FileOp::read_file(void)
{
    std::fstream rf;
    rf.open(c_patch_file_path.c_str(),std::ios::in);
    if(!rf)
        std::cout << "Error: Cannot open the file. Please check the file path." << std::endl;
    string s;
    std::stringstream ss;
    string file_name;
    char useless;
    int modilines;
    string modistatus;

    while(getline(rf,s))
    {
        if(s == "---")
        {
            getline(rf,s);
            for(;s != "";getline(rf,s))
            {
                string::size_type idx = s.find("file changed");
                string::size_type idx1 = s.find("files changed");
                if(idx != string::npos || idx1 != string::npos)
                {
                    s.erase(0, s.find_first_not_of(" "));
                    pt.hd_get_modiinfo(s);
                }
                else
                {
                    ss.clear();
                    ss.str(s);
                    ss >> file_name;
                    ss >> useless;
                    ss >> modilines;
                    ss >> modistatus;

                    pt.hd_get_file_name(file_name);
                    pt.hd_get_modilines(modilines);
                    pt.hd_get_modistatus(modistatus);

                }
            }

        }
        
    }
    string fn;
    for(int i = 1;i <= pt.hd_show_file_name_size();i ++)
    {
	std::ostringstream oss,oss1;        
	string fn = pt.hd_show_info("file_name",i-1);
    	oss << "filterdiff " << o_patch_file_path << " --files=" << i << " > "  << "/tmp/" << i << "__hunkdetails.txt";
    	string temp = oss.str();
    	const char *command = temp.data();
    	system(command);
	oss1 << "/tmp/" << i << "__hunkdetails.txt";     
	string hf_path = oss1.str();
    	read_hunk(hf_path,fn);
    }

}



#endif // FILEOP_H_INCLUDED


