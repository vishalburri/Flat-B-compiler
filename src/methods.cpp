#include <bits/stdc++.h>
#include "classes.h"
using namespace std;
using namespace llvm;
extern Visitor* v;
extern Visitor* t;

class symboltable table ;
static Module *MyModule = new Module("FlatB compiler",getGlobalContext());
static LLVMContext &Context = getGlobalContext();
static IRBuilder<> Builder(Context);
llvm::FunctionType *funcType = llvm::FunctionType::get(Builder.getVoidTy(), false);
llvm::Function *mainFunc = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "main", MyModule);

/* --------------------- Constructors ---------------------*/
Var::Var(string declType, string name, unsigned int length){
  this->declType = declType;
  this->name = name;
  this->length = length;
}

Var::Var(string declType, string name){
  this->declType = declType;
  this->name = name;
}

Decl::Decl(string dataType, class Vars* vars){
  this->dataType = dataType;
  this->var_list = vars->getVarsList();
  for(int i = 0; i < var_list.size(); i++){
    var_list[i]->setDataType(dataType);
  }
}

Decls::Decls(){
  this->cnt = 0;
}
Readdecl::Readdecl(class Readvars* vars){
  this->readvarlist = vars->getReadVarsList();
}
Readvar::Readvar(string declType,string name){
  this->declType=declType;
  this->name=name;
}
Readvar::Readvar(string declType,string name,class Expr* expr){
  this->declType=declType;
  this->name=name;
  this->expr=expr;
}

Printdecl::Printdecl(string text,class Printvars* vars,string type){
  this->type=type;
  if(text!="None")
  this->text = text.substr(1,text.length()-2);
  this->printvarlist = vars->getPrintVarsList();
}
Printvar::Printvar(string declType,string name){
  this->declType=declType;
  this->name=name;
}
Printvar::Printvar(string declType,string name,class Expr* expr){
  this->declType=declType;
  this->name=name;
  this->expr=expr;
}




ArthExpr::ArthExpr(class Expr* lhs, string opr, class Expr* rhs){
  this->lhs = lhs;
  this->rhs = rhs;
  this->opr = opr;
}
BoolExpr::BoolExpr(class Expr* lhs, string opr, class Expr* rhs){
  this->lhs = lhs;
  this->rhs = rhs;
  this->opr = opr;
}

Location::Location(string var, string location_type, class Expr* expr){
  this->var = var;
  this->location_type = location_type;
  this->expr = expr;
}

Location::Location(string var, string location_type){
  this->var = var;
  this->location_type = location_type;
}

intLiteral::intLiteral(int value){
  this->value = value;
}


Stmts::Stmts(){
  this->cnt = 0;
}

Goto::Goto(string label,class BoolExpr* expr){
  this->label=label;
  this->expr=expr;
}

NormalSt::NormalSt(string type,string name,class Stmt* stmt){
  this->stmt=stmt;
  this->type=type;
  this->name=name;
  if(type=="goto")
  table.gotoinsert(type,name);
}


Block::Block( class Stmts* stmts){
  this->stmts_list = stmts;
}

Assignment::Assignment(class Location* loc, string oper, class Expr* expr){
  this->loc = loc;
  this->opr = oper;
  this->expr = expr;
}
Ifstmt::Ifstmt(class BoolExpr* cond, class Block* block1, class Block* block2){
  this->condition = cond;
  this->if_block = block1;
  this->else_block = block2;
}
Whilestmt::Whilestmt(class BoolExpr* cond, class Block* block1){
  this->condition = cond;
  this->while_block = block1;
}
Forstmt::Forstmt(string loc, class Expr* init, class Expr* cond,int incre, class Block* block){
  this->var = loc;
  this->init = init;
  this->condition = cond;
  this->incre = incre;
  this->body = block;
}

Prog::Prog(class Decls* decls, class Stmts* stmts){
  this->stmts = stmts;
  this->fields = decls;
}

/* --------------------- Methods --------------------- */

bool Var::isArray(){
  return (declType.compare("Array") == 0);
}
bool Readvar::isArray(){
  return (declType.compare("Array") == 0);
}

bool Printvar::isArray(){
  return (declType.compare("Array") == 0);
}
bool Printdecl::isnewline(){
  return (type.compare("Newline") == 0);
}
bool Printdecl::istext(){
  return (text.compare("None") != 0);
}

void Var::setDataType(string datatype){
  this->dataType = datatype;
}

string Var::getName(){
  return name;
}

vector<class Var*> Vars::getVarsList(){
  return vars_list;
}

void Vars::push_back(class Var* var){
  vars_list.push_back(var);
  cnt++;
}

vector<class Var*> Decl::getVarsList(){
  return var_list;
}


vector<class Readvar*> Readdecl::getReadVarsList(){
  return readvarlist;
}

vector<class Printvar*> Printdecl::getPrintVarsList(){
  return printvarlist;
}

void Readvars::push_back(class Readvar* var){
  readvarslist.push_back(var);
  cnt++;
}

void Printvars::push_back(class Printvar* var){
  printvarslist.push_back(var);
  cnt++;
}

vector<class Readvar*> Readvars::getReadVarsList(){
  return readvarslist;
}
vector<class Printvar*> Printvars::getPrintVarsList(){
  return printvarslist;
}

void Decls::push_back(class Decl* var){
  decl_list.push_back(var);
  cnt++;
}


int intLiteral::getValue(){
  return value;
}


string Location::getVar(){

  return var;
}

bool Location::is_array(){
  return location_type.compare("Array") == 0;
}


class Expr* Location::getExpr(){
  return expr;
}

void Stmts::push_back(class St* stmt){
  stmts.push_back(stmt);
  cnt++;
}

void symboltable::gotoinsert(string a,string b){
  for(int i=0;i<symlist.size();i++){
    if(symlist[i]->name==b){
        cout<<b<<":Label redeclared\n";
        exit(0);
      }
    }
    struct symtable* temp = new symtable();
    temp->declType=a;
    temp->name=b;
    symlist.push_back(temp);

}
void symboltable::insert(string a,string b,string c,unsigned int d,int e){
  for(int i=0;i<symlist.size();i++){
    if(symlist[i]->name==b){
        cout<<b<<":Variable redeclared\n";
        exit(0);
      }
    }
    struct symtable* temp = new symtable();
    temp->declType=a;
    temp->name=b;
    temp->dataType=c;
    temp->length=d;
    int i=0;
    while(i<d){
    temp->arr.push_back(e);
    i++;
  }
  symlist.push_back(temp);

}
void symboltable::update(string b,int pos,int val){
  for(int i=0;i<symlist.size();i++){
    if(symlist[i]->name==b){
        symlist[i]->arr[pos]=val;
        return;
      }
    }
}
void symboltable::gotoupdate(string b,int pos){
  for(int i=0;i<symlist.size();i++){
    if(symlist[i]->name==b){
        symlist[i]->index=pos;
        return;
      }
    }
}
void symboltable::gotoirupdate(string b,BasicBlock* ptr){
  for(int i=0;i<symlist.size();i++){
    if(symlist[i]->name==b){
        symlist[i]->gotoptr=ptr;
        return;
      }
    }
  }
void symboltable::lookup(string b){
  int fl=0;
  for(int i=0;i<symlist.size();i++){
    if(symlist[i]->name==b){
        fl=1;
        return;
      }
    }

if(fl==0){
  cout<<b<<":Variable not declared\n";
  exit(0);
}

  }
//VISITOR DESIGN PATTERN


//traversals


void Traverse::visit(Decls* p){
  for (int i = 0; i < p->decl_list.size(); i++){
    p->decl_list[i]->accept(t);
  }
}

void Traverse::visit(Decl* p){
  for(int i = 0;  i < p->var_list.size(); i++){
    p->var_list[i]->accept(t);
  }
}

int Traverse::visit(Readdecl* p){
  for(int i = 0;  i < p->readvarlist.size(); i++){
    int a = p->readvarlist[i]->accept(t);
  }
  return 1;
}

int Traverse::visit(Printdecl* p){

  for(int i = 0;  i < p->printvarlist.size(); i++){
    int a = p->printvarlist[i]->accept(t);
  }
  return 1;
}


void Traverse::visit(Var* p){
  if(p->declType.compare("Array") == 0){
    table.insert(p->declType,p->name,p->dataType,p->length,0);
  }
  else
    table.insert(p->declType,p->name,p->dataType,1,0);
}
void Traverse::visit(Vars* p){
  p->cnt++;
}

int Traverse::visit(Readvar* p){
  table.lookup(p->name);
  if(p->declType.compare("Array") == 0){
    p->expr->accept(t);
  }
  return 1;
}


int Traverse::visit(Printvar* p){
  table.lookup(p->name);
  if(p->declType.compare("Array") == 0){
    p->expr->accept(t);
  }
}


void Traverse::visit(Stmts* p){
  //table.vecst = p->stmts;
  for(int i = 0; i < p->stmts.size(); i++){
    table.vecst = p->stmts;

    int a= p->stmts[i]->accept(t);
  }
}
int Traverse::visit(Block* p){
  p->stmts_list->accept(t);
  return 1;
}
int Traverse::visit(Goto* p){
  table.lookup(p->label);
  if(p->expr!=NULL)
  p->expr->accept(t);
  return 1;
}
int Traverse::visit(NormalSt* p){
  if(p->type=="goto"){
    for(int i=0;i<table.vecst.size();i++){
      if(table.vecst[i]==p)
        table.gotoupdate(p->name,i);
    }
    BasicBlock *gotobl = BasicBlock::Create(Context,"goto",mainFunc);
    table.gotoirupdate(p->name,gotobl);
  }
  int a= p->stmt->accept(t);
  return 1;
}

int Traverse::visit(Assignment* p){
  table.lookup(p->loc->getVar());

  if(p->loc->is_array()){
    for(int i=0;i<table.symlist.size();i++){
      if(table.symlist[i]->name==p->loc->getVar() && table.symlist[i]->declType=="Normal"){
          cout<<p->loc->getVar()<<":Given type is normal not array\n ";
          exit(0);
        }
      }
    p->loc->getExpr()->accept(t);
  }
  p->expr->accept(t);
  return 1;
}

int Traverse::visit(Ifstmt* p){
  p->condition->accept(t);
  p->if_block->accept(t);
  if(p->else_block!=NULL){
    p->else_block->accept(t);
  }
  return 1;
}

int Traverse::visit(Whilestmt* p){
  p->condition->accept(t);
  p->while_block->accept(t);
  return 1;
}

int Traverse::visit(Forstmt* p){
  table.lookup(p->var);
  p->init->accept(t);
  p->condition->accept(t);
  p->body->accept(t);
  return 1;
}

int Traverse::visit(Location* p){
  if(p->is_array()){
    p->expr->accept(t);
  }
  return 1;
}

int Traverse::visit(intLiteral* p){
return 1;
}


int Traverse::visit(ArthExpr* p){
  p->lhs->accept(t);
  p->rhs->accept(t);
  return 1;
}

int Traverse::visit(BoolExpr* p){
  p->lhs->accept(t);
  p->rhs->accept(t);
  return 1;
}

int Traverse::visit(Readvars* p){
  p->cnt++;
  return 1;
}


int Traverse::visit(Printvars* p){
  p->cnt++;
  return 1;
}


void Traverse::visit(Prog* p){
  p->fields->accept(t);
  llvm::BasicBlock *entry = llvm::BasicBlock::Create(Context, "entrypoint", mainFunc);
  Builder.SetInsertPoint(entry);
  p->stmts->accept(t);
}


//interpreter
////////////////////////////////////

void Evaluate::visit(Decls* p){
  p->cnt++;
}
void Evaluate::visit(Decl* p){
  p->cnt++;
}
void Evaluate::visit(Vars* p){
  p->cnt++;
}
void Evaluate::visit(Var* p){
  p->cnt++;

}

void Evaluate::visit(Prog* p){
  p->stmts->accept(v);
}

void Evaluate::visit(Stmts* p){
  for(int i = 0; i < p->stmts.size(); i++){
    int a = p->stmts[i]->accept(v);
    if(a==0)
    break;
  }
}

int Evaluate::visit(NormalSt* p){

  int a = p->stmt->accept(v);
}

int Evaluate::visit(Assignment* p){

  if(!p->loc->is_array()){
  for(int i=0;i<table.symlist.size();i++){
    if(table.symlist[i]->name==p->loc->getVar()){
        table.symlist[i]->arr[0] = p->expr->accept(v);
      }
    }
  }
  if(p->loc->is_array()){
    for(int i=0;i<table.symlist.size();i++){
      if(table.symlist[i]->name==p->loc->getVar()){
          table.symlist[i]->arr[p->loc->getExpr()->accept(v)] = p->expr->accept(v);
        }
      }
  }
  return 1;
}
int Evaluate::visit(intLiteral* p){
  return p->value;
}

int Evaluate::visit(Location* p){
  if(p->is_array()){
    for(int i=0;i<table.symlist.size();i++){
      if(table.symlist[i]->name==p->var){
      if(p->expr->accept(v)<table.symlist[i]->length && p->expr->accept(v)>=0)
              return table.symlist[i]->arr[p->expr->accept(v)];
      else{
              cout<<table.symlist[i]->name<<":Array out of bound\n";
                exit(0);
          }

      }
    }
  }
  if(!p->is_array()){
  for(int i=0;i<table.symlist.size();i++){
    if(table.symlist[i]->name==p->var){
        return table.symlist[i]->arr[0];
      }
    }
  }
  return 1;
}

int Evaluate::visit(Printdecl* p){

  if(p->istext())
  cout<<p->text;
  for(int i = 0;  i < p->printvarlist.size(); i++){
    int a= p->printvarlist[i]->accept(v);
  }
  if(p->isnewline())
  cout<<"\n";
  return 1;
}



int Evaluate::visit(Printvar* p){
  if(p->declType.compare("Array") != 0){

  for(int i=0;i<table.symlist.size();i++){
    if(table.symlist[i]->name==p->name){
        printf("%d",table.symlist[i]->arr[0]);
        return 1;
      }
    }
  }
  if(p->declType.compare("Array") == 0){
    for(int i=0;i<table.symlist.size();i++){
      if(table.symlist[i]->name==p->name){
          printf("%d",table.symlist[i]->arr[p->expr->accept(v)]);
          return 1;
        }
      }
  }
  return 1;
}

int Evaluate::visit(Readdecl* p){
  for(int i = 0;  i < p->readvarlist.size(); i++){
    int a= p->readvarlist[i]->accept(v);
  }
  return 1;
}

int Evaluate::visit(Readvars* p){
  p->cnt++;
  return 1;
}

int Evaluate::visit(Printvars* p){
  p->cnt++;
  return 1;
}

int Evaluate::visit(Readvar* p){
  if(p->declType.compare("Array") != 0){

  for(int i=0;i<table.symlist.size();i++){
    if(table.symlist[i]->name==p->name){
        cin>>table.symlist[i]->arr[0];
        return 1;
      }
    }
  }
  if(p->declType.compare("Array") == 0){
    for(int i=0;i<table.symlist.size();i++){
      if(table.symlist[i]->name==p->name){
          cin>>table.symlist[i]->arr[p->expr->accept(v)];
          return 1;
        }
      }
  }
  return 1;
}
int Evaluate::visit(Forstmt* p){
  int j;
  for(int i=0;i<table.symlist.size();i++){
    if(table.symlist[i]->name==p->var){
        table.symlist[i]->arr[0]=p->init->accept(v);
        j=i;
        break;
      }
    }
  for(int i=p->init->accept(v);i<p->condition->accept(v);i=i+p->incre){
    int a= p->body->accept(v);
    table.symlist[j]->arr[0]+=p->incre;
  }
  return 1;
}

int Evaluate::visit(Block* p){
   p->stmts_list->accept(v);
  return 1;
}
int Evaluate::visit(Whilestmt* p){
   while(p->condition->accept(v)==1){
   int a= p->while_block->accept(v);
  }
  return 1;
}
int Evaluate::visit(Ifstmt* p){
  if(p->condition->accept(v)==1){
  int a= p->if_block->accept(v);
  }
  else if(p->else_block!=NULL)
  int a= p->else_block->accept(v);
  return 1;
}

int Evaluate::visit(Goto* p){
  int pos,a;
    if(p->expr==NULL){
    a=1;
    }
  else if(p->expr!=NULL){
     a = p->expr->accept(v);
  }
    if(a==1){
    for(int i=0;i<table.symlist.size();i++){
      if(table.symlist[i]->name==p->label){
        pos = table.symlist[i]->index;
        break;
        }
    }

  for(int i=pos;i<table.vecst.size();i++){
     int z= table.vecst[i]->accept(v);
     if(z==0)
     {
       break;
     }
  }
  return 0;
}
return 1;
}

int Evaluate::visit(BoolExpr* p){
  if(p==NULL)
  return 1;
  if(p->opr=="<" && p->lhs->accept(v) < p->rhs->accept(v)){
      return 1;
  }
  if(p->opr==">" && p->lhs->accept(v) > p->rhs->accept(v)){
      return 1;
  }
  if(p->opr==">=" && p->lhs->accept(v) >= p->rhs->accept(v)){
      return 1;
  }
  if(p->opr=="<=" && p->lhs->accept(v) <= p->rhs->accept(v)){
      return 1;
  }
  if(p->opr=="==" && p->lhs->accept(v) == p->rhs->accept(v)){
      return 1;
  }
  if(p->opr=="!=" && p->lhs->accept(v) != p->rhs->accept(v)){
      return 1;
  }

  else
    return 0;
}
int Evaluate::visit(ArthExpr* p){
  if(p->opr=="+"){
      return p->lhs->accept(v)+p->rhs->accept(v);
  }
  if(p->opr=="-"){
      return p->lhs->accept(v)-p->rhs->accept(v);
  }
  if(p->opr=="*"){
      return p->lhs->accept(v)*p->rhs->accept(v);
  }
  if(p->opr=="/"){
      return p->lhs->accept(v)/p->rhs->accept(v);
  }
  if(p->opr=="%"){
      return p->lhs->accept(v)%p->rhs->accept(v);
  }

}
void Prog::startCode_gen(){
  MyModule->dump();
}
Value* Prog::Code_gen(){
  Value* v = ConstantInt::get(getGlobalContext(), APInt(32,0));
  fields->Code_gen();
  stmts->Code_gen();
  Builder.CreateRetVoid();
  return v;
}
Value* Decls::Code_gen(){
  Value* v = ConstantInt::get(getGlobalContext(), APInt(32,0));
  for(int i=0;i<decl_list.size();i++){
     decl_list[i]->Code_gen();
  }
  return v;
}
Value* Decl::Code_gen(){
  Value* v = ConstantInt::get(getGlobalContext(), APInt(32,0));
  for(int i=0;i<var_list.size();i++){
     var_list[i]->Code_gen();
  }
  return v;
}
Value* Vars::Code_gen(){
  Value* v = ConstantInt::get(getGlobalContext(), APInt(32,0));
  return v;
}
Value* Var::Code_gen(){
  llvm::Type *type ;
  type  = Type::getInt32Ty(Context);
  if(declType=="Array"){
    ArrayType* arrtype = ArrayType::get(type,length);
    MyModule->getOrInsertGlobal(name,arrtype);
    GlobalVariable *glv = MyModule->getNamedGlobal(name);
    glv->setLinkage(GlobalValue::ExternalLinkage);
    glv->setInitializer(ConstantAggregateZero::get(arrtype));
  }
  if(declType=="Normal"){
    MyModule->getOrInsertGlobal(name,Builder.getInt32Ty());
    GlobalVariable *glv = MyModule->getNamedGlobal(name);
    glv->setLinkage(GlobalValue::ExternalLinkage);
    glv->setAlignment(4);
    ConstantInt* const_int_val = ConstantInt::get(Context, APInt(32,0));
    glv->setInitializer(const_int_val);
  }
  Value* v = ConstantInt::get(getGlobalContext(), APInt(32,0));
  return v;
}
Value* Stmts::Code_gen(){
  Value* v = ConstantInt::get(getGlobalContext(), APInt(32,0));
  for(int i=0;i<stmts.size();i++){
    stmts[i]->Code_gen();
  }
  return v;
}
Value* NormalSt::Code_gen(){
  Value* v = ConstantInt::get(getGlobalContext(), APInt(32,0));
  if(type=="goto")
  {
    BasicBlock *preheader = Builder.GetInsertBlock();
    BasicBlock* gotobl;

    for(int i=0;i<table.symlist.size();i++){
      if(table.symlist[i]->name==name){
          gotobl = table.symlist[i]->gotoptr;
          break;
        }
      }
    Builder.CreateBr(gotobl);
    Builder.SetInsertPoint(gotobl);
  }
  stmt->Code_gen();
  return v;
}

Value* Goto::Code_gen(){
  Value* v = ConstantInt::get(getGlobalContext(), APInt(32,0));
  BasicBlock* x;
  for(int i=0;i<table.symlist.size();i++){
    if(table.symlist[i]->name==label){
        x = table.symlist[i]->gotoptr;
        break;
      }
    }
    BasicBlock *aftergoto = BasicBlock::Create(Context,"aftergoto",mainFunc);
  if(expr==NULL){
    Builder.CreateBr(x);
    Builder.SetInsertPoint(aftergoto);
  }
  else{
    Value* con = expr->Code_gen(1);
    Builder.CreateCondBr(con,x,aftergoto);
    Builder.SetInsertPoint(aftergoto);
  }
  return v;
}


Value* Assignment::Code_gen(){
  Value* value = expr->Code_gen(1);
  Value* var = loc->Code_gen(0);
  return Builder.CreateStore(value,var);
}
Value* Location::Code_gen(int ty){
  Value* v  = MyModule->getNamedGlobal(var);
  if(location_type =="Normal"){
    if(ty==0)
    return v;
    else
    return Builder.CreateLoad(v);
  }
  else{
    Value* pos = expr->Code_gen(1);
    vector<Value*> arrpos;
    arrpos.push_back(Builder.getInt32(0));
    arrpos.push_back(pos);
    v = Builder.CreateGEP(v,arrpos,"Pos");
    if(ty==1)
      v=Builder.CreateLoad(v);
    return v;
  }
}
Value* intLiteral::Code_gen(int ty){
  Value* v = ConstantInt::get(getGlobalContext(), APInt(32,value));
  return v;
}
Value* ArthExpr::Code_gen(int ty){
  Value* l = lhs->Code_gen(1);
  Value* r = rhs->Code_gen(1);
  Value* v;
  if(opr=="+")
    v = Builder.CreateAdd(l,r,"add");
  if(opr=="-")
    v = Builder.CreateSub(l,r,"sub");
  if(opr=="*")
    v = Builder.CreateMul(l,r,"mul");
  if(opr=="/")
    v = Builder.CreateUDiv(l,r,"div");
  if(opr=="%")
    v = Builder.CreateURem(l,r,"rem");
  return v;
}
Value* BoolExpr::Code_gen(int ty){
  Value* l = lhs->Code_gen(1);
  Value* r = rhs->Code_gen(1);
  Value* v;
  if(opr==">")
    v = Builder.CreateICmpUGT(l,r,"gt");
  if(opr=="<")
    v = Builder.CreateICmpULT(l,r,"lt");
  if(opr==">=")
    v = Builder.CreateICmpUGE(l,r,"ge");
  if(opr=="<=")
    v = Builder.CreateICmpULE(l,r,"le");
  if(opr=="==")
    v = Builder.CreateICmpEQ(l,r,"eq");
  if(opr=="!=")
    v = Builder.CreateICmpNE(l,r,"ne");

  return v;
}
Value* Printdecl::Code_gen(){
  Value* v ;
  vector<Value*>args;
  vector<Type *>type;
  string s;
  if(text=="None")
  text="";
  s = text+"%d ";
  string z = "%d";
  for(int i=1;i<printvarlist.size();i++)
  s = s+z;
  if(this->type=="Newline")
  s=s+"\n";
  Value* x = Builder.CreateGlobalStringPtr(s);
  args.push_back(x);
  type.push_back(x->getType());

  for(int i=0;i<printvarlist.size();i++){
    v = printvarlist[i]->Code_gen();
    args.push_back(v);
    llvm::Type *ty ;
    ty  = Type::getInt32Ty(Context);
    type.push_back(v->getType());
  }
  llvm::ArrayRef<Type*> typeargs(type);
  llvm::ArrayRef<Value*> refargs(args);
  llvm::FunctionType *FType = FunctionType::get(Type::getInt32Ty(Context), typeargs, false);
  Constant* printfunc = MyModule->getOrInsertFunction("printf", FType);
  return Builder.CreateCall(printfunc,refargs);
}
Value* Printvars::Code_gen(){
  Value* v = ConstantInt::get(getGlobalContext(), APInt(32,0));
  return v;
}
Value* Printvar::Code_gen(){
  Value* v = MyModule->getNamedGlobal(name);
  if(declType=="Normal"){
    v = Builder.CreateLoad(v);
    return v;
  }
  else{
    Value* pos = expr->Code_gen(1);
    vector<Value*> arrpos;
    arrpos.push_back(Builder.getInt32(0));
    arrpos.push_back(pos);
    v = Builder.CreateGEP(v,arrpos,"Pos");
    v = Builder.CreateLoad(v);
    return v;
  }
}

Value* Readdecl::Code_gen(){
  Value* v ;
  vector<Value*>args;
  vector<Type *>type;
  string s = "%d";
  for(int i=1;i<readvarlist.size();i++)
  s = s+s;
  Value* x = Builder.CreateGlobalStringPtr(s);
  args.push_back(x);
  type.push_back(x->getType());
  for(int i=0;i<readvarlist.size();i++){
    v = readvarlist[i]->Code_gen();
    args.push_back(v);
    type.push_back(v->getType());
  }
  llvm::ArrayRef<Type*> typeargs(type);
  llvm::ArrayRef<Value*> refargs(args);
  llvm::FunctionType *FType = FunctionType::get(Type::getInt32Ty(Context), typeargs, false);
  Constant* printfunc = MyModule->getOrInsertFunction("scanf", FType);
  return Builder.CreateCall(printfunc,refargs);
}
Value* Readvars::Code_gen(){
  Value* v = ConstantInt::get(getGlobalContext(), APInt(32,0));
  return v;
}
Value* Readvar::Code_gen(){
  Value* v = MyModule->getNamedGlobal(name);
  if(declType=="Normal"){

    return v;
  }
  else{
    Value* pos = expr->Code_gen(1);
    vector<Value*> arrpos;
    arrpos.push_back(Builder.getInt32(0));
    arrpos.push_back(pos);
    v = Builder.CreateGEP(v,arrpos,"Pos");
    return v;
  }
}
Value* Ifstmt::Code_gen(){
  Value *v = ConstantInt::get(getGlobalContext(), APInt(32,0));
  Value* c = condition->Code_gen(0);
  BasicBlock *ifbl = BasicBlock::Create(Context,"if",mainFunc);
  BasicBlock *elsebl = BasicBlock::Create(Context,"else",mainFunc);
  BasicBlock *mergebl = BasicBlock::Create(Context,"ifmerge",mainFunc);
  Builder.CreateCondBr(c,ifbl,elsebl);
  Builder.SetInsertPoint(ifbl);
  Value* ifv;
  Value* elsev;
  ifv = if_block->Code_gen();
  Builder.CreateBr(mergebl);
  ifbl = Builder.GetInsertBlock();
  Builder.SetInsertPoint(elsebl);
  if(else_block!=NULL)
  elsev = else_block->Code_gen();
  Builder.CreateBr(mergebl);
  elsebl = Builder.GetInsertBlock();
  Builder.SetInsertPoint(mergebl);
  return v;
}
Value* Whilestmt::Code_gen(){
  Value *v = ConstantInt::get(getGlobalContext(), APInt(32,0));
  Value* c = condition->Code_gen(0);
  BasicBlock *whilebl = BasicBlock::Create(Context,"while",mainFunc);
  BasicBlock *mergebl = BasicBlock::Create(Context,"whilemerge",mainFunc);
  Builder.CreateCondBr(c,whilebl,mergebl);
  Builder.SetInsertPoint(whilebl);
  Value* whilev = while_block->Code_gen();
  c = condition->Code_gen(0);
  Builder.CreateCondBr(c,whilebl,mergebl);
  Builder.SetInsertPoint(mergebl);
  return v;
}
Value* Forstmt::Code_gen(){
  Value *V = ConstantInt::get(getGlobalContext(), APInt(32,0));
  Value* start = init->Code_gen(1);
  Value* step_val = ConstantInt::get(getGlobalContext(),APInt(32,incre));
  BasicBlock *preheaderBB = Builder.GetInsertBlock();
  BasicBlock* loop_body = BasicBlock::Create(getGlobalContext(), "loop", mainFunc);
  BasicBlock *afterBB = BasicBlock::Create(getGlobalContext(), "afterloop", mainFunc);
  Value* t = MyModule->getNamedGlobal(var);
  Builder.CreateStore(start,t);
  Value* cond = condition->Code_gen(1);
  cond = Builder.CreateICmpULT(start, cond, "loopcondition");
  Builder.CreateCondBr(cond, loop_body, afterBB);
  Builder.SetInsertPoint(loop_body);
  cond = condition->Code_gen(1);
  Value* forv =  body->Code_gen();
  Value* x = Builder.CreateLoad(t);
  Value* nextval =  Builder.CreateAdd(x,step_val,"add");
  Builder.CreateStore(nextval,t);
  cond = Builder.CreateICmpULT(nextval, cond, "loopcondition");
  Builder.CreateCondBr(cond, loop_body, afterBB);
  Builder.SetInsertPoint(afterBB);
  return V;
}
Value* Block::Code_gen(){
  Value *v = ConstantInt::get(getGlobalContext(), APInt(32,0));
  stmts_list->Code_gen();
  return v;
}
