#include <iostream>
#include <string>
#include <stdio.h>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <map>
#include <set>
#include "commonLib.h"
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
/*
find binary_raw/ > listdataset_objasm
g++ -o doasm doAsm.cpp asm.h asm.cpp
*/

int main(int argc, char *argv[])
{
  time_t start, end;
  time(&start);

  ASM a1;
  a1.config.fileList = "./listdataset_objasm";
  a1.config.step = 256; //not used
  a1.config.lengthExtractable = 3;
  a1.config.type = 1;
  a1.config.n = 1;
  a1.config.shift = 1;
  a1.config.simplify = 1;
  a1.getFileList();

  /*
  string s1 = "[ITS][ITS][ITS]push-mov-movl-[[call]]-movl-movl-[ITS]movl-[[call]]-movl-movl-movl-[[call]]-movl";

  cout<<a1.preprocForNgram(s1)<<endl;
  cout<<a1.preprocForBasicBlock(s1)<<endl;

  return 0;
*/
  int comm = -1; //strtol(input.c_str(),NULL,10);
  int batchSize = 65536;
  if (argc == 1) //print help
  {
    cout << "CHOOSE MODE" << endl;
    cout << "0 = countAllBasicBlock" << endl;

    cout << "20 = exportFastBasicBlock" << endl;
  }
  else if (argc == 3) //assign input to the variable, seperate by case
  {
    comm = strtol(argv[1], NULL, 10);
    batchSize = strtol(argv[2], NULL, 10);
  }
  else
  {
    cout << "just return 0" << endl;
    return 0;
  }
  cout << commonLib::exec("date");

  
  switch (comm)
  {
  case 0: //run Evaluate only 1 case
  {
    int found = a1.countAllBasicBlock();
    cout << found << endl;
  }
  break;

  case 20: //run Evaluate only 1 case
  {
    a1.countAllBasicBlock();
    a1.exportBasicBlockFastProcess();
  }
  break;

  default:
    cout << "Default case here." << endl;
  }
  cout << commonLib::exec("date");
  time(&end);
  double dif = difftime(end, start);
  printf("Elasped time is %.2lf seconds.", dif);
  cout<<"\n";
  return 0;
  //a1.exportObjasmToPcd();
  /*
  a1.countAllBasicBlock();
  return 0;
  for(int i=0;i<=3;i++)
  {
    a1.config.n=i+1;
    cout<<"n = "<<(i+1)<<"   last size is "<<a1.countAllngramPossFound()<<endl;
  }
  //*/
  return 0;
}
