#include <iostream>
#include <cstring>
#include <map>
#include <math.h>
#include <sstream>
#include <algorithm>
#include <stack>

typedef std::map <std::string, double> mymap;
class Token
{
	public:
		enum Type{ADD='+',SUB='-',MULT='*',DIV='/',POW='^',LOG='l',VAL,PER,NO}; 
		Token(Type type2=NO,double dvalue2=0,std::string value2="")
		{
			type=type2;
			dvalue=dvalue2;
			value=value2;
		}
		Type gettype()
		{
			return type;
		}
		double getdvalue()
		{
			return dvalue;
		}
		std::string getvalue()
		{
			return value;
		}
	private:
		Type type;
		double dvalue;
		std::string value;
};

template<class T>
class Tree
{
	public:
		struct Node
				{
					Node *l,*r;
					T elem;;	
					Node(T elem2):l(0),r(0)
					{
						elem=elem2;
					}
					double count(mymap m)
					{
						double rez=0;
						switch(elem.gettype())
						{
							case Token::ADD:
								rez=l->count(m)+r->count(m);
								break;
							case Token::SUB:
								rez=l->count(m)-r->count(m);
								break;
							case Token::MULT:
								rez=l->count(m)*r->count(m);
								break;
							case Token::DIV:
								rez=l->count(m)/r->count(m);
								break;
							case Token::LOG:
								rez=log(r->count(m));
								break;
							case Token::POW:
								rez=pow(l->count(m),r->count(m));
								break;
							case Token::VAL:
								rez=elem.getdvalue();
								break;
							case Token::PER:
								rez=m[elem.getvalue()];
								break;
							default:
								break;
						}
						return rez;
					}
				};
		Node *c;
		Tree():c(0)
		{
		}
		double rezult(mymap m)
		{
			return c->count(m);
		}
		
};
	
class ParseTree
{
	public:
        ParseTree():pars_tree(Tree<Token>())
        {
        }
        void createtree(Tree<Token>::Node** p_tree, std::string& str);
        Tree<Token> pars_tree;
    private:
        std::pair<std::string, std::string> crtree(Tree<Token>::Node** p_tree, std::string& str, int f);
};

bool isNumeric(std::string& some)
{
        double one=0;
        std::stringstream ss(some);
        ss >> one;
        std::stringstream ss2;
        ss2<<one;
        return (one && some.find_first_not_of(ss2.str()) == std::string::npos); 
}

bool isPer(std::string& some)
{
        if(((some[0]>='a' && some[0]<='z') || (some[0]>='A' && some[0]<='Z')) && some.length()<2)
        {
			return true;
		}
		else
		{
			return false;
		} 
}
 
std::string::iterator skobk(std::string& some, const char first, const char second, int f)
{
        std::stack<char> lays;
        std::stack<char> lays2;
        for(std::string::iterator iter=some.begin()+f; iter != some.end(); ++iter)
        {
			if(*iter == '(')
			{
				lays.push(*iter);
			}
			else 
			{
				if(*iter == ')')
				{
					lays.pop();
				}
			}
			if(lays.empty() && (*iter == first || *iter == second))
			{
				if(f == 0)
				{
					return iter;
				}
				else
				{
					some.erase(std::find(some.begin(), some.end(), '('));
					for(std::string::const_iterator iter2=iter; iter2 != some.end(); ++iter2)
					{
						if(lays2.empty() && *iter2 == ')')
						{
							some.erase(iter2);
							return iter-1;
						}
						if(*iter2 == '(')
						{
							lays2.push(*iter);
						}
						else 
						{
							if(*iter2 == ')')
							{
								lays2.pop();
							}
						}
					}
				}
			}
        }
        return some.end();
}
 
std::pair<std::pair<std::string,std::string>, bool> skobki(std::string& some, const char first, const char second, int f)
{
        if(isNumeric(some))
        {
			return std::make_pair(std::make_pair(some, ""), true);
		}
        if(isPer(some))
        {
			return std::make_pair(std::make_pair(some, ""), true);
		}
        std::string::iterator iter=skobk(some, first, second, f);
        if(iter == some.end())
        {
			return std::make_pair(std::make_pair("", ""), false);
		}
		std::string begin,end;
		if(*iter=='l')
        {
			begin.assign(some.begin(), ++iter);
			end.assign(iter+1, some.end());
		}
		else
		{
			begin.assign(some.begin(), ++iter);
			end.assign(iter, some.end());
		}
		return std::make_pair(std::make_pair(begin, end), true);
}

std::pair<std::string, std::string> ParseTree::crtree(Tree<Token>::Node** p_tree, std::string& str, int f)
{
        const std::string znak="+-*/^l";
        std::string l,r;
        for(std::string::const_iterator iter=znak.begin(); iter != znak.end(); iter+=2)
        {
			std::pair<std::pair<std::string, std::string>, bool> pair;
			pair=skobki(str, *iter, *(iter+1), f);
			if(pair.second == true)
			{
				l=pair.first.first;
				r=pair.first.second;
				char c='\n';
				if(znak.find(l[l.size()-1]) != std::string::npos && !l.empty())
				{
					c=l[l.size()-1];
					*p_tree=new Tree<Token>::Node(Token(static_cast<Token::Type>(c), 0, std::string(1,c)));
					l.resize(l.size()-1);
				}
				if(isNumeric(l))
				{
					(*p_tree)->l=new Tree<Token>::Node(Token(Token::VAL, stod(l), l));
					l.clear();
				}
				if(isNumeric(r))
				{
					(*p_tree)->r=new Tree<Token>::Node(Token(Token::VAL, stod(r), r));
					r.clear();
				}
				if(isPer(l))
				{
					(*p_tree)->l=new Tree<Token>::Node(Token(Token::PER, 0, l));
					l.clear();
				}
				if(isPer(r))
				{
					(*p_tree)->r=new Tree<Token>::Node(Token(Token::PER, 0, r));
					r.clear();
				}
				break;
			}
        }
        return std::make_pair(l, r);
}

void ParseTree::createtree(Tree<Token>::Node** p_tree, std::string& str)
{ 
        std::string l,r;
        std::pair<std::string, std::string> lr=crtree(p_tree, str, 0);
        if(lr.first.empty() && lr.second.empty() && str.find('(') != std::string::npos)
        {
            lr=crtree(p_tree, str, 1);
		}
        l=lr.first;
        r=lr.second;
        if(!l.empty())
        {
			createtree(&(*p_tree)->l, l);
		}
        if(!r.empty())
        {
			createtree(&(*p_tree)->r, r);
		}
}

int main(int n, char **arg)
{
	ParseTree vir; 
	mymap mp;
	std::string inputline,chisl;
	int i=1;
	std::string templine(arg[1]);
	if(templine[0]=='\"')
	{
		templine.erase(0);
		templine.erase(templine.length());
	}
	vir.createtree(&vir.pars_tree.c, templine);
	getline(std::cin, inputline);
	auto ravn=inputline.find("="); 
	while(!inputline.empty())
	{
		while(ravn!=std::string::npos)
		{
			while(inputline[ravn+i]>='0' && inputline[ravn+i]<='9')
			{
				chisl+=inputline[ravn+i++];
			}
			mp[inputline.substr(ravn-1,1)]=stod(chisl);
			inputline.erase(0,ravn+i+1);
			ravn=inputline.find("=");
			i=1;
			chisl.clear();	
		}
		for(auto& item : mp)
		{
			std::cout << item.first << " : " << item.second << std::endl;
		}
		std::cout << vir.pars_tree.rezult(mp) << std::endl;
		mp.clear();
		getline(std::cin, inputline);
		ravn=inputline.find("="); 
	}
	
    
}
