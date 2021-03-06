// global.cpp
#include "main.h"

//--------------------------------------------------------------------------------------------------
MAIN::MAIN(){
	FreeLaver=0;
	NfreeOBJ=1;
	indexFile=1;
}


MAIN::~MAIN(){
	GlobalSpace.SubSpace.clear();
	for(int i=0;i<Files.size();++i)if(Files[i]){
		delete Files[i];
		}
	M_ILAVER::iterator ti=tableLavers.begin();
	for(;ti!=tableLavers.end();++ti)if(ti->second)delete ti->second;
}



void MAIN::unloadOverlay(File*f){
	--f->Nusers;
	if(f->Nusers>0)return;
	f->controlSave();
	int i;
	for(i=0;i<f->modules.size();++i){
		string s=f->modules[i];
		int j=s.find('.');
		if(j>=0)s=s.substr(0,j);
		File*subf=findModule(s);
		if(subf)unloadOverlay(subf);
		}
	/*
	for(i=0;i<f->Classes.size();++i)delete f->Classes[i];
		//GlobalSpace.SubSpace.erase(f->Classes[i]->name);
	for(i=0;i<f->functions.size();++i){
		L_IS(f->functions[i]);
		delete f->functions[i];
		}
	f->functions.clear();
	*/
	eraseModule(f->id);
}



void MAIN::unloadOverlay(const char*fulname){
	string module=fulname;
	int i=module.find('.');
	if(i>=0)module=module.substr(0,i);
	File*f=findModule(module);
	if(f)unloadOverlay(f);
}



File* MAIN::findModule(string module){
	CVARIANT K;
	K.avtoSet("string");
	*K.DATA.ps="WAY";
	K=(*GlobalSpace.Map.DATA.mapVal)[K];
	K.TransformType("string");
	string WAY=*K.DATA.ps;
	if(module.substr(0,WAY.size())==WAY)module=module.substr(WAY.size()+1);
	M_iFile::iterator it=Files.begin();
	for(;it!=Files.end();++it){
		File*R=it->second;
		string g=R->way+"/"+R->name;
		if(module==g)return R;
		}
	return NULL;
}



void MAIN::eraseModule(int id){
	File*f=Files[id];
	Files.erase(id);
	delete f;
}


Function* MAIN::getFunction(I*Pset,string fullnsme){
	File*file=NULL;
	if(Pset)file=Pset->sub->ActivF->file;
	Function*f=NULL;
	if(file)f=file->FindFunction(fullnsme);
	if(f)return f;
	M_iFile::iterator it=Files.begin();
	for(;it!=Files.end();++it)if(it->second!=file){
		File*file2=it->second;
		f=file2->FindFunction(fullnsme);
		if(f)return f;
		}
	// fullnsme
	for(it=Files.begin();it!=Files.end();++it){
		File*file2=it->second;
		string way=file2->toPsets();
		if(fullnsme.substr(0,way.size())==way){
			way=fullnsme.substr(way.size()+2);
			f=file2->FindFunction(way);
			if(f)break;
			}
		}
	return f;
}


int MAIN::IncludeFILE(const char*fulname){
	string module=fulname;
	int i=module.find_last_of('.');
	if(i>0)module=module.substr(0,i);
	File*f=findModule(module);
	if(f){
		++f->Nusers;
		return 0;
		}
	CVARIANT K;
	K.avtoSet("string");
	*K.DATA.ps="WAY";
	K=(*GlobalSpace.Map.DATA.mapVal)[K];
	K.TransformType("string");
	string WAY=*K.DATA.ps;
	string W=fulname;
	if(W.substr(0,WAY.size())!=WAY)W=WAY+"/"+W;
	
	string FN;
	i=CompareFileTime(W.c_str(),FN);
	if(!i){
		PHTML+="<font class='yellow'>";
		PHTML+="���� <b>\"";
		PHTML+=fulname;
		PHTML+="\"</b> �� ������. � �� ������ ��� ��������.";
		PHTML+="</font><br/>\n";
		return 1;
		}
	if(i<0)FN=W;
	{
	Assemble A(PHTML,this,FN.c_str());
	i=A.Load(FN.c_str(),i<0);
	f=A.lastFile;
	}
	if(i){
		PHTML+="<font class='red'>";
		PHTML+="���� <b>\"";PHTML+=fulname;PHTML+="\"</b> �������� ������.";
		PHTML+="</font><br/>\n";
		return 2;
		}
	Files[f->id]=f;
	//include all sub files.
	V_S::iterator it=f->modules.begin();
	for(;it!=f->modules.end();++it){
		int t=IncludeFILE(it->c_str());
		if(!t)continue;
		PHTML+="<font class='red'>";
		PHTML+="���� <b>\"";PHTML+=fulname;PHTML+="\"</b> �������� ������.";
		PHTML+="</font><br/>\n";
		i=1;
		break;
		}
	if(i){
		V_S::iterator jt=f->modules.begin();
		for(;jt!=f->modules.end()&&jt!=it;++jt)unloadOverlay(jt->c_str());
		eraseModule(f->id);
		return 2;
		}
	return 0;
}



bool MAIN::GetFileTime(const char*fileName,tm&stLocal){
    struct stat attrib;  //1. create a file attribute structure
    int t=stat(fileName, &attrib); //2. get the attributes of afile.txt
    if(t<0)return false;
    stLocal = *gmtime(&(attrib.st_mtime)); //3. Get the last modified time and put it into the time structure    
	return true;
	/*
	HANDLE hFile=CreateFile(fileName,1,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
	FILETIME ftCreate, ftAccess, ftWrite;
	SYSTEMTIME stUTC;
	if (!::GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))return FALSE;
	FileTimeToSystemTime(&ftWrite, &stUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
	CloseHandle(hFile);
	return TRUE;
	*/
}



int MAIN::CompareFileTime(const char*fileName,string&strFN){//0-error  1-code  -1-txt
	strFN=fileName;
	int tt=strFN.find_last_of('.');
	strFN=strFN.substr(0,tt);
	strFN+=".code";
	bool bb;
	tm fa,fb;
	bb=GetFileTime(strFN.c_str(),fa);
	tt=0;
	if(!bb)tt=1;
	bb=GetFileTime(fileName,fb);
	if(!bb)return !tt;
	if(tt)return -1;
	if(fa.tm_year>fb.tm_year)return 1;
	if(fa.tm_year<fb.tm_year)return -1;
	if(fa.tm_mon>fb.tm_mon)return 1;
	if(fa.tm_mon<fb.tm_mon)return -1;
	if(fa.tm_mday>fb.tm_mday)return 1;
	if(fa.tm_mday<fb.tm_mday)return -1;
	if(fa.tm_hour>fb.tm_hour)return 1;
	if(fa.tm_hour<fb.tm_hour)return -1;
	if(fa.tm_min>fb.tm_min)return 1;
	if(fa.tm_min<fb.tm_min)return -1;
	return (fa.tm_sec>fb.tm_sec)?1:-1;
}



void MAIN::outHTML(string&HTML,string fname){
	if(HTML.empty())return;
	string s;
	::ofstream of(fname.c_str());
	of<<"<html>\n<head>\n<title>AUTHOR: Speaks and Shows</title>\n"
	"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=windows-1251\" />\n";
	of<<"<style type=\"text/css\" media=\"screen\">/* <![CDATA[ */ ";
	of<<"@import url("<<s.c_str()<<"SYS/style.css);  /* ]]> */ </style>\n";
	of<<"</head>\n\n<body>\n";
	of<<HTML.c_str();
	of<<"\n</body>\n</html>\n";
	of.close();
}


//--------------------------------------------------------------------------------------------------
File::File(MAIN*M){
	Nusers=0;
	Main=M;
	id=Main->indexFile++;
}



File::~File(){
	controlSave();
	int i=0;
	for(;i<functions.size();++i)
		if(functions[i])delete functions[i];
	for(i=0;i<Classes.size();++i)
		if(Classes[i])delete Classes[i];
}



string File::toString(){
	CVARIANT K;
	K.avtoSet("string");
	*K.DATA.ps="WAY";
	K=(*Main->GlobalSpace.Map.DATA.mapVal)[K];
	K.TransformType("string");
	string WAY=*K.DATA.ps;
	string x="// "+name+".code";
	string s=x;
	s+="\n";
	int j,i=0;
	for(;i<modules.size();++i){
		string t=modules[i];
		j=WAY.size();
		if(j)++j;
		t=t.substr(j,t.size()-j);
		s+="\n#include <"+t+">";
		}
	for(i=0;i<Classes.size();++i){
		s+="\n\n\n\n";
		s+=Classes[i]->toString();
		}
	for(i=0;i<functions.size();++i){
		s+="\n\n\n\n";
		if(!functions[i]->Body)if(functions[i]->sxema){
			Sxema*Sxem=functions[i]->sxema->copy();
			functions[i]->Body=Sxem->Coder();
			delete Sxem;
			}
		s+=functions[i]->toString();
		}
	s+="\n\n\n";
	s+=x+"	:-|";
	return s;
}



void File::controlSave(){
	if(!NeedSave)return;
	NeedSave=0;
	string g=way+"/"+name+".code";
	string x=toString();
	if(x.empty()){
		cout<<"Error: Empty body file modyle."<<endl;
		return;
		}
	CVARIANT K;
	K.avtoSet("string");
	*K.DATA.ps="WAY";
	K=(*Main->GlobalSpace.Map.DATA.mapVal)[K];
	K.TransformType("string");
	string WAY=*K.DATA.ps;
	WAY+="/"+g;
	::ofstream of(WAY.c_str());
	of<<x.c_str();
	of.close();
}


Function* File::FindFunction(string fullname,bool create){
	Function*f=NULL;
	int i=fullname.find("::");
	if(i<0){
		for(int t=0;t<functions.size();++t){
			f=functions[t];
			if(f->name==fullname && !f->Space)return f;
			}
		f=NULL;
		if(create){
			f=new Function();
			f->name=fullname;
			f->Space=NULL;
			f->file=this;
			functions.push_back(f);
			}
		return f;
		}
	for(int j=0;j<Classes.size();++j){
		CLASS*C=Classes[j];
		f=C->FindFunction(fullname,create);
		if(f)return f;
		}
	if(create){
		CLASS*C=new CLASS;
		Classes.push_back(C);
		C->file=this;
		C->name=fullname.substr(0,i);
		f=C->FindFunction(fullname,create);
		}
	return f;
}



string File::toPsets(){
	CVARIANT K;
	K.avtoSet("string");
	*K.DATA.ps="WAY";
	K=(*Main->GlobalSpace.Map.DATA.mapVal)[K];
	K.TransformType("string");
	string WAY=*K.DATA.ps;
	int n=WAY.size()+1;
	if(n>=way.size())WAY="";else WAY=way.substr(n);
	int index=0;
	while((index=(int)WAY.find("/",index))>=0){
		WAY.replace(index,1,"::");
		index+=2;
		}
	WAY+="::"+name;
	return WAY;
}



//--------------------------------------------------------------------------------------------------
CLASS::CLASS(){
	file=NULL;
	Parent=Space=NULL;
}

CLASS::~CLASS(){
	M_SC::iterator it=SubSpace.begin();
	for(;it!=SubSpace.end();++it)
		if(it->second)delete it->second;
}


//������ ��� ������� ����� � ������� ������� ���������
CLASS* CLASS::FindSpace(string row,bool create){
	if(row.empty())return this;
	int i=0;
	if(row[i]==':')++i;
	if(row[i]==':')++i;
	row=row.substr(i);
	i=row.find(':');
	string one;
	if(i>=0){
		one=row.substr(0,i);
		row=row.substr(i);
		}else{
		one=row;
		row="";
		}
	CLASS*Sub=SubSpace[one];
	if(!Sub){
		if(!create)return NULL;
		SubSpace[one]=new(CLASS);
		Sub=SubSpace[one];
		Sub->name=one;
		Sub->Space=this;
		Sub->file=file;
		}
	return Sub->FindSpace(row);
}

/*
//������ ��� ������� ������� �� ����� + ���� ���������� + ��� ����������
Function* CLASS::FindFunction(string name,V_TIP*tips,bool Creat){
	for(int i=0;i<functions.size();++i)
		if(functions[i]->name==name){
			int k=0,m=functions[i]->tips.size();
			if(m==tips->size())
			for(int j=0;j<m;++j){
				if(functions[i]->tips[j]==(*tips)[j]){++k;continue;}
				if(functions[i]->tips[j].n && (*tips)[j].n)++k;
				}
			if(k==m)return functions[i];
			}
	if(!Creat)return NULL;
	Function*F=new(Function);
	//if(tips)F->tips=*tips; else F->isDestructor=1;
	F->name=name;
	functions.push_back(F);
	return F;
}



Function* CLASS::FindFunction2(string name){
	for(int i=0;i<functions.size();++i){
		if(functions[i]->name==name)return functions[i];
		}
	return NULL;
}
*/


Function* CLASS::FindFunction(string fullname,bool create){
	int i=fullname.find("::");
	string spacename=fullname.substr(0,i);
	if(name!=spacename)return NULL;
	Function*f=NULL;
	fullname=fullname.substr(i+2);
	i=fullname.find("::");
	if(i<0){
		for(int t=0;t<functions.size();++t){
			f=functions[t];
			if(f->name==fullname)return f;
			}
		f=NULL;
		if(create){
			f=new Function();
			f->name=fullname;
			f->Space=this;
			f->file=file;
			functions.push_back(f);
			file->functions.push_back(f);
			}
		return f;
		}
	M_SC::iterator it=SubSpace.begin();
	for(;it!=SubSpace.end();++it){
		f=it->second->FindFunction(fullname,create);
		if(f)return f;
		}
	if(create){
		CLASS*C=new CLASS;
		string name=fullname.substr(0,i);
		SubSpace[name]=C;
		C->file=file;
		C->name=name;
		C->Space=this;
		f=C->FindFunction(fullname,create);
		}
	return f;
}



string CLASS::toString(){
	string s="class ";
	s+=name;
	if(Parent){
		s+=" : public ";
		s+=Parent->name;
		}
	s+="{\n";
	M_SC::iterator it=SubSpace.begin();
	for(;it!=SubSpace.end();++it){
		s+="	";
		string u=it->second->toString();
		for(int i=0;i<u.size();++i)
			if(u[i]=='\n'){
				++i;
				u.insert(i,"	");
				}
		s+=u;
		s+="\n";
		}
	string u=init.toString();
	s+=u.substr(2,u.size()-4);
	for(int i=0;i<functions.size();++i){
		s+="	";
		s+=functions[i]->getHead();
		s+=";\n";
		}
	s+="};\n";
	return s;
}


// global.cpp	:-|