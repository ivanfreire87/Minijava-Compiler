#ifndef _VISITOR_H
#define _VISITOR_H 1

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
class ASTCallMethodNode;
class ASTThisNode;

class Visitor {
public:
	virtual void visit(ASTTypesVecNode& n) = 0;
	virtual void visit(ASTProgramNode& n) = 0;
	virtual void visit(ASTMainNode& n) = 0;
	virtual void visit(ASTClassRefNode& n) = 0;
	virtual void visit(ASTClassNode& n) = 0;
	virtual void visit(ASTClassesNode& n) = 0;
	virtual void visit(ASTMethodRefNode& n) = 0;
	virtual void visit(ASTMethodNode& n) = 0;
	virtual void visit(ASTMethodsNode& n) = 0;
  virtual void visit(ASTStmtsNode& n) = 0;
  virtual void visit(ASTAssignStmtNode& n) = 0;
  virtual void visit(ASTAssignIntVecStmtNode& n) = 0;
  virtual void visit(ASTIfStmtNode& n) = 0;
  virtual void visit(ASTWhileStmtNode& n) = 0;
  virtual void visit(ASTPrintStmtNode& n) = 0;
  virtual void visit(ASTOpExprNode& n) = 0;
  virtual void visit(ASTIntConstNode& n) = 0;
  virtual void visit(ASTBoolConstNode& n) = 0;
  virtual void visit(ASTVarRefNode& n) = 0;
  virtual void visit(ASTVarDeclsNode& n) = 0;
  virtual void visit(ASTNewIdNode& n) = 0;
  virtual void visit(ASTIntVecNode& n) = 0;
  virtual void visit(ASTVecValueNode& n) = 0;
  virtual void visit(ASTLengthNode& n) = 0;
  virtual void visit(ASTNegationNode& n) = 0;
  virtual void visit(ASTCallMethodNode& n) = 0;
  virtual void visit(ASTThisNode& n) = 0;
};

#endif
