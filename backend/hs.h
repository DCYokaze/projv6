#ifndef _HS_H_
#define _HS_H_
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "commonLib.h"
#include "stdpatch.h"
using namespace patch;

#define FilesList "../FilesList"
#define FilePath "Files"
#define CONSTANT_TO_LESS_ERROR 10000

using namespace std;
struct attrValue  // attribute with value from feature selection
{
 public:
  string name;
  double val;  // value from weka
  bool operator<(const attrValue &rhs) const { return (val > rhs.val); }
  // int lengthExtractable;// If extracted and less than this value, it's mean
  // failed to extract feature.
};
struct attrFreq {
 public:
  string name;
  long freq;

  bool operator<(const attrFreq &rhs) const {
    return (name.compare(rhs.name) < 0);
  }
  bool operator==(const attrFreq &rhs) { return (name.compare(rhs.name) == 0); }
  bool operator!=(const attrFreq &rhs) {
    return !(name.compare(rhs.name) == 0);
  }
};
class HS {
 public:
  string initARFF_file;
  struct  // configuration
  {
    // public:
    string fileList;
    // int byteLength;
    // int shift;
    long step;
    int lengthExtractable;  // If extracted and less than this value, it's mean
                            // failed to extract feature.
  } config;
  // private:
 public:
  string fileNameToLongHex_old(string);
  string fileNameToLongHex(string);
  int *longHexToARFFValueHexNStrArrIdxMethod(int, string);
  int longHexToArray(int *, int, int, string);
  int getFileList();
  int genARFFFile(string, int, int, int);
  int initARFF(
      string outName);  //,int bytes, unsigned int min, unsigned int max);
  int updateARFF(string inName, string outName, int bytes, int shift, long min,
                 long arrLen);
  int *longHexToArrInterval(int bytes, int shift, string &longLine, long min,
                            long arrLen);
  int longHexToArrInterval_nged(int *arrIn, int bytes, int shift,
                                string fileName, long min, long arrLen);

  // private:
  std::string longToHex(long l, int bytes);
  string featureSelectionToFile(int bytes, int shift, int h_start = 0,
                                int h_doForNSegment = 0);
  std::vector<attrValue> wekaFeatureSelection(int method, string filePath);

  //*SPECIAL*//
  int filterSomeFiles();

  // Part 2
  // Additionally for create arff [after feature selection]
  string fsattrToarff(string fileInName, int attrNum, int shift);
  int basicCount(string attr, int shift, string longLine);  // private
  int basicCount2(std::vector<attrFreq> &vS, int shift, string longLine);
  int basicCount3(std::map<string, int> &mS, int shift, string longLine);

  // Part 3
  // Send file to weka
  double arffToWeka(string fileInName, int method);

  // Part EX01
  // Extract all files to HEX
  int extractAllFileToHEX();

  int exportObjbyteToPcd();

  // Linked list generation.
  // int fileToMemLong(int bytes,int shift);
  // int fileToMemString(int bytes,int shift);

  // for debug and checking purpose
  int isHEXAble();

  int nged2mapInitialize(int);
  int readFromNged(std::map<long, int> &, string, int);
  int readFromNgedArr(int *arr, string fileName, int ngram, long min,
                      long arrLen);
  int readFromNgedArrBool(bool *arrBool, string fileName, int ngram, long min,
                          long arrLen);

  int IGBinaryToFile(int, long, long, long);
  std::vector<attrValue> IGBinaryEval(int, long, long, double, bool **,
                                      double *);

  int TfidfToFile(int bytes, long inStart, long inEnd, long batchSize);
  std::vector<attrValue> TfidfEval(int, long, long, double, int **, double *,
                                   long *, int *);

  int readFromNgedsArr(int *, string, int, long, long, long &);
  // std::map<long,int> ngramMap[6400];//Compile time allocate mem
  // private:
  vector<string> vectorOfFile;
  int fsToArff(string, int, int);
  // std::vector< std::map<long,int> > vlData;//Over ALL
  // std::vector< std::map<std::string,int> > vsData;//Over ALL
};
#endif  //_HS_H_