//jpms:bc
/*----------------------------------------------------------------------------*\
 * File:        ast.hh
 *
 * Description: Implements AST construction and traversal methods.
 *
 * Author:      jpms
 * 
 *                                     Copyright (c) 2015, Joao Marques-Silva
\*----------------------------------------------------------------------------*/
//jpms:ec

#ifndef _AST_H
#define _AST_H 1

#include "includes.hh"
#include "xdefs.hh"
#include "visitor.hh"

class ASTTypesVecNode;
class ASTProgramNode;
class ASTMainNode;
class ASTClassRefNode;
class ASTClassNode;
class ASTClassesNode;
class ASTMethodRefNode;
class ASTMethodNode;
class ASTMethodsNode;
class ASTStmtsNode;
class ASTAssignStmtNode;
class ASTAssignIntVecStmtNode;
class ASTIfStmtNode;
class ASTWhileStmtNode;
class ASTPrintStmtNode;
class ASTOpExprNode;
class ASTIntConstNode;
class ASTBoolConstNode;
class ASTVarDeclsNode;
class ASTVarRefNode;
class ASTNewIdNode;
class ASTIntVecNode;
class ASTVecValueNode;
class ASTLengthNode;
class ASTNegationNode;
class ASTThisNode;

class ASTNode {
public:
    ASTNode() : value(), lnum(0) {}
    virtual ~ASTNode() { }
    virtual void accept(Visitor& v) = 0;
    virtual string get_type() = 0;
    virtual void set_type(string t) = 0;
    virtual void set_lineno(int ln) { lnum = ln; }
    virtual int get_lineno() { return lnum; }
    virtual void dump() = 0;
public:
    XValue value;
    int lnum; 
    vector<int> vec; 
    int instance_num;
 
};

class ASTTypesVecNode : public ASTNode {
public:
	ASTTypesVecNode() : ASTNode() { }
    ASTTypesVecNode(vector<string> v1, vector<ASTNewIdNode*> v2, vector<ASTAssignStmtNode*> v3) : ASTNode(), typesVec(v1), newIds(v2), paramAssigns(v3) { }
    virtual ~ASTTypesVecNode() { }
    virtual void accept(Visitor& v) { v.visit(*this); }
    virtual string get_type() { return "none"; }
    virtual void set_type(string t) { }
    virtual void dump() { }
    virtual bool type_defined(string s) { return (find(typesVec.begin(), typesVec.end(), s) != typesVec.end()); }
	virtual int number_of_types_defined() { return typesVec.size(); }
	virtual vector<string> get_types_vector() { return typesVec;}
	virtual void set_types_vector(vector<string> v) { typesVec = v; }
public:
    vector<string> typesVec;
    vector<ASTNewIdNode*> newIds;
    vector<ASTAssignStmtNode*> paramAssigns;
};

class ASTProgramNode : public ASTNode {
public:
    ASTProgramNode() : ASTNode(){ }
    ASTProgramNode(ASTNode* n1, ASTNode* n2) : ASTNode(), node1(n1), node2(n2) { }
    virtual ~ASTProgramNode() { }
    virtual void accept(Visitor& v) { v.visit(*this); }
    virtual string get_type() { return "prog"; }
    virtual void set_type(string t) { }
    virtual void dump() { }
public:
    ASTNode* node1;
    ASTNode* node2;
};

class ASTMainNode : public ASTNode {
public:
    ASTMainNode() : ASTNode() { }
    ASTMainNode(string s,ASTNode* n1) : ASTNode(), id(s), node1(n1) { }
    virtual ~ASTMainNode() { }
    virtual void accept(Visitor& v) { v.visit(*this); }
    virtual string get_type() { return id; }
    virtual void set_type(string t) { }
    virtual void dump() { }
public:
	string id;
    ASTNode* node1;
};

class ASTClassRefNode : public ASTNode {
public:
	ASTClassRefNode() : ASTNode() { }
    ASTClassRefNode(string s) : ASTNode(), id(s) { }
    virtual ~ASTClassRefNode() { }
    virtual void accept(Visitor& v) { v.visit(*this); }
    virtual string get_type() { return id; }
    virtual void set_type(string t) { }
    virtual void dump() { }
public:
	string id;
    ASTNode* node1;
};

class ASTClassNode : public ASTNode {
public:
	ASTClassNode() : ASTNode() { }
    ASTClassNode(string* i, string s, ASTNode* n1, ASTNode* n2) : ASTNode(), id(i), name(s), node1(n1), node2(n2) { }
    virtual ~ASTClassNode() { }
    virtual void accept(Visitor& v) { v.visit(*this); }
    virtual string get_type() { return name; }
    virtual void set_type(string t) { }
    virtual void dump() { }
public:
	string* id;
	string name;
    ASTNode* node1;
    ASTNode* node2;
};

class ASTClassesNode : public ASTNode {
public:
    ASTClassesNode() : ASTNode() { }
    ASTClassesNode(ASTNode* n1, ASTNode* n2) : ASTNode(), node1(n1), node2(n2) { }
    virtual ~ASTClassesNode() { }
    virtual void accept(Visitor& v) { v.visit(*this); }
    virtual string get_type() { return node1->get_type(); }
    virtual void set_type(string t) { }
    virtual void dump() { }
public:
    ASTNode* node1;
    ASTNode* node2;
};

class ASTMethodRefNode : public ASTNode {
public:
	ASTMethodRefNode() : ASTNode() { }
    ASTMethodRefNode(string s) : ASTNode(), id(s) { }
    virtual ~ASTMethodRefNode() { }
    virtual void accept(Visitor& v) { v.visit(*this); }
    virtual string get_type() { return id; }
    virtual void set_type(string t) { }
    virtual void dump() { }
public:
	string id;
    ASTNode* node1;
};

class ASTMethodNode : public ASTNode {
public:
    ASTMethodNode() : ASTNode() { }
    ASTMethodNode(string type,string id, ASTNode* n1, ASTNode* n2, ASTNode* n3) : ASTNode(), mtype(type), mname(id), node1(n1), node2(n2), node3(n3) { }
    virtual ~ASTMethodNode() { }
    virtual void accept(Visitor& v) { v.visit(*this); }
    virtual string get_type() { return mtype; }
    virtual void set_type(string t) { }
    virtual string get_name() { return mname; }
    virtual void dump() { }
public:
	int num_p;
	int num;
	string mtype;
	string mname;
    ASTNode* node1;
    ASTNode* node2;
    ASTNode* node3;
};

class ASTMethodsNode : public ASTNode {
public:
    ASTMethodsNode() : ASTNode() { }
    ASTMethodsNode(ASTNode* n1, ASTNode* n2) : ASTNode(), node1(n1), node2(n2) { }
    virtual ~ASTMethodsNode() { }
    virtual void accept(Visitor& v) { v.visit(*this); }
    virtual string get_type() { return "method"; }
    virtual void set_type(string t) { }
    virtual void dump() { }
public:
    ASTNode* node1;
    ASTNode* node2;
};

class ASTStmtsNode : public ASTNode {
public:
    ASTStmtsNode() : ASTNode() { }
    ASTStmtsNode(ASTNode* n1, ASTNode* n2) : ASTNode(), node1(n1), node2(n2) { }
    virtual ~ASTStmtsNode() { }
    virtual void accept(Visitor& v) { v.visit(*this); }
    virtual string get_type() { return node1->get_type(); }
    virtual void set_type(string t) { }
    virtual void dump() { }
public:
    ASTNode* node1;
    ASTNode* node2;
};

class ASTAssignStmtNode : public ASTNode {
public:
    ASTAssignStmtNode() : ASTNode() { }
    ASTAssignStmtNode(ASTNode* n1, ASTNode* n2) : ASTNode(), node1(n1), node2(n2), created(true) { }
    virtual ~ASTAssignStmtNode() { }
    virtual void accept(Visitor& v) { v.visit(*this); }
    virtual string get_type() { return node2->get_type(); }
    virtual void set_type(string t) { }
    virtual void dump() { }
public:
	bool created;
    ASTNode* node1;
    ASTNode* node2;
};

class ASTAssignIntVecStmtNode : public ASTNode {
public:
    ASTAssignIntVecStmtNode() : ASTNode() { }
    ASTAssignIntVecStmtNode(ASTNode* n1, ASTNode* n2, ASTNode* n3) : ASTNode(), node1(n1), node2(n2), node3(n3) { }
    virtual ~ASTAssignIntVecStmtNode() { }
    virtual void accept(Visitor& v) { v.visit(*this); }
    virtual string get_type() { return node1->get_type(); }/*node1 deve ser preciso mudar*/
    virtual void set_type(string t) { }
    virtual void dump() { }
public:
    ASTNode* node1;
    ASTNode* node2;
    ASTNode* node3;
};

class ASTIfStmtNode : public ASTNode {
public:
    ASTIfStmtNode() : ASTNode() { }
    ASTIfStmtNode(ASTNode* n0, ASTNode* n1, ASTNode* n2) : ASTNode(), node0(n0), node1(n1), node2(n2) { }
    virtual ~ASTIfStmtNode() { }
    virtual void accept(Visitor& v) { v.visit(*this); }
    virtual string get_type() { return node1->get_type(); }
    virtual void set_type(string t) { }
    virtual void dump() { }
public:
    ASTNode* node0;
    ASTNode* node1;
    ASTNode* node2;
};

class ASTWhileStmtNode : public ASTNode {
public:
    ASTWhileStmtNode() : ASTNode() { }
    ASTWhileStmtNode(ASTNode* n1, ASTNode* n2) : ASTNode(), node1(n1), node2(n2) { }
    virtual ~ASTWhileStmtNode() { }
    virtual void accept(Visitor& v) { v.visit(*this); }
    virtual string get_type() { return node1->get_type(); }
    virtual void set_type(string t) { }
    virtual void dump() { }
public:
    ASTNode* node1;
    ASTNode* node2;
};

class ASTPrintStmtNode : public ASTNode {
public:
    ASTPrintStmtNode() : ASTNode() {}
    ASTPrintStmtNode(ASTNode* n1) : ASTNode(), node1(n1) {}
    virtual ~ASTPrintStmtNode() { }
    virtual void accept(Visitor& v) { v.visit(*this); }
    virtual string get_type() { return node1->get_type();}
    virtual void set_type(string t) { }
    virtual void dump() { }
public:
    ASTNode* node1;
};

class ASTOpExprNode : public ASTNode {
public:
    ASTOpExprNode() : ASTNode() { }
    ASTOpExprNode(string o, ASTNode* n1, ASTNode* n2) : ASTNode(), op(o), node1(n1), node2(n2), etype("none") { }
    virtual ~ASTOpExprNode() { }
    virtual void accept(Visitor& v) { v.visit(*this); }
    virtual string get_type() { return etype; }
    virtual void set_type(string t) { etype = t; }
    virtual void dump() { }
public:
	string etype;
    string op;
    ASTNode* node1;
    ASTNode* node2;
};

class ASTIntConstNode : public ASTNode {
public:
    ASTIntConstNode() : ASTNode() { }
    ASTIntConstNode(int ival) : ASTNode() { value.ival = ival;}
    virtual ~ASTIntConstNode() { }
    virtual void accept(Visitor& v) { v.visit(*this); }
    virtual string get_type() { return "int"; }
    virtual void set_type(string t) { }
    virtual void dump() { }
public:
};

class ASTBoolConstNode : public ASTNode {
public:
    ASTBoolConstNode() : ASTNode() { }
    ASTBoolConstNode(bool bval) : ASTNode() { value.bval = bval; }
    virtual ~ASTBoolConstNode() { }
    virtual void accept(Visitor& v) { v.visit(*this); }
    virtual string get_type() { return "boolean"; }
    virtual void set_type(string t) { }
    virtual void dump() { }
public:
};

class ASTVarRefNode : public ASTNode {
public:
    ASTVarRefNode() : ASTNode() {}
    ASTVarRefNode(string name) : ASTNode(), vname(name) {}
    ASTVarRefNode(string* i, string type,string name) : ASTNode(), id(i), vtype(type), vname(name), type_num(0) {}
    virtual ~ASTVarRefNode() { }
    virtual void accept(Visitor& v) { v.visit(*this); }
    virtual string get_type() { return vtype; }
    virtual void set_type(string t) { }
    virtual void dump() { }
public:
	int type_num;
	string* id;
    string vtype;
    string vname;
};

class ASTVarDeclsNode : public ASTNode {
public:
    ASTVarDeclsNode() : ASTNode() { }
    ASTVarDeclsNode(ASTNode* n1, ASTNode* n2) : ASTNode(), node1(n1), node2(n2) { }
    virtual ~ASTVarDeclsNode() { }
    virtual void accept(Visitor& v) { v.visit(*this); }
    virtual string get_type() { return "vars"; }
    virtual void set_type(string t) { }
    virtual void dump() { }
public:
    ASTNode* node1;
    ASTNode* node2;
};

class ASTNewIdNode : public ASTNode {
public:
    ASTNewIdNode() : ASTNode() {}
    ASTNewIdNode(string* s, ClassTabData* cd) : ASTNode(), id(s), cdata(cd)  {}
    virtual ~ASTNewIdNode() { }
    virtual void accept(Visitor& v) { v.visit(*this); }
    virtual string get_type() { 
		string &s0 = *id;
		char* value; 
		value = (char*)malloc(s0.size() + 1);
		memcpy(value, s0.c_str(), s0.size() + 1);	
		return value;
	}
    virtual void set_type(string t) { }
    virtual void dump() { }
public:
	int num;
    string* id;
    ClassTabData* cdata;
};

class ASTIntVecNode : public ASTNode {
public:
    ASTIntVecNode() : ASTNode() { }
    ASTIntVecNode(ASTNode* n1) : ASTNode(), node1(n1) { }
    virtual ~ASTIntVecNode() { }
    virtual void accept(Visitor& v) { v.visit(*this); }
    virtual string get_type() { return "int[]"; }
    virtual void set_type(string t) { }
    virtual void dump() { }
public:
    ASTNode* node1;
};

class ASTVecValueNode : public ASTNode {
public:
    ASTVecValueNode() : ASTNode() { }
    ASTVecValueNode(ASTNode* n1, ASTNode* n2) : ASTNode(), node1(n1), node2(n2) { }
    virtual ~ASTVecValueNode() { }
    virtual void accept(Visitor& v) { v.visit(*this); }
    virtual string get_type() { return "int"; }
    virtual void set_type(string t) { }
    virtual void dump() { }
public:
    ASTNode* node1;
    ASTNode* node2;
};

class ASTLengthNode : public ASTNode {
public:
    ASTLengthNode() : ASTNode() { }
    ASTLengthNode(ASTNode* n1) : ASTNode(), node1(n1) { }
    virtual ~ASTLengthNode() { }
    virtual void accept(Visitor& v) { v.visit(*this); }
    virtual string get_type() { return "int"; }
    virtual void set_type(string t) { }
    virtual void dump() { }
public:
    ASTNode* node1;
};

class ASTNegationNode : public ASTNode {
public:
    ASTNegationNode() : ASTNode() { }
    ASTNegationNode(ASTNode* n1) : ASTNode(), node1(n1) { }
    virtual ~ASTNegationNode() { }
    virtual void accept(Visitor& v) { v.visit(*this); }
    virtual string get_type() { return "boolean"; }
    virtual void set_type(string t) { }
    virtual void dump() { }
public:
    ASTNode* node1;
};

class ASTCallMethodNode : public ASTNode {
public:
    ASTCallMethodNode() : ASTNode() {}
    ASTCallMethodNode(string* s, MethodTabData* md) : ASTNode(), id(s), mdata(md) {}
    virtual ~ASTCallMethodNode() { }
    virtual void accept(Visitor& v) { v.visit(*this); }
    virtual string get_type() { return type; }
    virtual void set_type(string t) { type = t; }
    virtual void dump() { }
public:
	int num_p;
	string mclass;
	int num;
	string type;
    string* id;
    MethodTabData* mdata;
    ASTNode* node1;
};

class ASTThisNode : public ASTNode {
public:
    ASTThisNode() : ASTNode(), type("this") {}
    virtual ~ASTThisNode() { }
    virtual void accept(Visitor& v) { v.visit(*this); }
    virtual string get_type() { return type; }
    virtual void set_type(string t) { }
    virtual void dump() { }
public:
	int instance;
	string type;
};

#endif /* _AST_H */

/*----------------------------------------------------------------------------*/
