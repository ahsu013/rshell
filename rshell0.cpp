#include <pwd.h>
#include <cstdlib>
#include <string>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <iostream>

using namespace std; 

bool letsgedit(bool simi, bool aand, bool oor, int status)
{
	if (simi)
		return true; 
	if (aand)
	{
		if (status ==0)
			return true; 
		else 
			return false;
	}
	if (oor)
	{
		if (status ==0)
			return false; 
		else
			return true; 
	}
	return true; 
}
int main()
{
while(1) {
	cout << getlogin() << "$ "; 
	string b ; 
	getline(cin, b); 
	b = b.substr(0, b.find("#")); 
	if (b.find("exit")!=string::npos)
		break;
	char * cstring = new char[b.size()+1];
	strcpy(cstring,b.c_str());  	
	bool simi = false;
	bool aand = false; 
	bool oor = false; 

	vector <string> cmd; 
	if (b.find(";")!=string::npos)
	{
		simi = true; 
		char *token = strtok(cstring, ";");
		while (token!=NULL)
		{
			cmd.push_back(token);
			token = strtok (NULL, ";"); 
		}
	}
	else if (b.find("&&")!= string::npos)
	{	
		aand = true; 
		char *token = strtok(cstring, "&&");
		while (token!=NULL)
		{
			cmd.push_back(token);
			token = strtok (NULL, "&&"); 
		}
	}
	else if (b.find("||")!= string::npos)
	{
		oor = true; 
		char *token = strtok(cstring, "||");
		while (token!=NULL)
		{
			cmd.push_back(token);
			token = strtok (NULL, "||"); 
		}
	}
	else
		cmd.push_back(b);


	for (int i = 0 ; i < cmd.size() ; i++) // for each command
	{
		vector< string> subcmd; 
		char ccstring [cmd.at(i).size()+1]; 
		strcpy(ccstring, cmd.at(i).c_str()); 


		char *token = strtok(ccstring, " ");
		while (token!=NULL)
		{
			subcmd.push_back(token);
			token = strtok (NULL, " "); 
		}
		char *argv[subcmd.size()+1];
		for (int j = 0 ; j < subcmd.size() ; j++)
		{
			argv[j] = new char[subcmd.at(j).size()+1];
			strcpy (argv[j], subcmd.at(j).c_str()); 
		}
		argv[subcmd.size()] = 0; 
		int pid = fork();
		if (pid == 0)
		{
			execvp( argv[0], argv); 
			perror("execvp");
	
		exit(1);
		}
		int success;
		waitpid(pid, &success, 0);
		if (!letsgedit(simi, aand, oor, success))
			break;
	}
	}
	return 0; 

}

