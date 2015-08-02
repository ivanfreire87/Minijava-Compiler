#include "ast_utils.hh"

extern int yylineno;
static bool varsok = true;
static unsigned inwhile = 0;
static int paramn = 0;
static string* current_class;
static string* current_method;
static string* current_assigning_method;
static string* current_assigning_method_class;
static ASTNode* current_assigning_method_class_node;
static bool reading_params = false;
static bool assigning_method_params = false;
static vector<string> typesVec;
static vector<ASTNewIdNode*> newIds;
static vector<ASTAssignStmtNode*> paramAssigns;
static ASTNode* troot = NULL;
static ASTTypesVecNode* typesVec_node = NULL;
static SymbolTable* stab = NULL;
static int num = 0;
static int current_num = 0;
static int num_params = 0;
static int num_params_a = 0;

void
ASTUtils::init_ast_utils(SymbolTable& st){
    stab = &st;
}

bool
ASTUtils::get_varsok(){
    return varsok;
}

ASTNode*
ASTUtils::get_root_node(){	
    return troot;
}

ASTTypesVecNode*
ASTUtils::get_typesVec_node(){	
    return typesVec_node;
}

void
ASTUtils::set_root_node(ASTNode* rn){
    troot = rn;
}

void
ASTUtils::set_current_class(string* id){
    current_class = id;
}

void
ASTUtils::unset_current_class(){
    current_class = NULL;
}

void
ASTUtils::set_current_method(string* id){
    current_method = id;
}

void
ASTUtils::unset_current_method(){
    current_method = NULL;
}

void
ASTUtils::set_reading_params(){
	num_params = 0;
    reading_params = true;
}

void
ASTUtils::unset_reading_params(){
	paramn=0;
    reading_params = false;
}

void
ASTUtils::set_assigning_method_params(ASTNode* n1, string* id){
	num_params_a = 0;
	current_assigning_method = id;
	string* type = new string(n1->get_type());
	/*if n1 is ASTCallMethodNode*/
	if(!n1->get_type().compare("")){		
		string* t = new string(((ASTCallMethodNode*) n1)->mclass);
		current_assigning_method_class = t;
	}	
	else{
		string* type = new string(n1->get_type());
		current_assigning_method_class = type;
	}
	
	ClassTabData* cdata;

	cdata = stab->lookup_class(*current_assigning_method_class);
	if(cdata == NULL)
		cdata = stab->insert_class(*current_assigning_method_class);	
	MethodTabData* mdata = cdata->lookup_method(*current_assigning_method);
	if(mdata == NULL)
		mdata = cdata->insert_method(*current_assigning_method);	
	
	vector<ASTNode*> vec;
	mdata->methodParamAssigns.push_back(vec);
	mdata->methodParamsMaps.push_back(new TabMap());	
	
	current_assigning_method_class_node = n1;
	assigning_method_params = true;
}

void
ASTUtils::unset_assigning_method_params(){
	current_assigning_method = NULL;
	current_assigning_method_class = NULL;
	current_assigning_method_class_node = NULL;
	assigning_method_params = false;
}

void
ASTUtils::push_scope(){
    assert(stab != NULL);
    stab->push_scope();
}

void
ASTUtils::pop_scope(){
    assert(stab != NULL);
    stab->pop_scope();
}

void
ASTUtils::enter_while(){
    inwhile++;
}

void
ASTUtils::exit_while(){
    inwhile--;
}

ASTNode*
ASTUtils::mk_program_node(ASTNode* n1, ASTNode* n2){
    assert(n1 != NULL);
    
    
    return new ASTProgramNode(n1, n2);
}

ASTNode*
ASTUtils::mk_main_node(string* id, ASTNode* n1){
	string s(get_string_value(id));
	typesVec.push_back(s);
    return new ASTMainNode(s,n1);
}

void
ASTUtils::mk_cref(string* id){
	assert(stab != NULL);
    ClassTabData* cdata = stab->lookup_class(*id);
    if(cdata != NULL){
		if(cdata->get_node() != NULL){
			cout << "Already defined class ";
			cout << *id << ". Line " << yylineno << endl;
			varsok = false;
		}
		
    }else
		stab->insert_class(*id);	 
}

void
ASTUtils::mk_cref(string* id, string* ext){
	assert(stab != NULL);
    ClassTabData* cdata = stab->lookup_class(*id);
    if(cdata != NULL){
		if(cdata->get_node() != NULL){
			cout << "Already defined class ";
			cout << *id << ". Line " << yylineno << endl;
			varsok = false;
		}
		cdata->extends = ext;
    }else{
		cdata = stab->insert_class(*id);
		cdata->extends = ext;
		
		
	}	 
}

ASTNode*
ASTUtils::mk_class_node(string* id, ASTNode* n1, ASTNode* n2){
	assert(stab != NULL);
    ASTClassNode* cn = NULL;
    ClassTabData* cdata = stab->lookup_class(*id);
    	
	string s(get_string_value(id));
	typesVec.push_back(s);
	cn = new ASTClassNode(id,s,n1,n2);
	cn->set_lineno(yylineno); 
	cdata->set_node(cn);
    
    return cn;	
}

ASTNode*
ASTUtils::mk_class_node(string* id, ASTNode* n1, ASTNode* n2, string* ext){
	assert(stab != NULL);
	
    ASTClassNode* cn = NULL;
    ClassTabData* cdata = stab->lookup_class(*id);
    	
	string s(get_string_value(id));
	typesVec.push_back(s);
	cn = new ASTClassNode(id,s,n1,n2);
	cn->set_lineno(yylineno);
	cdata->extends = ext;
	cdata->set_node(cn);

    return cn;	
}

ASTNode*
ASTUtils::mk_classes_node(ASTNode* n1, ASTNode* n2){
	if(n2 == NULL) return n1;	
    return new ASTClassesNode(n1, n2);
}

void
ASTUtils::mk_mref(string* id, char* type){
	assert(stab != NULL);
    ClassTabData* cdata = stab->lookup_class(*current_class);
    MethodTabData* mdata = cdata->lookup_method(*id); 
    if(mdata != NULL){
		if(cdata->get_node() != NULL){
			cout << "Already defined method ";
			cout << *id << ". Line " << yylineno << endl;
			varsok = false;
		}
    }else{
		string mt(type);
		mdata = cdata->insert_method(*id);
		mdata->return_type = mt;			
		
	} 
}

ASTNode*
ASTUtils::mk_method_node(char *type, string *id, ASTNode* n1, ASTNode* n2, ASTNode* n3){  
	assert(stab != NULL);
    ASTMethodNode* mn = NULL;
   
    ClassTabData* cdata = stab->lookup_class(*current_class);
    MethodTabData* mdata = cdata->lookup_method(*id);
        
   
	string n(get_string_value(id));
	string mt(type);
	mdata->return_type = mt;		
	mn = new ASTMethodNode(mt,n,n1,n2,n3);

	mn->num_p = num_params;
	mdata->num_p = num_params;
	
	mn->set_lineno(yylineno);
	mdata->set_node(mn);
    
    return mn;	
}

ASTNode*
ASTUtils::mk_methods_node(ASTNode* n1, ASTNode* n2){ 
	if(n2 == NULL) return n1;
	     
    return new ASTMethodsNode(n1, n2);
}

ASTNode*
ASTUtils::mk_stmts_node(ASTNode* n1, ASTNode* n2){
    if(n2 == NULL) return n1;

    return new ASTStmtsNode(n1, n2);
}

ASTNode*
ASTUtils::mk_astmt_node(string* name, ASTNode* n1){
    assert(n1 != NULL);
    assert(stab != NULL);
    TabData* vdata = NULL;
    MethodTabData* mdata = NULL;
    ClassTabData* cdata = stab->lookup_class(*current_class);
    
	vdata = cdata->lookup_class_var(*name);
	
	if(vdata == NULL){
		mdata = cdata->lookup_method(*current_method);
		vdata = mdata->lookup_method_var(*name);
		if(vdata == NULL){
			vdata = mdata->lookup_method_param(*name);
			if(vdata == NULL){
				if(cdata->extends != NULL){
					ClassTabData* cdata_extended = stab->lookup_class(*cdata->extends);
					vdata = cdata_extended->lookup_class_var(*name);					
				}								
			}
		}
	}
    if(vdata == NULL){
		cout << "Assigning undefined variable ";
		cout << *name << ". Line " << yylineno << endl;
		if(current_method == NULL)			
			vdata = cdata->insert_class_var(*name);
		else{
			vdata = mdata->insert_method_var(*name);
			if(vdata == NULL){
				vdata = mdata->insert_method_param(*name);
			}
		}
		ASTNode* vn = new ASTVarRefNode(NULL,"none","none");
		vn->set_lineno(yylineno);
		vdata->set_node(vn);
		varsok = false;
    }
    ASTNode* vn = vdata->get_node();
    vn->set_lineno(yylineno);    
    ASTAssignStmtNode* rval = new ASTAssignStmtNode(vn,n1);
    rval->set_lineno(yylineno);
      
    return rval;
}

ASTNode*
ASTUtils::mk_aivecstmt_node(string* name, ASTNode* n1, ASTNode* n2){
    assert(n1 != NULL);
    assert(n2 != NULL);
    assert(stab != NULL);
    TabData* vdata = NULL;
    MethodTabData* mdata = NULL;
    ClassTabData* cdata = stab->lookup_class(*current_class);

	vdata = cdata->lookup_class_var(*name);
	if(vdata == NULL){
		mdata = cdata->lookup_method(*current_method);
		vdata = mdata->lookup_method_var(*name);
		if(vdata == NULL){
			vdata = mdata->lookup_method_param(*name);
			if(vdata == NULL){
				if(cdata->extends != NULL){
					ClassTabData* cdata_extended = stab->lookup_class(*cdata->extends);
					vdata = cdata_extended->lookup_class_var(*name);					
				}								
			}
		}
	}
    if(vdata == NULL){
		cout << "Assigning undefined variable ";
		cout << *name << ". Line " << yylineno << endl;
		if(current_method == NULL)			
			vdata = cdata->insert_class_var(*name);
		else 
			vdata = mdata->insert_method_var(*name);
		ASTNode* vn = new ASTVarRefNode(NULL,"none","none");
		vn->set_lineno(yylineno);
		vdata->set_node(vn);
		varsok = false;
    }
    
    ASTNode* vn = vdata->get_node();
    vn->set_lineno(yylineno);
    ASTAssignIntVecStmtNode* rval = new ASTAssignIntVecStmtNode((ASTIntVecNode*)vn,n1,n2);
    rval->set_lineno(yylineno);
 
    delete name;
    return rval;
}

ASTNode*
ASTUtils::mk_istmt_node(ASTNode* n0, ASTNode* n1, ASTNode* n2){
    assert(n0 != NULL);
    ASTIfStmtNode* rval = new ASTIfStmtNode(n0, n1, n2);
    rval->set_lineno(yylineno);
    return rval;
}

ASTNode*
ASTUtils::mk_wstmt_node(ASTNode* n1, ASTNode* n2){
    assert(n1 != NULL && n2 != NULL);
    ASTWhileStmtNode *rval = new ASTWhileStmtNode(n1, n2);
    rval->set_lineno(yylineno);
    return rval;
}

ASTNode*
ASTUtils::mk_pstmt_node(ASTNode* n1){
    assert(n1 != NULL);
    ASTPrintStmtNode* rval = new ASTPrintStmtNode(n1);
    rval->set_lineno(yylineno);
    return rval;
}

ASTNode*
ASTUtils::mk_opexpr_node(string* s, ASTNode* n1, ASTNode* n2){
    assert((n1 != NULL && n2 != NULL) || not varsok);
    string op(get_string_value(s));  

    ASTOpExprNode* rval = new ASTOpExprNode(op, n1, n2);
	if(!op.compare("&&")) rval->set_type("boolean");
	if(!op.compare("<")) rval->set_type("boolean");
	if(!op.compare("+")) rval->set_type("int");
	if(!op.compare("-")) rval->set_type("int");
	if(!op.compare("*")) rval->set_type("int");
    rval->set_lineno(yylineno);
    return rval;
}

ASTNode*
ASTUtils::mk_iconst_node(int ival){
    ASTIntConstNode* rval = new ASTIntConstNode(ival);
    rval->set_lineno(yylineno);
    return rval;
}

ASTNode*
ASTUtils::mk_bconst_node(bool bval){
    ASTBoolConstNode* rval = new ASTBoolConstNode(bval);
    rval->set_lineno(yylineno);
    return rval;
}

ASTNode*
ASTUtils::mk_varDecls_node(ASTNode* n1, ASTNode* n2){		
    if(n2 == NULL) return n1;

    return new ASTVarDeclsNode(n1, n2);
}

ASTNode*
ASTUtils::mk_vref_node(string* name, char* type){		
    assert(stab != NULL);
    ASTVarRefNode* vn = NULL;
    TabData* vdata = NULL;
    TabData* mvdata = NULL;
    TabData* mpdata = NULL;
    MethodTabData* mdata = NULL;
    ClassTabData* cdata = stab->lookup_class(*current_class);
    
    if(current_method == NULL)
		vdata = cdata->lookup_class_var(*name);
	else{
		vdata = cdata->lookup_class_var(*name);
		mdata = cdata->lookup_method(*current_method);
		mvdata = mdata->lookup_method_var(*name);
		mpdata = mdata->lookup_method_param(*name);
	}
    if(vdata != NULL || mvdata != NULL || mpdata != NULL){
		cout << "Already defined variable ";
		cout << *name << ". Line " << yylineno << endl;
		varsok = false;
		string vt(type);
		string vname(get_string_value(name));
		vn = new ASTVarRefNode(name,vt,vname);
    }else{
		if(current_method == NULL){			
			vdata = cdata->insert_class_var(*name);
		}
		else if(reading_params){
			
			int pos = mdata->methodParamsMaps.size()-1;							
			vdata = mdata->insert_method_param(*name);
			paramn++;
		}
		else
			vdata = mdata->insert_method_var(*name);
			
		string vt(type);
		string vname(get_string_value(name));			
		
		if(reading_params){
			num_params++;
			int pos = mdata->methodParamsMaps.size()-1;
			for(int i=pos; i>=0; i--){
				vdata = (*mdata->methodParamsMaps[i])[vname];
				vn = new ASTVarRefNode(name,vt,vname);
				vn->set_lineno(yylineno);
				vdata->set_node(vn);
								
			}									
		
		}else{				
			vn = new ASTVarRefNode(name,vt,vname);
			vn->set_lineno(yylineno);
			vdata->set_node(vn);
			
		}						
    }	
    return vn;
}

ASTNode*
ASTUtils::get_vref_node(string* name){
    assert(stab != NULL);
    ASTVarRefNode* vn = NULL;
    TabData* vdata = NULL;
    TabData* mvdata = NULL;
    TabData* mpdata = NULL;
    MethodTabData* mdata = NULL;
    ClassTabData* cdata = stab->lookup_class(*current_class);
    if(current_method == NULL){
		vdata = cdata->lookup_class_var(*name);
	}else{
		
		vdata = cdata->lookup_class_var(*name);
		mdata = cdata->lookup_method(*current_method);
		mvdata = mdata->lookup_method_var(*name);
		mpdata = mdata->lookup_method_param(*name);
	}
	
	if(vdata == NULL && mvdata == NULL  && mpdata == NULL){
		if(cdata->extends != NULL){
			ClassTabData* cdata_extended = stab->lookup_class(*cdata->extends);
			vdata = cdata_extended->lookup_class_var(*name);					
		}											
	}
	
    if(vdata == NULL && mvdata == NULL  && mpdata == NULL){
		cout << "Undefined variable ";
		cout << *name << " in expression. Line " << yylineno << endl;
		if(current_method == NULL)			
			vdata = cdata->insert_class_var(*name);
		else 
			vdata = mdata->insert_method_var(*name);
		vn = new ASTVarRefNode(NULL,"none","none");
		vn->set_lineno(yylineno);
		varsok = false;
    }
    else if(mvdata != NULL)
		vn = (ASTVarRefNode*) mvdata->get_node(); 
    else if(mpdata != NULL)
		vn = (ASTVarRefNode*) mpdata->get_node();
    else{
		for(int i=0; i<newIds.size(); i++){
			string s1(get_string_value(newIds[i]->id));
			string s2(get_string_value(current_class));
			if(!s1.compare(s2))
				vn = (ASTVarRefNode*) vdata->get_node();
		}
		vn = (ASTVarRefNode*) vdata->get_node();
		vn->type_num = 1;
	}
    delete name;
    vn->set_lineno(yylineno);
    return vn;
}

ASTNode*
ASTUtils::mk_newId(string* id){

	assert(stab!=NULL);
	ASTNewIdNode* rval = NULL;
	ClassTabData* cdata1 = stab->lookup_class(*id);

	if(cdata1 == NULL)
		cdata1 = stab->insert_class(*id);
		
	ClassTabData* cdata = new ClassTabData(*id);
	cdata->classVarsMap = new TabMap();
	MethodTabMap* mmap = new MethodTabMap();
	cdata->methodMaps.push_back(mmap);
    rval = new ASTNewIdNode(id,cdata);
    newIds.push_back(rval);
    rval->set_lineno(yylineno);
    rval->num = num;
    num++;
    return rval;
}

void
ASTUtils::assign_new_ids(){
		
	for(int i=0; i<newIds.size() ; i++){
		string s(get_string_value(newIds[i]->id));
		ClassTabData* new_cdata = newIds[i]->cdata;
		ClassTabData* cdata_copy = stab->lookup_class(*newIds[i]->id);
		new_cdata->node = cdata_copy->node;		

		//vars		
		TabMap::iterator mpos = (*cdata_copy->classVarsMap).begin();
		for(mpos ; mpos != (*cdata_copy->classVarsMap).end(); mpos++){	
			string f(mpos->first);
			string s(((ASTVarRefNode*)mpos->second->get_node())->vtype);
			TabData* vdata = new TabData(f);
			(*new_cdata->classVarsMap)[f] = vdata;
			vdata->node = new ASTVarRefNode(f);			
		}
		//methods
		
		MethodTabData* mdata = NULL;
		MethodTabData* mdata2 = NULL;
		
		for(int j=cdata_copy->methodMaps.size()-1; j>=0; j--) {
			MethodTabMap::iterator cmpos = (*cdata_copy->methodMaps[j]).begin();
			
			while(cmpos != (*cdata_copy->methodMaps[j]).end()){			
				string t(cmpos->first);
				mdata = cdata_copy->lookup_method(t);
				int pos = mdata->methodParamsMaps.size()-1;
				int pos2 = 0;			
				while(pos>=0){
					if(mdata->methodParamsMaps[pos]->size() > 0){
						TabMap::iterator mpos = (*mdata->methodParamsMaps[pos]).begin();
							
						for(int l = mdata->methodParamAssigns[pos2].size()-1; l >= 0; l--){
							ASTNode* left = mpos->second->get_node();
							ASTNode* right = mdata->methodParamAssigns[pos2][l];
							ASTAssignStmtNode* as =	new ASTAssignStmtNode(left,right);
							((ASTNode*) as)->set_lineno(right->get_lineno());								
							paramAssigns.push_back(as);
							mpos++;				
						}
					}
					pos2++;
					pos--;	
				}
				cmpos++;
			}
			
			
		}
		new_cdata->methodMaps = cdata_copy->methodMaps;			
	}
	
	
	
}

ASTNode*
ASTUtils::mk_int_vec_node(ASTNode* n1){
	ASTIntVecNode* rval = new ASTIntVecNode(n1); 
	
	rval->set_lineno(yylineno);
	return rval;
}

ASTNode*
ASTUtils::mk_vec_value_node(ASTNode* n1,ASTNode* n2){
	ASTVecValueNode* rval = new ASTVecValueNode(n1,n2);
	
    rval->set_lineno(yylineno);
    return rval;	
}

ASTNode*
ASTUtils::mk_length_node(ASTNode* n1){
	ASTLengthNode* rval = new ASTLengthNode(n1);
    rval->set_lineno(yylineno);
    return rval;	
}

ASTNode*
ASTUtils::mk_negation_node(ASTNode* n1){
	ASTNegationNode* rval = new ASTNegationNode(n1);
    rval->set_lineno(yylineno);
    return rval;	
}

ASTNode*
ASTUtils::mk_call_method_node(ASTNode* n1, string* mid){
	ASTCallMethodNode* rval = NULL;
	string type(n1->get_type());
		
	ClassTabData* cdata;
	/*if n1 is ASTCallMethodNode*/
	if(!n1->get_type().compare("this")){
		
		string t(((ASTCallMethodNode*) n1)->mclass);	
		cdata = stab->lookup_class(t);
	}
	else if(n1->get_type().empty()){
		string t(((ASTCallMethodNode*) n1)->mclass);	
		cdata = stab->lookup_class(t);
	}
	else
		cdata = stab->lookup_class(type);
	
	MethodTabData* mdata = NULL;	
	mdata = cdata->lookup_method(*mid);

	rval = new ASTCallMethodNode(mid,mdata);
	rval->num_p = num_params_a;
	if(!n1->get_type().compare("this")){
		string t(((ASTCallMethodNode*) n1)->mclass);
		rval->type = "this";
	}
	else{
		rval->mclass = type;
		rval->type = mdata->return_type;
	}
	current_num = num;
	rval->num = ((ASTNewIdNode*)n1)->num;
    rval->set_lineno(yylineno);
    rval->node1 = n1;
    assert(rval != NULL);
    return rval;	
}

void
ASTUtils::mk_param_value(ASTNode* n1){

			
		if(current_assigning_method_class == NULL){
			string* t = new string(((ASTCallMethodNode*) n1)->mclass);
			current_assigning_method_class = t;
			ClassTabData* cdata = stab->lookup_class(*current_assigning_method_class);
			MethodTabData* mdata = ((ASTCallMethodNode*) n1)->mdata;
			string* m = new string(mdata->get_name());
			current_assigning_method = m;
		}
		
		ClassTabData* cdata = stab->lookup_class(*current_assigning_method_class);
		MethodTabData* mdata = NULL;
		if(cdata == NULL){
			stab->insert_class(*current_assigning_method_class);
		}
		
		mdata = cdata->lookup_method(*current_assigning_method);
		if(mdata == NULL)
			mdata = cdata->insert_method(*current_assigning_method);
		n1->set_lineno(yylineno);
		mdata->insert_method_param_assign(n1);
		
		int i = mdata->methodParamAssigns.size()-1;
		int j = mdata->methodParamAssigns[i].size()-1;
		num_params_a++;
		//cout << "param val : " << (mdata->methodParamAssigns[i])[j]->value.ival << " at pos: " << i << " " << j << endl;
	

}
ASTNode*
ASTUtils::get_current_class_node(){
	assert(stab!=NULL);
	ASTThisNode* rval = new ASTThisNode();

    return rval;
}

char*
ASTUtils::get_string_value(string* s){
	string &s0 = *s;
	char* value; 
	value = (char*)malloc(s0.size() + 1);
    memcpy(value, s0.c_str(), s0.size() + 1);
	
	return value;
}

void
ASTUtils::mk_typesVec_node(){
	typesVec_node = new ASTTypesVecNode(typesVec,newIds,paramAssigns);
}



/*----------------------------------------------------------------------------*/
