#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <istream>
#include <fstream>
#include <ostream>

using std::vector;
using std::string;

struct hunk
{
	string file_name;
	int ins_time, del_time;
	int del_line[2] = {-1,-1};
	int ins_line[2] = {-1,-1};
	vector<string> diff_code;
	vector<int> insertion_lines;
	vector<int> deletion_lines;
};

class patch 
{
public:
	patch() = default;
	void get_line_num(string str, int(&del)[2], int(&ins)[2]);
	void hd_get_modiinfo(string mi);
	void hd_get_modilines(int ml);
	void hd_get_modistatus(string ms);
	void hd_get_file_name(string fn);
	string hd_show_info(string chs, int i);
	void hd_show_info(string chs);
	int hd_show_file_name_size(void);
	void whether_hunk_finish(void);
	void get_hunk_content(string pfn,string s);
	void get_hunk_code(string s);
	void hk_show_patch(void);
	//void get_file_name(string fn);
	void hk_get_lines(hunk &hk, int del[2], int ins[2]);
	void hk_show_file_name(hunk hk);
	void hk_store_diff_code(string str,hunk &hk);
	int hk_whether_get_number(hunk hk);
	void hk_show_hunk_code(hunk hk);//for test, delete when the project is done
	void hk_gen_line_num(hunk &hk);//generate line numbers of changed content in new version
	void hk_show_changed_lines(hunk hk);
	void assign_current_hunk(hunk chk);
	void add_hunk_vec(void);
	hunk currenthk;
private:
	vector<string> hd_file_name;
	int hd_file_name_size;
	vector<int> hd_modilines;
	vector<string> hd_modistatus;
	string hd_modiinfo;	
	vector<hunk> HK;
};


void patch::add_hunk_vec(void)
{
	HK.push_back(currenthk);
}

void patch::assign_current_hunk(hunk chk)
{
	currenthk = chk;
}

void patch::get_line_num(string str, int(&del)[2], int(&ins)[2])
{
	std::istringstream is(str);
	int i;
	int cnt = 0;
	char ch;
	vector<int> v;
	while (is >> ch && cnt <= 3)
	{
		if (ch == '@')
			cnt++;//To control that we only get the numbers in the structure of @@...@@
		else if (ch >= '0'&&ch <= '9')
		{
			is.putback(ch);//Put ch back into the input flow, so that is>>i can get the full integer
			is >> i;
			v.push_back(i);
			//cout<<" i: "<<i<<endl<<endl;
		}
	}
	
	del[0] = v[0];
	del[1] = v[1];
	ins[0] = v[2];
	ins[1] = v[3];
}

string patch::hd_show_info(string chs, int i)
{
	if (chs == "file_name")
		return hd_file_name[i];
	else if (chs == "modistatus")
		return hd_modistatus[i];
	else
		std::cout << "Error: Please check the string value of the first parameter!" << std::endl;
}

void patch::hd_show_info(string chs)
{
	if (chs != "modiinfo")
	{
		std::cout << "Error: Please check the function call of Header::show_info!" << std::endl;
		return;
	}
	std::cout << "General Information:" << std::endl << hd_modiinfo << std::endl << std::endl;
}

void patch::hd_get_modiinfo(string mi)
{
	hd_modiinfo = mi;
}

void patch::hd_get_modilines(int ml)
{
	hd_modilines.push_back(ml);
}

void patch::hd_get_modistatus(string ms)
{
	hd_modistatus.push_back(ms);
}

void patch::hd_get_file_name(string fn)
{
	hd_file_name.push_back(fn);
}

int patch::hd_show_file_name_size()
{
	hd_file_name_size = hd_file_name.size();
	return hd_file_name_size;
}

int patch::hk_whether_get_number(hunk hk)
{
	if (hk.del_line[0] == -1 && hk.del_line[1] == -1 && hk.ins_line[0] == -1 && hk.ins_line[1] == -1)
	{
		return 0;
	}
	else
		return 1;
}

void patch::hk_show_file_name(hunk hk)
{
	std::cout << "File Name:" << hk.file_name << std::endl << std::endl;
}

void patch::hk_show_hunk_code(hunk hk)
{
	std::cout << "The context code:" << std::endl << std::endl;
	for (int i = 0; i < hk.diff_code.size(); i++)
	{
		std::cout << hk.diff_code[i] << std::endl;
	}
}

void patch::hk_gen_line_num(hunk &hk)
{
	int ins_offset = 0;
	int del_offset = 0;
	int ins_line_num = 0;
	int del_size = 0;
	int ins_size = 0;
	int del_line_num = 0;
	for (int i = 0; i < hk.diff_code.size(); i++)
	{
		if (hk.diff_code[i][0] == '-')
		{
			del_line_num = hk.del_line[0] + del_offset;
			hk.deletion_lines.push_back(del_line_num);
			del_offset++;
                        del_size++;
		}
		else if (hk.diff_code[i][0] == '+')
		{
			ins_line_num = hk.ins_line[0] + ins_offset;
			hk.insertion_lines.push_back(ins_line_num);
			ins_offset++;
                        ins_size++;
		}
		else
		{
			del_offset++;
			ins_offset++;
		}

	}
        if (del_size == 0)
            hk.deletion_lines.push_back(0);
        else if (ins_size == 0)
            hk.insertion_lines.push_back(0);
}

void patch::hk_show_changed_lines(hunk hk)
{
	std::cout << "The insertion lines in the patch are:" << std::endl;
	for (int i = 0; i < hk.insertion_lines.size(); i++)
	{
		std::cout << hk.insertion_lines[i] << std::endl;
	}
	std::cout << "The deletion lines in the patch are:" << std::endl;
	for (int i = 0; i < hk.deletion_lines.size(); i++)
	{
		std::cout << hk.deletion_lines[i] << std::endl;
	}
}

void patch::whether_hunk_finish(void)
{
	if (hk_whether_get_number(currenthk))
	{		
		hk_show_file_name(currenthk);
		hk_show_hunk_code(currenthk);
		hk_gen_line_num(currenthk);
		hk_show_changed_lines(currenthk);
		HK.push_back(currenthk);
	}
}

void patch::hk_get_lines(hunk &hk, int del[2], int ins[2])
{
	for (int i = 0; i < 2; i++)
	{
		hk.del_line[i] = del[i];
		hk.ins_line[i] = ins[i];
	}
}

void patch::get_hunk_content(string pfn,string s)
{
	hunk tmphk;
	tmphk.file_name = pfn;
	int del_line[2], ins_line[2];
	get_line_num(s, del_line, ins_line);
	hk_get_lines(tmphk, del_line, ins_line);
	currenthk = tmphk;
}

void patch::hk_store_diff_code(string str, hunk &hk)
{
	hk.diff_code.push_back(str);
}

void patch::get_hunk_code(string s)
{
	if (hk_whether_get_number(currenthk))
	{
		hk_store_diff_code(s,currenthk);
	}
}
