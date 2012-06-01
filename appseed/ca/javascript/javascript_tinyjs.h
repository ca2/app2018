/*
 * TinyJS
 *
 * A single-file Javascript-alike engine
 *
 * Authored By Gordon Williams <gw@pur3.co.uk>
 *
 * Copyright (C) 2009 Pur3 Ltd
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef TINYJS_H
#define TINYJS_H


#ifndef __GNUC__
#	define __attribute__(x)
#endif


/*xxx #undef TRACE
#ifndef TRACE
#define TRACE printf
#endif // TRACE
*/
const int TINYJS_LOOP_MAX_ITERATIONS = 8192;

enum LEX_TYPES {
	LEX_EOF = 0,
	LEX_ID = 256,
	LEX_INT,
	LEX_FLOAT,
	LEX_STR,

	LEX_EQUAL,
	LEX_TYPEEQUAL,
	LEX_NEQUAL,
	LEX_NTYPEEQUAL,
	LEX_LEQUAL,
	LEX_LSHIFT,
	LEX_LSHIFTEQUAL,
	LEX_GEQUAL,
	LEX_RSHIFT,
	LEX_RSHIFTEQUAL,
	LEX_PLUSEQUAL,
	LEX_MINUSEQUAL,
	LEX_PLUSPLUS,
	LEX_MINUSMINUS,
	LEX_ANDEQUAL,
	LEX_ANDAND,
	LEX_OREQUAL,
	LEX_OROR,
	LEX_XOREQUAL,
	LEX_ASTERISKEQUAL,
	LEX_SLASHEQUAL,
	LEX_PERCENTEQUAL,
	// reserved words
#define LEX_R_LIST_START LEX_R_IF
	LEX_R_IF,
	LEX_R_ELSE,
	LEX_R_DO,
	LEX_R_WHILE,
	LEX_R_FOR,
	LEX_R_IN,
	LEX_R_BREAK,
	LEX_R_CONTINUE,
	LEX_R_FUNCTION,
	LEX_R_RETURN,
	LEX_R_VAR,
	LEX_R_TRUE,
	LEX_R_FALSE,
	LEX_R_NULL,
	LEX_R_UNDEFINED,
	LEX_R_INFINITY,
	LEX_R_NAN,
	LEX_R_NEW,
	LEX_R_TRY,
	LEX_R_CATCH,
	LEX_R_FINALLY,
	LEX_R_THROW,
	LEX_R_TYPEOF,
	LEX_R_VOID,
	LEX_R_DELETE,
	LEX_R_INSTANCEOF,
	LEX_R_SWITCH,
	LEX_R_CASE,
	LEX_R_DEFAULT,

	LEX_R_LIST_END /* always the last entry */
};

enum SCRIPTVAR_FLAGS {
	SCRIPTVAR_UNDEFINED		= 0,
	SCRIPTVAR_FUNCTION		= 1,
	SCRIPTVAR_OBJECT			= 2,
	SCRIPTVAR_ARRAY			= 4,
	SCRIPTVAR_DOUBLE			= 8,  // floating point double
	SCRIPTVAR_INTEGER			= 16, // integer number
	SCRIPTVAR_BOOLEAN			= 32, // boolean
	SCRIPTVAR_STRING			= 64, // string
	SCRIPTVAR_NULL				= 128, // it seems null is its own data type
	SCRIPTVAR_INFINITY		= 256, // it seems infinity is its own data type
	SCRIPTVAR_NAN				= 512, // it seems NaN is its own data type

	SCRIPTVAR_NATIVE_FNC		= 1024, // to specify this is a native function
	SCRIPTVAR_NATIVE_MFNC	= 2048, // to specify this is a native function from class->memberFunc
	SCRIPTVAR_NUMERICMASK	= SCRIPTVAR_NULL |
									  SCRIPTVAR_DOUBLE |
									  SCRIPTVAR_INTEGER |
									  SCRIPTVAR_BOOLEAN,
	SCRIPTVAR_VARTYPEMASK	= SCRIPTVAR_DOUBLE |
									  SCRIPTVAR_INTEGER |
									  SCRIPTVAR_BOOLEAN |
									  SCRIPTVAR_STRING |
									  SCRIPTVAR_FUNCTION |
									  SCRIPTVAR_OBJECT |
									  SCRIPTVAR_ARRAY |
									  SCRIPTVAR_NULL |
									  SCRIPTVAR_INFINITY |
									  SCRIPTVAR_NAN,

	SCRIPTVAR_NATIVE			= SCRIPTVAR_NATIVE_FNC |
									  SCRIPTVAR_NATIVE_MFNC,
};
enum RUNTIME_FLAGS {
	RUNTIME_CANRETURN			= 1<<0,

	RUNTIME_CANBREAK			= 1<<1,
	RUNTIME_BREAK				= 1<<2,
	RUNTIME_BREAK_MASK		= RUNTIME_CANBREAK | RUNTIME_BREAK,

	RUNTIME_CANCONTINUE		= 1<<3,
	RUNTIME_CONTINUE			= 1<<4,
	RUNTIME_LOOP_MASK			= RUNTIME_BREAK_MASK | RUNTIME_CANCONTINUE | RUNTIME_CONTINUE,

	RUNTIME_NEW					= 1<<5,

	RUNTIME_CANTHROW			= 1<<6,
	RUNTIME_THROW				= 1<<7,
	RUNTIME_THROW_MASK		= RUNTIME_CANTHROW | RUNTIME_THROW,

	RUNTIME_PASS_1				= 1<<8,
	RUNTIME_PASS_2				= 1<<9,
	RUNTIME_PASS_MASK			= RUNTIME_PASS_1 | RUNTIME_PASS_2,
};

#define SAVE_RUNTIME_RETURN	int old_return_runtimeFlags = runtimeFlags & RUNTIME_CANRETURN
#define RESTORE_RUNTIME_RETURN	runtimeFlags = (runtimeFlags & ~RUNTIME_CANRETURN) | old_pass_runtimeFlags
#define SET_RUNTIME_CANRETURN runtimeFlags |= RUNTIME_CANRETURN
#define IS_RUNTIME_CANRETURN ((runtimeFlags & RUNTIME_CANRETURN) == RUNTIME_CANRETURN)

#define SAVE_RUNTIME_PASS	int old_pass_runtimeFlags = runtimeFlags & RUNTIME_PASS_MASK
#define RESTORE_RUNTIME_PASS	runtimeFlags = (runtimeFlags & ~RUNTIME_PASS_MASK) | old_pass_runtimeFlags
#define SET_RUNTIME_PASS_SINGLE runtimeFlags = (runtimeFlags & ~RUNTIME_PASS_MASK)
#define IS_RUNTIME_PASS_SINGLE ((runtimeFlags & RUNTIME_PASS_MASK) == 0)
#define SET_RUNTIME_PASS_TWO_1 runtimeFlags = (runtimeFlags & ~RUNTIME_PASS_MASK) | RUNTIME_PASS_1
#define IS_RUNTIME_PASS_TWO_1 ((runtimeFlags & RUNTIME_PASS_MASK) == RUNTIME_PASS_1)
#define SET_RUNTIME_PASS_TWO_2 runtimeFlags = (runtimeFlags & ~RUNTIME_PASS_MASK) | RUNTIME_PASS_2
#define IS_RUNTIME_PASS_TWO_2 ((runtimeFlags & RUNTIME_PASS_MASK) == RUNTIME_PASS_2)

#define TINYJS_RETURN_VAR "return"
#define TINYJS_LOKALE_VAR "__locale__"
#define TINYJS_ANONYMOUS_VAR "__anonymous__"
#define TINYJS_ARGUMENTS_VAR "arguments"
#define TINYJS_PROTOTYPE_CLASS "prototype"
#define TINYJS_TEMP_NAME ""
#define TINYJS_BLANK_DATA ""

/// convert the given string into a quoted string suitable for javascript
string getJSString(const string &str);

class CScriptException {
public:
	string text;
	strsize pos;
	CScriptException(const string &exceptionText, strsize Pos=-1);
};

class CScriptLex
{
protected:

   /* When we go into a loop, we use getSubLex to get a lexer for just the sub-part of the
	   relevant string. This doesn't re-allocate and copy the string, but instead copies
	   the data pointer and sets dataOwned to false, and dataStart/dataEnd to the relevant things. */
	char *data; ///< Data string to get tokens from
	strsize dataStart, dataEnd; ///< Start and end position in data string
	bool dataOwned; ///< Do we own this data string?

	strsize dataPos; ///< Position in data (we CAN go past the end of the string here)

public:
	CScriptLex(const string &input);
	CScriptLex(CScriptLex *owner, strsize startChar, strsize endChar);
	~CScriptLex();

	char currCh, nextCh;
	int tk; ///< The type of the token that we have
	strsize tokenStart; ///< Position in the data at the beginning of the token we have here
	strsize tokenEnd; ///< Position in the data at the last character of the token we have here
	strsize tokenLastEnd; ///< Position in the data at the last character of the last token
	string tkStr; ///< Data contained in the token we have here

	void match(int expected_tk); ///< Lexical match wotsit
	static string getTokenStr(int token); ///< Get the string representation of the given token
	void reset(strsize toPos=0); ///< Reset this lex so we can start again

	string getSubString(strsize pos); ///< Return a sub-string from the given position up until right now
	CScriptLex *getSubLex(strsize lastPosition); ///< Return a sub-lexer from the given position up until right now
	strsize getDataPos();
	string getPosition(strsize pos=-1); ///< Return a string representing the position in lines and columns of the character pos given

protected:

	void getNextCh();
	void getNextToken(); ///< Get the text token from our text string

};

class CScriptVar;

typedef void (*JSCallback)(CScriptVar *var, void *userdata);

class CScriptVarLink
{
public:
	string name;
	CScriptVar *var;
	CScriptVar *owner; // pointer to the owner CScriptVar
	struct {
		bool owned				:1;
		bool dontDelete		:1;
		bool dontEnumerable	:1;
		bool hidden				:1;
	};
	CScriptVarLink(CScriptVar *var, const string &name = TINYJS_TEMP_NAME);
	CScriptVarLink(const CScriptVarLink &link); ///< Copy constructor
	~CScriptVarLink();
	void replaceWith(CScriptVar *newVar); ///< Replace the Variable pointed to
	void replaceWith(CScriptVarLink *newVar); ///< Replace the Variable pointed to (just dereferences)
};

typedef comparable_array<CScriptVar *> RECURSION_VECT;
typedef comparable_array<CScriptVar *> RECURSION_SET;
class RECURSION_SET_VAR
{
public:
	RECURSION_SET_VAR(CScriptVar *PathBase) : sumRefs(0), sumInternalRefs(0), recursionPathBase(PathBase) {}
	RECURSION_SET recursionSet;		///< contains all Vars in the Set
	int sumRefs;							///< Sum of Refs
	int sumInternalRefs;					///< Sum of Internal Refs --> sumRefs - sumInternalRefs = sumExternalRefs
	CScriptVar *recursionPathBase;	///< temporary used in recoursionCheck() and trace()
};


class NativeFncBase;
typedef	::collection::string_to_pointer <CScriptVarLink> SCRIPTVAR_CHILDS;
/// Variable class (containing a doubly-linked list of children)
class CScriptVar
{
public:
	CScriptVar(); ///< Create undefined
	CScriptVar(const string &varData, int varFlags); ///< User defined
	CScriptVar(const string &str); ///< Create a string
	CScriptVar(const char *str); ///< Create a string
	CScriptVar(double varData);
   CScriptVar(int val);
	CScriptVar(int64_t val);
	CScriptVar(bool val);
	~CScriptVar();

	CScriptVar *getReturnVar(); ///< If this is a function, get the result value (for use by native functions)
	void setReturnVar(CScriptVar *var); ///< Set the result value. Use this when setting complex return data as it avoids a deepCopy()
	CScriptVar *getParameter(const string &name); ///< If this is a function, get the parameter with the given name (for use by native functions)
	CScriptVar *getParameter(index Idx); ///< If this is a function, get the parameter with the given index (for use by native functions)
	int64_t getParameterLength(); ///< If this is a function, get the count of parameters
	CScriptVarLink *findChild(const string &childName); ///< Tries to find a child with the given name, may return 0
	CScriptVarLink *findChildOrCreate(const string &childName, int varFlags=SCRIPTVAR_UNDEFINED); ///< Tries to find a child with the given name, or will create it with the given flags
	CScriptVarLink *findChildOrCreateByPath(const string &path); ///< Tries to find a child with the given path (separated by dots)
	CScriptVarLink *addChild(const string &childName, CScriptVar *child=NULL);
	CScriptVarLink *addChildNoDup(const string &childName, CScriptVar *child=NULL); ///< add a child overwriting any with the same name
	bool removeLink(CScriptVarLink *&link); ///< Remove a specific link (this is faster than finding via a child)
	void removeAllChildren();
	CScriptVar *getArrayIndex(index idx); ///< The the value at an array index
	void setArrayIndex(index idx, CScriptVar *value); ///< Set the value at an array index
	count getArrayLength(); ///< If this is an array, return the number of items in it (else 0)
	count getChildren() { return (int) Childs.get_size(); } ///< Get the number of children

	int64_t getInt();
	bool getBool();
	double getDouble();
	const string &getString();
	string getParsableString(); ///< get Data as a parsable javascript string
	string getVarType(); ///< get Data as a parsable javascript string
   void setInt(int64_t num);
	void setBool(bool val);
	void setDouble(double val);
	void setString(const string &str);
	void setUndefined();

	bool isInt() { return (flags&SCRIPTVAR_INTEGER)!=0; }
	bool isBool() { return (flags&SCRIPTVAR_BOOLEAN)!=0; }
	bool isDouble() { return (flags&SCRIPTVAR_DOUBLE)!=0; }
	bool isString() { return (flags&SCRIPTVAR_STRING)!=0; }
	bool isNumeric() { return (flags&SCRIPTVAR_NUMERICMASK)!=0; }
	bool isFunction() { return (flags&SCRIPTVAR_FUNCTION)!=0; }
	bool isObject() { return (flags&SCRIPTVAR_OBJECT)!=0; }
	bool isArray() { return (flags&SCRIPTVAR_ARRAY)!=0; }
	bool isNative() { return (flags&SCRIPTVAR_NATIVE)!=0; }
	bool isNative_ClassMemberFnc() { return (flags&SCRIPTVAR_NATIVE_MFNC)!=0; }
	bool isUndefined() { return (flags & SCRIPTVAR_VARTYPEMASK) == SCRIPTVAR_UNDEFINED; }
	bool isNull() { return (flags & SCRIPTVAR_NULL)!=0; }
	bool isNaN() { return (flags & SCRIPTVAR_NAN)!=0; }
	bool isInfinity() { return (flags&SCRIPTVAR_INFINITY)!=0; }
	bool isBasic() { return Childs.is_empty() != FALSE; } ///< Is this *not* an array/object/etc

	CScriptVar *mathsOp(CScriptVar *b, int op); ///< do a maths op with another script variable
	void copyValue(CScriptVar *val); ///< copy the value from the value given
	CScriptVar *deepCopy(); ///< deep copy this node and return the result

	void trace(const char * name = ""); ///< Dump out the contents of this using trace
	void trace(string & indentStr, const char * name = ""); ///< Dump out the contents of this using trace
	string getFlagsAsString(); ///< For debugging - just dump a string version of the flags
	void getJSON(_template_std_ostringstream &destination, const string linePrefix=""); ///< Write out all the JS code needed to recreate this script variable to the stream (as JSON)
	void setCallback(JSCallback callback, void *userdata);
	void setCallback(NativeFncBase *callback, void *userdata);

	SCRIPTVAR_CHILDS Childs;

	/// For memory management/garbage collection
	void recoursionCheck(CScriptVar *Owner=0);
	CScriptVar *ref(); ///< Add reference to this variable
	void unref(CScriptVar* Owner); ///< Remove a reference, and delete this variable if required
	int getRefs(); ///< Get the number of references to this script variable
private:
	RECURSION_SET_VAR *unrefInternal();
protected:
	int refs; ///< The number of references held to this - used for garbage collection
	int internalRefs;
	CScriptVarLink *__proto__;
	string data; ///< The contents of this variable if it is a string
	int64_t intData; ///< The contents of this variable if it is an int
	double doubleData; ///< The contents of this variable if it is a double
	int flags; ///< the flags determine the type of the variable - int/double/string/etc
	union
	{
		JSCallback jsCallback; ///< Callback for native functions
		NativeFncBase *jsCallbackClass; ///< Wrapper for Class-Member-Functions as Callback for native functions
	};
	void *jsCallbackUserData; ///< user data passed as second argument to native functions

	void init(); ///< initialisation of data members

	/** Copy the basic data and flags from the variable given, with no
		* children. Should be used internally only - by copyValue and deepCopy */
	void copySimpleData(CScriptVar *val);
	void recoursionCheck(RECURSION_VECT &recursionPath);
	int recursionFlag;
	RECURSION_SET_VAR *recursionSet;
	friend class tinyjs;
};

class NativeFncBase
{
public:
	virtual void operator()(CScriptVar *v, void *userdata)=0;
};
template <class native>
class NativeFnc : public NativeFncBase
{
public:
	NativeFnc(native *ClassPtr, void (native::*ClassFnc)(CScriptVar *, void *))
		: classPtr(ClassPtr), classFnc(ClassFnc){}
	void operator()(CScriptVar *v, void *userdata)
	{
		(classPtr->*classFnc)(v, userdata);
	}
	native *classPtr;
	void (native::*classFnc)(CScriptVar *v, void *userdata);
};

class CScriptVarSmartLink
{
public:
	CScriptVarSmartLink ();
	CScriptVarSmartLink (CScriptVarLink *Link);
	explicit CScriptVarSmartLink (CScriptVar *Var);
	~ CScriptVarSmartLink ();
	// Copy
	CScriptVarSmartLink (const CScriptVarSmartLink &Link);
	CScriptVarSmartLink &operator = (const CScriptVarSmartLink &Link);
	CScriptVarSmartLink &operator = (CScriptVarLink *Link);
	CScriptVarSmartLink &operator = (CScriptVar *Var);
	CScriptVarLink *operator ->() { return link; }
	CScriptVarLink &operator *() { return *link; }
	CScriptVarSmartLink &operator << (CScriptVarSmartLink &Link);
	operator bool() { return link != 0; }
//	operator CScriptVarLink *() { return link; }
	CScriptVarLink *&getLink() { return link; };
private:
	CScriptVarLink *link;
};

class tinyjs :
   virtual public ::radix::object
{
public:
	tinyjs(::ca::application * papp, bool TwoPass=true);
	~tinyjs();

	void execute(const string &code);
	/** Evaluate the given code and return a link to a javascript object,
	 * useful for (dangerous) JSON parsing. If nothing to return, will return
	 * 'undefined' variable type. CScriptVarLink is returned as this will
	 * automatically unref the result as it goes out of scope. If you want to
	 * keep it, you must use ref() and unref() */
	CScriptVarLink evaluateComplex(const string &code);
	/** Evaluate the given code and return a string. If nothing to return, will return
	 * 'undefined' */
	string evaluate(const string &code);

	/// add a native function to be called from TinyJS
	/** example:
		\code
			void scRandInt(CScriptVar *c, void *userdata) { ... }
			tinyJS->addNative("function randInt(min, max)", scRandInt, 0);
		\endcode

		or

		\code
			void scSubstring(CScriptVar *c, void *userdata) { ... }
			tinyJS->addNative("function String.substring(lo, hi)", scSubstring, 0);
		\endcode
		or

		\code
			class Class
			{
			public:
				void scSubstring(CScriptVar *c, void *userdata) { ... }
			};
			Class Instanz;
			tinyJS->addNative("function String.substring(lo, hi)", &Instanz, &Class::*scSubstring, 0);
		\endcode
	*/
	void addNative(const string &funcDesc, JSCallback ptr, void *userdata=0);
	void addNative(const string &funcDesc, NativeFncBase *ptr, void *userdata=0);
	template<class C>
	void addNative(const string &funcDesc, C *class_ptr, void(C::*class_fnc)(CScriptVar *, void *), void *userdata=0)
	{
		addNative(funcDesc, new NativeFnc<C>(class_ptr, class_fnc), userdata);
	}

	/// Get the value of the given variable, or return 0
	const string *getVariable(const string &path);

	/// Send all variables to stdout
	void trace();

	CScriptVar *root;   /// root of symbol table
private:
	bool twoPass;
	strsize funcOffset;
	CScriptLex *l;             /// current lexer
	int runtimeFlags;
	comparable_array <CScriptVar*> scopes; /// stack of scopes when parsing
	CScriptVar *stringClass; /// Built in string class
	CScriptVar *objectClass; /// Built in object class
	CScriptVar *arrayClass; /// Built in array class
	CScriptVar *exceptionVar; /// containing the exception var by (runtimeFlags&RUNTIME_THROW) == true;
	void CheckRightHandVar(bool &execute, CScriptVarSmartLink &link, int pos=-1)
	{
		if(execute && link && !link->owned && !link->owner && link->name.get_length()>0)
			throwError(execute, link->name + " is not defined", pos);
	}

	// parsing - in order of precedence
	CScriptVarSmartLink factor(bool &execute);
	CScriptVarSmartLink unary(bool &execute);
	CScriptVarSmartLink term(bool &execute);
	CScriptVarSmartLink expression(bool &execute);
	CScriptVarSmartLink binary_shift(bool &execute);
	CScriptVarSmartLink relation(bool &execute, int set=LEX_EQUAL, int set_n='<');
	CScriptVarSmartLink logic_binary(bool &execute, int op='|', int op_n1='^', int op_n2='&');
	CScriptVarSmartLink logic(bool &execute, int op=LEX_OROR, int op_n=LEX_ANDAND);
	CScriptVarSmartLink condition(bool &execute);
	CScriptVarSmartLink assignment(bool &execute);
	CScriptVarSmartLink base(bool &execute);
	void block(bool &execute);
	CScriptVarSmartLink statement(bool &execute);
	// parsing utility functions
	CScriptVarSmartLink parseFunctionDefinition();
	void parseFunctionArguments(CScriptVar *funcVar);

	CScriptVarLink *findInScopes(const string &childName); ///< Finds a child, looking recursively up the scopes
	/// Look up in any parent classes of the given object
	CScriptVarLink *findInParentClasses(CScriptVar *object, const string &name);
	CScriptVar *addNative(const string &funcDesc);

	/// throws an Error
	void throwError(bool &execute, const string &message, strsize pos=-1);

	/// native Functions
	void scEval(CScriptVar *c, void *data);

};

#endif

