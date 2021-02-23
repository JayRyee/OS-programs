
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

using namespace std;


struct record{
	
	bool V_bit = 0;
	bool M_bit = 0;
	unsigned int tag = 0;
	unsigned int line = 0;
	int data[32];
	
} rec;


record cache[8];

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
	
	offset = (hex_add & 0x0000F);
	l = (hex_add & 0x70) >> 4;
	tag = (hex_add & 0xFFF80) >> 7;
		
	printf("\n%05x %c %04x %01x %01x\n", hex_add, opp[0], tag, l, offset);
	// Read operation
	if(opp.find("R") != string::npos)
	{
		record r = cache[l];
		
		// Miss
		if (r.V_bit == 0)
		{
			//r.V_bit = 1;
			//r.M_bit = 0;
			//r.tag = tag;
			//cache[l] = r;
		}
		// Hit
		else if(r.V_bit == 1 && r.tag == tag)
		{
			// Processing read
		}
		// Miss, but V = 1
		else if(r.V_bit == 1 && r.tag != tag)
		{
				//r.V_bit = 1;
				//r.M_bit = 0;
				//r.tag = tag;
				//cache[l] = r;
		}


	}
	else if (opp.find("W") != string::npos)
	{

	
		record r = cache[l];
		if (r.V_bit == 0)
		{
			//r.V_bit = 1;
			//r.M_bit = 0;
			//r.tag = tag;
			//cache[l] = r;
		}
		else if (r.V_bit == 1 && tag == r.tag)
		{
			unsigned int one, two, three, four, five, six, seven, eight;
			string s1, s2, s3, s4, s5, s6, s7, s8;
			s1 = result[2][0];		
			s2 = result[2][1];
			s3 = result[3][0];
			s4 = result[3][1];
			s5 = result[4][0];		
			s6 = result[4][1];
			s7 = result[5][0];
			s8 = result[5][1];
			
			stringstream s;
			s << hex << s1;
			s >> one;
			s.clear();
			
			s << hex << s2;
			s >> two;
			s.clear();
			
			s << hex << s3;
			s >> three;
			s.clear();
			
			s << hex << s4;
			s >> four;
			s.clear();
			
			s << hex << s5;
			s >> five;
			s.clear();
			
			s << hex << s6;
			s >> six;
			s.clear();
			
			s << hex << s7;
			s >> seven;
			s.clear();
			
			s << hex << s8;
			s >> eight;
			s.clear();


			int pos = offset * 2;
			//r.data[pos] = one;
			//r.data[pos+1] = two;
			//r.data[pos+2] = three;
			//r.data[pos+3] = four;
			//r.data[pos+4] = five;
			//r.data[pos+5] = six;
			//r.data[pos+6] = seven;
			//r.data[pos+7] = eight;


			//r.M_bit = 1;
			//cache[l] = r;
		}
	}

	
	if(DEBUG_MODE == true)
	{
		printf("\n");
		cout << "     V M Tag   Block Contents:" << endl; 
		cout << "     ---------------------------------------------------------" << endl;

		
		for(int i = 0; i < 8; i++)
		{
			record re = cache[i];

			printf("[%d]: %d %d %04x ", i, re.V_bit, re.M_bit, re.tag);
			
			//unsigned int v = re.data;
			for(int j = 0; j < 32; j += 2)
			{
					printf("%x%x ", re.data[j], re.data[j+1]);
			}
			printf("\n");
		}
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
	
	//for(int i = 0; i < cache.size(); i++)
	//{
			
	//}
	if(DEBUG_MODE == true)
	{
		printf("\n");
		cout << "     V M Tag   Block Contents:" << endl; 
		cout << "     ---------------------------------------------------------" << endl;

		
		for(int i = 0; i < 8; i++)
		{
			record re = cache[i];
	
			printf("[%d]: %d %d %06d", i, re.V_bit, re.M_bit, re.tag);
			
			//unsigned int v = re.data;
			for(int j = 0; j < 32; j += 2)
			{
					printf("%u%u ", re.data[j], re.data[j+1]);
			}
			printf("\n");
		}
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
	
	
	printf("\n");
	for(int i = 0; i < 8; i++)
	{
		record re = cache[i];
		printf("[%d]: %d %d %04x ", i, re.V_bit, re.M_bit, re.tag);		
		//unsigned int v = re.data;
		for(int j = 0; j < 32; j += 2)
		{
			printf("%x%x ", re.data[j], re.data[j+1]);
		}
		printf("\n");
	}

	in_file.close();
	return 0;
}
