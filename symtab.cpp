#include "symtab.hh"

ClassTabData* SymbolTable::lookup_class(string& var) {
    ClassTabData *td = NULL;
    for(int i=classMaps.size()-1; i>=0; --i) {
	ClassTabMap::iterator mpos = (*classMaps[i]).find(var);
	if (mpos != (*classMaps[i]).end()) {
	    td = mpos->second;
	    break;
	}
    }

    return td;
}

MethodTabData* ClassTabData::lookup_method(string& var) {
    MethodTabData *td = NULL;
    for(int i=methodMaps.size()-1; i>=0; --i) {
	MethodTabMap::iterator mpos = (*methodMaps[i]).find(var);
	if (mpos != (*methodMaps[i]).end()) {
	    td = mpos->second;
	    break;
	}
    }
    return td;
}

TabData* ClassTabData::lookup_class_var(string& var) {
	
	TabMap::iterator mpos = (*classVarsMap).find(var);
	TabData* td = (mpos != (*classVarsMap).end()) ? mpos->second : NULL;
    return td;
}

TabData* MethodTabData::lookup_method_var(string& var) {

	TabMap::iterator mpos = (*methodVarsMap).find(var);
	TabData* td = (mpos != (*methodVarsMap).end()) ? mpos->second : NULL;

    return td;
}

TabData* MethodTabData::lookup_method_param(string& var) {
	assert(methodParamsMaps.size() >0);
	TabMap::iterator mpos = (*methodParamsMaps[methodParamsMaps.size()-1]).find(var);
	TabData* td = (mpos != (*methodParamsMaps[methodParamsMaps.size()-1]).end()) ? mpos->second : NULL;

    return td;
}

ClassTabData* SymbolTable::insert_class(string& var){
    ClassTabData* vdata = new ClassTabData(var);
    (*classMaps[classMaps.size()-1])[var] = vdata;
    vdata->classVarsMap = new TabMap();
    MethodTabMap* mmap = new MethodTabMap();
    vdata->methodMaps.push_back(mmap);
    return vdata;
}

void SymbolTable::push_scope(){

	
	
    ClassTabMap* cmap = new ClassTabMap();
    classMaps.push_back(cmap);

}

void SymbolTable::pop_scope(){	
    assert(classMaps.size() >= 1);
    ClassTabMap* cmap = classMaps[classMaps.size()-1];
    delete cmap;
    classMaps.pop_back();
}

TabData* ClassTabData::insert_class_var(string& var){
    TabData* vdata = new TabData(var);
    (*classVarsMap)[var] = vdata;
    return vdata;
}

MethodTabData* ClassTabData::insert_method(string& var){
    MethodTabData* mdata = new MethodTabData(var);
    (*methodMaps[methodMaps.size()-1])[var] = mdata;
    mdata->methodVarsMap = new TabMap();
    vector<ASTNode*> vec;
	mdata->methodParamAssigns.push_back(vec);
	mdata->methodParamsMaps.push_back(new TabMap());
    return mdata;
}

TabData* MethodTabData::insert_method_var(string& var){
    TabData* vdata = new TabData(var);
    (*methodVarsMap)[var] = vdata;
    return vdata;
}

TabData* MethodTabData::insert_method_param(string& var){   
    TabData* vdata;
    
	for(int i = methodParamsMaps.size()-1; i>=0 ; i--){
		vdata = new TabData(var);
		(*methodParamsMaps[i])[var] = vdata;
	}
       
    return vdata;
}

void MethodTabData::insert_method_param_assign(ASTNode* n1){	
	int i = methodParamAssigns.size()-1;
    methodParamAssigns[i].push_back(n1);
}
