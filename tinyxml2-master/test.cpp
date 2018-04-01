#include <iostream>
#include "tinyxml2.h"
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <string>
#include <algorithm>

using namespace tinyxml2;
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
  XMLElement* attributeApproachElement = doc.FirstChildElement();

  const char *seperator = " ,";
  char *p;

  int building_no = 1;
  double Max_X_Array [214] = {};
  double Min_X_Array [214] = {};
  double Max_Y_Array [214] = {};
  double Min_Y_Array [214] = {};

  for(XMLElement* e = attributeApproachElement->FirstChildElement("poly"); e != NULL; e = e->NextSiblingElement("poly"))
  {
  
  int n = 1;
  name = e->Attribute("shape");
  cout<<"Reading complete, next round !"<<endl;
  p = strtok((char *)name.c_str(),seperator);
  static double X_axis[50];
  static double Y_axis[50];
  while(p)
  { 
//    cout<<"Number "<< n << " Element"<< endl;
//    cout<<p<<endl;
    
    if (n == 1)
    {
      for (int i =0; i < 50; i++)
      {
        X_axis[i] = atof(p);
      }
    }
    else if (n == 2)
    {
      for (int j =0; j < 50; j++)
      {
        Y_axis[j] = atof(p);
      } 
    }

    if ( n % 2 ==1 )
    {
      X_axis[n/2] = atof(p);
//      cout<<"X axis = "<< X_axis[n/2] << endl;
    }

    else
    { 
      Y_axis[n/2] = atof(p);
//      cout<<"Y axis = "<< Y_axis[n/2] << endl;
    }   

    n++;
    p = strtok(NULL, seperator);

  }
    // cout<<name<<endl;
    cout<<"Building Number = "<< building_no << endl;
    std::cout << "max X axis = " << *max_element( X_axis, X_axis + 50) << '\n';
    std::cout << "min X axis = " << *min_element( X_axis, X_axis + 50) << '\n';
    std::cout << "max Y axis = " << *max_element( Y_axis, Y_axis + 50) << '\n';
    std::cout << "min Y axis = " << *min_element( Y_axis, Y_axis + 50) << '\n';
    Max_X_Array [building_no-1] = *max_element( X_axis, X_axis + 50);
    Min_X_Array [building_no-1] = *min_element( X_axis, X_axis + 50);
    Max_Y_Array [building_no-1] = *max_element( Y_axis, Y_axis + 50);
    Min_Y_Array [building_no-1] = *min_element( Y_axis, Y_axis + 50);
    building_no++;
  }
  //cout<<" Max_X_Array [214] = {"<<  Max_X_Array [214] << endl;


  cout<< "Max_X_Array[214]= {";
  for (int i=0; i<214;i++)
  {
    cout<< Max_X_Array[i] << ",";
  }
  cout<< "}"<< endl;

  cout<< "Min_X_Array[214]= {";
  for (int i=0; i<214;i++)
  {
    cout<< Min_X_Array[i] << ",";
  }
  cout<< "}"<< endl;

  cout<< "Max_Y_Array[214]= {";
  for (int i=0; i<214;i++)
  {
    cout<< Max_Y_Array[i] << ",";
  }
  cout<< "}"<< endl;

  cout<< "Min_Y_Array[214]= {";
  for (int i=0; i<214;i++)
  {
    cout<< Min_Y_Array[i] << ",";
  }
  cout<< "}"<< endl;

}



int main()

{
	Xml_Checker();
	Xml_Reader();
	cout<<"Finish!"<<endl;
	return 0;
}