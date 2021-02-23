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
#include <queue>

using namespace std; 

bool DEBUG_MODE = false;

void init_table();
void print_table();
void parse_line(string line);
void print_counts();
void parse_frames(string line);
void parse_pages(string line);
int process_page_fault(int page_index);


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
int page_faults = 0;
int write_backs = 0;

int free_frames = 0;
int frame_count = 0;
queue <int> frames;
int valid_pages = 0;


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
	
	ifstream config_file;
	config_file.open("config");
	if (!in_file)
	{
		cout << "Could not open config file for reading. Check that file name was saved correctly" << endl;
		return -1;
	}
	
	string l;
	getline(config_file, l);
	getline(config_file, l);
	parse_frames(l);
	getline(config_file, l);
	parse_pages(l);

	
	
	

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
	string physical_address;
	
	
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
	
	bool fault = false;
	int write_back_flag = 0;
	
	int page_index = stoi(page_num, 0, 16);
	
	if (page_table[page_index].v == 0)
	{
		printf("Fatal Error - page number is not valid");
		exit(-1);
	}
	else
	{
			// Check for page fault
		if(page_table[page_index].v == 1 and page_table[page_index].p == 0)
		{
			fault = true;
			page_faults += 1;
			write_back_flag = process_page_fault(page_index);
		}
		
	

		if (operation.find("R") != string::npos)
		{
			page_table[page_index].r = 1;
		}
		else if (operation.find("W") != string::npos)
		{
			page_table[page_index].r = 1;
			page_table[page_index].m = 1;
		}
		
		// Create physical address frame + offset
		int f = page_table[page_index].frame;
		
		stringstream sss;
		if (f < 16){
			sss << hex << '0';
		}
		sss << hex << f;
		
		sss << hex << page_offset;
		physical_address = sss.str();
		
	}
	
	cout << endl << v_address << " " << page_num << " " << operation << " " << page_offset << " ";
	if (fault)
	{
		cout << "F "; 
	}
	else
	{
		cout << "  ";
	}
	
	if (write_back_flag == 1)
	{
		cout << "B ";
	}
	else
	{
		cout << "  ";
	}
	
	cout << physical_address << endl;
	

	if (DEBUG_MODE == true)
	{
		print_table();
	}
	

	
	
}

void print_counts()
{
	cout << endl;
	cout << "Number of read operations: " << read_opps << endl;
	cout << "Number of write operations: " << write_opps << endl;
	cout << "Number of page faults: " << page_faults << endl;
	cout << "Number of write backs: " << write_backs << endl; 
	
}

void parse_frames(string line)
{
	vector<string> result;
	istringstream iss(line);

	for(string str; iss >> str; )
	{
		result.push_back(str);
	}	
	
	free_frames = stoi(result[0]);
	
	for (int i = 1; i <= free_frames; i++)
	{
		int number = stoi(result[i], 0, 16);
		frames.push(number);
	
	}

}

void parse_pages(string line)
{
	vector<string> result;
	istringstream iss(line);

	for(string str; iss >> str; )
	{
		result.push_back(str);
	}	
	valid_pages = stoi(result[0]);
	for (int i = 1; i <= valid_pages; i++)
	{
		int page_num = stoi(result[i], 0, 16);
		page_table[page_num].v = 1;
	}
	
}

int process_page_fault(int page_index)
{
	
	int write_back = 0;
	
	if (free_frames < 1)
	{
		int victim_page = -1;
		while(victim_page < 0)
		{
			
			for(int i = 0; i < max_size+1; i++)
			{
				
				if (page_table[i].v == 1)
				{
					
					if(page_table[i].p == 1)
					{
						if(page_table[i].frame == frames.front())
						{
							victim_page = i;
							break;

						}
					}
						
				}

			}
			
		}
		
		// Write Back?
		if (page_table[victim_page].m == 1)
		{
			write_back = 1;
			write_backs += 1;
		}
		
		// Update victim page
		page_table[victim_page].p = 0;
		
		//printf("%d\n",page_table[victim_page].frame);
		frames.push(page_table[victim_page].frame);
		free_frames += 1;

	}
	
	
	int allocated_frame = frames.front();
	frames.pop();
	free_frames -= 1;
	frames.push(allocated_frame);
	
	page_table[page_index].p = 1;
	page_table[page_index].r = 0;
	page_table[page_index].m = 0;
	page_table[page_index].frame = allocated_frame;
	
	
	return write_back;

}































