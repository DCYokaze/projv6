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
    cout << "1 = countAllngramPossFound" << endl;
    cout << "2 = countAllngramPossFound" << endl;
    cout << "3 = countAllngramPossFound" << endl;
    cout << "4 = countAllngramPossFound" << endl;

    cout << "10 = TFIDFBasicBlock" << endl;
    cout << "11 = TFIDFNG1" << endl;
    cout << "12 = TFIDFNG2" << endl;
    cout << "13 = TFIDFNG3" << endl;
    cout << "14 = TFIDFNG4" << endl;

    cout << "20 = exportFastBasicBlock" << endl;
    cout << "21 = exportFastNG1" << endl;
    cout << "22 = exportFastNG2" << endl;
    cout << "23 = exportFastNG3" << endl;
    cout << "24 = exportFastNG4" << endl;
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
  case 1: //run Evaluate only 1 case
  {
    a1.config.n = 1;
    int found = a1.countAllngramPossFound();
    cout << found << endl;
  }
  break;
  case 2: //run Evaluate only 1 case
  {
    a1.config.n = 2;
    int found = a1.countAllngramPossFound();
    cout << found << endl;
  }
  break;
  case 3: //run Evaluate only 1 case
  {
    a1.config.n = 3;
    int found = a1.countAllngramPossFound();
    cout << found << endl;
  }
  break;
  case 4: //run Evaluate only 1 case
  {
    a1.config.n = 4;
    int found = a1.countAllngramPossFound();
    cout << found << endl;
  }
  break;
    /*
    case 10://run Evaluate only 1 case
    {
      int found = a1.TfidfToFileBB(batchSize);
    }break;
    case 11://run Evaluate only 1 case
    {
      a1.config.n=1;
      int found = a1.TfidfToFileNG(batchSize);
    }break;
    case 12://run Evaluate only 1 case
    {
      a1.config.n=2;
      int found = a1.TfidfToFileNG(batchSize);
    }break;
    case 13://run Evaluate only 1 case
    {
      a1.config.n=3;
      int found = a1.TfidfToFileNG(batchSize);
    }break;
    case 14://run Evaluate only 1 case
    {
      a1.config.n=4;
      int found = a1.TfidfToFileNG(batchSize);
    }break;
*/
  case 20: //run Evaluate only 1 case
  {
    a1.countAllBasicBlock();
    a1.exportBasicBlockFastProcess();
  }
  break;
  case 21: //run Evaluate only 1 case
  {
    a1.config.n = 1;
    a1.countAllngramPossFound();
    a1.exportNgramFastProcess();
  }
  break;
  case 22: //run Evaluate only 1 case
  {
    a1.config.n = 2;
    a1.countAllngramPossFound();
    a1.exportNgramFastProcess();
  }
  break;
  case 23: //run Evaluate only 1 case
  {
    a1.config.n = 3;
    a1.countAllngramPossFound();
    a1.exportNgramFastProcess();
  }
  break;
  case 24: //run Evaluate only 1 case
  {
    a1.config.n = 4;
    a1.countAllngramPossFound();
    a1.exportNgramFastProcess();
  }
  break;

  case 90:
  {
    a1.fsToArff("OutARFF/tfidf_asm_basicblock.fsarff", 8192, 0);
  }
  break;
  case 91:
  {
    a1.fsToArff("OutARFF/tfidf_asm_n1gram_b1016.fsarff", 1016, 1);
  }
  break;
  case 92:
  {
    a1.fsToArff("OutARFF/tfidf_asm_n2gram_b2048.fsarff", 8192, 2);
  }
  break;
  case 93:
  {
    a1.fsToArff("OutARFF/tfidf_asm_n3gram_b2048.fsarff", 8192, 3);
  }
  break;
  case 94:
  {
    a1.fsToArff("OutARFF/tfidf_asm_n4gram_b2048.fsarff", 8192, 4);
  }
  break;

  default:
    cout << "what?" << endl;
  }
  cout << commonLib::exec("date");
  time(&end);
  double dif = difftime(end, start);
  printf("Elasped time is %.2lf seconds.", dif);
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
