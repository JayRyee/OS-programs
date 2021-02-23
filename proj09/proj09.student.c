#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <algorithm>
#include <vector>
#include <string>
#include <utility>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string.h>

using namespace std; 

bool DEBUG_MODE = false;

void init_table();
void print_table();
void parse_line(string line);
void print_counts();


struct page_table_entry{

	unsigned char index;
	bool v;
	bool p;
	bool r;
	bool m;
	unsigned short frame;


} entry;

int const max_size = 16;
struct page_table_entry page_table[max_size];
int memory_refs = 0;
int read_opps = 0;
int write_opps = 0;


int main(int argc, char* argv[]) {

	init_table();

	ifstream in_file;
	string file;
	for (int i = 1; i < argc; i++)
	{
		string str = argv[i];

		if (str.find("-refs") != string::npos)
		{
			file = argv[i + 1];
		}

		if (str.find("-debug") != string::npos)
		{
			DEBUG_MODE = true;
			print_table();
		}

	}

	if (file.length() < 1)
	{
		cout << "No File to entered, Exiting simulation" << endl;
		return -1;
	}

	in_file.open(file);
	if (!in_file)
	{
		cout << "Could not open input file for reading. Check that file name was entered correctly" << endl;
		return -1;
	}
	
	string line;
	while(!in_file.eof())
	{
		getline(in_file, line);
		if(line.length() > 0)
		{
			parse_line(line);
		}
	}
	print_table();
	print_counts();
	in_file.close();

	return 0;
}

void init_table()
{
	for (int i = 0; i < max_size; i++)
	{
		struct page_table_entry entry;
		entry.v = 0;
		entry.p = 0;
		entry.r = 0;
		entry.m = 0;
		entry.index = 0;
		entry.frame = 0;
		page_table[i] = entry;
	}

}

void print_table()
{
	cout << endl;
	printf("--------------Contents of Page Table:--------------\n\n");
	cout << "     V P R M FF" << endl;
	for (int i = 0; i < max_size; i++)
	{
		page_table_entry e = page_table[i];
		printf("[%x]: %d %d %d %d ", i, e.v, e.p, e.r, e.m);
		if (e.frame < 16)
		{
			printf("0%x\n", e.frame);
		}
		else
		{
			printf("%x\n", e.frame);
		}
	
	
		
	}
	
}

void parse_line(string line)
{
	vector<string> result;
	istringstream iss(line);

	for(string str; iss >> str; )
	{
		result.push_back(str);
	}	
	
	string v_address = result[0];
	string page_num = result[0].substr(0,1);
	string operation = result[1];
	string page_offset = result[0].substr(1,3);
	
	cout << endl << v_address << " " << page_num << " " << operation << " " << page_offset << endl;
	
	//printf("%s %d %d %d\n", &v_address, page_num, operation, page_offset);
	
	memory_refs += 1;
	if (operation.find("R") != string::npos)
	{
		read_opps += 1;
	}
	if (operation.find("W") != string::npos)
	{
		write_opps += 1;
	}
	
	if (DEBUG_MODE == true)
	{
		print_table();
	}
	
	
}

void print_counts()
{
	cout << endl;
	cout << "Number of memory references: " << memory_refs << endl;
	cout << "Number of read operations: " << read_opps << endl;
	cout << "Number of write operations: " << write_opps << endl;
	
}







