// author.cpp
#include "main.h"

//--------------------------------------------------------------------------------------------------
int main(int argc, char* argv[]){
	if(access("SYS",F_OK)){
		printf("Cennot find folder SYS.\n");
		return 0;
		}
	srand(time(NULL)%518);
	MAIN Main;
	{
		Main.GlobalSpace.Map.avtoSet("map");
		M_CVARIANT&GM=*Main.GlobalSpace.Map.DATA.mapVal;
		CVARIANT A,B;
		A.avtoSet("string");
		B.avtoSet("string");
		*A.DATA.ps="WAY";
		*B.DATA.ps=".";
		GM[A]=B;
		B.avtoSet("vector");
		for(int t=0;t<argc;++t){
			*A.DATA.ps=argv[t];
			B.DATA.vectorVal->push_back(A);
			}
		*A.DATA.ps="ARGS";
		GM[A]=B;
		B.avtoSet("map");
		*A.DATA.ps="GLOBALS";
		GM[A]=B;
		/*
		*A.DATA.ps="DLLS";
		B.avtoSet("vector");
		GM[A]=B;
		*/
	}
	int i;
	i=Main.IncludeFILE("./SYS/main.txt");
	if(i){
		//Main.outHTML(Main.PHTML,"out.html");
		cout<<Main.PHTML<<endl;
		return 0;
		}
	Main.RUN();
	//*/
	return 0;
}


// author.cpp	:-|