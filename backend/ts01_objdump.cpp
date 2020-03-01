#include <iostream>
#include <string>
#include <stdio.h>
#include <sstream>
#include <algorithm>
#include "commonLib.h"
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <map>
#include <set>
#include "stdpatch.h"
#include "asm.h"
#include <time.h>
using namespace patch;
using namespace std;

//g++ -std=gnu++11 -o doasm doAsm.cpp
/*
 will have 2 type
 normal non-simple (ns) - normal extraction.
 simple - To remove all profix
//*/
int main(int argc,char *argv[])
{
  time_t start,end;
  time (&start);
  

  ifstream iFile;

  //objdump -s for hex
  time (&start);
  iFile.open("listdataset_raw");
  if(iFile.is_open()){
    string fileName;
    while(getline(iFile,fileName)){
      // cout<<line<<endl;
      string command = "objdump -s "+fileName;
      string objdumpCommRes = commonLib::exec(command);
      //cout<<objdumpCommRes<<endl;
    }
  }
  else
  {
    cout<<"cannot open file"<<endl;
  }
  iFile.close();
  time (&end);
  double dif = difftime (end,start);
  printf ("Elasped time is %.2lf seconds.", dif );

  //objdump -d for asm
  time (&start);
  iFile.open("listdataset_raw");
  if(iFile.is_open()){
    string fileName;
    while(getline(iFile,fileName)){
      // cout<<line<<endl;
      string command = "objdump -D "+fileName;
      string objdumpCommRes = commonLib::exec(command);
      //cout<<objdumpCommRes<<endl;
    }
  }
  else
  {
    cout<<"cannot open file"<<endl;
  }
  iFile.close();
  time (&end);
  double dif = difftime (end,start);
  printf ("Elasped time is %.2lf seconds.", dif );

  return 0;
}

