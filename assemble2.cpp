// assemble2.cpp

#include "main.h"
//--------------------------------------------------------------------------------------------------

void Algorithm::initUP(Algorithm*P){up=P;}
//--------------------------------------------------------------------------------------------------
void Base2::initUP(Algorithm*P){
	up=P;
	A->initUP(this);
	B->initUP(this);
}
//--------------------------------------------------------------------------------------------------
void Base3::initUP(Algorithm*P){
	up=P;
	A->initUP(this);
	B->initUP(this);
	V->initUP(this);
}
//--------------------------------------------------------------------------------------------------
void Prefix::initUP(Algorithm*P){
	up=P;
	X->initUP(this);
}
//--------------------------------------------------------------------------------------------------
void Sufix::initUP(Algorithm*P){
	up=P;
	X->initUP(this);
}
//--------------------------------------------------------------------------------------------------
void AccesMasiv::initUP(Algorithm*a){
	up=a;
	X->initUP(this);
	P->initUP(this);
}
//--------------------------------------------------------------------------------------------------
void Interval::initUP(Algorithm*P){
	up=P;
	A->initUP(this);
	B->initUP(this);
}
//--------------------------------------------------------------------------------------------------
void AlgoSet::initUP(Algorithm*P){
	up=P;
	for(int i=0;i<nabor.size();++i)
		nabor[i]->initUP(this);
}
//--------------------------------------------------------------------------------------------------
void AComposition::initUP(Algorithm*P){
	up=P;
	M_SA::iterator it=table.begin();
	for(;it!=table.end();++it)
		it->second->initUP(this);
}
//--------------------------------------------------------------------------------------------------
void Sequence::initUP(Algorithm*P){
	up=P;
	L_AL::iterator it=nabor.begin();
	for(;it!=nabor.end();++it)if(*it)(*it)->initUP(this);
}
//--------------------------------------------------------------------------------------------------
void CreateVar::initUP(Algorithm*P){
	up=P;
	if(Init)Init->initUP(this);
	for(int i=0;i<params.size();++i)
		if(params[i])params[i]->initUP(this);
}
//--------------------------------------------------------------------------------------------------
void SpecSumbol2::initUP(Algorithm*P){
	up=P;
	X->initUP(this);
}
//--------------------------------------------------------------------------------------------------
void IF::initUP(Algorithm*P){
	up=P;
	X->initUP(this);
	if(A)A->initUP(this);
	if(E)E->initUP(this);
}
//--------------------------------------------------------------------------------------------------
void WHILE::initUP(Algorithm*P){
	up=P;
	if(X)X->initUP(this);
	if(A)A->initUP(this);
}
//--------------------------------------------------------------------------------------------------
void FOR::initUP(Algorithm*a){
	up=a;
	if(X)X->initUP(this);
	if(B)B->initUP(this);
	if(P)P->initUP(this);
}
//--------------------------------------------------------------------------------------------------
void CallFunc::initUP(Algorithm*P){
	up=P;
	X->initUP(this);
	for(int i=0;i<params.size();++i)
		params[i]->initUP(this);
}



//--------------------------------------------------------------------------------------------------
Function::Function(){
	isOperator=isStatic=isVirtual=0;
	Body=NULL;
	file=NULL;
	Space=NULL;
	sxema=NULL;
}


Function::~Function(){
	if(Body)delete Body;
	if(sxema)delete sxema;
	for(int i=0;i<defaults.size();++i)
		if(defaults[i])delete defaults[i];
}


string Function::getPsetWay(){
	string way;
	CLASS*X=Space;
	while(X){
		if(!X->name.empty())way=X->name+"::"+way;
		X=X->Space;
		}
	return way;
}


string Function::getHead(bool full){
	string s=ret.toString(),tmp;
	if(!s.empty())s+=" ";
	if(full){
		s+=getPsetWay();
		}else{
		string p;
		if(isVirtual)p="virtual ";
		if(isStatic)p+="static ";
		s=p+s;
		}
	if(isOperator)s+=" operator ";
	//if(isDestructor)s+="~";
	s+=name;
	if(isOperator)s+=" ";
	s+="(";
	for(int i=0;i<tips.size();++i){
		if(i)s+=",";
		tmp=tips[i].toString();;
		s+=tmp;
		if(!tmp.empty())s+=" ";
		if(i>=names.size())continue;
		s+=names[i];
		if(!full)
		if(defaults[i])s+="="+defaults[i]->toString();
		}
	s+=")";
	return s;
}


string Function::toString(){
	if(sxema&&!Body){
		string s="SXEMA: ";
		s+=getHead(1);
		s+="{\n";
		s+=sxema->toString();
		s+="}";
		return s;
		}
	string s=getHead(1);
	if(!Body){
		s+=";";
		return s;
		}
	Sequence*SBody=dynamic_cast<Sequence*>(Body);
	L_AL::iterator it=SBody->nabor.begin();
	for(bool first=1;it!=SBody->nabor.end();++it){
		CreateVar*cv=dynamic_cast<CreateVar*>(*it);
		if(!cv)continue;
		if(!cv->tip.name.empty())continue;
		if(first)s+=":"; else s+=",";
		first=0;
		s+=cv->toString2();
		}
	s+=Body->toString();
	if(s[s.size()-2]=='	')s.erase(s.size()-2,1);
	return s;
}



Algorithm* Function::findLabelSUB(Algorithm*A,int id){
	Label*L=dynamic_cast<Label*>(A);
	if(L)if(L->id==id)return A;
	Algorithm*F;
	IF*iif=dynamic_cast<IF*>(A);
	if(iif){
		if(iif->A){
			F=findLabelSUB(iif->A,id);
			if(F)return F;
			}
		if(iif->E){
			F=findLabelSUB(iif->E,id);
			if(F)return F;
			}
		}
	WHILE*W=dynamic_cast<WHILE*>(A);
	if(W)if(W->A){
		F=findLabelSUB(W->A,id);
		if(F)return F;
		}
	Sequence*S=dynamic_cast<Sequence*>(A);
	if(S){
		L_AL::iterator it=S->nabor.begin();
		for(;it!=S->nabor.end();++it){
			F=findLabelSUB(*it,id);
			if(F)return F;
			}
		}
	return NULL;
}



void Function::controlSxema(){
	if(sxema)return;
	sxema=new(Sxema);
	sxema->ToSxema(Body);
	delete Body;
	Body=NULL;
	sxema->f=this;
}


Function* Function::copy() const{
	Function*R=new Function;
	R->isOperator=isOperator;
	R->isStatic=isStatic;
	R->isVirtual=isVirtual;
	R->ret=ret;
	//R->file=file;
	R->tips=tips;
	R->name=name;
	R->names=names;
	R->Space=Space;
	cout<<"Function::copy"<<endl;
	if(Body){
		R->Body=Body->copy();
		R->Body->initUP(NULL);
		}
	if(sxema)R->sxema=sxema->copy();
	cout<<"Function::copy end."<<endl;
	return R;
}


bool Function::operator == (Function&t){
	if(t.isOperator!=isOperator)return 0;
	if(t.isStatic!=isStatic)return 0;
	if(t.isVirtual!=isVirtual)return 0;
	t.controlSxema();
	t.sxema->controlTable();
	controlSxema();
	sxema->controlTable();
	if(t.sxema->table.size()!=sxema->table.size())return 0;
	return *t.sxema==*sxema;
}


bool Function:: operator > (Function&t){
	sxema->controlRang();
	t.sxema->controlRang();
	return sxema->HeadEnd->Less(t.sxema->HeadEnd);
}


bool Function:: operator < (Function&t){
	sxema->controlRang();
	t.sxema->controlRang();
	return t.sxema->HeadEnd->Less(sxema->HeadEnd);
}


//--------------------------------------------------------------------------------------------------
//::A::B
string Assemble::getRowNAMES(const char*&s){
	string R;
	string one;
	int dt=2,v;
	do{
		SCANER::noProbel(s);
		const char*s2=s;
		v=0;
		if(SCANER::scanSlovo("::",s))v=1; else{
			int d=0;
			if(*s=='~'){
				d=1;
				++s;
				SCANER::noProbel(s);
				}
			one=SCANER::readName(s);
			if(d)one="~"+one;
			}
		if(dt==v){
			s=s2;
			break;
			}
		dt=v;
		if(v)R+="::"; else {
			if(one.empty())break;
			R+=one;
			}
		}while(1);
	return R;
}



int Assemble::getSpisokArgs(V_TIP&tips,V_S&names,V_AL&defaults,const char*&s){
	SCANER::noProbel(s);
	const char*ss=s;
	while(*s!=')'){
		Type t;
		string name;
		Algorithm*D=NULL;
		if(SCANER::scanSlovo("const",s)){
			const char*s3=s;
			SCANER::noProbel(s);
			if(s>s3)t.isConst=1; else s=ss;
			}
		bool nfirst=0;
		string one;
		do{
			const char*s3=s;
			one=SCANER::readName(s);
			SCANER::noProbel(s);
			if(*s==',' || *s==':' || *s==')' || *s=='='){s=s3;break;}
			if(nfirst)t.name+=" ";
			nfirst=1;
			t.name+=one;
			}while(*s!='*' && *s!='&');
		if(t.name.empty())t.name=getRowNAMES(s);
		do{
			SCANER::noProbel(s);
			if(*s=='*'){++t.n;++s;}else break;
			}while(1);
		SCANER::noProbel(s);
		if(*s=='&'){t.isAmpersent=1;++s;}
		SCANER::noProbel(s);
		name=SCANER::readName(s);
		SCANER::noProbel(s);
		if(*s=='='){
			++s;
			D=Razbor(s,0);
			if(!D)return 2;
			D->initUP(NULL);
			if(*(s-1)==',')--s;
			}
		if(name.empty()){
			name=t.name;
			t.name="var";
			t.n=0;
			t.isAmpersent=0;
			}
		tips.push_back(t);
		names.push_back(name);
		defaults.push_back(D);
		if(*s==',')++s;else if(*s!=')'){s=ss;return 0;}
		}
	++s;
	return 1;// ok
}



Assemble::SX::SX(){B=NULL;}
Assemble::SX::~SX(){if(B)delete B;};


Sxema* Assemble::getSxema(const char*&s){
	SCANER::noProbel(s);
	if(*s!='{')return NULL;
	++s;
	typedef map<int,SX> M_ISX;
	M_ISX table;
	int nstart=0,k;
	while(*s!='}'){
		SX sx;
		SCANER::noProbel(s);
		if(SCANER::scanSlovo("start:",s))nstart=0;
		SCANER::noProbel(s);
		string t=SCANER::readDigit(s);
		sscanf(t.c_str(),"%d",&sx.n);
		if(!nstart)nstart=sx.n;
		SCANER::noProbel(s);
		if(*s!=':')return NULL;
		++s;
		SCANER::noProbel(s);
		int er=0;
		sx.B=new(Bloc);
		if(*s==';')++s;else{
			Sequence S;
			sx.B->A=getONE(s,er,&S);
			if(er)return NULL;
			if(!sx.B->A && S.nabor.size())sx.B->A=(*S.nabor.begin())->copy();
			sx.B->A->initUP(NULL);
			}
		SCANER::noProbel(s);
		if(*s==';')++s;
		SCANER::noProbel(s);
		if(*s!='{')return NULL;
		++s;
		SCANER::noProbel(s);
		while(*s!='}'){
			t=SCANER::readDigit(s);
			sscanf(t.c_str(),"%d",&k);
			sx.next.push_back(k);
			SCANER::noProbel(s);
			if(*s==',')++s;
			SCANER::noProbel(s);
			}
		++s;
		SCANER::noProbel(s);
		table[sx.n]=sx;
		sx.B=NULL;
		}
	++s;
	SX*E;
	M_ISX::iterator it=table.begin();
	for(;it!=table.end();++it){
		SX*sx=&it->second;
		for(int i=0;i<sx->next.size();++i){
			sx->B->down.push_back(table[sx->next[i]].B);
			E=&table[sx->next[i]];
			E->B->up.push_back(sx->B);
			}
		}
	Sxema*SXE=new(Sxema);
	SXE->HeadEnd=table[nstart].B;

	it=table.begin();
	for(;it!=table.end();++it){
		SX*sx=&it->second;
		sx->B=NULL;
		}
	return SXE;
}



Function* Assemble::getFunction(const char*&s,int&er,File*file){
	SCANER::noProbel(s);
	const char*s2=s,*ss=s,*s3;
	while(*s2&&*s2!='(')++s2;
	if(*s2!='(')return NULL;
	bool virt=0,stat=0,isop=0,isSxema=0;
	if(SCANER::scanSlovo("SXEMA",s)){
		SCANER::noProbel(s);
		if(*s==':'){
			++s;
			isSxema=1;
			}
		}
	if(SCANER::scanSlovo("virtual",s)){
		s3=s;
		SCANER::noProbel(s);
		if(s>s3)virt=1; else s=s3;
		}
	if(SCANER::scanSlovo("static",s)){
		s3=s;
		SCANER::noProbel(s);
		if(s>s3)stat=1; else s=s3;
		}
	Type t;
	if(SCANER::scanSlovo("const",s)){
		s3=s;
		SCANER::noProbel(s);
		if(s>s3)t.isConst=1; else s=ss;
		}
	// unsigned long int
	string one;
	bool nfirst=0;
	do{
		const char*s3=s;
		one=SCANER::readName(s);
		if(one.empty())
			if(t.name.empty())break; else {s=ss;return NULL;}
		SCANER::noProbel(s);
		if(*s=='(' || *s==':'){s=s3;break;}
		if(nfirst)t.name+=" ";
		nfirst=1;
		t.name+=one;
		}while(*s!='*' && *s!='&' && *s!='~');
	string row;
	if(t.name.empty()){
		// ::dsg::tre::x
		one=getRowNAMES(s);
		SCANER::noProbel(s);
		if(*s=='(')row=one; else t.name=one;
		}
	do{
		SCANER::noProbel(s);
		if(*s=='*'){++t.n;++s;}else break;
		}while(1);
	SCANER::noProbel(s);
	if(*s=='&'){t.isAmpersent=1;++s;}
	SCANER::noProbel(s);
	if(t.name.empty())t.name="int";
	if(row.empty())row=getRowNAMES(s);
	int i=row.find_last_of(':');
	one=row.substr(i+1);
	//if(i>0)row=row.substr(0,i-1);else row="";
	if(one.empty()){
		s=ss;
		return NULL;
		}
	SCANER::noProbel(s);
	//if(isDes)one.erase(0,1);
	if(one=="operator"){
		isop=1;
		const char*r=oneterm::getSTerm(s);
		if(r)one=r;
		SCANER::noProbel(s);
		}
	if(*s!='('){
		if(isop){
			er=1;
			PHTML+="<font class='red'>";
			PHTML+="������� ������������ �������� � ������: ";
			PHTML+=SCANER::toString(SCANER::findNumberStringLine(this->F.text,s));
			PHTML+="</font><br/>\n";
			}
		s=ss;
		return NULL;
		}
	++s;
	V_TIP	tips;
	V_S		names;
	V_AL	defaults;
	i=getSpisokArgs(tips,names,defaults,s);
	if(i==2){
		er=1;
		for(i=0;i<defaults.size();++i)
			if(defaults[i])delete defaults[i];
		i=0;
		}
	if(!i){
		s=ss;
		return NULL;
		}
	SCANER::noProbel(s);
	SCANER::scanSlovo("const",s);
	SCANER::noProbel(s);
	i=0;
	if(*s=='='){
		++s;
		SCANER::noProbel(s);
		if(*s!='0')i=1;
		++s;
		SCANER::noProbel(s);
		if(*s!=';')i=1;
		++s;
		if(!i)i=3;
		}else if(*s==';'){i=3;++s;}
	if(i==1){
		er=1;
		PHTML+="<font class='red'>";
		PHTML+="���������� ������� ������ ������������� �������� ';' � ������: ";
		PHTML+=SCANER::toString(SCANER::findNumberStringLine(F.text,ss));
		PHTML+="</font><br/>\n";
		return NULL;
		}
	Function*F=file->FindFunction(row,1);
		//Space->FindFunction(one,&tips,1);
	if(virt)F->isVirtual=1;
	if(stat)F->isStatic=1;
	F->isOperator=isop;
	F->ret=t;
	F->tips=tips;
	if(F->names.empty())F->names=names;else{
		for(int j=0;j<names.size();++j)
			if(!names[j].empty())F->names[j]=names[j];
		}
	if(F->defaults.empty())F->defaults=defaults;else{
		for(int j=0;j<defaults.size();++j){
			if(!F->defaults[j] && defaults[j]){
				F->defaults[j]=defaults[j];
				continue;
				}
			if(F->defaults[j] && defaults[j]){
				delete F->defaults[j];
				F->defaults[j]=defaults[j];
				}
			}
		}
	if(i)return F;
	if(F->Body){
		er=1;
		PHTML+="<font class='red'>";
		PHTML+="������� \"";PHTML+=F->getHead();
		PHTML+="\" ��� ������. �� ������ ���� ���������� ��������� ������� ������� � ������: ";
		PHTML+=SCANER::toString(SCANER::findNumberStringLine(this->F.text,ss));
		PHTML+="</font><br/>\n";
		return NULL;
		}
	SCANER::noProbel(s);
	Sequence SE;
	if(*s==':'){
		do{
			++s;
			SCANER::noProbel(s);
			CreateVar cv;
			cv.isInit=1;
			cv.name=SCANER::readName(s);
			if(cv.name.empty())er=1;
			SCANER::noProbel(s);
			if(*s!='(')er=1;
			++s;
			if(!er)while(*s!=')'){
				SCANER::noProbel(s);
				Algorithm*A=Razbor(s,0);
				if(A)cv.params.push_back(A);else er=2;
				SCANER::noProbel(s);
				if(*s==',')++s;
				}
			SE.nabor.push_back(cv.copy());
			if(er){
				PHTML+="<font class='red'>";
				PHTML+="�������������� ������ � ��������� ������� � ������: ";
				PHTML+=SCANER::toString(SCANER::findNumberStringLine(this->F.text,ss));
				PHTML+=". ������: f(int i):a(i),p(i,i){}</font><br/>\n";
				return NULL;
				}
			++s;
			SCANER::noProbel(s);
			}while(*s==',');
		}
	SCANER::noProbel(s);
	if(*s!='{')er=1;
	if(isSxema){
		F->sxema=getSxema(s);
		if(F->sxema)F->sxema->f=F;
		if(!F->sxema){
			PHTML+="<font class='red'>";
			PHTML+="������ � ���� �������-����� � ������: ";
			PHTML+=SCANER::toString(SCANER::findNumberStringLine(this->F.text,ss));
			PHTML+="</font><br/>\n";
			}
		return F;
		}
	F->Body=getNabor(s,er);
	if(F->Body){
		Sequence*SBody=dynamic_cast<Sequence*>(F->Body);
		L_AL::reverse_iterator it=SE.nabor.rbegin();
		for(;it!=SE.nabor.rend();++it){
			SBody->nabor.insert(SBody->nabor.begin(),*it);
			*it=NULL;
			}
		}else F->Body=SE.copy();
	if(F->Body)F->Body->initUP(NULL);
	if(er){
		PHTML+="<font class='red'>";
		PHTML+="������ � ���� ������� � ������: ";
		PHTML+=SCANER::toString(SCANER::findNumberStringLine(this->F.text,ss));
		PHTML+="</font><br/>\n";
		}
	return F;
}




CLASS* Assemble::getClass(const char*&s,int&er,File*ff,CLASS*Space){
	if(!Space)Space=GlobalSpace;
	SCANER::noProbel(s);
	if(!SCANER::scanSlovo("class",s))return NULL;
	SCANER::noProbel(s);
	string str=SCANER::readName(s);
	Space->file=ff;
	CLASS*S=Space->FindSpace(str,1);
	SCANER::noProbel(s);
	if(*s==':'){
		++s;
		SCANER::noProbel(s);
		SCANER::scanSlovo("public",s);
		SCANER::noProbel(s);
		str=SCANER::readName(s);
		SCANER::noProbel(s);
		CLASS*P=Space->FindSpace(str,0);
		if(!P)P=GlobalSpace->FindSpace(str,1);
		S->Parent=P;
		}
	if(*s==';'){++s;return S;}
	if(*s!='{'){
		er=1;
		PHTML+="<font class='red'>";
		PHTML+="� �������� ������ ����� ������ '{' � ������: ";
		PHTML+=SCANER::toString(SCANER::findNumberStringLine(F.text,s));
		PHTML+="</font><br/>\n";
		return S;
		}
	++s;
	do{
		SCANER::noProbel(s);
		CLASS*Sub=getClass(s,er,ff,S);
		if(er)break;
		if(Sub)continue;
		Function*F=getFunction(s,er,ff);
		if(F){
			F->Space=S;
			/*
			if(F->Body)if(!F->file){
				F->file=ff;
				ff->functions.push_back(F);
				}
			*/
			continue;
			}
		if(er)break;
		if(probaVarRead(s,S->init.nabor)==2)er=1;
		if(er)break;
		SCANER::noProbel(s);
		SCANER::scanSlovo("public",s);
		SCANER::noProbel(s);
		SCANER::scanSlovo(":",s);
		SCANER::noProbel(s);
		}while(*s!='}');
	if(er){
		PHTML+="<font class='red'>";
		PHTML+="�������������� ������ � �������� ������ � ������: ";
		PHTML+=SCANER::toString(SCANER::findNumberStringLine(F.text,s));
		PHTML+="</font><br/>\n";
		return S;
		}
	++s;
	SCANER::noProbel(s);
	if(*s!=';'){
		er=2;
		PHTML+="<font class='red'>";
		PHTML+="��������� ������ ';' ����� �������� ������ � ������: ";
		PHTML+=SCANER::toString(SCANER::findNumberStringLine(F.text,s));
		PHTML+="</font><br/>\n";
		}
	++s;
	S->init.initUP(NULL);
	return S;
}



int Assemble::Load(const char*way,bool nedsav){
	File*ff=lastFile=new File(Main);
	ff->Nusers=1;
	ff->NeedSave=nedsav;
	const char*s=F.text,*ss;
	int er=0;
	string sway=way;
	int a,b;
	a=sway.find_last_of('/');
	ff->way=sway.substr(0,a);
	++a;
	b=sway.find_last_of('.');
	ff->name=sway.substr(a,b-a);
	SCANER::noProbel(s);
	CVARIANT K;
	K.avtoSet("string");
	*K.DATA.ps="WAY";
	K=(*GlobalSpace->Map.DATA.mapVal)[K];
	K.TransformType("string");
	string WAY=*K.DATA.ps;
	sway=ff->way.substr(0,WAY.size());
	if(WAY==sway){
		if(ff->way.size()==WAY.size())ff->way="";else
			ff->way=ff->way.substr(WAY.size()+1);
		}
	while(*s){
		ss=s;
		if(SCANER::scanSlovo("#include",s)){
			SCANER::noProbel(s);
			bool nolocal=(*s=='<');
			++s;
			string str;
			while(*s && *s!='\n' && *s!='>' && *s!='"'){
				str+=*s;
				++s;
				}
			if(*s=='>' || *s=='"')++s;
			str=WAY+"/"+str;
			ff->modules.push_back(str);
			}
		CLASS*S=getClass(s,er,ff);
		if(S){
			if(!S->file){
				S->file=ff;
				ff->Classes.push_back(S);
				}
			continue;
			}
		if(er)break;
		Function*f=getFunction(s,er,ff);
		if(er)break;
		if(ss==s){
			er=1;
			break;
			}
		SCANER::noProbel(s);
		}
	//cout<<ff->toString().c_str()<<endl;//
	if(er)ff->NeedSave=0;
	return er;
}



//--------------------------------------------------------------------------------------------------
Algorithm* Algorithm::findLabel(int n){return NULL;}

Algorithm* Label::findLabel(int n){
	if(id==n)return this;
	return NULL;
}


Algorithm* Sequence::findLabel(int n){
	L_AL::iterator it=nabor.begin();
	for(;it!=nabor.end();++it){
		Algorithm*X=(*it)->findLabel(n);
		if(X)return X;
		}
	return NULL;
}


Algorithm* IF::findLabel(int n){
	Algorithm*X=NULL;
	if(A)A->findLabel(n);
	if(X)return X;
	if(E)X=E->findLabel(n);
	return X;
}


Algorithm* WHILE::findLabel(int n){
	return A?A->findLabel(n):NULL;
}


Algorithm* FOR::findLabel(int n){
	return B?B->findLabel(n):NULL;
}



// assemble2.cpp	:-|