#include <iostream>
#include "tinyxml.h"
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <string>


using namespace std;

void Xml_Checker()
{
    XMLDocument doc;
    doc.LoadFile( "building.xml" );
    cout<<"File exist"<<endl;
    cout<< doc.ErrorID() <<endl;

}

void 
Xml_Reader()
{
  string name;
  XMLDocument doc;
  doc.LoadFile( "building.xml" );
  TiXmlElement* attributeApproachElement = doc.FirstChildElement();

  for(TiXmlElement* e = attributeApproachElement->FirstChildElement("poly"); e != NULL; e = e->NextSiblingElement("poly"))
  {
  name = e->Attribute("shape");
  cout<<"Reading complete, next round !"<<endl;
  cout<<name<<endl;
  }
}

int main()

{
	Xml_Checker();
	Xml_Reader();
	cout<<"HELLO"<<endl;
	return 0;
}