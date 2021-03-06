//processor.h

#ifdef DEF_PCONS_INCLUDE
	#include<deque>
	#include<list>
#endif


#ifdef DEF_PCONS_CLASS
	class Methods;
	class CVARIANT;
	class CInterval;
	class POS;
	class CModule;
#endif




#ifdef DEF_PCONS_TYPES
	typedef vector<string>		Vstr;
	typedef deque<int>			Dint; 
	typedef vector<CVARIANT>		V_CVARIANT;
	typedef vector<CVARIANT*>		V_pCVARIANT;
	typedef set<CVARIANT>			S_CVARIANT;
	typedef deque<CVARIANT>			deque_CVARIANT;
	typedef map<CVARIANT,CVARIANT>	M_CVARIANT;
	typedef map<string,CVARIANT>	M_SV;
#endif




#ifdef DEF_PCONS_CONTENT

//--------------------------------------------------------------------------------------------------
class CInterval{
public:
	double A,B;
	bool a,b;

	CInterval();
	CInterval(bool,double,double,bool);
	CInterval(const CInterval&);

	CInterval& operator = (const CInterval&);
	bool operator == (const CInterval&) const;
	bool operator >  (const CInterval&) const;
	bool operator <  (const CInterval&) const;

	bool operator &=  (const CInterval&);
	bool operator |=  (const CInterval&);

	operator string();
	void toString(string&);

	void control();
};



class CFunction{
public:
	Function*F;
	bool isOriginal;

	CFunction(Function*f=NULL);
	~CFunction();
	CFunction(const CFunction&);
	CFunction& operator = (const CFunction&);

	bool operator == (const CFunction&) const;
	bool operator >  (const CFunction&) const;
	bool operator <  (const CFunction&) const;

	string toString();
};


class CModule{
public:
	int id; // Main-> File[id]
	CModule(int i=0);
	CModule(const CModule&);
	CModule& operator = (const CModule&);

	bool operator == (const CModule&) const;
	bool operator >  (const CModule&) const;
	bool operator <  (const CModule&) const;

};



//--------------------------------------------------------------------------------------------------
class CVARIANT{
	void wplus(bool);//1++ 0--
public:
	int Ntype;
	union t_DATA{
		void*			voidVal;
		char			bVal;
		short			iVal;
		unsigned short	uiVal;
		long			lVal;
		unsigned long	ulVal;
		float			fltVal;
		double			dblVal;
		bool			boolVal;
		int				intVal;
		string*			ps;
		S_CVARIANT*		setVal;
		deque_CVARIANT*	dequeVal;
		V_CVARIANT*		vectorVal;//vector CVARIANT
		M_CVARIANT*		mapVal;
		CInterval*		intervalVal;
		CFunction*		functionVal;
		Algorithm*		programVal;
		UGraf*			grafVal;
		Digits*			digitVal;
		CModule*		moduleVal;
		}DATA;

	static const char*name[];

	CVARIANT();
	CVARIANT(const CVARIANT&);
	CVARIANT& operator = (const CVARIANT&);
	bool operator == (const CVARIANT&) const;
	bool operator != (const CVARIANT&) const;
	bool operator >  (const CVARIANT&) const;
	bool operator <  (const CVARIANT&) const;
	bool operator >= (const CVARIANT&) const;
	bool operator <= (const CVARIANT&) const;
	~CVARIANT();
	CVARIANT& operator -- ();
	CVARIANT& operator ++ ();
	void avtoSet(string&type);
	void avtoSet(const char*);
	void clear();
	static int  getN(const char*);
	bool isType(const char*) const;
	//bool isPrimitive() const;
	//static bool isObject(const char*);
	void toString(string&) const;
	static bool OPETATOR(CVARIANT*,CVARIANT*,const char*);
	static bool compare(CVARIANT*,CVARIANT*,const char*);
	static int getSizeType(int);
	static int getSizeType(const char*);
	void INVERT(const char*);
	void TransformType(const char*);
	void TransformCollection(const char*);
	static bool sBelongs(const char*s,const char**);
	CVARIANT*copy();

	//
	string getString();

	//double
	double getDouble();

	//static int getQuantum(string);
};

#endif
//processor.h	:-|