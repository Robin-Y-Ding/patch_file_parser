#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include "FileOp.h"
//#include "Header.h"
//#include "HunkObj.h"
using namespace std;

int main()
{
    string str, fp;
    cout << "Please input the file path:" << endl;
    getline(cin, str);
    string origin_path = str;
    string::size_type pos = 0;
    while ((pos = str.find('\\', pos)) != string::npos)
    {
	str.replace(pos, 1, "\\\\");
	pos += 2;
    }
    FileOp fop(str);
    fop.o_patch_file_path = origin_path;
    fop.read_file();
    return 1;
}



