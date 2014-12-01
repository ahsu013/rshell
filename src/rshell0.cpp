#include <cstddef>
#include <fcntl.h>

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


int globalvar = -1; 
void hd1 (int sig)
{
	kill(globalvar, SIGTERM);
}
//takes in a string a that contains input or output redirction.
//takes in the mode/flag that indicated wwhat input or output redirction
	
string inputoutput (mode_t *tt , vector<string> *io, string a , int mode,int *flag)
{

	string bad;
	if (mode == 0)
		{
			*flag = 1;
			return bad;
		}
	if((mode&1)>=1)
		// only input
		{
		string b; 
		int d = a.find_last_of('<'); 
		int e =0 ;
		b = a.substr(a.find_last_of('<'));
		if (b.find_first_not_of("< ") == string::npos)
		{
			*flag = 1 ; 
			cout << "missing file name"<< endl; 
			return bad;
		}
		b.find_first_not_of("< "); 
		b =b.substr(b.find_first_not_of("< "));
		if ((b.find(">") != string::npos) || b.find(' ') != string::npos)
{
			b= b.substr(0 ,b.find_first_of(" >")); 
			e = a.find(b)- d;  
}
		if (e!=0)
		a.erase(d,b.size()+e); 
		else
		a.erase(d); 
		io->at(0) = b; 	
//		io[0] =const_cast<char*>( b.c_str()); 
		//int fdi=open(const_cast <char*>(b.c_str()),O_RDONLY);
		//if (fdi<0) {
		//	perror("Read Error"); 
		//	*flag = 1; 
		//	return bad; 
		//}
		//close(0);
		//dup(fdi);

		}
	else if((mode&2)>0)
		{
		string b; 
		b = a.substr(a.find_last_of('<'));
		int d = a.find_first_of('<'); 
		int e =0; 
		if (b.find_first_not_of("< ") == string::npos)
		{
			*flag = 1; 
			cout << "missing file name"<< endl; 
			return bad;
		}
		b =b.substr(b.find_first_not_of("< "));
		if (b.find('"') == string::npos)
		if ((b.find(">") != string::npos) || b.find(' ') != string::npos)
{
			b= b.substr(0 ,b.find_first_of(" >")); 
			e = a.find(b) - d; 
}
		if (b.find('"') !=string::npos)
		b= b.substr(1); 
		b = b.substr(0, b.find('"'));
		e= a.find(b) -d; 
		if(e!=0)
		a.erase(d,b.size()+e+1); 
		else
			a.erase(d+e+1); 
		io->at(2) = b; 	
//		io[0] =const_cast<char*>( b.c_str()); 
//		int fdi=open(const_cast <char*>(b.c_str()),O_RDONLY);
//		if (fdi<0) {
//			perror("Read Error"); 
//			*flag = 1; 
//			return bad; 
//		}
//		close(0);
//		dup(fdi);
//

		}
		//extra credit input
	if((mode&4) >0)
	{
		io->at(3) = "0"; 
		if (isdigit(a.at(a.find_first_of('>')-1)))
		{
			io->at(3) = a.at(a.find_first_of('>')-1);
			a.at(a.find_first_of('>')-1) = ' ' ; 
		}
		string b; 
		b = a.substr(a.find_last_of('>'));
	int d = a.find_last_of('>'); 
	int e = 0; 
		if (b.find_first_not_of("> ") == string::npos)
		{
			*flag = 1; 
			cout << "missing file name"<< endl; 
			return bad;
		}
		b =b.substr(b.find_first_not_of("> "));
		if(b.find(' ') != string::npos || (b.find('<') != string::npos))
{
			b= b.substr(0 ,b.find_first_of(" <"));	
			e= a.find(b)- d; 
}
		if (e!=0) 
		a.erase(d,b.size()+e); 
		else 
		a.erase(d); 
		io->at(1) = b; 
		*tt = (O_WRONLY|O_CREAT|O_TRUNC); //S_IRUSR |S_IWUSR);
//		io[1] = const_cast<char*> (b.c_str()); 
//		int fdo=open(const_cast <char*>(b.c_str()),O_WRONLY|O_CREAT|O_TRUNC, permission);
//		if (fdo<0) {
//			perror("Read Error"); 
//			*flag = 1; 
//			return bad; 
//		}
//		close(1);
//		dup(fdo);


	}
		//regular ouput redirction
	
	else if((mode&8) >0)
	{
		io->at(3) = "0"; 
		if (isdigit(a.at(a.find_first_of('>')-1)))
		{
			io->at(3) = a.at(a.find_first_of('>')-1);
			a.at(a.find_first_of('>')-1) = ' ' ; 
		}
		string b; 
		b = a.substr(a.find_last_of('>'));
		int d = a.find_last_of('>')-1; 
		int e =0 ;
		if (b.find_first_not_of("> ") == string::npos)
		{
			*flag = 1 ;
			cout << "missing file name"<< endl; 
			return bad;
		}
		b =b.substr(b.find_first_not_of("> "));
		if(b.find(' ') != string::npos || (b.find('<') != string::npos))
{
			b= b.substr(0 ,b.find_first_of(" <"));	
			e = a.find(b)-d; 
}
		if (e!=0) 
		a.erase(d,b.size()+e); 
		else 
		a.erase(d); 

		io->at(1) = b; 
		*tt = (O_WRONLY|O_CREAT |O_APPEND); // S_IRUSR |S_IWUSR);
//		io[1] = const_cast<char*> (b.c_str()); 

//		int fdo=open(const_cast <char*>(b.c_str()),O_WRONLY|O_CREAT |O_APPEND, permission);
//		if (fdo<0) {
//			perror("Read Error"); 
//			*flag = 1; 
//			return bad; 
//		}
//		close(1);
//		dup(fdo);
	}
		//double output redirction
	return a; 	
//	if (c == string::npos)
//		return a.substr(0, b-1);
//	if (b == string::npos)
//		return a.substr(0, c-1);
//	if (c>b)
//		return a.substr(0,b-1);
//	if (b>c)
//		return a.substr(0,c-1);
//	*flag = 1; 
//	return bad; 
	
}

// each int return represent a case of which < > >> <<< is found.
// return 0 if none or mismatching
int findthesign ( string a)
{
	int yoloflags =0 ; 
	if (a.find('<') == string::npos &&  a.find('>') == string::npos)
		return yoloflags; 	
	if( a.find('<') != string::npos && (a.find_last_of('<') == a.find_first_of('<')))
		yoloflags  |= 1; 
	else if (a.at(a.find_first_of('<')+1) == '<' && (a.find_last_of('<') == a.find_first_of('<')+2))
		yoloflags  |= 2; 
	else if (a.find('<') !=string::npos) 
		yoloflags = -1;
	if (a.find('>') != string::npos && (a.find_last_of('>') == a.find_first_of('>')))
		yoloflags |= 4; 
	else if (a.at(a.find_first_of('>')+1) == '>' && (a.find_last_of('>') == a.find_first_of('>')+1))	
		yoloflags |=8;
	else if (a.find('>') != string::npos)
		yoloflags = -1; 
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
	char* loginname =getlogin() ;
	if (loginname== NULL)
		perror("getlogin error");
	char *cwd = get_current_dir_name(); 
	cout << cwd << "/" <<   loginname<< "$ "; 

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
	delete cstring; 
	return flags; 
}
int cpipe(string a)
{
	int count = 0 ;
	while (1)
	{
		if (a.find('|') != string::npos)
		{
			count ++;	
			if(a.at(a.find('|')) == a.at(a.find_last_not_of(' '))) 
				return -1; 
			a= a.substr(a.find('|')+1);
		}
		else
			break; 
	}
	return count; 
}
int run_cmd(int mode, vector <string> *cmd, int flags)
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
		vector <string > pipes ;  
		int pipecount = cpipe(cmd->at(i)); 
		if (pipecount < 0)
		{
			cout << "error" << endl;
			break; 
		}
		string backup = cmd->at(i); 
		int curcount = 0; 
		string backup2 = cmd->at(i); ; 
		vector<string> pipecmd; 
		if (pipecount> 0 )
		{
			backup2 = cmd->at(i).substr(cmd->at(i).find_last_of('|')+1);
			string tempo = cmd->at(i); 
			for (int j = 0 ; j <=pipecount ; j++)
			{
				pipecmd.push_back(tempo.substr(0, tempo.find('|')-1)); 		
				tempo = tempo.substr(tempo.find('|')+1); 
			}
		}
		int   fd_in = 0;
//		int copy_stdi = dup(0); 
//		int copy_stdo = dup(1); 
//		cout << "copy: " << copy_stdo << endl; 
		int pd[2]; 
for (int z = 0; z != pipecount+1; z++)
{

	if (pipecount > 0) 
	{
			int err= pipe(pd); 
			if (err<0) 	
			{
				perror ("pipe");
				break; 
			}
		cmd->at(i) = pipecmd.at(z); 
	}

	int pid = fork();
	globalvar= pid; 
//	int success;
	
mode = findthesign(cmd->at(0));

	if (pid > 0)
	{

		int err = wait(0); 
		if (err<0)
			perror("wait"); 
//		waitpid(pid, &success, 0);
		if (pipecount>=1)
		{
			int err2 = close(pd[1]); 
			if (err2< 0)
				perror("close");
			fd_in = pd[0]; 
			curcount++; 
		}
		//if (!letsgedit(simi, aand, oor, success))
		//	break;
		
	}
	if (pid == 0)
	{
		if (pipecount > 0 )
		{
			int err2 = dup2(fd_in, 0); 	
			if (err2<0)
				perror("dup2"); 
			if ( z != pipecount)
			{
				int err3 = dup2(pd[1],1);
				if (err3< 0 )
					perror("dup2"); 
			}
			int err3=close(pd[0]);
			if (err3< 0)
				perror("close");
		}
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
		if (cmd->at(i).find_first_of("<>")==string::npos)
			mode = 0; 
		if (mode!=0) {
			int placeholder = 0; 

			mode_t permission  ; 
			mode_t permis = S_IRUSR |S_IWUSR;
			vector <string> io; 
			io.resize(5); 
			int *flag = &placeholder; 
			string a = 	inputoutput(&permission, &io, cmd->at(i), mode, flag);

		if (!io.at(0).empty() )
		{
			
			int fdi=open(const_cast <char*>(io.at(0).c_str()),O_RDONLY);	
			if (fdi<0) {
				perror("Read Error"); 
				*flag = 1; 
				return 1; 
			}
			int erro4 = close(0);
				
			int erro5 = dup(fdi);
			if (erro4<0)
				perror ("close");
			if(erro5< 0 )
				perror("dup"); 

		}
		if (!io.at(1).empty() )
		{
			int fdo=open(const_cast <char*>(io.at(1).c_str()),permission, permis);	
			if (fdo<0) {
				perror("writeError"); 
				*flag = 1; 
				return 1; 
			}
			int erro6; 
			int erro7; 
			if (io.at(3)== "0" )
			{
				erro6 = close(1);
				erro7 =dup(fdo); 
				if (erro6 < 0 )
				perror("close"); 
			if (erro7 < 0 )
				perror("dup"); 


			}
			else
			{
				erro6 = close( atoi(const_cast<char*>(io.at(3).c_str()))); 	
				erro7 = dup(fdo); 
			}
			if (erro6 < 0 )
				perror("close"); 
			if (erro7 < 0 )
				perror("dup"); 

		}
		if(!io.at(2).empty())
		{
			char* yolo = new char(io.at(2).size()); 
			strcpy(yolo, const_cast<char*>(io.at(2).c_str()));  
			cout << "need to do " << endl; 
//			int fpd[2]; 
//			pipe(fpd); 
//int zz =	write(fpd[1], yolo ,sizeof(yolo));
//		if (zz<0)
//			perror("writejdksl" );
//			close(0); 
//			dup(fpd[1]) ; 
//			close(fpd[1]); 
//			close(fpd[0]); 

		}
			if (*flag==1) {
				cout << "Error" << endl; 
				continue; 
			}
			cmd->at(i) = a;
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

			if (cmd->at(i).find_first_not_of(" ") == string::npos && cmd->at(i).size() >=1)
			{	
				cout << "Syntax Error "<< endl; 
				exit(1); 
			}
			string path = getenv("PATH");
			vector<string>paths;
			while(1)
			{
				if(path.find_first_of(":") == string::npos)
					break;
				paths.push_back(path.substr(0,path.find(":")));
				path = path.substr(path.find(":")+1);
			}

			for(unsigned i = 0 ; i < paths.size(); i++)
			{

				string temp = argv[0]; 
				paths.at(i) += "/" + temp; 

					if ( access(const_cast<char*> (paths.at(i).c_str()) , X_OK) == 0)
					{
						execv( const_cast<char*> (paths.at(i).c_str()), argv); 
						perror("execv"); 
					}
			}
			cout << "could not find excutable" << endl;
			exit(1); 
		}

	if (pid == -1)
	{
		perror("fork); exit(1)"); 
	}
	
}
	}// for each cmd
	
	return 0; 

}
int main()
{
	signal(SIGINT, &hd1); 
	while (1) {
	vector< string> *cmd= new vector<string> ; 
	int flags = prompt(cmd); 
	if (cmd->at(0).empty())
		continue; 
	int mode= findthesign(cmd->at(0)); 
	if (mode<0)
	{
		cout << "Multiple redirection error" << endl;
		exit(1); 
	}
	run_cmd(mode, cmd, flags);
	delete cmd;
	} 
	return 0;
}
