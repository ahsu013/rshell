#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include "Timer.h"
#include <fstream>
#include <ostream>
using namespace std;


//use ifstream in.get() and out.put()
void method1(char* input, char* output)		
{
	ifstream in(input);
	if(in.is_open())
	{
		ofstream out(output); 
		while (in.good())
		{
			char c = in.get(); 
			if(in.good()) 
				out.put(c); 
		}
		in.close();
		out.close(); 
	}
	else
		cout << "Cannot open file" << endl; 
}
//use system call read() and write()
void method2(char* input, char* output)
{
	char *c = new char [1];
	int fd, sz,fd2, sz2; 

	fd = open(input,  O_RDONLY);
	if (fd<0)
	{
		perror("open");
		exit(1); 	
	}
	fd2 = open(output, O_WRONLY | O_CREAT ,0644 ); 
	if (fd < 0)
	{
		perror("open output");
		exit(1);
	}
	
	sz = read(fd, c, 1);// 1 byte read
	if (sz <0)
	{
		perror("read error");
		exit(1);
	}
	while(sz>0)
	{
		sz2 = write (fd2, c, 1); //write 1 byte to output
		if(sz2< 0 )
		{
			perror ("write error");
			exit (1);
		}
		sz = read(fd, c, 1);

		//then read again
		//repeat
	}
	if (sz<0)
	{
		perror("read error");
		exit(1);
	}

	sz2 = write(fd2, 0, 1); 
	close(fd);
	close(fd2);
	delete c; 

}
void method3(char* input, char* output)
{
	char *c = new char [BUFSIZ];
	int fd, sz,fd2, sz2; 

	fd = open(input,  O_RDONLY);
	if (fd<0)
	{
		perror("open");
		exit(1); 	
	}
	fd2 = open(output, O_WRONLY | O_CREAT ,0644 ); 
	if (fd < 0)
	{
		perror("open output");
		exit(1);
	}
	
	sz = read(fd, c, BUFSIZ);// 1 byte read
	if (sz <0)
	{
		perror("read error");
		exit(1);
	}
	while(sz>0)
	{
		sz2 = write (fd2, c, sz); //write 1 byte to output
		if(sz2< 0 )
		{
			perror ("write error");
			exit (1);
		}
		sz = read(fd, c, BUFSIZ);

		//then read again
		//repeat
	}
	if (sz<0)
	{
		perror("read error");
		exit(1);
	}

	sz2 = write(fd2, 0, 1); 
	close(fd);
	close(fd2);
	delete c; 

}
int main(int argc, char**  argv)
{
	if (argc <4)
		method3(argv[1],argv[2]); 
	else if( argv[4][2] == 'o') 
	{
		// set up the Timer
		Timer t1;
		double eTime1, eTime2, eTime3;
		
		cout << "Testing method one: in.get() and out.put()" << endl;
		t1.start();
		
		method1(argv[1], argv[2]);

		t1.elapsedTime(eTime1, eTime2, eTime3);
		cout << "Wallclock: " << eTime1 << endl;
		cout << "User time: " << eTime2 << endl;
		cout << "Sys. time: " << eTime3 << endl;

		// Set up Timer 2
		Timer t2;
		cout << "Testing method two: read/write sys calls. Buff sz: 1" << endl;
		t2.start();
		method2(argv[1],argv[2]);
	
		t2.elapsedTime(eTime1, eTime2, eTime3);
		cout << "Wallclock: " << eTime1 << endl;
		cout << "User time: " << eTime2 << endl;
		cout << "Sys. time: " << eTime3 << endl;


		// set up timer 3
			
		Timer t3;
		cout << "Testing method three: read/write sys calls. Buff sz: max" << endl;
		t3.start();
		method3(argv[1], argv[2]);
	
		t3.elapsedTime(eTime1, eTime2, eTime3);
		cout << "Wallclock: " << eTime1 << endl;
		cout << "User time: " << eTime2 << endl;
		cout << "Sys. time: " << eTime3 << endl;
	}

	return 0;

}



 
