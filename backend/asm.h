#ifndef _ASM_H_
#define _ASM_H_
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

struct attrValue // attribute with value from feature selection
{
public:
  std::string name;
  double val; //value from weka
  bool operator<(const attrValue &rhs) const
  {
    return (val > rhs.val);
  }
  //int lengthExtractable;// If extracted and less than this value, it's mean failed to extract feature.
};
class ASM
{
public:
  struct // configuration
  {
    //public:
    std::string fileList;
    // int byteLength;
    // int shift;
    long step;             //this used in attr selection.
    int lengthExtractable; // If extracted and less than this value, it's mean failed to extract feature.
    //more for ASM?
    int type;  //0=basic block (ignore step), 1=n-gram
    int n;     //from n-gram
    int shift; //this must be less or equal n
    int simplify;
  } config;

public:
  std::string fileNameToLongAsmV2NG(std::string);
  std::string fileNameToLongAsmV2BB(std::string);
  std::string lineToAsmSymbol(std::string);
  int filterFlowChange(std::string);
  int getFileList();
  // std::vector<std::string> inline StringSplit(const std::string &, const char *, bool );
  std::vector<std::string> inline StringSplit(const std::string &source, const char *delimiter = " ", bool keepEmpty = false);
  //int genARFFFileAsm(string ,int );
  //std::vector<std::string> CleanLastDash(std::vector<std::string>);
  // private:
  std::vector<std::string> vectorOfFile;

  int countAllBasicBlock();
  int writeARFFFileThreshFreq(int);
  int writeARFFFileBaseLine01();
  //double arffToWeka(std::string, int);

  //int isASMAble();

  //ASMn

  int countAllngramPossFound();

  int exportObjasmToPcd();

  int getSizeBB(std::string);
  int getSizeNG(std::string);
  //int TfidfToFileBB(long );
  //int TfidfToFileNG(long );
  //int writeFsarffBB();
  int exportBasicBlockFastProcess();
  int fastReadDataBB(int **, std::vector<std::string>);
  int exportNgramFastProcess();
  int fastReadDataNG(int **, std::vector<std::string>);

  int fsToArff(std::string, int, int);
};

/*
 will have 2 type
 normal non-simple (ns) - normal extraction.
 simple - To remove all profix
//*/
// std::set<std::string> sset;

#endif //_ASM_H_