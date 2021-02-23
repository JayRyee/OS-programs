
#include <stdio.h>
#include <stdlib.h>
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

void print_cache();

struct record{
	
	bool V_bit = 0;
	bool M_bit = 0;
	unsigned int tag = 0;
	unsigned int line = 0;
	int data[16];
	
} rec;


record cache[8];
unsigned short int RAM[1048576];

bool DEBUG_MODE = false;

void parse_line(string line)
{
	vector<string> result;
	istringstream iss(line);

	for(string str; iss >> str; )
	{
		result.push_back(str);
	}	
	
	unsigned int hex_add;
	stringstream ss;
	ss << std::hex << result[0];
	ss >> hex_add;
	ss.str("");
	string opp = result[1];
	
	unsigned int offset;
	unsigned int l;
	unsigned int tag;
	unsigned int ram_add;
	unsigned int ram_old;
	
	offset = (hex_add & 0x0000F);
	l = (hex_add & 0x70) >> 4;
	tag = (hex_add & 0xFFF80) >> 7;
	ram_add = (hex_add & 0xFFFF0);
	ram_old = l * 10;
	ram_old = ram_old + 20000;

	int miss = 1;
	
	record r = cache[l];
	if(!(cache[l].V_bit == 1 and cache[l].tag == tag))
	{
		miss = 0;
		if(cache[l].V_bit == 1 and cache[l].M_bit == 1)
		{
			unsigned int old_tag = cache[l].tag;
			unsigned int old_l = cache[l].line;
			unsigned int old_add;
			old_add = (old_tag << 7);
			old_l = (old_l << 4);
			old_add = (old_add | old_l);
			// Copy old block from cache block to lower level
			
			
			for(int i = 0; i < 16; i++)
			{
				
				RAM[old_add + i] = cache[l].data[i];
			}
		}
		// copy new block from lower level to cache block
		for(int i = 0; i < 16; i++)
		{
			cache[l].data[i] = RAM[ram_add+i];
		}
		cache[l].M_bit = 0;
		cache[l].tag = tag;
		cache[l].V_bit = 1;
		cache[l].line = l;
	}
	char res = 'M';
	if (miss == 1)
	{
		res = 'H';
	}
	
	if(opp.find("R") != string::npos)
	{
		// copy item from cache block to cpu reg
		int r1, r2, r3, r4;
		r1 = cache[l].data[offset];
		r2 = cache[l].data[offset];
		r3 = cache[l].data[offset];
		r4 = cache[l].data[offset];
		
		
		printf("\n%05x %c %04x %01x %01x %c ", hex_add, opp[0], tag, l, offset, res);
		if (r1 < 10)
		{
			cout << '0' << r1 << ' ';
		}
		else
		{
			cout << r1 << ' ';
		}
		
		if (r2 < 10)
		{
			cout << '0' << r2 << ' ';
		}
		else
		{
			cout << r2 << ' ';
		}
		
		if (r3 < 10)
		{
			cout << '0' << r3 << ' ';
		}
		else
		{
			cout << r3 << ' ';
		}
		
		if (r4 < 10)
		{
			cout << '0' << r4 << ' ';
		}
		else
		{
			cout << r4 << ' ';
		}

		cout << endl;
		
	}
	else if (opp.find("W") != string::npos)
	{
		// Copy item form cpu reg to cache block
		int r1, r2, r3, r4;
		r1 = stoi(result[2]);
		r2 = stoi(result[3]);
		r3 = stoi(result[4]);
		r4 = stoi(result[5]);

		cache[l].data[offset] = stoi(result[2]);
		cache[l].data[offset+1] = stoi(result[3]);
		cache[l].data[offset+2] = stoi(result[4]);
		cache[l].data[offset+3] = stoi(result[5]);

		cache[l].M_bit = 1;
		printf("\n%05x %c %04x %01x %01x %c ", hex_add, opp[0], tag, l, offset, res);
		if (r1 < 10)
		{
			cout << '0' << r1 << ' ';
		}
		else
		{
			cout << r1 << ' ';
		}
		
		if (r2 < 10)
		{
			cout << '0' << r2 << ' ';
		}
		else
		{
			cout << r2 << ' ';
		}
		
		if (r3 < 10)
		{
			cout << '0' << r3 << ' ';
		}
		else
		{
			cout << r3 << ' ';
		}
		
		if (r4 < 10)
		{
			cout << '0' << r4 << ' ';
		}
		else
		{
			cout << r4 << ' ';
		}

		cout << endl;
		
	}
	
	
	if(DEBUG_MODE == true)
	{
		printf("\n");
		cout << "     V M Tag   Block Contents:" << endl; 
		cout << "     ---------------------------------------------------------" << endl;

		print_cache();
	}

}

int main(int argc, char *argv[]){
	

	ifstream in_file;
	string file;
	for (int i = 1; i < argc; i++)
	{
		string str = argv[i];
		
		if(str.find("-refs") != string::npos)
		{
			file = argv[i+1];
		}
		
		if(str.find("-debug") != string::npos)
		{
			DEBUG_MODE = true;
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
	
	if(DEBUG_MODE == true)
	{
		printf("\n");
		cout << "     V M Tag   Block Contents:" << endl; 
		cout << "     ---------------------------------------------------------" << endl;

		print_cache();
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
	

	in_file.close();
	cout << endl;
	
	for(int i = 0; i < 8; i++)
	{
				
		cout << 20000 + i*10 << ": ";
		
		for(int j = 0; j < 16; j++)
		{
			if (RAM[131072+j+i*16] < 10)
			{
				cout << '0' << RAM[131072+j+i*16] << " ";
			}
			else
			{
				cout << RAM[131072+j+i*16] << " ";
			}
		}
		
		cout << endl;
	}
	
	
	return 0;
}

void print_cache()
{
	printf("\n");
	for(int i = 0; i < 8; i++)
	{
		record re = cache[i];
		printf("[%d]: %d %d %04x ", i, re.V_bit, re.M_bit, re.tag);		
		int* v = re.data;
		for(int j=0; j < 16; j += 1)
		{
			if(re.data[j] == 0)
			{
				printf("00 ");
			}
			else
			{
				printf("%0i ", re.data[j]);
			}
		}
		printf("\n");
		//for(int j = 0; j < 32; j += 2)
		//{
		//	printf("%x%x ", re.data[j], re.data[j+1]);
		//}
		//printf("\n");
	}

	
}
