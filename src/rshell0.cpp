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
//takes in a string a that contains input or output redirction.
//takes in the mode/flag that indicated wwhat input or output redirction
void inputoutput (string a , int mode)
{
	cout << "Mode:"<<  mode << endl ;
	if((mode&1)==1)
		// only input
		{
		string b; 
		b = a.substr(a.find_last_of('<'));
		if (b.find_first_not_of("< ") == string::npos)
		{
			cout << "missing file name"<< endl; 
			return;
		}
		b =b.substr(b.find_first_not_of("< "));
		if ((b.find(">") != string::npos) || b.find(' ') != string::npos)
			b= b.substr(0 ,b.find_first_of(" >")); 
		cout << "input:";
		cout << b;  
		cout<< '|' << endl ;
		}
	else if((mode&2)>0)
		{
		string b; 
		b = a.substr(a.find_last_of('<'));
		if (b.find_first_not_of("< ") == string::npos)
		{
			cout << "missing file name"<< endl; 
			return;
		}
		b =b.substr(b.find_first_not_of("< "));
		if ((b.find(">") != string::npos) || b.find(' ') != string::npos)
			b= b.substr(0 ,b.find_first_of(" >")); 
		cout << "input:";
		cout << b;  
		cout<< '|' << endl ;
		}
		//extra credit input
	if((mode&4) >0)
	{
		string b; 
		b = a.substr(a.find_last_of('>'));
		if (b.find_first_not_of("> ") == string::npos)
		{
			cout << "missing file name"<< endl; 
			return;
		}
		b =b.substr(b.find_first_not_of("> "));
		if(b.find(' ') != string::npos || (b.find('<') != string::npos))
			b= b.substr(0 ,b.find_first_of(" <"));	
		cout << "output:";
		cout << b;  
		cout <<'|' <<  endl; 
	}
		//regular ouput redirction
	
	else if((mode&8) >0)
	{
		string b; 
		b = a.substr(a.find_last_of('>'));
		if (b.find_first_not_of("> ") == string::npos)
		{
			cout << "missing file name"<< endl; 
			return;
		}
		b =b.substr(b.find_first_not_of("> "));
		if(b.find(' ') != string::npos || (b.find('<') != string::npos))
			b= b.substr(0 ,b.find_first_of(" <"));	
		cout << "output:";
		cout << b;  
		cout <<'|' <<  endl; 

	}
		//double output redirction
	
}

// each int return represent a case of which < > >> <<< is found.
// return 0 if none or mismatching
int findthesign ( string a)
{
	int yoloflags =0 ; 
	if (a.find('<') == string::npos || a.find('>') == string::npos)
		yoloflags  = 0 ;
	if( a.find('<') != string::npos && (a.find_last_of('<') == a.find_first_of('<')))
		yoloflags  |= 1; 
	else if (a.at(a.find_first_of('<')+1) == '<' && (a.find_last_of('<') == a.find_first_of('<')+2))
		yoloflags  |= 2; 
	if (a.find('>') != string::npos && (a.find_last_of('>') == a.find_first_of('>')))
		yoloflags |= 4; 
	else if (a.at(a.find_first_of('>')+1) == '>' && (a.find_last_of('>') == a.find_first_of('>')+1))	
		yoloflags |=8; 
	return yoloflags; 
}
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
int prompt(vector<string> *cmd)
{
	bool firsterror  = false; 
	cout << getlogin() << "$ "; 
	string b ; 
	getline(cin, b); 
	b = b.substr(0, b.find("#")); 
	//if (b.find("exit")!=string::npos)
	//	break;
	char * cstring = new char[b.size()+1];
	strcpy(cstring,b.c_str());  	
	int flags = 0 ; 
	if (b.find(";")!= string::npos)
		flags = flags | 1; 
	if (b.find("&&")!= string::npos)
		flags = flags | 2; 
	if (b.find("||")!= string::npos)
		flags = flags | 4; 
	if (b.find(";")!=string::npos)
	{
		flags = flags | 1; 
		char *token = strtok(cstring, ";");
		if (token==NULL)
			firsterror = true; 
		else if (strcmp(token, "exit") == 0)
			exit(1); 	
		while (token!=NULL)
		{
			cmd->push_back(token);
			token = strtok (NULL, ";"); 
		}
	}
	else if (b.find("&&")!= string::npos)
	{	
		flags = flags | 2; 
		char *token = strtok(cstring, "&&");
		if (token == NULL)
			firsterror = true; 
		else if (strcmp(token, "exit") == 0)
                        exit(1);
		while (token!=NULL)
		{
			cmd->push_back(token);
			token = strtok (NULL, "&&"); 
		}
	}
	else if (b.find("||")!= string::npos)
	{
		flags = flags | 4; 
		char *token = strtok(cstring, "||");
		if (token == NULL)
			firsterror  = true; 
		else if (strcmp(token, "exit") == 0)
                        exit(1);
		while (token!=NULL)
		{
			cmd->push_back(token);
			token = strtok (NULL, "||"); 
		}
	}
	else
	{
		if (strcmp(b.c_str(), "exit") == 0)
                        exit(1);
		cmd->push_back(b);
	}
	if (firsterror)
	{	
		flags |= 8; 
		cout << "syntax Error" << endl; 
	}
	return flags; 
}
int run_cmd(vector <string> *cmd, int flags)
{
	bool simi = 0 ;
	bool oor = 0 ; 
	bool aand = 0 ; 
	if( ( flags & 1) == 1)
		simi = 1; 
	if ((flags & 2) == 1)
		aand = 1; 
	if ((flags | 4) == 1)
		oor = 1; 
	for (unsigned int i = 0 ; i < cmd->size() ; i++) // for each command
	{
		if ((flags | 8) == 1)
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
		unsigned int cmdsize = cmd->at(i).size()+1;

		char *ccstring = new char[cmdsize]; 
		strcpy(ccstring, cmd->at(i).c_str()); 


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
			if (cmd->at(i).find_first_not_of(" ") == string::npos && cmd->at(i).size() >=1)
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
	
	return 0; 

}
int main()
{
	while (1) {
//	string a ;
//	 getline(cin,a);
	vector< string> *cmd= new vector<string> ; 
	int flags = prompt(cmd); 
	cout << flags << endl; 
int mode=  findthesign(cmd->at(0)); 
	inputoutput(cmd->at(0), mode); 
//	run_cmd(cmd, flags);
//	delete cmd;
	} 
	return 0;
}
