#ifndef _TYPECHECKER_H
#define _TYPECHECKER_H 1

#include "visitor.hh"

class TypeChecker : public Visitor {
public:
    TypeChecker() : Visitor(), typeok(true) {}

	void visit(ASTTypesVecNode& n){
		typesVector = n.typesVec;
		newIds = n.newIds;
		paramAssigns = n.paramAssigns;
    }

	void visit(ASTProgramNode& n){
		if(n.node1 != NULL)	n.node1->accept(*this);
		if(n.node2 != NULL)	n.node2->accept(*this);
	}

	void visit(ASTMainNode& n){
		if(n.node1 != NULL)	n.node1->accept(*this);

    }
    
    void visit(ASTClassRefNode& n){}
    
    void visit(ASTClassNode& n){
		if(n.node1 != NULL) n.node1->accept(*this);
		if(n.node2 != NULL) n.node2->accept(*this);
    }
    
    void visit(ASTClassesNode& n){
		if(n.node2 != NULL) n.node2->accept(*this);
		if(n.node1 != NULL) n.node1->accept(*this);
    }

    void visit(ASTMethodRefNode& n){
		if(n.node1 != NULL) n.node1->accept(*this);
    }
    
    void visit(ASTMethodNode& n){
		if(n.node1 != NULL) n.node1->accept(*this);
		if(n.node2 != NULL) n.node2->accept(*this);
		if(n.node3 != NULL) n.node3->accept(*this);
		/* type_num - 0=method var , 1 = class var */
		if(((ASTVarRefNode*)n.node3)->type_num == 1){
			ClassTabData* cdata = ((ASTNewIdNode*)newIds[num])->cdata;
			TabData* tdata = cdata->lookup_class_var(*((ASTVarRefNode*)n.node3)->id);
			if(tdata!=NULL)
				n.node3 = tdata->get_node();			
		}
		bool tok = !(n.get_type().compare(n.node3->get_type()));
		if(not tok)
			cout << "Type checking failed in line " << n.get_lineno() << 
			". Return value of type '" << n.node3->get_type() <<
			"'. Must be of type '" << n.get_type() << "'." << endl;
		typeok = typeok && tok;
    }
    
    void visit(ASTMethodsNode& n){
		if(n.node1 != NULL) n.node1->accept(*this);
		if(n.node2 != NULL) n.node2->accept(*this);
    }
    
    void visit(ASTStmtsNode& n){
		if(n.node1 != NULL) n.node1->accept(*this);
		if(n.node2 != NULL) n.node2->accept(*this);
    }

    void visit(ASTAssignStmtNode& n){
		((ASTNewIdNode*)n.node1)->num = ((ASTNewIdNode*)n.node2)->num;
		n.node1->accept(*this);
		n.node2->accept(*this);
		/* type_num - 0=method var , 1 = class var */
		if(((ASTVarRefNode*)n.node1)->type_num == 1){
			ClassTabData* cdata = ((ASTNewIdNode*)newIds[num])->cdata;
			TabData* tdata = cdata->lookup_class_var(*((ASTVarRefNode*)n.node1)->id);
			if(tdata!=NULL)
				n.node1 = tdata->get_node();			
		}
		bool tok = true;
		
		if(not assigning_f){
		tok = !(n.node1->get_type().compare(n.node2->get_type()));
		if(not tok)
			cout << "Type checking failed in line " << n.get_lineno() << ". Assigning type '" << n.node2->get_type()
			<< "' to type '" << n.node1->get_type() << "'." << endl;
		}
		typeok = typeok && tok;
		bool new_id = false;
		for(int i=0; i<newIds.size(); i++){
			if(newIds[i] == n.node2){
				new_id = true;
				n.node1->instance_num = i;
			}
			
		}
		if(not new_id)
			n.node1->instance_num = n.node2->instance_num;
    }
    
    void visit(ASTAssignIntVecStmtNode& n){
		ASTVarRefNode* v = (ASTVarRefNode*) n.node1;
		n.node2->accept(*this);
		n.node3->accept(*this);
		bool tok = !(n.node2->get_type().compare("int"))  && !(n.node3->get_type().compare("int"));
		if(not tok){
			if(!(n.node3->get_type().compare("int")))
				cout << "Type checking failed in line " << n.get_lineno() 
				<< ". Assigning '" << n.node2->get_type() << "' to '" << n.node3->get_type() << "'." << endl;
			else
				cout << "Type checking failed in line " << n.get_lineno() << ". Value in '" << v->vname << "[value]' is of type '" 
				<< n.node3->get_type()  << "'. Must be an integer." << endl;				
		}
		typeok = typeok && tok;
    }

    void visit(ASTIfStmtNode& n){
		n.node0->accept(*this);
		if(n.node1 != NULL) n.node1->accept(*this);
		if(n.node2 != NULL) n.node2->accept(*this);
		
		bool tok = !(n.node0->get_type().compare("boolean"));
		if(not tok)
			cout << "Type checking failed in line " << n.get_lineno() << 
			". 'If' condition is not of type boolean." << endl;
		typeok = typeok && tok;
    }

    void visit(ASTWhileStmtNode& n){
		n.node1->accept(*this);
		if(n.node2 != NULL) n.node2->accept(*this);
		
		bool tok = !(n.node1->get_type().compare("boolean"));
		if(not tok)
			cout << "Type checking failed in line " << n.get_lineno() << 
			". 'While' condition is not of type boolean." << endl;
		typeok = typeok && tok;
    }

    void visit(ASTPrintStmtNode& n){

    }

    void visit(ASTOpExprNode& n){
		n.node1->accept(*this);
		n.node2->accept(*this);
		
		bool tok = true;		
		if(!n.op.compare("&&")) tok = !n.node1->get_type().compare("boolean") && !n.node2->get_type().compare("boolean");
	    if(!n.op.compare("<")) tok = !n.node1->get_type().compare("int") && !n.node2->get_type().compare("int");
	    if(!n.op.compare("+")) tok = !n.node1->get_type().compare("int") && !n.node2->get_type().compare("int");
	    if(!n.op.compare("-")) tok = !n.node1->get_type().compare("int") && !n.node2->get_type().compare("int");
	    if(!n.op.compare("*")) tok = !n.node1->get_type().compare("int") && !n.node2->get_type().compare("int");
			
		if(not tok){
			if(!n.op.compare("&&")) 
				cout << "Type checking failed in line " << n.get_lineno() << 
				". Evaluating relational result of two non boolean expressions." << endl;
			if(!n.op.compare("<")) 
				cout << "Type checking failed in line " << n.get_lineno() << 
				". Evaluating 'greater than' relational result of two non 'int' expressions." << endl;
			if(!n.op.compare("+")) 
				cout << "Type checking failed in line " << n.get_lineno() << 
				". Adding two incompatible expressions. Both must be of type 'int'." << endl;
			if(!n.op.compare("-")) 
				cout << "Type checking failed in line " << n.get_lineno() << 
				". Subtracting two incompatible expressions. Both must be of type 'int'." << endl;
			if(!n.op.compare("*")) 
				cout << "Type checking failed in line " << n.get_lineno() << 
				". Multiplying two incompatible expressions. Both must be of type 'int'." << endl;
			
		}
		typeok = typeok && tok;
    }
    
	void visit(ASTIntConstNode& n){}

    void visit(ASTBoolConstNode& n){}
    
    void visit(ASTVarDeclsNode& n){
		if(n.node1 != NULL) n.node1->accept(*this);
		if(n.node2 != NULL) n.node2->accept(*this);
    }

    void visit(ASTVarRefNode& n){			
		bool tok = true;
		if(n.get_type().compare("int") && n.get_type().compare("boolean") && n.get_type().compare("int[]"))	
			tok = find(typesVector.begin(), typesVector.end(), n.get_type())!=typesVector.end();
		if(not tok)
			cout << "Type checking failed in line " << n.get_lineno() << ". '" << n.get_type() << "' not defined." << endl;
		typeok = typeok && tok;		
	}
	
	void visit(ASTNewIdNode& n){}		
    
    void visit(ASTIntVecNode& n){
		n.node1->accept(*this);
		
		bool tok = true;
		tok = !n.node1->get_type().compare("int");		
		if(not tok){
			cout << "Type checking failed in line " << n.get_lineno() << ". Argument in 'new int[arg]' is of type '" 
			<< n.node1->get_type()  << "'. Must be an integer." << endl;
		}
		typeok = typeok && tok;	
    }
    
    void visit(ASTVecValueNode& n){}   
    
    void visit(ASTLengthNode& n){
		n.node1->accept(*this);
		
		bool tok = true;
		tok = !n.node1->get_type().compare("int[]");			
		if (not tok) {
			cout << "Type checking failed in line " << n.get_lineno() << ".  '.length' applied to type '" 
			<< n.node1->get_type()  << "'. Must be applied to an 'int[]'." << endl;
		}
		typeok = typeok && tok;			
	}
	
	void visit(ASTNegationNode& n){
		n.node1->accept(*this);
		
		bool tok = true;
		tok = !n.node1->get_type().compare("boolean");			
		if (not tok) {
			cout << "Type checking failed in line " << n.get_lineno() << ".  Negation symbol '!' applied to type '" 
			<< n.node1->get_type()  << "'. Must be applied to a 'boolean'." << endl;
		}
		typeok = typeok && tok;				
	}
	
	void visit(ASTCallMethodNode& n){	
		n.instance_num = n.node1->instance_num;
		current_instance = n.node1->instance_num;
		bool tok = true;	
		if(!n.get_type().compare("this")){
			ClassTabData* cdata = newIds[current_instance]->cdata;			
			MethodTabData* mdata = cdata->lookup_method(*n.id);
			int i = 0;
			while(mdata == NULL || i < newIds.size()){
				cdata = newIds[i]->cdata;			
				mdata = cdata->lookup_method(*n.id);
				i++;
			}
						
			n.type = mdata->return_type;	
			mdata->get_node()->accept(*this);
			tok = (n.num_p == ((ASTMethodNode*) mdata->get_node())->num_p) ;
			if (not tok)
				cout << "Type checking failed in line " << n.get_lineno() << ". Read " << n.num_p << " parameter values for" <<
				" a method with " << ((ASTMethodNode*) mdata->get_node())->num_p <<  " parameters." <<endl;
									
		}else{
					
			ASTMethodNode* node = (ASTMethodNode*)n.mdata->get_node();
			
			if(node != NULL){
				node->num = n.num;
				num = n.num;
				n.set_type(node->get_type());			
				node->accept(*this);
				tok = (n.num_p == (node->num_p));
				if (not tok)
					cout << "Type checking failed in line " << n.get_lineno() << ". Read " << n.num_p << " parameter values for" <<
					" a method with " << node->num_p << " parameters." <<endl;		
				
				if(!n.get_type().size() == 0)
					n.type = n.mdata->return_type;
										
			}
		}

		
		assigning_f = true;
		for(int i=paramAssigns.size()-1; i>=0; i--)
			paramAssigns[i]->accept(*this);
		assigning_f = false;
		
		
		typeok = typeok && tok;	
			
		/*
		cout << "instancias existentes: " << endl;
		for(int i=0; i<newIds.size(); i++){
			cout << newIds[i]->get_type() << " endereço: " << newIds[i] << endl;
			
		}
		cout << "&&&&&& " << n.node1->get_type() << " instance-num: " << n.node1->instance_num << "endredeco: " <<  n.node1 <<endl;
		cout << "_______________________________: " << endl;
		*/
	}
	
	void visit(ASTThisNode& n){
		n.instance = current_instance;
		n.instance_num = current_instance;
		n.type = newIds[current_instance]->get_type();	
	}	 	 	   

public:
	vector<string> typesVector;
	vector<ASTNewIdNode*> newIds;
	vector<ASTAssignStmtNode*> paramAssigns;
	bool assigning_f = false;
	int num;
	int current_instance; /* muda pa vector e fazer push, e pop qdo se sai d um metodo*/
    bool typeok;
};

#endif
