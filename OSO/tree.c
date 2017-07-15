#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dmanager.h"
#include "stack.h"


void printTree(struct Node* node){

	int tot = node->tot_nodes;
	printf("%s\n",printNode(node));

	int i;
	for(i = 0 ; i < tot ; i++){
		printTree(node->nodes[i]);
	}
	
}

char* printNode(struct Node* n){
	printf("Entro a printNode");
	char* to_ret;
	to_ret = malloc(sizeof(char) * MAX_LENGTH);
	sprintf(to_ret, "Type: %d - Value: %s - Total Leaves: %d",n->type,n->value,n->tot_nodes);
	return to_ret;
}

struct Node* createNewVariableIntegerNode(char* name, int value){

	struct Node* n = malloc(sizeof(struct Node));

	if (!add_variable(name)){
		return NULL;
	}

	char* v;
	v = malloc(sizeof(char) * MAX_LENGTH);
	sprintf(v,"int %s = %d;",name, value);

	n->type = VINT;
	n->value = v;
	n->tot_nodes = 0;

	char* to_conv = malloc(sizeof(char) * MAX_JCONV);
	sprintf(to_conv, "Integer %s = %d; \n", name, value);
	n->jconv = to_conv;

	add(n);
	return n;
}

struct Node* createNewVariableInteger2Node(char* name){

	struct Node* n = malloc(sizeof(struct Node));

	if (!add_variable(name)){
		return NULL;
	}

	n->type = VINT;
	n->value = name;
	n->tot_nodes = 0;

	struct Node* node_pop = pop();

	char* to_conv = malloc(sizeof(char) * MAX_JCONV);
	sprintf(to_conv, "Integer %s = %s; \n", name, node_pop->jconv);
	n->jconv = to_conv;

	add(n);
	return n;
}

struct Node* createNewVariableStringNode(char* name, char* value){

	struct Node* n = malloc(sizeof(struct Node));

	if (!add_variable(name)){
		return NULL;
	}

	char* v;
	v = malloc(sizeof(char) * MAX_LENGTH);
	sprintf(v,"string %s = %s;",name, value);

	n->type = VSTRING;
	n->value = v;
	n->tot_nodes = 0;


	char* to_conv = malloc(sizeof(char) * MAX_JCONV);
	sprintf(to_conv, "String %s = %s;\n", name, value);
	n->jconv = to_conv;

	add(n);
	return n;
}

struct Node* createIntegerNode(int value){
	struct Node* n = createIntegerNodeNoToStack(value);
	add(n);
	return n;

}

struct Node* createIntegerNodeNoToStack(int value){
	struct Node* n = malloc(sizeof(struct Node));

	n->type = INTEGERNODE;

	char* v;
	v = malloc(sizeof(char) * MAX_LENGTH);
	sprintf(v,"%d",value);
	n->value = v;
	n->tot_nodes = 0;

	char* to_conv = malloc(sizeof(char) * MAX_JCONV);
	sprintf(to_conv,"%d",value);
	n->jconv = to_conv;

	return n;

}

struct Node* createStringNode(char* v){
	struct Node* n = createStringNodeNoToStack(v);
	add(n);
	return n;
}

struct Node* createStringNodeNoToStack(char* v){
	struct Node* n = malloc(sizeof(struct Node));

	n->type = STRINGNODE;
	n->value = v;
	n->tot_nodes = 0;

	n->jconv = v;

	return n;

}

struct Node* createVariableCreated(char* v){
	struct Node* n = createStringNodeNoToStack(v);
	n->type = VVAR;
	return n;
}


struct Node* createCMPNode(char* symbol, struct Node* left, struct Node* right){
	struct Node* n = malloc(sizeof(struct Node));

	n->type = CMP;

	n->value = symbol;
	n->tot_nodes = 0;

	char* to_conv = malloc(sizeof(char) * MAX_JCONV);
	sprintf(to_conv, "%s %s %s", left->jconv, symbol, right->jconv);
	n->jconv = to_conv;

	addLeaves(n,left);
	addLeaves(n,right);

	add(n);

	return n;
}

struct Node* createIfNode(){
	struct Node* n = malloc(sizeof(struct Node));

	n->type = IFNODE;

	int condition_while = 0;

	while(condition_while == 0){
		struct Node* node_pop = pop();
		
		if(node_pop->type == CMP){
			condition_while = 1;
		}

		addLeaves(n,node_pop);

	}

	char* to_conv = malloc(sizeof(char) * MAX_JCONV);
	sprintf(to_conv, "if ( %s ){ \n", n->nodes[n->tot_nodes - 1]->jconv);
	
	int i;
	for(i = n->tot_nodes - 2 ; i >= 0; i--){
		sprintf(to_conv, "%s %s", to_conv, n->nodes[i]->jconv); //OJO ACA!!! HAY QUE REVISARLO!!!
	}

	sprintf(to_conv, "%s %s \n", to_conv, "}");
	n->jconv = to_conv;

	add(n);
	return n;
}

struct Node* createMainNode(char* ret, char* function_name){ 
	struct Node* main_node = malloc(sizeof(struct Node));
	main_node->type = MAIN;

	char* v;
	v = malloc(sizeof(char) * MAX_LENGTH);
	sprintf(v,"%s %s", ret, function_name);
	main_node->value = v;

	int t_nodes = get_tot_stack(); 

	char* to_conv = malloc(sizeof(char) * MAX_JCONV);
	if (strcmp(function_name,"main") == 0 ){
		sprintf(to_conv, "public static void main(String[] args) { \n");	
	} else {
		sprintf(to_conv, "public static %s %s() { \n",ret,function_name);
	}

	while (get_tot_stack() != 0){
		struct Node* node_pop = pop();
		t_nodes--;
		addLeaveAtPosition(main_node, node_pop, t_nodes);
	}

	int i;
	for(i = 0 ; i < main_node->tot_nodes ; i++){
		sprintf(to_conv,"%s %s", to_conv, main_node->nodes[i]->jconv);
	}

	sprintf(to_conv,"%s %s\n",to_conv,"}");
	main_node->jconv = to_conv;

	clear_vars();

	return main_node;

}

struct Node* createFromToNode(char* name, int from, int to){

	struct Node* n = malloc(sizeof(struct Node));

	if (!add_variable(name)){
		return NULL;
	}

	n->type=FROMTO;

	addLeaves(n,createIntegerNodeNoToStack(from));
	addLeaves(n,createIntegerNodeNoToStack(to));

	char* to_conv = malloc(sizeof(char) * MAX_JCONV);
	sprintf(to_conv,"int %s = %d ; %s < %d ; %s++",name,from,name,to,name);
	n->jconv = to_conv;

	add(n);
	return n;

}

struct Node* createFromTo2Node(char* name, int from, char* to){

	struct Node* n = malloc(sizeof(struct Node));

	if (!add_variable(name)){
		return NULL;
	}

	n->type=FROMTO;

	addLeaves(n,createIntegerNodeNoToStack(from));
	addLeaves(n,createVariableCreated(to));

	char* to_conv = malloc(sizeof(char) * MAX_JCONV);
	sprintf(to_conv,"int %s = %d ; %s < %s ; %s++",name,from,name,to,name);
	n->jconv = to_conv;

	add(n);
	return n;

}

struct Node* createFromTo3Node(char* name, char* from, char* to){

	struct Node* n = malloc(sizeof(struct Node));

	if (!add_variable(name)){
		return n;
	}

	n->type=FROMTO;

	addLeaves(n,createVariableCreated(from));
	addLeaves(n,createVariableCreated(to));

	char* to_conv = malloc(sizeof(char) * MAX_JCONV);
	sprintf(to_conv,"int %s = %s ; %s < %s ; %s++",name,from,name,to,name);
	n->jconv = to_conv;

	add(n);
	return n;

}

struct Node* createForNode(){
	struct Node* n = malloc(sizeof(struct Node));

	n->type = FORNODE;

	int condition_while = 0;

	while(condition_while == 0){
		struct Node* node_pop = pop();
		
		if(node_pop->type == FROMTO){
			condition_while = 1;
		}

		addLeaves(n,node_pop);

	}

	int tot_elems = n->tot_nodes;
	char* to_conv = malloc(sizeof(char) * MAX_JCONV);
	sprintf(to_conv,"for ( %s ) { \n", n->nodes[tot_elems - 1]->jconv);

	int i;
	for(i = tot_elems - 2 ; i >= 0 ; i--){
		sprintf(to_conv,"%s %s",to_conv,n->nodes[i]->jconv);
	}

	sprintf(to_conv,"%s %s\n",to_conv,"}");
	n->jconv = to_conv;

	add(n);
	return n;
}

struct Node* createWhileNode(){

	struct Node* n = malloc(sizeof(struct Node));

	n->type = WHILENODE;

	int condition_while = 0;

	while(condition_while == 0){
		struct Node* node_pop = pop();
		
		if(node_pop->type == CMP){
			condition_while = 1;
		}

		addLeaves(n,node_pop);

	}

	int tot_elems = n->tot_nodes;
	char* to_conv = malloc(sizeof(char) * MAX_JCONV);
	sprintf(to_conv,"while ( %s ) { \n", n->nodes[tot_elems - 1]->jconv);

	int i;
	for(i = tot_elems - 2 ; i >= 0 ; i--){
		sprintf(to_conv,"%s %s",to_conv,n->nodes[i]->jconv);
	}

	sprintf(to_conv,"%s %s\n",to_conv,"}");
	n->jconv = to_conv;	

	add(n);
	return n;

}

struct Node* createFunParam(char* t, char* name){
	struct Node* n = malloc(sizeof(struct Node));

	n->type = FUNPARAM;

	char* v;
	v = malloc(sizeof(char) * MAX_LENGTH);
	sprintf(v,"%s %s", t, name);

	n->value = v;
	n->tot_nodes = 0;

	add(n);
	return n;

}

struct Node* createReadNode(char* var_to_read){
	struct Node* n = malloc(sizeof(struct Node));

	n->type = READNODE;
	n->value = var_to_read;
	n->tot_nodes = 0;

	char* to_conv = malloc(sizeof(char) * MAX_JCONV); 

	sprintf(to_conv, "Scanner _v_%s_s_ = new Scanner(System.in); String _v_%s_s_aux_ = _v_%s_s_.next(); try { %s = Integer.parseInt(_v_%s_s_aux_); } catch (Exception e) { System.out.println(\"Input Error. OSO ended.\"); return; }\n", 
		var_to_read, var_to_read, var_to_read, var_to_read, var_to_read);
	
	n->jconv = to_conv;

	add(n);
	return n;
}

struct Node* createShowIntegerNode(int number){
	struct Node* n = malloc(sizeof(struct Node));

	n->type = SHOWNODE;

	char* v = malloc(sizeof(char) * MAX_LENGTH);
	sprintf(v,"print -> %d",number);
	n->value = v;
	n->tot_nodes = 0;

	char* to_conv = malloc(sizeof(char) * MAX_JCONV);
	sprintf(to_conv,"System.out.print(%d);", number);
	n->jconv = to_conv;	

	add(n);
	return n;
}

struct Node* createShowStringNode(char* var_or_text){
	struct Node* n = malloc(sizeof(struct Node));

	n->type = SHOWNODE;

	char* v = malloc(sizeof(char) * MAX_LENGTH);
	sprintf(v,"%s",var_or_text);
	n->value = v;
	n->tot_nodes = 0;

	char* to_conv = malloc(sizeof(char) * MAX_JCONV);

	sprintf(to_conv,"System.out.print(%s);\n", var_or_text);

	n->jconv = to_conv;	

	add(n);
	return n;
}

struct Node* createSetIntegerNode(char* var_to_set, int val){
	struct Node* n = malloc(sizeof(struct Node));

	n->type = SETNODE;
	char* v = malloc(sizeof(char) * MAX_LENGTH);
	sprintf(v,"%s = %d",var_to_set,val);
	n->value = v;
	n->tot_nodes = 0;

	char* to_conv = malloc(sizeof(char) * MAX_JCONV);
	sprintf(to_conv,"%s = %d;\n",var_to_set,val);
	n->jconv = to_conv;	

	add(n);
	return n;
}

struct Node* createSetInteger2Node(char* var_to_set){
	struct Node* n = malloc(sizeof(struct Node));

	n->type = SETNODE;
	n->value = "set node";
	n->tot_nodes = 0;

	struct Node* node_pop = pop();

	char* to_conv = malloc(sizeof(char) * MAX_JCONV);
	sprintf(to_conv,"%s = %s;\n",var_to_set,node_pop->jconv);
	n->jconv = to_conv;	

	add(n);
	return n;
}

struct Node* createSetStringNode(char* var_to_set, char* val){
	struct Node* n = malloc(sizeof(struct Node));

	n->type = SETNODE;
	char* v = malloc(sizeof(char) * MAX_LENGTH);
	sprintf(v,"%s = %s",var_to_set,val);
	n->value = v;
	n->tot_nodes = 0;

	char* to_conv = malloc(sizeof(char) * MAX_JCONV);
	sprintf(to_conv,"%s = \"%s\";\n",var_to_set,val);
	n->jconv = to_conv;	

	add(n);
	return n;
}

struct Node* createCMPAuxiliarNode(char* symbol){
	struct Node* n = malloc(sizeof(struct Node));

	n->type = CMP;
	n->value = symbol;
	n->tot_nodes = 0;

	struct Node* node_pop = pop();
	addLeaves(n,node_pop);

	char* to_conv = malloc(sizeof(char) * MAX_JCONV);
	sprintf(to_conv,"%s ( %s )",symbol, node_pop->jconv);
	n->jconv = to_conv;	

	add(n);
	return n;

}

struct Node* createCMPAuxiliar2Node(char* symbol){
	struct Node* n = malloc(sizeof(struct Node));

	n->type = CMP;
	n->value = symbol;
	n->tot_nodes = 0;

	struct Node* node_pop1 = pop();
	struct Node* node_pop2 = pop();
	addLeaves(n,node_pop1);
	addLeaves(n,node_pop2);

	char* to_conv = malloc(sizeof(char) * MAX_JCONV);
	sprintf(to_conv,"%s %s %s\n",node_pop2->jconv,symbol,node_pop1->jconv);
	n->jconv = to_conv;	

	add(n);
	return n;

}

struct Node* createCallFunctionNode(char* function_name){
	struct Node* n = malloc (sizeof(struct Node));

	n->type = CALLFUNCTION;
	n->value = function_name;
	n->tot_nodes = 0;

	char* to_conv = malloc(sizeof(char) * MAX_JCONV);
	sprintf(to_conv,"%s();\n",function_name);
	n->jconv = to_conv;	

	add(n);
	return n;

}

struct Node* createParenthesisNode(){
	struct Node* n = malloc (sizeof(struct Node));

	n->type = OP_INT;
	n->value = "()";
	n->tot_nodes = 1;

	struct Node* node_pop = pop();

	char* to_conv = malloc(sizeof(char) * MAX_JCONV);
	sprintf(to_conv,"(%s)",node_pop->jconv);
	n->jconv = to_conv;	

	add(n);
	return n;

}

struct Node* createOPNode(char* symbol){
	struct Node* n = malloc (sizeof(struct Node));

	n->type = OP_INT;
	n->value = symbol;
	n->tot_nodes = 1;

	struct Node* node_pop1 = pop();
	struct Node* node_pop2 = pop();

	char* to_conv = malloc(sizeof(char) * MAX_JCONV);
	sprintf(to_conv,"%s %s %s",node_pop2->jconv, symbol, node_pop1->jconv);
	n->jconv = to_conv;	

	add(n);
	return n;
}

struct Node* createCallVariableNode(char* symbol){
	struct Node* n = malloc (sizeof(struct Node));

	n->type = VINT;
	n->value = symbol;
	n->tot_nodes = 0;

	char* to_conv = malloc(sizeof(char) * MAX_JCONV);
	sprintf(to_conv,"%s",symbol);
	n->jconv = to_conv;	

	add(n);
	return n;
}

void addLeaveAtPosition(struct Node* root, struct Node* leave, int pos){
	root->nodes[pos] = leave;
	root->tot_nodes++;
	return;
}

void addLeaves(struct Node* root, struct Node* leave){
	addLeaveAtPosition(root,leave,root->tot_nodes);
	return;
}