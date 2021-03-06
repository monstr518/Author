//	Digit.h

#ifdef DEF_PCONS_CLASS
	class Digits;
#endif



#ifdef DEF_PCONS_TYPES
	typedef vector<char>	V_C;
#endif




#ifdef DEF_PCONS_CONTENT
//--------------------------------------------------------------------------------------------------

class Digits{
	bool znak;//1+ 0-
	void clear();

	void zdvigUP1E(char c=0);
	char zdvigUP1M();
	void zdvigUP2E(char c=0);
	char zdvigUP2M();

	char zdvigDOWN1E();
	void zdvigDOWN1M(char c=0);
	char zdvigDOWN2E();
	void zdvigDOWN2M(char c=0);

	void plus1();
	void minus1();
	void Not();
	bool isZero();

	static void antiZero(V_C&);

	char compareABS(Digits&);
	void umnogenie(char);
public:
	static const char*cpi;
	static const char*ce;

	V_C exponenta,mantisa;
	int mlimit;

	Digits();
	Digits(const Digits&);
	Digits& operator = (const Digits&);

	void invertZnak();

	void set(int);
	void set(double);
	void set(const char*);

	void zdvigUP1();
	void zdvigUP2();

	void zdvigDOWN1();
	void zdvigDOWN2();

	void operator << (int);
	void operator >> (int);

	Digits& operator ++ ();
	Digits& operator -- ();

	operator string ();
	operator int ();
	operator double ();

	bool operator == (Digits&);
	bool operator > (Digits&);
	bool operator < (Digits&);

	Digits& operator += (Digits&);
	Digits& operator -= (Digits&);

	Digits& operator *= (Digits&);
	Digits& operator /= (Digits&);
	Digits& operator %= (Digits&);


	string toString();
	char compare(Digits&);

	void suma(Digits&);
	void minus(Digits&);

	void away(int);
	void round(int);

};


#endif



//	Digit.h	:-|