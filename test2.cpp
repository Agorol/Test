#include <iostream>
#include <map>
#include <cstring>
#include <stdio.h>
#include <sstream>
#include <math.h>

using namespace std;

map <string, double> mp;
int flagS=0;

string Convert (double number)
{
    ostringstream buff;
    buff << fixed << number;
    return buff.str();   
}

double chastb1(int poz, string skobki, int& nach)
{
	string chast1;
	if((skobki[poz-nach]<='9' && skobki[poz-nach] >='0') || skobki[poz-nach]=='-')
	{
		while((poz-nach)>=0 && ((skobki[poz-nach]<='9' && skobki[poz-nach] >='0') || skobki[poz-nach]=='.' || skobki[poz-nach]=='-'))
			chast1=skobki[poz-nach++]+chast1;
		return stod(chast1);
	}
	if(!(mp.empty()))
		if((skobki[poz-nach]<='z' && skobki[poz-nach] >='a') || (skobki[poz-nach]<='Z' && skobki[poz-nach] >='A'))
		{
			chast1=skobki[poz-nach]+chast1;
			nach++;
			return mp[chast1];
		}
	return 0;
}

double chastb2(int poz, string skobki, int& kon)
{
	string chast2;
	int length = skobki.length();
	if((skobki[poz+kon]<='9' && skobki[poz+kon] >='0') || skobki[poz+kon] =='-')
	{
		while((poz+kon)<length && ((skobki[poz+kon]<='9' && skobki[poz+kon] >='0') || skobki[poz+kon]=='.' || skobki[poz+kon]=='-'))
			chast2+=skobki[poz+kon++];
		return stod(chast2);
	}
	if(!(mp.empty()))
		if((skobki[poz+kon]<='z' && skobki[poz+kon] >='a') || (skobki[poz+kon]<='Z' && skobki[poz+kon] >='A'))
			{
				chast2=skobki[poz+kon]+chast2;
				kon++;
				return mp[chast2];
			}
	return 0;
}

string nol(double rez)
{
	string trez;
	trez = Convert(rez);
		if (trez[trez.size()-1]=='0')
			for (size_t i=trez.size()-1; trez[i]=='0'; i--)    
				trez.erase(i,1); 
		if (trez[trez.size()-1]=='.')
			trez.erase(trez.size()-1,1);
	return trez;
}

string znak(string skobki,string zn, int poz)
{
	int nach=0,kon=0;
	double chas1,chas2,rez;
	string chast1,chast2,trez;
	if(zn=="^")
	{	nach=1;
		kon=1;
		chas1=chastb1(poz,skobki,nach);
		chas2=chastb2(poz,skobki,kon);
		rez=pow(chas1,chas2);
		skobki.replace(poz-nach+1,kon+nach-1,nol(rez));
	}
	if(zn[0]=='l')
	{
		if(zn[1]=='n' || zn[1]=='g')
		{
			kon=3;
			chas2=chastb2(poz,skobki,kon);
			switch(zn[1])
			{
				case 'n':
					rez=log(chas2);
					break;
				case 'g':
					rez=log10(chas2);
					break;
				default:
					break;
			}
			skobki.replace(poz,kon+1,nol(rez));	
		}
		if(zn[1]=='o' && zn[2]=='g')
		{
			nach=3;
			chas1=chastb2(poz,skobki,nach);
			kon=nach+1;
			chas2=chastb2(poz,skobki,kon);
			rez=log(chas2)/log(chas1);
			skobki.replace(poz,kon+1,nol(rez));	
		}
	}
	if(zn=="*" || zn=="/" || zn=="+" || zn=="-")
	{	nach=2;
		kon=2;
		chas1=chastb1(poz,skobki,nach);
		chas2=chastb2(poz,skobki,kon);
		switch(zn[0])
		{
			case '*':
				rez=chas1*chas2;
				break;
			case '/':
				rez=chas1/chas2;
				break;
			case '+':
				rez=chas1+chas2;
				break;
			case '-':
				rez=chas1-chas2;
				break;
			default:
				break;
		}
		skobki.replace(poz-nach+1,kon+nach-1,nol(rez));
	}
	return skobki;
}
string resh(string skobki)
{
	int length = skobki.length();
	bool flag=false;
	string temp;
	for(int i=0;i<length;i++)
	{
		if(skobki[i]=='^')
		{
			skobki=znak(skobki,skobki.substr(i,1),i);
			flag = true;
		}
		if(skobki[i]=='l')
		{
			if(skobki[i+1]=='g' || skobki[i+1]=='n')
			{
				skobki=znak(skobki,skobki.substr(i,2),i);
				flag = true;
			}
			if(skobki[i+1]=='o' && skobki[i+2]=='g')
			{
				skobki=znak(skobki,skobki.substr(i,3),i);
				flag = true;
			}
		}	
		if(flag)
		{
			flag=false;
			length=skobki.length();
			i=-1;
			continue;
		}
	}
	for(int i=1;i<length;i++)
	{
		if(skobki[i-1]==' ' && skobki[i+1]==' ' && (skobki[i]=='*' || skobki[i]=='/'))
		{
			skobki=znak(skobki,skobki.substr(i,1),i);
			flag = true;
		}	
		if(flag)
		{
			flag=false;
			length=skobki.length();
			i=-1;
			continue;
		}
	}
	for(int i=1;i<length;i++)
	{
		if(skobki[i-1]==' ' && skobki[i+1]==' ' && (skobki[i]=='-' || skobki[i]=='+'))
		{
			skobki=znak(skobki,skobki.substr(i,1),i);
			flag = true;
		}	
		if(flag)
		{
			flag=false;
			length=skobki.length();
			i=0;
			continue;
		}
	}
	auto it = mp.find(skobki);
	if(it!=mp.end())
		return nol(mp[skobki]);
	else 
		return skobki;
}

string skobki(string vir, int poz, int kol, string s1, string s2)
{
	string temp;
	int length = vir.length(),j;
	bool flagz=false;
	for(int i=poz;i<length;i++)
	{
		if(vir[i]=='(') 
		{
			if(i>1 && ((vir[i-2]=='l' && (vir[i-1]=='n' || vir[i-1]=='g')) || (vir[i-1]>='0' && vir[i-1]<='9'))) 
			{
				j=i+1;
				while(j<length && vir[j]!=')')
				{
					if(vir[j]=='+' || vir[j]=='*' || vir[j]=='/' || vir[j]=='^' || (vir[j]=='l' && (vir[j+1]=='n' || vir[j+1]=='g' || vir[j+1]=='o')) || (vir[j]=='-' && vir[j+1]==' '))
					{
						vir=skobki(vir, i+1, kol+1, "(", ")");
						i=poz;
						length = vir.length();
						flagz=true;
						break;
					}
					j++;
					
				}
				if(flagz)
				{
					flagz=false;
					continue;
				}
				else
					{
						flagS++;
						continue;
					}
			}
			vir=skobki(vir, i+1, kol+1, "", "");
			i=poz;
			length = vir.length();
			continue;
		}
		if(vir[i]==')') 
		{
			if(flagS==0)
			{
				if(poz>1 && vir[poz-2]=='-')
					vir.replace(poz-2,i-poz+3,"-1 * " + vir.substr(poz-1,i-poz+2));
				else
					vir.replace(poz-1,i-poz+2,s1+resh(vir.substr(poz,i-poz))+s2);
				return vir;
			}
			else flagS--;
		}
	}
	length = vir.length();
	vir.replace(0,length,resh(vir));	
	return vir;
}

string del_skobki(string vir, int poz, int kol)
{
	int check;
	vir=skobki(vir,poz,kol,"","");
	check=vir.find('(');
	while(check>=0)
	{
		vir.replace(0,vir.length(),vir.substr(1,vir.length()-2));
		check=vir.find('(');
	}
	return vir;
}

int main(int n, char **arg)
{
	string inputline;
	int ravn,prob;
	getline(cin, inputline);
	ravn=inputline.find("=");
	prob=inputline.find(" ");
	if(ravn!=-1)
		while(prob!=-1)
		{
			
			mp[inputline.substr(0,ravn)]=stod(inputline.substr(ravn+1,prob-ravn-1));
			inputline.erase(0,prob+1);
			ravn=inputline.find("=");
			prob=inputline.find(" ");
			if(prob==-1)
				mp[inputline.substr(0,ravn)]=stod(inputline.substr(ravn+1,inputline.length()-ravn));	
		}
	string templine(arg[1]);
	if(templine[0]=='\"')
	{
		templine.erase(0);
		templine.erase(templine.length());
	}
	cout << del_skobki(templine,0,0) << endl;
}
