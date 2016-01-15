#include <iostream>
#include <fstream>
#include <map>
#include <string>

using namespace std;

map<string,string> dict;

string deal(string inp){
    while (inp.find("$(") != string::npos){
        int i = inp.find("$(") + 2;
        int j = inp.find(')') - 1;
        string ident = inp.substr(i, j-i+1);
        if (dict.find(ident) != dict.end())
	    inp.replace(i-2, ident.length() + 3, dict[ident]);
	else return "";
    } 
    return inp;
}

void print(int depth){
    for (int i=0;i<depth;i++) 
        cout<<"  ";
    cout<<"|-";
}


void dfs(string cur, int depth){
    ifstream ff;
    ff.open(cur.c_str());
    if (!ff){
	print(depth);
        cout<<"Not found: "<<cur<<endl;
	return;
    }
    map<string,int> flag;
    char st[1024];
    while (ff.getline(st,1024)){
        string str(st);
        if (str.substr(0,7) == "include"){
            string next = deal(str.substr(8));
            if (flag.find(str) == flag.end()){
		print(depth);
		if (next.length() > 0){
		    cout<<next<<endl;
                    flag[str] = 1;
                    dfs(deal(next),depth+1);
	        }else{
		    cout<<"Not Found: "<<str<<endl;
		    flag[str] = 1;
	        }
	    }
	}
    }
}

int main(int argc, char **argv, char** envp){
    char** env;
    for (env = envp; *env != 0; env++){
        char* thisEnv = *env;
        string tmp(thisEnv);
        int i = tmp.find('=');
        if (i != string::npos)
	    dict[tmp.substr(0,i)] = tmp.substr(i+1);	
    }
    dict["TOPDIR"]="";
    dict["BUILD_SYSTEM"]="build/core";
    cout<<"|-Makefile"<<endl;
    dfs("Makefile",1);
}
