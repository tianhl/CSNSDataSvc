#ifndef DYNAMIC_TREENODE_H
#define DYNAMIC_TREENODE_H


#include <map>
#include <vector>
#include <iostream>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

using namespace std;


//=======================================================================
template<class O>
class DynamicNode{
	public:
		DynamicNode(){m_ref=0;}

		virtual ~DynamicNode() {};

		// map
		DynamicNode<O>* find(const std::string& name);
		bool regist(const std::string& name, DynamicNode<O>* node);

		// obj
		bool regObj(O* o); 
		O* getObj(); 

	private:
		O*               m_ref;
	        std::map<std::string, DynamicNode<O>*> _map;

};

//=======================================================================
template<class O>
class DynamicTree: public DynamicNode<O>{
	public:
		// sington in one datamgr
		DynamicTree(){ m_parent = 0; m_path   = "/"; m_root   = this; }

		// Branch 
		DynamicTree<O>* find(const std::string& name);

		DynamicTree<O>* branch(const std::string& name);

		// obj
		template<class C>
                C* getObj(std::string path){return dynamic_cast<C*>(find(path)->getObj());}
		// ????
		//O* getObj(){return DynamicNode<O>::getObj();}; 
                using DynamicNode<O>::getObj;

		bool regObj(std::string path, O* o){return find(path)->regObj(o);}
		// ????
		//bool regObj(O* o){return DynamicNode<O>::regObj(o);}
                using DynamicNode<O>::regObj;

		// tree
		std::string     path(){return m_path;}
		DynamicTree<O>* root(){return m_root;}
		DynamicTree<O>* parent(){return m_parent;}

	private:
		DynamicTree(std::string name, DynamicTree<O>* parent);

	private:
		std::string      m_path;
		DynamicTree<O>*  m_root;
		DynamicTree<O>*  m_parent;
};

//=======================================================================
template<class O>
DynamicNode<O>* DynamicNode<O>::find(const std::string& name){
	if(!name.empty() && _map.find(name) != _map.end()) return _map[name];
	else return NULL;
};

template<class O>
bool DynamicNode<O>::regist(const std::string& name, DynamicNode<O>* node){
	if(!name.empty() && _map.find(name) == _map.end()){
		_map.insert(std::pair<std::string, DynamicNode<O>*>(name, node));
		return true;
	}
	return false;
};

template<class O>
bool DynamicNode<O>::regObj(O* o){ 
	if(m_ref) {
		std::cout << "Object has been registed!" << std::endl;
		return false;
	}
	else {
		m_ref = o; 
		return true;
	}
};

template<class O>
O* DynamicNode<O>::getObj(){ 
	if(m_ref) return m_ref; 
	else throw std::runtime_error("Object is not registed.\n");
};

//=======================================================================
template<class O>
DynamicTree<O>* DynamicTree<O>::find(const std::string& name){
	vector<std::string> dividedName;
	vector<std::string>::iterator itName;
	boost::split(dividedName, name, boost::is_any_of("/"), boost::token_compress_on);
	DynamicNode<O>* current = (0==name.find("/"))?m_root:this;
	for(itName=dividedName.begin(); itName!=dividedName.end(); itName++){
		if(0==(*itName).size()) continue;
		current = current->find(*itName);
		if(not current )return NULL;
	}
	return dynamic_cast<DynamicTree<O>*>(current);
};

template<class O>
DynamicTree<O>* DynamicTree<O>::branch(const std::string& name){
	if(name.empty()) throw std::runtime_error("make branch error: no name for branch");
	DynamicTree<O>* br = new DynamicTree<O>(name, this);
	if(not this->regist(name, br)){
		delete br;
		return NULL;
	}
	return br;
};

template<class O>
DynamicTree<O>::DynamicTree(std::string name, DynamicTree<O>* parent){
	m_parent = parent;
	m_root   = parent->root();
	m_path   = m_parent->path()+(m_parent->path()=="/"?name:"/"+name);
};

//=======================================================================

#endif
