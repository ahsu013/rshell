#include <iostream>
#include <time.h>       /* time_t, struct tm, time, localtime */

#include <grp.h>
#include <pwd.h>

#include <sys/stat.h>

#include <iomanip>

#include <stdlib.h>  
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
using namespace std;

#include <vector> 
#include <algorithm>    // std::sort

#include <ctype.h>
#include <boost/algorithm/string.hpp>   


#define FLAG_a 1
#define FLAG_l 2
#define FLAG_r 4
#define FLAG_R 8

bool compare(string a , string b)
{
	boost::algorithm::to_lower(a);
	boost::algorithm::to_lower(b); 
	string::iterator bi = b.begin(); 
for (string::iterator ai = a.begin(); (ai!=a.end() || bi!=b.end()) ; ai++, bi++)
	{
		if (*ai == '.'&& ai == a.begin())
			ai++;
		if (*bi == '.'&& bi == b.begin())
			bi++;
		if (*ai == *bi)
			continue; 
		else
			return *ai<*bi;
	}
	return a<b; 

}

int highestchar(vector<char*> filenamelist)
{
	unsigned a = 0 ; 
	for (unsigned i = 0 ; i < filenamelist.size() ; i++)
	{
		unsigned charcount=0 ; 
		for (int j = 0 ; filenamelist.at(i)[j]!='\0'; j++)
			charcount++;
		if (charcount>a)	
			a = charcount; 
	}
	return a; 
}
			
void dothething(int flags, char* filepath)
{
	DIR *dirp;
	struct dirent *dp;
    if ((dirp = opendir(filepath)) == NULL) {
        perror(filepath);
        return;
    }

	vector <char*> filenames; 
    do {
        errno = 0;
        if ((dp = readdir(dirp)) != NULL) 
            filenames.push_back(dp->d_name);
    } while (dp != NULL);
if (errno != 0)
        perror("error reading directory");


	sort(filenames.begin(),filenames.end(), compare);  
	vector <char*> foldernames; 
	vector <struct stat> statbuff; 
 
	if (!(flags & FLAG_a))
		for(unsigned i = 0; i < filenames.size() ; i++)
		{
			if (filenames.at(i)[0]== '.')
			{
				filenames.erase(filenames.begin()+i); 
				i--;
			}	
		}
	if((flags & FLAG_r))
	{
		reverse(filenames.begin(), filenames.end()); 
		reverse(statbuff.begin(), statbuff.end()); 
	}
	for (unsigned i = 0; i < filenames.size(); i++)
	{

		struct stat buff;
		string dat = filenames.at(i); 
		if (flags & FLAG_a)
			if ((filenames.at(i)[0] == '.' && filenames.at(i)[1] == '\0')|| strcmp(const_cast<char*>(dat.c_str()), "..") == 0)
			{
				 stat(filenames.at(i) ,&buff); 
				statbuff.push_back(buff);
				continue; 
			}
		string temp = filepath; 
		string path; 
		if (temp.length()!=1)
		{	
		path = temp.substr(2); 
		path+= '/'; 
		path+= filenames.at(i); 
		}
		else
			path = filenames.at(i); 
		int check = stat(const_cast<char*>(path.c_str()), &buff);
		if (check == 0)
		{	
			statbuff.push_back(buff); 
			if(S_ISDIR(buff.st_mode))
			{
				foldernames.push_back(filenames.at(i));
			 }
		}
		else
			perror(path.c_str()); 
	}
	int i =0; 
	int size = filenames.size(); 
	int count = highestchar(filenames); 
	int skipcount = 0; 
 
	if (flags & FLAG_R)
		cout << filepath<< ':' << endl; 
	off_t filesize; 
	for (unsigned i = 0 ; i < statbuff.size() ; i++)
	{
		if (i == 0)
			filesize = statbuff.at(i).st_size; 
		else if (filesize< statbuff.at(i).st_size)		
			filesize = statbuff.at(i).st_size;
	}
	int maxfilesize = 1; 
	for (maxfilesize = 0 ; filesize > 0 ; maxfilesize++)
		filesize/=10; 
	//Output for short format 
	if (!(flags & FLAG_l))
	{	while(i != size) {
		if (skipcount> (80/(count+3)))
		{
			cout << endl; 
			skipcount= 0 ; 
		}
		cout << setw( count+3) << left << filenames.at(i) << "  ";	
			i++; 
			skipcount++;
		}
		cout << endl ;  
	}
	//output for long format
	else 
	{
		cout << "Total: " << filenames.size() << endl ; 
		cout << "statbuff :" << statbuff.size() << endl; 
		int buffcount = 0; 
		while(i!=size)
		{
			switch(statbuff.at(i).st_mode & S_IFMT)
			{
				default: cout << '?'; break; 
				case S_IFBLK: cout << 'b';break; 
				case S_IFCHR: cout << 'c'; break; 
				case S_IFIFO: cout << 'p'; break;
				case S_IFREG: cout << '-'; break; 
				case S_IFDIR: cout << 'd'; break; 
				case S_IFLNK: cout << 'l'; break; 
				case S_IFSOCK: cout << 's'; break; 
			}
			if(statbuff.at(i).st_mode &S_IRUSR) 
				cout << 'r';  	
			else
				cout << '-'; 
			if (statbuff.at(i).st_mode & S_IWUSR)
				cout << 'w'; 
			else 
				cout << '-'; 
			if (statbuff.at(i).st_mode & S_IXUSR)
				cout << 'x';
			else
				cout << '-'; 
			  if(statbuff.at(i).st_mode &S_IRGRP) 
                                cout << 'r';    
                        else
                                cout << '-';
                        if (statbuff.at(i).st_mode & S_IWGRP)
                                cout << 'w';
                        else
                                cout << '-';
                        if (statbuff.at(i).st_mode & S_IXGRP)
                                cout << 'x';
                        else
                                cout << '-';  
			 if(statbuff.at(i).st_mode &S_IROTH)
                                cout << 'r';
                        else
                                cout << '-';
                        if (statbuff.at(i).st_mode & S_IWOTH)
                                cout << 'w';
                        else
                                cout << '-';
                        if (statbuff.at(i).st_mode & S_IXOTH)
                                cout << 'x';
                        else
                                cout << '-'; 
			cout << ' ' << statbuff.at(i).st_nlink; 
			cout << ' ' << getpwuid(statbuff.at(i).st_uid)->pw_name;
			cout << ' ' << getgrgid(statbuff.at(i).st_gid)->gr_name;
			cout << ' ' <<setw(maxfilesize) <<right<< statbuff.at(i).st_size;
			string date = asctime(localtime(const_cast<time_t*>(&statbuff.at(i).st_mtime)));
			date= date.substr(4, 12); 
			cout <<' ' <<date; 
			cout << " "; 
			cout << filenames.at(i) << endl; 
			i++; 
			buffcount++; 
		}	
		cout << endl << endl; 
	}
	while ((flags&FLAG_R) && foldernames.size() != 0) 
	{
		for (unsigned i = 0; i < foldernames.size(); i++)
		{
			string cupath = filepath; 
			cupath+= '/'; 
			cupath+=foldernames.at(i); 
			dothething(flags,const_cast<char*>(cupath.c_str()));  

			foldernames.erase(foldernames.begin()+i);
			i--; 
		}
	} 	
        (void) closedir(dirp);

}

int main(int argc, char** argv)
{
	int flags=0;

	for (int i=1; i<argc; i++)
		if (argv[i][0] == '-')
		{
			for(int j=1; argv[i][j] !=0; j++)
			{
				if(argv[i][j] =='a')
					flags |= FLAG_a; 
				else if(argv[i][j] =='R')
					flags |= FLAG_R;
				else if(argv[i][j] == 'r')
					flags |= FLAG_r; 
				else if(argv[i][j] == 'l')
                                        flags |= FLAG_l;
				else { 
					cout <<"No matching flag" << endl;
					exit(1); 
				}
			}
		}	
		else
		{	
			cout <<"Syntax Error" << endl; 
			exit(1); 
		}
	char * filepath;
	filepath = new char [4]; 
	strcat(filepath, "."); 
	dothething(flags, filepath);
	delete filepath; 
	return 0; 
}
