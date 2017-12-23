#include <bits/stdc++.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/IR/PassManager.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/CallingConv.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/IR/Verifier.h>
//#include <llvm/Assembly/PrintModulePass.h>
#include <llvm/IR/IRBuilder.h>
//#include <llvm/ModuleProvider.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/Support/raw_ostream.h>
using namespace std;
using namespace llvm;

union Node{
	int number;
	char* value;
	class Prog* prog;
	class Decls* fields;
	class Decl* field;
	class Vars* vars;
	class Var* var;
	class Stmts* stmts;
	class Stmt* stmt;

	class Expr* expr;
	class ArthExpr* arthexpr;
	class BoolExpr* boolexpr;

	class Block* block;
	class Location* location;
	class Assignment* assignment;
	class St* st;

	class Ifstmt* ifst;
	class Whilestmt* whilest;
	class Forstmt* forst;
	class Goto* go;

	class Readdecl* readstmt;
	class Readvars* readvars;
	class Readvar* readvar;

	class Printdecl* printstmt;
	class Printvars* printvars;
  class Printvar* printvar;


	class intLiteral* lit;
	class NormalSt* ns;

	class astNode* ast;
	class Literal* literal;

	Node(){
		number = 0;
		value = NULL;
		fields = NULL;
		field = NULL;
		stmts = NULL;
	}
	~Node(){};
};

typedef union Node YYSTYPE;

#define YYSTYPE_IS_DECLARED 1

struct symtable {
	string declType;
	string name;
	string dataType;
	unsigned int length;
	vector<int> arr;
	class Stmts* stmts;
	int index;
	BasicBlock* gotoptr;

};

class symboltable{
public:
	vector<struct symtable*> symlist;
	vector<class St*> vecst;
	void insert(string,string,string,unsigned int,int);
	void gotoinsert(string,string);
	void lookup(string);
	void update(string,int,int);
	void gotoupdate(string,int);
	void gotoirupdate(string,BasicBlock*);


};

class Visitor
{
  public:
		virtual void visit(Decls*)=0;
		virtual void visit(Decl*)=0;
		virtual void visit(Vars*)=0;
		virtual void visit(Var*)=0;
    virtual void visit(Stmts*) =0;
    virtual void visit(Prog*) =0;
    virtual int visit(Assignment*) = 0;
		virtual int visit(NormalSt*) = 0;
		virtual int visit(intLiteral*) = 0;
		virtual int visit(Location*) = 0;
		virtual int visit(Ifstmt*) = 0;
		virtual int visit(Whilestmt*) = 0;
		virtual int visit(Forstmt*) = 0;
		virtual int visit(Goto*) = 0;
		virtual int visit(Readdecl*) = 0;
		virtual int visit(Readvars*) = 0;
		virtual int visit(Readvar*) = 0;
		virtual int visit(Printdecl*) = 0;
		virtual int visit(Printvars*) = 0;
		virtual int visit(Printvar*) = 0;
		virtual int visit(ArthExpr*) = 0;
		virtual int visit(BoolExpr*) = 0;
		virtual int visit(Block*) = 0;
};

class astNode{
};

class Evaluate:public Visitor{
public:
	void visit(Decls* p);
	void visit(Decl* p);
	void visit(Vars* p);
	void visit(Var* p);
	void visit(Stmts* p);
	void visit(Prog* p);
	int visit(Assignment* p);
	int visit(NormalSt* p);
	int visit(intLiteral* p);
	int visit(Location* p);
	int visit(Ifstmt* p);
	int visit(Whilestmt* p);
	int visit(Forstmt* p);
	int visit(Goto* p);
	int visit(Readdecl* p);
	int visit(Readvars* p);
	int visit(Readvar* p);
	int visit(Printdecl* p);
  int visit(Printvars* p);
	int visit(Printvar* p);
  int visit(ArthExpr* p);
	int visit(BoolExpr* p);
	int visit(Block* p);
};

class Traverse:public Visitor{
public:
	 void visit(Decls* p);
	 void visit(Decl* p);
	 void visit(Vars* p);
	 void visit(Var* p);
	void visit(Stmts* p);
	void visit(Prog* p);
	int visit(Assignment* p);
	int visit(NormalSt* p);
	int visit(intLiteral* p);
	int visit(Location* p);
	int visit(Ifstmt* p);
	int visit(Whilestmt* p);
	int visit(Forstmt* p);
	int visit(Goto* p);
	int visit(Readdecl* p);
	int visit(Readvars* p);
	int visit(Readvar* p);
	int visit(Printdecl* p);
	int visit(Printvars* p);
	int visit(Printvar* p);
	int visit(ArthExpr* p);
	int visit(BoolExpr* p);
	int visit(Block* p);
};

class Var:public astNode{
public:
	int cnt;
	string declType;
	string name;
	string dataType;
	unsigned int length;
public:
	Var(string,string,unsigned int);
	Var(string,string);
	bool isArray();
	void setDataType(string);
	string getName();
	int getLength(){return length;}
	void accept(Visitor* v){
		return v->visit(this);
	}
	Value* Code_gen();

};

class Vars:public astNode{
public:
	vector<class Var*> vars_list;
	int cnt;
public:
	Vars(){}
	void push_back(class Var*);
	vector<class Var*> getVarsList();
	void accept(Visitor* v){
		return v->visit(this);
	}
	Value* Code_gen();

};

class Decl:public astNode{
public:
	string dataType;
	int cnt;
	vector<class Var*> var_list;
public:
	Decl(string,class Vars*);
	vector<class Var*> getVarsList();
	void accept(Visitor* v){
		return v->visit(this);
	}
	Value* Code_gen();

};

class Decls:public astNode{
public:
	vector<class Decl*> decl_list;
	int cnt;
public:
	Decls();
	void push_back(class Decl*);
	void accept(Visitor* v){
		return v->visit(this);
	}
	Value* Code_gen();

};

class Expr:public astNode{
public:
  virtual	int accept( Visitor* v){}
	virtual Value* Code_gen(int ty){}

};

class ArthExpr:public Expr{
public:
	class Expr* lhs;
	class Expr* rhs;
	string opr;
public:
	ArthExpr(class Expr*, string, class Expr*);
	int accept( Visitor* v){
		return v->visit(this);
	}
	Value* Code_gen(int ty);

};

class BoolExpr:public Expr{
public:
	class Expr* lhs;
	class Expr* rhs;
	string opr;
public:
	BoolExpr(class Expr*, string, class Expr*);
	int accept(Visitor* v){
	return v->visit(this);
	}
	Value* Code_gen(int ty);

};



class Location:public Expr{
public:
	string var;
	string location_type;
	class Expr* expr;
public:
	Location(string,string,class Expr*);
	Location(string,string);
	string getVar();
	bool is_array();
	class Expr* getExpr();
	int accept(Visitor* v){
		return v->visit(this);
	}
	Value* Code_gen(int ty);

};

class Literal:public Expr{
public:
	virtual int accept(Visitor* v){}
	virtual Value* Code_gen(int ty){}

};

class intLiteral:public Literal{
public:
	int value;
public:
	int accept(Visitor* v){
		return v->visit(this);
	}
	intLiteral(int);
	int getValue();
	Value* Code_gen(int ty);


};


class Stmt:public astNode{
public:
	virtual int accept( Visitor* v){}
	virtual Value* Code_gen(){}

};

class St:public astNode{
public:
	virtual int accept( Visitor* v){}
	virtual Value* Code_gen(){}

};



class Block:public Stmt{
public:
	class Stmts* stmts_list;
public:
	Block(class Stmts*);
	int accept(Visitor* v){
		return v->visit(this);
	}
	Value* Code_gen();

};

class Stmts:public astNode{
public:
	vector<class St*> stmts;
	int cnt;
public:
	Stmts();
  void accept( Visitor* v){
		return v->visit(this);
	}
	void push_back(class St*);
	Value* Code_gen();

};

class NormalSt:public St{
public:
	class Stmt* stmt;
	string type;
	string name;
public:
	NormalSt(string,string,class Stmt*);
	int accept(Visitor* v){
		return v->visit(this);
	}
	Value* Code_gen();

};

class Goto:public Stmt{
public:
	string label;
	class BoolExpr* expr;
public:
	Goto(string,class BoolExpr*);
	int accept( Visitor* v){
		return v->visit(this);
	}
	Value* Code_gen();

};

class Assignment:public Stmt{
public:
	class Location* loc;
	class Expr* expr;
	string opr;
public:

	Assignment(class Location*, string, class Expr*);
	int accept(Visitor* v){
		return v->visit(this);
	}
	Value* Code_gen();

};

class Ifstmt:public Stmt{
public:
	class BoolExpr* condition;
	class Block* if_block;
	class Block* else_block;
public:
	Ifstmt(class BoolExpr*, class Block*, class Block*);
	int accept( Visitor* v){
		return v->visit(this);
	}
	Value* Code_gen();

};

class Whilestmt:public Stmt{
public:
	class BoolExpr* condition;
	class Block* while_block;
public:
	Whilestmt(class BoolExpr*, class Block*);
	int accept( Visitor* v){
		return v->visit(this);
	}
	Value* Code_gen();

};

class Forstmt:public Stmt{
public:
	string var;
	class Expr* init;
	class Expr* condition;
	int incre;
	class Block* body;
public:
	Forstmt(string, class Expr*, class Expr*,int,class Block*);
	int accept( Visitor* v){
		return v->visit(this);
	}
	Value* Code_gen();

};

class Readdecl:public Stmt {
public:
	vector<class Readvar*> readvarlist;
public:
	Readdecl(class Readvars*);
	vector<class Readvar*> getReadVarsList();
	int accept( Visitor* v){
		return v->visit(this);
	}
	Value* Code_gen();

};

class Readvars:public astNode {
public:
	vector<class Readvar*> readvarslist;
	int cnt;
public:
	Readvars(){}
	void push_back(class Readvar*);
	vector<class Readvar*> getReadVarsList();
	int accept( Visitor* v){
		return v->visit(this);
	}
	Value* Code_gen();

};

class Readvar:public astNode {
public:
	string declType;
	string name;
	unsigned int length;
	class Expr* expr;
public:
	Readvar(string,string,class Expr*);
	Readvar(string,string);
	bool isArray();
	int accept( Visitor* v){
		return v->visit(this);
	}
	Value* Code_gen();

};

class Printdecl:public Stmt {
public:
	vector<class Printvar*> printvarlist;
	string text;
	string type;
public:
	Printdecl(string,class Printvars*,string);
	vector<class Printvar*> getPrintVarsList();
	bool isnewline();
	bool istext();
	int accept( Visitor* v){
		return v->visit(this);
	}
	Value* Code_gen();

};

class Printvars:public astNode {
public:
	vector<class Printvar*> printvarslist;
	int cnt;
public:
	Printvars(){}
	void push_back(class Printvar*);
	vector<class Printvar*> getPrintVarsList();
	int accept( Visitor* v){
	return v->visit(this);
	}
	Value* Code_gen();

};

class Printvar:public astNode {
public:
	string declType;
	string name;
	unsigned int length;
	class Expr* expr;
public:
	Printvar(string,string,class Expr*);
	Printvar(string,string);
	bool isArray();
	int accept( Visitor* v){
		return v->visit(this);
	}
	Value* Code_gen();

};

class Prog:public astNode{
public:
	class Stmts* stmts;
	class Decls* fields;
public:
	Prog(class Decls*,class Stmts*);
	void accept( Visitor* v){
		return v->visit(this);
	}
	Value* Code_gen();
	 void startCode_gen();
};
