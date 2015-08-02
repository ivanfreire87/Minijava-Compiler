#ifndef _SYMTAB_H
#define _SYMTAB_H 1

#include "includes.hh"
#include "xdefs.hh"

class TabData;
class ClassTabData;
class MethodTabData;
class ASTNode;

typedef unordered_map<string,TabData*> TabMap;
typedef unordered_map<string,ClassTabData*> ClassTabMap;
typedef unordered_map<string,MethodTabData*> MethodTabMap;


class SymbolTable {
public:
    SymbolTable() : classMaps() { }

    ClassTabData* lookup_class(string& var);      

    ClassTabData* insert_class(string& var);   

    void push_scope();

    void pop_scope();

public:
    vector<ClassTabMap*> classMaps;
};

class TabData {
public:
    TabData(string& nm) : name(nm), node() { }

    inline void set_node(ASTNode* nn) { node = nn; }

    inline ASTNode* get_node(){ return node; }

    inline string& get_name() { return name; }

public:

    string& name;

    ASTNode* node;   

};

class ClassTabData {
public:
    ClassTabData(string& nm) : name(nm), node(NULL)  { }

    inline void set_node(ASTNode* nn) { node = nn; }

    inline ASTNode* get_node(){ return node; }
    
    TabData* insert_class_var(string& var);
    MethodTabData* insert_method(string& var);
    MethodTabData* lookup_method(string& var); 
    TabData* lookup_class_var(string& var);

    inline string& get_name() { return name; }

public:
    string* extends;
    string& name;
    ASTNode* node;
    TabMap* classVarsMap;
    vector<MethodTabMap*> methodMaps;

};

class MethodTabData {
public:
    MethodTabData(string& nm) : name(nm), node(NULL), methodVarsMap() { }

    inline void set_node(ASTNode* nn) { node = nn; }

    inline ASTNode* get_node(){ return node; }
    
    TabData* insert_method_var(string& var);
    TabData* lookup_method_var(string& var);
    TabData* insert_method_param(string& var);
    TabData* lookup_method_param(string& var);
    void insert_method_param_assign(ASTNode* n1);
    void init_vars_map(){ methodVarsMap = new TabMap(); }

    inline string& get_name() { return name; }

public:
    int num_p;
    string& name;
    string return_type;
    ASTNode* node;
    TabMap* methodVarsMap;
    vector<TabMap*> methodParamsMaps;
    vector<vector<ASTNode*>> methodParamAssigns;

};


#endif 
