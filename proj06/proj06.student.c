/*
 * 	Project 06 - Jon Ryan
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <errno.h>
#include <string>
#include <map>
#include <iterator>
#include <fstream>
#include <sstream>
#include <pthread.h>
#include <semaphore.h>
#include <memory>
#include <utility>
#include <iomanip>

using namespace std;

void make_item(string l); 
void *producer(void *arg);
void *consumer(void *arg);
//struct make_item(vector<string> arr[]);


// Struct used to define transactions for an account
struct transactions{
	int account;
	int deposit;	// 0 for deposit, 1 for withdrawl
	float amount;
	int num;
} trans;


struct transactions* BUFFER;
sem_t mutex;	// Buffer Access
sem_t n;		// Prevents underflow -> # of items in buffer
sem_t e;		// buffersize,. prevents overflow
int in = 0;
int out = 0;
int buffersize = 20;
int producers = 1;


map<int, float> account_map; // ACCOUNT DICTIONARY

int main(int argc, char *argv[]){
	
	
	int records = 5;
	
	
	for (int i = 1; i < argc-1; i++)
	{
		std::string str = argv[i];
		string val = argv[i+1];
		
		int val2 = stoi(val);
		if(str.find("-p") != string::npos)
			{
				
				if(val2 < 11 && val2 > 0)
				{
					producers = val2;
					i = i+1;
				}
			}
		else if(str.find("-b") != string::npos)
			{
				if(val2 < 21 && val2 > 0)
				{
					records = val2;
					i = i+1;
				}
			}
		
	}
	
	BUFFER = (transactions*) malloc(records * sizeof(struct transactions));
	buffersize = records;

	
	// Read account.old input file
	ifstream in_file;
	in_file.open("accounts.old");
	string line;
	if(!in_file)
	{
			printf("Could not open \'accounts.old\' for reading.\n");
			//return NULL;
	}
	
	int t_count = 0;
	while(!in_file.eof())
	{
		getline(in_file, line);
		vector<string> results;
		istringstream iss(line);
		
		for(string str; iss >> str; )
		{
			results.push_back(str);
		}
		

		
		if(results.size() >= 2)
		{
			string s = results[1];
			const char* c = results[1].c_str();
			float flt;
			sscanf(c, "%f", &flt);
			account_map[stoi(results[0])] = flt;
			t_count += 1;
		}	
		
		
	}
	
	pthread_t producer_thread[t_count];
	pthread_t consumer_thread;
	
	sem_init(&mutex, 0, 1);
	sem_init(&n, 0, 0);
	sem_init(&e, 0, records);
	
	long t;
	for (t = 0; t < t_count; t++)
	{
		int rc = pthread_create(&producer_thread[t], NULL, producer, (void *) t);
		
		//f(rc != 0)
		//{
			//printf("ERROR; return code from pthread_create() is %d\n", rc);
			//exit(-1);
		//}
	
	}
	
	int ct = pthread_create(&consumer_thread, NULL, consumer, NULL);
	
	
	for(t = 0; t < t_count; t++)
	{
		if (pthread_join(producer_thread[t], NULL))
		{
				cout << "***Error joining thread***" << endl;
				exit(-2);
		}
	}
	
	pthread_join(consumer_thread, NULL);
	
	//cout << account_map[1111] << endl;
	//cout << account_map[3333] << endl;
	//cout << account_map[7777] << endl;
	//cout << t_count << endl;
	//cout << account_map["6666"] << endl;

	
	
	sem_destroy(&e);
	sem_destroy(&n);
	sem_destroy(&mutex);
	return 0;
	
	
}

void *producer(void *arg)
{
	long tnum = (long) arg;
	char result[1];
	sprintf(result, "%ld", tnum);

	char in_file_name[7] = {'t', 'r', 'a', 'n', 's', result[0], '\0'};
	//cout << result << endl;
	
	ifstream in_file2;
	in_file2.open(in_file_name);
	
	if(!in_file2)
	{
		
	}
	else
	{

		string line;
		getline(in_file2, line);
		while (!in_file2.eof())
		{
			
			//make_item(line);
			
			
			sem_wait(&e);
			sem_wait(&mutex);
			
			vector<string> res;
			istringstream iss2(line);
			
			string one;
			string two;
			string three;
			
			iss2 >> one;
			iss2 >> two;
			iss2 >> three;
			
			//cout << one << two << three << endl;
			trans.account = stoi(one);
			if (two.find("deposit") != string::npos)
			{
				trans.deposit = 0;
			}
			else
			{
				trans.deposit = 1;
			}

			trans.amount =stof(three);
			trans.num = atoi(result);
			
			
			BUFFER[in] = trans;
			in = (in + 1) % buffersize;
		
			
		
			sem_post(&mutex);
			sem_post(&n);
			
			getline(in_file2, line);

		}
		
		
		sem_wait(&e);
		sem_wait(&mutex);
		
		trans.account = -1;
		BUFFER[in] = trans;
		in = (in + 1) % buffersize;
		
		
		sem_post(&mutex);
		sem_post(&n);
		
	}
	in_file2.close();
	pthread_exit(0);
}

void *consumer(void *arg)
{
	
	ofstream outfile;
	outfile.open("accounts.new");
	
	int p_count = 0;
	while(p_count <= producers)
	{
	
			sem_wait(&n);
			sem_wait(&mutex);
			
			// take from buffer
			
			trans = BUFFER[out];
			out = (out + 1) % buffersize;
			
			int thread_num = trans.num;
			int acc_num = trans.account;
			float curr_balance = account_map[acc_num];
			
			if (trans.account == -1)
			{
					p_count += 1;
			}
			
			map<int, float>::iterator iter;
			iter = account_map.find(trans.account);
			if(iter != account_map.end())
			{
				// Account exists already 
				
				string t_type;
				int thread_num = trans.num;
				int acc_num = trans.account;
				float curr_balance = account_map[acc_num];
				float trans_amount = trans.amount;
			
				if (trans.deposit == 0)
				{
					// Deposit
					account_map[trans.account] += trans.amount;
					t_type = "Deposit";
				}
				else 
				{
					// withdraw
					account_map[trans.account] -= trans.amount;
					t_type = "Withdraw";

				}
				
				float updated_balance = account_map[acc_num];
				
			
				if(acc_num >= 0)
				{
					cout << setw(7) <<left<< thread_num <<
							setw(10) << left << acc_num <<
							setw(12) << left << t_type <<
							"$" << setw(12) << left << curr_balance <<
							"$" << setw(10) << left << trans_amount <<
							"$" << setw(10) << left << updated_balance << endl;
				}
			}
			
			
			

			sem_post(&mutex);
			sem_post(&e);
			
			// write data to log file
		
	}
	
	
	outfile.close();
	pthread_exit(0);
}

