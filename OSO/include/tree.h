#ifndef TREE_H
#define TREE_H

#define MAX_NODES 30
#define MAX_LENGTH 1000
#define MAX_JCONV 10000

typedef enum {CMP, IFNODE, INTEGERNODE, VINT, MAIN, FROMTO, 
					FORNODE, WHILENODE, FUNPARAM, READNODE, 
					SHOWNODE, VSTRING, STRINGNODE, VVAR, SETNODE, CALLFUNCTION,
					OP_INT} types;

struct Node {
	types type;
	char* value; //cambiar!! deberia ser algo mas generico
	int tot_nodes;
	struct Node* nodes[MAX_NODES];

	char* jconv;

};

char* printNode(struct Node* n);
void printTree(struct Node* node);

struct Node* createNewVariableIntegerNode(char* name, int value);
struct Node* createNewVariableInteger2Node(char* name);
struct Node* createIntegerNode(int value);
struct Node* createCMPNode(char* symbol, struct Node* left, struct Node* right);
struct Node* createIfNode();
struct Node* createMainNode(char* ret, char* function_name);
struct Node* createForNode();
struct Node* createFromToNode(char* name, int from, int to);
struct Node* createFromTo2Node(char* name, int from, char* to);
struct Node* createFromTo3Node(char* name, char* from, char* to);
struct Node* createIntegerNodeNoToStack(int value);
struct Node* createWhileNode();
struct Node* createFunParam(char* t, char* name);
struct Node* createReadNode(char* var_to_read);
struct Node* createShowIntegerNode(int number);
struct Node* createShowStringNode(char* var_or_text);
struct Node* createNewVariableStringNode(char* name, char* value);
struct Node* createStringNode(char* v);
struct Node* createStringNodeNoToStack(char* v);
struct Node* createVariableCreated(char* v);
struct Node* createSetStringNode(char* var_to_set, char* val);
struct Node* createSetIntegerNode(char* var_to_set, int val);
struct Node* createSetInteger2Node(char* var_to_set);
struct Node* createCMPAuxiliarNode(char* symbol);
struct Node* createCMPAuxiliar2Node(char* symbol);
struct Node* createCallFunctionNode(char* function_name);
struct Node* createCallVariableNode(char* symbol);
struct Node* createParenthesisNode();
struct Node* createOPNode(char* symbol);

void addLeaveAtPosition(struct Node* root, struct Node* leave, int pos);
void addLeaves(struct Node* root, struct Node* leave);

#endif
