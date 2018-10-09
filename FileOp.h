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
    string get_file_name(string a, string b);	
    string o_patch_file_path;

private:
    string c_patch_file_path;
    patch pt;
};

string FileOp::get_file_name(string a, string b) {
    if (a.size() > b.size())
        swap(a, b);
    string str_m;//store the longest common string
    for (int i = 0; i < a.size(); i++)
    {
        for (int j = i; j < a.size(); j++)
        {
            string temp = a.substr(i, j - i + 1);
            if (b.find(temp) == string::npos)
                continue;
            else if (str_m.size() < temp.size())
                str_m = temp;
        }
    }
    return str_m;
}

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
	if(s.find("@@")!= string::npos && s.substr(0,2) == "@@")
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
    string useless;
    int modilines;
    string modistatus;
    string old_version;
    string new_version;

    while(getline(rf,s))
    {
        if (s.find("diff --git") != string::npos)
        {
            ss.clear();
            ss.str(s);
            ss >> useless;
            ss >> useless;
            ss >> old_version;
            ss >> new_version;
            file_name = get_file_name(old_version, new_version);

            pt.hd_get_file_name(file_name);
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


