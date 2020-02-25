#include "asm.h"
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
#include <math.h>
#include <iomanip>
using namespace patch;
#define FilesList "FilesList"
#define FilePath "Files"
#define FLOWCHANGE "[FLC]"
#define INTERSECTION "[ITS]" //something like flowchange but not.
const std::string MEMORY_FOR_JAVA = "-Xmx2048m";
// #define CUTOUT_IF_LESSTHAN  10
// #define CONSTANT_TO_LESS_ERROR 10000
using namespace std;
#define NUMCLASS0 3267
#define NUMCLASS1 3052

//g++ -std=gnu++11 -o doasm doAsm.cpp
/*
 will have 2 type
 normal non-simple (ns) - normal extraction.
 simple - To remove all profix
//*/

//set of opcode
std::map<std::string,long> map_sl;
/*
  n = 1   last size is 1365
  n = 2   last size is 165163
  n = 3   last size is 3939756
  n = 4   last size is 24934745
*/

std::vector<std::string> CleanLastDash( std::vector<std::string> sv )
{
  for(int j=0;j<sv.size();j++){
    if(sv[j][sv[j].length()-1] == '-')
      sv[j]=sv[j].substr(0,sv[j].length()-1);
  }
  return sv;
}

int ASM::countAllngramPossFound()// called before ...
{
  map_sl.clear();
  int n = config.n;
  int shift = config.shift;
  for(int i=0;i<vectorOfFile.size();i++){
    string sAsm = fileNameToLongAsmV2NG(vectorOfFile[i]);
    std::vector<std::string> vStr = commonLib::StringSplit(sAsm,"-");
    for(int i=0;(i+n-1)<vStr.size();i+=shift){
      string s = "";
      for(int j=0;j<n;j++){
        s+=vStr[i+j];
        s+="-";
      }
      // n_sset.insert(s);
      if(map_sl.find(s) == map_sl.end())//never have this key before
        map_sl[s] = 1;
      else
        map_sl[s]++;
    }
  }

  double cutOutIfLessThan = 4+(4*n);//(1*vectorOfFile.size())/100;//1 percent

  cout<<"got all size = "<<map_sl.size()<<endl;
  cout<<"cutOutIfLessThan = "<<cutOutIfLessThan<<endl;

  std::vector<int> v01;
  map<string, long>::iterator it;
  for(it=map_sl.begin();it!=map_sl.end();)
  {
    v01.push_back(it->second);
    // cout<<it->first<<endl;
    if(it->second < cutOutIfLessThan){
      map<string, long>::iterator itDelete=it++;
      // cout<<itDelete->second<<" "<<itDelete->first<<endl;
      map_sl.erase(itDelete);
    }
    else{
      it++;
    }
  }
  
  std::sort(v01.begin(),v01.end());
  std::reverse(v01.begin(),v01.end());
  for(int i=0;i<v01.size();i+=10000)
  {
    cout<<"v["<<i<<"]="<<v01[i]<<'\n';
  }
  v01.clear();
  cout<<"survive after cut some out "<<map_sl.size()<<endl;

  return map_sl.size();
}


int ASM::countAllBasicBlock()// called before write arff, Append BB to all of them.
{
  map_sl.clear();
  //first loop to count all
  //to calculate and filter some out in loop 2
  int countbb=0;
  for(int i=0;i<vectorOfFile.size();i++)
  // for(int i=0;i<10;i++)//test with only 100 files
  {
    //cout<<"file..."<<i<<endl;
    string fileName = vectorOfFile[i];//don't mind the type of file ATM
    string longLine = fileNameToLongAsmV2BB(fileName);
    // longLine = preprocForBasicBlock(longLine);
    if(longLine.length()<config.lengthExtractable)//ADDITION PROTECTION
      continue;
    vector<string> sv = StringSplit(longLine,FLOWCHANGE);
    sv=CleanLastDash(sv);
    //cout<<"bbNum = "<<countbb<<endl;
    for(int j=0;j<sv.size();j++)
    {
      if(sv[j].length()>100){//a magic number to filter some out.
        continue;
      }
      countbb++;
      // basicBlockALL_map.insert(sv[j]);
      if(map_sl.find(sv[j]) == map_sl.end())//never have this key before
        map_sl[sv[j]] = 1;
      else
        map_sl[sv[j]]++;
    }
  }
  
  //loop2 filter some out
  //Cut out like the [Detecting Internet Worms Using Data Mining Techniques
  //A magic number
  double cutOutIfLessThan = 5;//(1*vectorOfFile.size())/100;//1 percent
  // for (auto& kv : mapAll) 
  cout<<"got all size = "<<map_sl.size()<<endl;
  cout<<"cutOutIfLessThan = "<<cutOutIfLessThan<<endl;
  
  //to try test some value.
  
  std::vector<int> v01;
  map<string, long>::iterator it;
  for(it=map_sl.begin();it!=map_sl.end();)
  {
    v01.push_back(it->second);
    // cout<<it->first<<endl;
    if(it->second < cutOutIfLessThan){
      map<string, long>::iterator itDelete=it++;
      // cout<<itDelete->second<<" "<<itDelete->first<<endl;
      map_sl.erase(itDelete);
    }
    else{
      it++;
    }
  }
  
  std::sort(v01.begin(),v01.end());
  std::reverse(v01.begin(),v01.end());
  for(int i=0;i<v01.size();i+=10000)
  {
    cout<<"v["<<i<<"]="<<v01[i]<<'\n';
  }
  v01.clear();
  cout<<"bbNumAll = "<<countbb<<endl;
  cout<<"survive after cut some out "<<map_sl.size()<<endl;
  
  //Then continue to write ARFF function
  //May have some statistics shown.
  return 1;
}

//run after countAllBasicBlock
int ASM::exportBasicBlockFastProcess()
{
  std::map<std::string,long> map_forCopy;

  ofstream oFile;
  oFile.open("OutARFF/asm_basicblock_meta");
  if(oFile.is_open()){
    string buf = "";
    for(map<string, long>::iterator it=map_sl.begin();it!=map_sl.end();it++){
      buf += it->first;
      buf += "\n";
      map_forCopy[it->first] = 0;//init the map
    }
    oFile<<buf;
  }
  oFile.close();


  for(int i=0;i<vectorOfFile.size();i++)
  {
    std::map<std::string,long> map_local;
    map_local.insert(map_forCopy.begin(), map_forCopy.end());

    string fileName = vectorOfFile[i];//don't mind the type of file ATM
    string longLine = fileNameToLongAsmV2BB(fileName);
    if(longLine.length()<config.lengthExtractable)//ADDITION PROTECTION
      continue;
    vector<string> sv = StringSplit(longLine,FLOWCHANGE);
    sv=CleanLastDash(sv);
    for(int j=0;j<sv.size();j++)
    {
      if(sv[j].length()>100){//a magic number to filter some out.
        continue;
      }
      if(map_local.find(sv[j])!=map_local.end())
        map_local[sv[j]]++;
    }

    string fileNameOut = fileName+".pcd_bb.fx";
    oFile.open(fileNameOut.c_str());
    if(oFile.is_open()){
      string buf = "";
      for(map<string, long>::iterator it=map_local.begin();it!=map_local.end();it++){
        if(it->second > 0){
          buf += it->first;
          buf += '\t';
          buf += patch::to_string(it->second);
          buf += '\n';
        }
      }
      oFile<<buf;
    }
    oFile.close();
  }
  map_forCopy.clear();
}

int ASM::fastReadDataBB(int ** arr,std::vector <std::string> vect)//(std::map <string,int*> & m)
{
  for(int i=0;i<vectorOfFile.size();i++){
    if(i%1000 == 0)
      cout<<"fast read file "<<i<<endl;
    string fileName = vectorOfFile[i];
    string fileNameOut = fileName+".pcd_bb.fx";
    //cout<<"reading "<<fileNameOut<<endl;
    ifstream iFile;
    iFile.open(fileNameOut.c_str());
    if(iFile.is_open()){
      string line;
      while(getline(iFile,line)){
        /*if(line.find("Pluma")!=string::npos){
          cout<<line<<endl;
        }*/
        size_t tabPos = line.find('\t');
        if(tabPos != string::npos){
          string sOpcode = line.substr(0,tabPos);
          if(std::binary_search (vect.begin(), vect.end(), sOpcode) == false)
            continue;
          vector<string>::iterator it = find(vect.begin(),vect.end(),sOpcode);//vect.find(sOpcode);
          if(it != vect.end()){
            int pos = it - vect.begin();//distance(vec.begin(), it);
            string sValue = line.substr(tabPos+1);
            arr[i][pos]=(int)std::strtol(sValue.c_str(),NULL,10);
          }
        }
      }
    }
    iFile.close();
  }
  return 0;
}
int ASM::fastReadDataNG(int ** arr,std::vector <std::string> vect)//(std::map <string,int*> & m)
{
  for(int i=0;i<vectorOfFile.size();i++){
    if(i%1000 == 0)
      cout<<"fast read file "<<i<<endl;
    string fileName = vectorOfFile[i];
    string fileNameOut = fileName+".pcd_bb.f"+patch::to_string(config.n);
    //cout<<"reading "<<fileNameOut<<endl;
    ifstream iFile;
    iFile.open(fileNameOut.c_str());
    if(iFile.is_open()){
      string line;
      while(getline(iFile,line)){
        size_t tabPos = line.find('\t');
        if(tabPos != string::npos){
          string sOpcode = line.substr(0,tabPos);
          if(std::binary_search (vect.begin(), vect.end(), sOpcode) == false)
            continue;
          vector<string>::iterator it = find(vect.begin(),vect.end(),sOpcode);//vect.find(sOpcode);
          if(it != vect.end()){
            int pos = it - vect.begin();//distance(vec.begin(), it);
            string sValue = line.substr(tabPos+1);
            arr[i][pos]=(int)std::strtol(sValue.c_str(),NULL,10);
          }
        }
      }
    }
    iFile.close();
  }
  return 0;
}
//run after countAllngramPossFound
int ASM::exportNgramFastProcess()
{
  std::map<std::string,long> map_forCopy;
  int n = config.n;
  int shift = config.shift;
  ofstream oFile;
  oFile.open(("OutARFF/asm_n"+patch::to_string(n)+"gram_meta").c_str());
  if(oFile.is_open()){
    string buf = "";
    for(map<string, long>::iterator it=map_sl.begin();it!=map_sl.end();it++){
      buf += it->first;
      buf += "\n";
      map_forCopy[it->first] = 0;//init the map
    }
    oFile<<buf;
  }
  oFile.close();



  for(int i=0;i<vectorOfFile.size();i++){
    string fileName = vectorOfFile[i];
    std::map<std::string,long> map_local;
    map_local.insert(map_forCopy.begin(), map_forCopy.end());
    string sAsm = fileNameToLongAsmV2NG(vectorOfFile[i]);
    std::vector<std::string> vStr = commonLib::StringSplit(sAsm,"-");
    for(int i=0;(i+n-1)<vStr.size();i+=shift){
      string s = "";
      for(int j=0;j<n;j++){
        s+=vStr[i+j];
        s+="-";
      }
      // n_sset.insert(s);
      if(map_local.find(s) != map_local.end())//never have this key before
        map_local[s]++;
    }


    string fileNameOut = fileName+".pcd_bb.f"+patch::to_string(n);
    oFile.open(fileNameOut.c_str());
    if(oFile.is_open()){
      string buf = "";
      for(map<string, long>::iterator it=map_local.begin();it!=map_local.end();it++){
        if(it->second > 0){
          buf += it->first;
          buf += '\t';
          buf += patch::to_string(it->second);
          buf += '\n';
        }
      }
      oFile<<buf;
    }
    oFile.close();
  }
  map_forCopy.clear();
}



//to use them in calculate tf-idf
//called after countAllBasicBlock
int ASM::getSizeBB(string fileName)
{
  int size = 0;
  ifstream iFile;
  iFile.open((fileName+".pcd_bb.fx").c_str());
  if(iFile.is_open()){
    string line;
    getline(iFile,line);
    size_t tabPos = line.find('\t');
    if(tabPos != string::npos){
        string sValue = line.substr(tabPos+1);
        int iValue = (int)std::strtol(sValue.c_str(),NULL,10);
        size+=iValue;
    }
  }
  iFile.close();
  return size;
}
int ASM::getSizeNG(string fileName)
{
  int n = config.n;
  int shift = config.shift;
  int size = 0;
  ifstream iFile;
  fileName += ".pcd_bb.f";
  fileName += patch::to_string(n);
  iFile.open(fileName.c_str());
  if(iFile.is_open()){
    string line;
    getline(iFile,line);
    size_t tabPos = line.find('\t');
    if(tabPos != string::npos){
        string sValue = line.substr(tabPos+1);
        int iValue = (int)std::strtol(sValue.c_str(),NULL,10);
        size+=iValue;
    }
  }
  iFile.close();
  return size;
}
int ASM::TfidfToFileNG(long batchSize)
{
  std::setprecision(12);
  int dataSetSize = vectorOfFile.size();
  //vector to easier list the attr
  std::vector <string> svect;
  ifstream iFile;
  iFile.open(("OutARFF/asm_n"+patch::to_string(config.n)+"gram_meta").c_str());
  if(iFile.is_open()){
    string line;
    while(getline(iFile,line)){
      svect.push_back(line);
      map_sl[line]=0;
    }
  }
  iFile.close();
  int * docLen = new int[dataSetSize];
  for(int i=0;i<dataSetSize;i++){
    docLen[i] = getSizeNG(vectorOfFile[i])+1;
    /*
    if(docLength[i]==0)
      cout<<"ALERT ALL ZEROS "<<vectorOfFile[i]<<endl;*/
  }
  int iEnd = svect.size();
  std::vector<attrValue> newVect;
  int ** arr = new int*[dataSetSize];
  for(int i=0;i<dataSetSize;i++){
    arr[i] = new int[batchSize]();
  }
  for(long progress=0;progress < iEnd;progress+=batchSize){
    cout<<"progress = "<<progress<<endl;
    cout<<commonLib::exec("date");
    for(int i=0;i<dataSetSize;i++){
      std::fill(arr[i],arr[i] + batchSize, 0);
      //readFromNgedsArr(arr[i],vectorOfFile[i] , bytes, lStart, batchSize, FileLinePointing[i] );
    }
    std::vector <string> vect;
    for(int j=0;((j+progress) < iEnd) && (j<batchSize);j++){
      vect.push_back(svect[j+progress]);
      // cout<<svect[j+progress]<<endl;
      // cout<<(j+progress)<<endl;
    }
    int thisBatchSize = vect.size();

    fastReadDataNG(arr, vect);
    cout<<"whatM"<<endl;


    cout<<"start calculate column with word"<<endl;
    int * sumColWithTheWord = new int[thisBatchSize]();
    std::fill(sumColWithTheWord, sumColWithTheWord+thisBatchSize, 0);
    for(int i=0;i<dataSetSize;i++){
      for(int j=0;j<thisBatchSize;j++){
        if(arr[i][j] > 0){
          sumColWithTheWord[j]++;
        }
      }
    }

    cout<<"start calculate tfidf with word"<<endl;
    double * diffTfidfArr = new double[thisBatchSize];
    for(int j=0;j<thisBatchSize;j++){
      if(sumColWithTheWord[j] == 0){
        diffTfidfArr[j] = 0.0;
        continue;
      }
      double sumClass0 = 0.0;
      double sumClass1 = 0.0;
      for(int i=0;i<dataSetSize;i++){
        double tf = (double)arr[i][j]/docLen[i];
        double idf = log((double)dataSetSize/sumColWithTheWord[j]);
        double tfidf = tf*idf;
        if(i<NUMCLASS0)
          sumClass0 += tfidf;
        else
          sumClass1 += tfidf;
      }
      double meanClass0 = sumClass0/NUMCLASS0;
      double meanClass1 = sumClass1/NUMCLASS1;
      if(meanClass0>meanClass1)
        diffTfidfArr[j] = meanClass0-meanClass1;
      else
        diffTfidfArr[j] = meanClass1-meanClass0;
    }

    delete [] sumColWithTheWord;
    cout<<"start add attrValue"<<endl;

    for(int j=0;j<thisBatchSize;j++){
      attrValue a;
      a.name = vect[j];
      a.val = 100000*diffTfidfArr[j];//100000 just to make them bigger and easier when merge
      newVect.push_back(a);
    }
    delete [] diffTfidfArr;

    cout<<"start sort attrValue"<<endl;
    std::sort(newVect.begin(), newVect.end());
    if(newVect.size()>20002)
      newVect.erase(newVect.begin()+20000,newVect.end());
  }
  cout<<"start write file, a few minutes ^^.";
  ofstream oFile;
  // string sFileName = "OutARFF/tfidf_asm_n"+patch::to_string(config.n)+"gram.fsarff";
  string sFileName = "OutARFF/tfidf_asm_n"+patch::to_string(config.n)+"gram_b"+patch::to_string(batchSize)+".fsarff";
  oFile.open(sFileName.c_str());
  for(int i=0;i<newVect.size();i++){
    oFile<<newVect[i].name<<endl;
    oFile<<newVect[i].val<<endl;
  }
  oFile.close();



  /*
  for(long progress=0;progress < iEnd;progress+=batchSize){
    std::map <string,int*> m;
    for(int i=0;((i+progress) < iEnd) && (i<batchSize);i++){
      m[svect[i+progress]] = new int[dataSetSize];
      cout<<svect[i+progress]<<endl;
      cout<<(i+progress)<<endl;
    }

    for(int i=0;((i+progress) < iEnd) && (i<batchSize);i++){
      delete [] m[svect[i+progress]];
    }
  }*/
  for(int i=0;i<dataSetSize;i++){
    delete [] arr[i];
  }
  delete [] arr;
  delete [] docLen;
}
int ASM::TfidfToFileBB(long batchSize)
{
  std::setprecision(12);
  int dataSetSize = vectorOfFile.size();
  //vector to easier list the attr
  std::vector <string> svect;
  ifstream iFile;
  iFile.open("OutARFF/asm_basicblock_meta");
  if(iFile.is_open()){
    string line;
    while(getline(iFile,line)){
      svect.push_back(line);
      map_sl[line]=0;
    }
  }
  iFile.close();
  int * docLen = new int[dataSetSize];
  for(int i=0;i<dataSetSize;i++){
    docLen[i] = getSizeBB(vectorOfFile[i])+1;
    /*
    if(docLength[i]==0)
      cout<<"ALERT ALL ZEROS "<<vectorOfFile[i]<<endl;*/
  }
  int iEnd = svect.size();
  std::vector<attrValue> newVect;
  int ** arr = new int*[dataSetSize];
  for(int i=0;i<dataSetSize;i++){
    arr[i] = new int[batchSize]();
  }
  for(long progress=0;progress < iEnd;progress+=batchSize){
    cout<<"progress = "<<progress<<endl;
    cout<<commonLib::exec("date");
    for(int i=0;i<dataSetSize;i++){
      std::fill(arr[i],arr[i] + batchSize, 0);
      //readFromNgedsArr(arr[i],vectorOfFile[i] , bytes, lStart, batchSize, FileLinePointing[i] );
    }
    std::vector <string> vect;
    for(int j=0;((j+progress) < iEnd) && (j<batchSize);j++){
      vect.push_back(svect[j+progress]);
      // cout<<svect[j+progress]<<endl;
      // cout<<(j+progress)<<endl;
    }
    int thisBatchSize = vect.size();

    fastReadDataBB(arr, vect);
    cout<<"whatM"<<endl;


    cout<<"start calculate column with word"<<endl;
    int * sumColWithTheWord = new int[thisBatchSize]();
    std::fill(sumColWithTheWord, sumColWithTheWord+thisBatchSize, 0);
    for(int i=0;i<dataSetSize;i++){
      for(int j=0;j<thisBatchSize;j++){
        if(arr[i][j] > 0){
          sumColWithTheWord[j]++;
        }
      }
    }

    cout<<"start calculate tfidf with word"<<endl;
    double * diffTfidfArr = new double[thisBatchSize];
    for(int j=0;j<thisBatchSize;j++){
      if(sumColWithTheWord[j] == 0){
        diffTfidfArr[j] = 0.0;
        continue;
      }
      double sumClass0 = 0.0;
      double sumClass1 = 0.0;
      for(int i=0;i<dataSetSize;i++){
        double tf = (double)arr[i][j]/docLen[i];
        double idf = log((double)dataSetSize/sumColWithTheWord[j]);
        double tfidf = tf*idf;
        if(i<NUMCLASS0)
          sumClass0 += tfidf;
        else
          sumClass1 += tfidf;
      }
      double meanClass0 = sumClass0/NUMCLASS0;
      double meanClass1 = sumClass1/NUMCLASS1;
      if(meanClass0>meanClass1)
        diffTfidfArr[j] = meanClass0-meanClass1;
      else
        diffTfidfArr[j] = meanClass1-meanClass0;
    }

    delete [] sumColWithTheWord;
    cout<<"start add attrValue"<<endl;

    for(int j=0;j<thisBatchSize;j++){
      attrValue a;
      a.name = vect[j];
      a.val = 100000*diffTfidfArr[j];//100000 just to make them bigger and easier when merge
      newVect.push_back(a);
    }
    delete [] diffTfidfArr;

    std::sort(newVect.begin(), newVect.end());
    if(newVect.size()>20002)
      newVect.erase(newVect.begin()+20000,newVect.end());
  }

  cout<<"start write file, a few minutes ^^.";
  ofstream oFile;
  string sFileName = "OutARFF/tfidf_asm_basicblock.fsarff";
  oFile.open(sFileName.c_str());
  for(int i=0;i<newVect.size();i++){
    oFile<<newVect[i].name<<endl;
    oFile<<newVect[i].val<<endl;
  }
  oFile.close();



  /*
  for(long progress=0;progress < iEnd;progress+=batchSize){
    std::map <string,int*> m;
    for(int i=0;((i+progress) < iEnd) && (i<batchSize);i++){
      m[svect[i+progress]] = new int[dataSetSize];
      cout<<svect[i+progress]<<endl;
      cout<<(i+progress)<<endl;
    }

    for(int i=0;((i+progress) < iEnd) && (i<batchSize);i++){
      delete [] m[svect[i+progress]];
    }
  }*/
  for(int i=0;i<dataSetSize;i++){
    delete [] arr[i];
  }
  delete [] arr;
  delete [] docLen;
}
//for the top 20k
int ASM::writeFsarffBB()
{
  return 0;
}
bool sort_pred(const pair<std::string,long>& left, const pair<std::string,long>& right)
{
  if(left.second == right.second){
    string s_left = left.first;
    string s_right = right.first;
    int i_left = 0;
    int i_right = 0;
    for(int i=0;i<s_left.length();i++)
    {
      if(s_left.c_str()[i]=='-')
        i_left++;
    }
    for(int i=0;i<s_right.length();i++)
    {
      if(s_right.c_str()[i]=='-')
        i_right++;
    }
    return i_left < i_right; //prefer less lenght
  }
  return left.second > right.second; //prefer more freq
}

int ASM::writeARFFFileThreshFreq(int attrNum)//trim the attr into attrNum, Sorted before trim.
{
  std::vector<pair<std::string,long> > v_sortPair;
  for(map<string, long>::iterator it=map_sl.begin();it!=map_sl.end();it++){
    pair<std::string,long> p;
    p.first = it->first;
    p.second = it->second;
    if(p.second >= 20)
      v_sortPair.push_back(p);
  }
  std::sort(v_sortPair.begin(),v_sortPair.end(),sort_pred);
  /*
  for(int i=0;i<1000;i++){
    cout<<v_sortPair[i].first<<"  "<<v_sortPair[i].second<<endl;
  }
  return 0;
  */
  
  string s_attrNum = patch::to_string(attrNum);
  while(s_attrNum.length()<4)
    s_attrNum = "0"+s_attrNum;
  string fileOutPath="./OutARFF/fs_ASMBB_TreshFreq"+s_attrNum+".arff";
  ofstream oFile;
  oFile.open(fileOutPath.c_str());
  if(oFile.is_open())
  {
    //Initialization.
    std::map<std::string,long> basicBlockEachFile_map;
    for(int i=0;i<attrNum;i++){
      basicBlockEachFile_map[v_sortPair[i].first] = 0;
    }
    
    //start write files
    oFile<<"@relation "<<fileOutPath<<endl;
    for(map<string, long>::iterator it=basicBlockEachFile_map.begin();it!=basicBlockEachFile_map.end();it++){
      oFile<<"@attribute "<< it->first <<" numeric"<<endl;
    }
    oFile<<"@attribute isVirus {0,1}"<<endl<<endl;
    oFile<<"@data"<<endl<<endl;
    
    // basicBlockEachFile_map.insert(basicBlockALL_map.begin(),basicBlockALL_map.end());
    //loop all extractable files
    for(int i=0;i<vectorOfFile.size();i++)
    {
      for(map<string, long>::iterator it=basicBlockEachFile_map.begin();it!=basicBlockEachFile_map.end();it++)
        it->second=0;//reset them.
      string fileName = vectorOfFile[i];
      string longLine = fileNameToLongAsmV2BB(fileName);
      if(longLine.length()<config.lengthExtractable)//ADDITION PROTECTION
        continue;
      vector<string> sv = StringSplit(longLine,FLOWCHANGE);
      sv=CleanLastDash(sv);
      for(int j=0;j<sv.size();j++)
      {
        if(sv[j].length()>100){//a magic number to filter some out.
          continue;
        }
        map<string, long>::iterator itFind = basicBlockEachFile_map.find(sv[j]);
        if(itFind == basicBlockEachFile_map.end())
          continue;
        else
          itFind->second++;
      }
      //well then write to file here.
      string s_aLine = "";
      for(map<string, long>::iterator it=basicBlockEachFile_map.begin();it!=basicBlockEachFile_map.end();it++)
      {
        s_aLine += patch::to_string(it->second);
        s_aLine += ",";
      }      
      if(fileName.find("/benign/")!=std::string::npos)
        s_aLine+="0";
      else if(fileName.find("/virus/")!=std::string::npos)
        s_aLine+="1";
      oFile<<s_aLine<<endl;
    }
  }
  else
  {
    cout<<"ERRROR open file "<<fileOutPath;
    return 1;
  }
  oFile.close();
}

double ASM::arffToWeka(string fileInName,int classifier)
{
  double result=-1;
  std::string command = "java " + MEMORY_FOR_JAVA ;
#ifdef ON_DIGITALOCEAN
  command += " -cp /root/weka-3-7-13/weka.jar ";
#else
  command += " ";
#endif
  command += "weka.Run -no-scan ";
  // std::string command = "java -cp /root/weka-3-7-13/weka.jar weka.Run -no-scan ";
  switch(classifier)
  {
    case 1:command += "weka.classifiers.trees.J48 ";break;
    case 2:command += "weka.classifiers.trees.RandomForest -I 100 -K 0 -S 1 -num-slots 1 ";break;
    case 3:command += "weka.classifiers.functions.LibSVM -C 0.25 -M 2 ";break;
    case 4:command += "weka.classifiers.functions.MultilayerPerceptron -L 0.3 -M 0.2 -N 500 -V 0 -S 0 -E 20 -H a ";break;
    case 5:command += "weka.classifiers.rules.JRip -F 3 -N 2.0 -O 2 -S 1 ";break;
    case 6:command += "weka.classifiers.bayes.NaiveBayes ";break;
    case 7:command += "weka.classifiers.bayes.NaiveBayesMultinomial ";break;
    case 8:command += "weka.classifiers.lazy.KStar -B 10 -M a ";break;
    // default:break;
  }
  command += "-t ";
  // command += fileInName;
  command += "\""+fileInName+"\"";
  
  // cout<<command<<endl;
  string resWeka = commonLib::exec(command);
  // cout<<resWeka<<endl;
  string fileOutName = fileInName + "_c" + patch::to_string(classifier);
  ofstream oFile;
  oFile.open(fileOutName.c_str());
  oFile<<resWeka<<endl;
  oFile.close();
  
  std::istringstream f(resWeka);
  string line;
  while (std::getline(f, line)){
    if(line.find("Stratified cross-validation")!=std::string::npos){
      std::getline(f, line);
      std::getline(f, line);
      // cout<<line<<endl;
      std::vector<std::string> vStr = commonLib::StringSplit(line);
      result=strtod(vStr[4].c_str(),NULL);
      //should got it here.
      break;
    }
  }
  
  
  //java -cp /root/weka-3-7-13/weka.jar weka.core.WekaPackageManager
  // java weka.Run -no-scan weka.classifiers.lazy.KStar -B 10 -M a -t "C:\cygwin64\home\DCMote\_copy_oldDoc\v_proj\SP2\FileBackup\ARFF2\hex\lastPack_swap128.arff"
  return result;
}


int ASM::getFileList()
{
  //vector<string> vectorOfFile;
  if(vectorOfFile.size()>0)
  {
    cout<<"Clear vectorOfFile"<<endl;
    vectorOfFile.clear();
  }
  string line;
  ifstream myfile (config.fileList.c_str());
  if (myfile.is_open())
  {
    while ( getline (myfile,line) )
    {
      vectorOfFile.push_back(line);
      // cout << line << '\n';
    }
    myfile.close();
  }
  else 
    cout << "Unable to open file"; 
  return 0;
}

string ASM::lineToAsmSymbol(string line)
{
  if(line.find("file format")!=string::npos)
    return INTERSECTION;
  if(line.find("Disassembly")!=string::npos)
    return INTERSECTION;
  if(line.find(">:") != string::npos)
    return INTERSECTION;
  if(line.length()<30)//Too short. Maybe some [...]
    return "";
    
  int firstTab = line.find("\t");
  int secondTab = line.find("\t",firstTab+1);
  if(secondTab==string::npos)
    return "";
  string lineTmp01 = line.substr(secondTab+1);
  if(lineTmp01.rfind("#")!=string::npos){
    int lastSpace = lineTmp01.rfind("#")-3;
    int trimSpace = lastSpace-1;
    while(lineTmp01[trimSpace] == ' '){
      trimSpace--;
    }
    lineTmp01 = lineTmp01.substr(0,trimSpace);
  }
  if(lineTmp01.rfind("<")!=string::npos){
    int lastSpace = lineTmp01.rfind("<")-3;
    int trimSpace = lastSpace-1;
    while(lineTmp01[trimSpace] == ' '){
      trimSpace--;
    }
    lineTmp01 = lineTmp01.substr(0,trimSpace);
  }
  
  
  int lastSpace = lineTmp01.rfind(" ");
  if(lastSpace == string::npos){//heh? cannot find last space?
  }
  else {
    int trimSpace = lastSpace;
    while(lineTmp01[trimSpace] == ' ') {
      trimSpace--;
    }
    // while(lineTmp01[])
    lineTmp01 = lineTmp01.substr(0,trimSpace+1);
  }
  /*
  if( (filterFlowChange(lineTmp01)>0 && config.type) )
  {
    string s = "[["+lineTmp01+"]]-";
    return s;
  }
  else //*/
  if( (int)lineTmp01.find("nop") != (string::npos) )
    return "";
  else{
    if(config.simplify) { //To get only last word
      int lastSpacePos = lineTmp01.rfind(" ")+1;
      lineTmp01=lineTmp01.substr(lastSpacePos);
    }
    return lineTmp01+"-";
  }
}
string ASM::fileNameToLongAsmV2NG(string fileName)
{
  string fileNameASM = fileName+".pcd_ng";
  string sOut="";
  ifstream iFile;
  iFile.open(fileNameASM.c_str());
  if(iFile.is_open()){
    getline(iFile,sOut);
  }
  iFile.close();
  return sOut;
}
string ASM::fileNameToLongAsmV2BB(string fileName)
{
  string fileNameASM = fileName+".pcd_bb";
  string sOut="";
  ifstream iFile;
  iFile.open(fileNameASM.c_str());
  if(iFile.is_open()){
    getline(iFile,sOut);
  }
  iFile.close();
  return sOut;
}

//condition in flow change.
int ASM::filterFlowChange(string sIn)
{
  vector<string> sv = StringSplit(sIn);
  for(int i=0;i<sv.size();i++)
  {
    if(sv[i].find("call")!=string::npos)
      return 1;
    if(sv[i].find("ret")!=string::npos)
      return 1;
    if(sv[i].find("leave")!=string::npos)
      return 1;
    if(sv[i].find("lj")!=string::npos)//ljmp
      return 1;
    if(sv[i][0]=='j')//some kind of jump
      return 1;
  }
  return 0;
}

std::vector<std::string> ASM::StringSplit(const std::string &source, const char *delimiter, bool keepEmpty)
{
  std::vector<std::string> results;
  size_t prev = 0;
  size_t next = 0;
  while ((next = source.find_first_of(delimiter, prev)) != std::string::npos)
  {
    if (keepEmpty || (next - prev != 0))
    {
      results.push_back(source.substr(prev, next - prev));
    }
    prev = next + 1;
  }
  if (prev < source.size())
  {
    results.push_back(source.substr(prev));
  }
  return results;
}
/*
int ASM::isASMAble()
{
  for(int i=0;i<vectorOfFile.size();i++)
  // for(int i=0;i<10;i++)//test with only 100 files
  {
    // cout<<"file..."<<i<<endl;
    string fileName = vectorOfFile[i];//don't mind the type of file ATM
    string longLine = fileNameToLongAsm(fileName);
    if(longLine.length()<config.lengthExtractable)//ADDITION PROTECTION
    {
      cout<<fileName<<endl;
    }
  }
  return 0;
}
*/
int ASM::exportObjasmToPcd() //export all of them
{
  //int i=0;
  ifstream iFile;
  iFile.open("./listdataset_objasm");
  if (iFile.is_open()){
    string fileNameObjasm;
    while (std::getline(iFile, fileNameObjasm)){
      cout<<"exportObjasmToPcd "+fileNameObjasm<<endl;

      ifstream iFileObjasm;
      iFileObjasm.open(fileNameObjasm.c_str());
      //string longLine = "";//i++;
      string longLine_ng = "";
      string longLine_bb = "";
      if(iFileObjasm.is_open())
      {
        string line;
        while (std::getline(iFileObjasm, line)){
          // cout<<line<<endl;
          // cout<<"     "<<lineToAsmSymbol(line)<<endl;
          string incSymbol = lineToAsmSymbol(line);
          if(incSymbol.compare(INTERSECTION) == 0){
            //longLine_ng += "";
            longLine_bb += FLOWCHANGE;
          }
          else if(filterFlowChange(incSymbol)>0){
            longLine_ng += incSymbol;
            longLine_bb += FLOWCHANGE;
          }
          else{
            longLine_ng += incSymbol;
            longLine_bb += incSymbol;
          }
          //longLine+=incSymbol;
        }
        if(longLine_ng.length()<config.lengthExtractable || 
           longLine_bb.length()<config.lengthExtractable){
          cout<<("something wrong with "+fileNameObjasm)<<endl;
        }
      }

      string fileOutPathName = fileNameObjasm + ".pcd";
      ofstream oFile;
      /*
      If want to write the longLine, use this, will get a useless file .pcd
      ofstream oFile;
      string fileOutPathName = fileNameObjasm + ".pcd";
      oFile.open(fileOutPathName.c_str());
      if(oFile.is_open()){
        oFile<<longLine<<endl;
      }
      else{cout<<("something wrong with "+fileOutPathName)<<endl;}
      oFile.close();
      */

      //write 2 more case
      string fileOutPathNameNG = fileOutPathName+"_ng";
      oFile.open(fileOutPathNameNG.c_str());
      if(oFile.is_open()){
        oFile<<(longLine_ng)<<endl;
      }
      else{cout<<("something wrong with "+fileOutPathNameNG)<<endl;}
      oFile.close();

      string fileOutPathNameBB = fileOutPathName+"_bb";
      oFile.open(fileOutPathNameBB.c_str());
      if(oFile.is_open()){
        oFile<<(longLine_bb)<<endl;
      }
      else{cout<<("something wrong with "+fileOutPathNameBB)<<endl;}
      oFile.close();
    }
  }
  iFile.close();
  return 0;
}

int ASM::fsToArff(string fsName, int attrNum, int n)
{
  cout<<"start fsToArff"<<endl;
  cout<<fsName<<" - "<<attrNum<<" - "<<n<<endl;
  string buf = "@relation "+fsName+".arff";
  buf+='\n';
  std::vector <string> svect;
  ifstream iFile;
  iFile.open(fsName.c_str());
  if(iFile.is_open()){
    string line;
    int aNum = 0;
    while(getline(iFile,line) && (aNum < attrNum)){
      // sset.insert(line);
      svect.push_back(line);
      aNum++;
      buf+="@attribute \"";
      buf+=line;
      buf+="\" numeric";
      buf+='\n';
      getline(iFile,line);//flush 1 out.
    }
  }
  else{
    cout<<"failed open file "<<fsName<<endl;
    return 1;
  }
  iFile.close();
  std::sort(svect.begin(),svect.end());
  buf+="@attribute isVirus {0,1}";
  buf+='\n';
  buf+="@data";
  buf+='\n';

  if(svect.size() != attrNum){
    cout<<"(svect.size() != attrNum)"<<endl;
    return 1;
  }
  int vSize = attrNum;
  int * v = new int[vSize];
  iFile.open(config.fileList.c_str());
  if(iFile.is_open()){
    string thoseFileName;
    int i=0;
    while(getline(iFile,thoseFileName)){
      i++;
      if(i%1000==0)
        cout<<i<<endl;
      ifstream theFile;
      thoseFileName+=".pcd_bb.f";//;cyg_[.exe_objasm.pcd_bb
      if(n>0)
        thoseFileName+=patch::to_string(n);
      else
        thoseFileName+="x";
      theFile.open(thoseFileName.c_str());
      //then read and count
      if(theFile.is_open()){
        std::fill(v,v + vSize,0);
        string line;
        while(getline(theFile,line)){
          size_t tabPos = line.find('\t');
          if(tabPos == string::npos)
            break;
          string sKey= line.substr(0,tabPos);
          if(std::binary_search (svect.begin(), svect.end(), sKey) == false)
            continue;
          vector<string>::iterator it = find(svect.begin(),svect.end(),sKey);//vect.find(sOpcode);
          if(it != svect.end()){
            int pos = it - svect.begin();//distance(vec.begin(), it);
            string sValue = line.substr(tabPos+1);
            int iValue = (int)std::strtol(sValue.c_str(),NULL,10);
            v[pos] = iValue;
          }
        }
      }
      else{
        cout<<"failed open file "<<thoseFileName<<endl;
      }
      theFile.close();
      for(int i=0;i<vSize;i++){
        buf+=patch::to_string(v[i]);
        buf+=",";
      }
      if(thoseFileName.find("/benign/")!=std::string::npos)
        buf+="0";
      else if(thoseFileName.find("/virus/")!=std::string::npos)
        buf+="1";
      buf+='\n';
    }
  }
  else{
    cout<<"failed open file "<<"listdataset_objasm"<<endl;
  }
  iFile.close();

  string fileOutName = fsName+patch::to_string(attrNum)+".arff";
  ofstream oFile;
  oFile.open(fileOutName.c_str());
  if(oFile.is_open()){
    oFile<<buf;
  }
  else{
    cout<<"problem open out file"<<endl;
  }
  oFile.close();
  return 0;
}