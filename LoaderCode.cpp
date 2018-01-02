//LoaderCode.cpp
#define DEF_PCONS_INCLUDE
#define DEF_PCONS_CONTENT
#include <string>
#include <iostream>
#include <string.h>
#include "LoaderCode.h"

using namespace std;

LOADER::LOADER(){text=NULL;}
LOADER::~LOADER(){if(text)delete[] text;}


char*LOADER::copy(string&s){
	char*c=new char[(s.size()+1)];
	int i=0;
	for(;i<s.size();++i)c[i]=s[i];
	c[i]=0;
	return c;
}


void LOADER::load(const char*name){
	ifstream f(name, ios::in );
	if(!f){
		cout<<"File "<<name<<" not found.\n";
		return;
		}
	
	string ms;
	char Part[1064];
	int si=sizeof(Part);
	while(!f.eof()){
		f.getline(Part,si);
		ms+=Part;
		if((strlen(Part)+1)<si)ms+='\n';
		}
	//ms+='\n';
	text=copy(ms);
	//cout<<text<<endl;
	f.close();
}




void LOADER::delete_komentary(){
	if(!text)return;
	string ee;
	ee.reserve(sizeof(text));
	int pp=0;// (\) (\n)
	int ww=1,vv=1; // ww - "    vv - '
	for(int j,i=0;text[i];++i){
		j=i;
		while(text[j]=='\\'&&text[j+1]==10){j+=2;i=j;++pp;}
		/*
		if(!ww)if(text[i]=='\\'){
			ee+=text[i++];
			ee+=text[i];
			continue;
			}
		*/
		if(ww&&vv&&text[i]=='/')if(text[i+1]=='/'){
			int j=2;
			for(;text[i+j]&&text[i+j]!=10;++j);
			i+=j;
			ee+='\n';
			continue;
			}
		if(ww&&text[i]=='/')if(text[i+1]=='*'){
			i+=2;
			for(;text[i];++i){
				if(text[i]=='*' && text[i+1]=='/')break;
				if(text[i]=='\n')ee+='\n';
				}
			if(text[i])++i;
			continue;
			}
		if(vv&&text[i]=='"') ww=1-ww;
		if(ww&&text[i]==39 ) vv=1-vv;
		if(text[i]==10){
			ww=1;
			vv=1;
			while(pp){ee+="\n";--pp;}
			}
		ee+=text[i];
		}
	delete[] text;
	text=copy(ee);
}




LOADER::LOADER(const char*name,bool on_komentaru){
	text=NULL;
	if(*name)load(name);
	if(on_komentaru)delete_komentary();
}



void LOADER::out(char*name){
	ofstream f(name);
	if(!f)cout<<"File "<<name<<" cen'not writed.\n";
	f.write(text,strlen(text));
}


//LoaderCode.h