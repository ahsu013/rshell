#include <cstddef>
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
	bool firsterror  = false; 
	cout << getlogin() << "$ "; 
	string b ; 
	getline(cin, b); 
	b = b.substr(0, b.find("#")); 
	//if (b.find("exit")!=string::npos)
	//	break;
	char * cstring = new char[b.size()+1];
	strcpy(cstring,b.c_str());  	
	bool simi = false;
	bool aand = false; 
	bool oor = false; 

	if (b.find(";")!= string::npos)
		simi = true;
	if (b.find("&&")!= string::npos)
		aand = true; 
	if (b.find("||")!= string::npos)
		oor = true; 
	vector <string> cmd; 
	if (b.find(";")!=string::npos)
	{
		simi = true; 
		char *token = strtok(cstring, ";");
		if (token==NULL)
			firsterror = true; 
		else if (strcmp(token, "exit") == 0)
			break; 	
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
		if (token == NULL)
			firsterror = true; 
		else if (strcmp(token, "exit") == 0)
                        break;
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
		if (token == NULL)
			firsterror  = true; 
		else if (strcmp(token, "exit") == 0)
                        break;
		while (token!=NULL)
		{
			cmd.push_back(token);
			token = strtok (NULL, "||"); 
		}
	}
	else
	{
		if (strcmp(b.c_str(), "exit") == 0)
                        break;
		cmd.push_back(b);
	}
	if (firsterror)
	{	
		cout << "syntax Error" << endl; 
	}
	for (unsigned int i = 0 ; i < cmd.size() ; i++) // for each command
	{
		if (firsterror)
		{
			cout << "syntax Error" << endl;
			break;
		}
		
		if ((simi && aand) || (simi && oor) || (aand && oor))
		{
			cout << "Syntax Error" << endl; 
			break; 
		}
		vector< string> subcmd; 
		unsigned int cmdsize = cmd.at(i).size()+1;

		char *ccstring = new char[cmdsize]; 
		strcpy(ccstring, cmd.at(i).c_str()); 


		char *token = strtok(ccstring, " ");
		while (token!=NULL)
		{
			subcmd.push_back(token);
			token = strtok (NULL, " "); 
		}
		unsigned int argvsize= subcmd.size()+1; 
		char **argv;
		argv = new char*[argvsize]; 
		for (unsigned int j = 0 ; j < subcmd.size() ; j++)
		{
			argv[j] = new char[subcmd.at(j).size()+1];
			strcpy (argv[j],subcmd.at(j).c_str());  
		}
		argv[subcmd.size()] = 0; 
		int pid = fork();
		if (pid == 0)
		{
			if (cmd.at(i).find_first_not_of(" ") == string::npos && cmd.at(i).size() >=1)
			{	
				cout << "Syntax Error "<< endl; 
				exit(1); 
			}
			execvp( argv[0], argv); 
			perror("execvp");
	
		exit(1);
		}
		int success;
		if (pid == -1)
		{
			perror("fork); exit(1)"); 
		}
		waitpid(pid, &success, 0);
		if (!letsgedit(simi, aand, oor, success))
			break;
	}
	}
	return 0; 

}

