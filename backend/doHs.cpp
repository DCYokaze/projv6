#include <iostream>
#include <string>
#include <stdio.h>
#include <sstream>
#include <algorithm>
#include "commonLib.h"
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <iomanip>
#include "hs.h"
#include "stdpatch.h"
#include <ctime>
#include <map>
#include <set>
#include <math.h>
// #include "getMemorySize.c"

// using namespace patch;
using namespace std;
/*
struct toSortDoubleRange
{
  int idx;
  double val;

    //MyStruct(int k, const std::string& s) : key(k), stringValue(s) {}
  bool operator < (const toSortDoubleRange& rhs) const
  {
    return (val < rhs.val);
  }
};
//*/

/*
compile command
g++ -c HS.cpp; g++ -o dohs doHS.cpp HS.o
/*
To fileList command
find ../rawFile/0/*X/*.exe > ../fileList_r
find ../rawFile/1/Vir* >> ../fileList_r
*/
//*/
int a1()
{
  return 0;
};

/*
NOTE STEP
0.extractAllFileToHEX got HEX (CASE 1)
1.featureSelectionToFile got fsarff
2.fsattrToarff got arff, and ready to go to weka.
*/
int main(int argc,char *argv[])
{
  /*
  double a = 1;
  a-1/2;
  cout<<a<<endl;return 0;
*/

  time_t start,end;
  time (&start);
  int comm = 0;//strtol(input.c_str(),NULL,10);
  
  if(argc==1)//print help
  {
    cout<<"CHOOSE MODE"<<endl;
    cout<<"1=featureSelectionToFile"<<endl;
    cout<<"105=EVALUATE only 1 case [expected 5 input - n shift fs attrNum classifier]"<<endl;
    cout<<"106=EVALUATE only 1 classifier [expected 2 input - n classifier]"<<endl;
    cout<<"50 = fsattr to arff n=1"<<endl;
    cout<<"51 = fsattr to arff n=2"<<endl;
    cout<<"52 = fsattr to arff n=3"<<endl;
  }
  else //assign input to the variable, seperate by case
  {
    comm = strtol(argv[1],NULL,10);
  }
  /*
  input
  must have
  n
  classifier
  shift

  fs
  attrNum

  n2,shift1,fs4,attrNum2048,class3

  n2,shift1,fs4,attrNum256,class4

  */
    
  
  int classifier = 0;//strtol(argv[2],NULL,10);
  // return 0;
  
  // int comm = 62;//get from main here.
  clock_t begin_time = clock();  
  
  // cout << float( clock () - begin_time ) / CLOCKS_PER_SEC<<endl;
  HS aTest;
  // aTest.config.fileList = "../fileList_r";
  // aTest.config.fileList = "../fileList_sync";
  aTest.config.fileList = "./listdataset_objbyte";
  aTest.config.step=256;
  aTest.config.lengthExtractable = 1;
  
  // return 0;
  aTest.getFileList();
  cout<<aTest.vectorOfFile.size()<<endl;

  //just want a last process...
  aTest.fsToArff("fsarff/byte/tfidf/tfidf_4_merged.fsarff",8192,4);
  return 0;

//*/

  // aTest.IGBinaryToFile(1,0,256,256);
  // aTest.IGBinaryToFile(3,0,256*256*256,256*256);
  
  if(argc != 5)
    return 0;
  
  int bytes = strtol(argv[1],NULL,10);
  long startB = strtol(argv[2],NULL,10);
  long endB = strtol(argv[3],NULL,10);
  long batchSize = strtol(argv[4],NULL,10);
  
  
  
  cout<<commonLib::exec("date");

  aTest.TfidfToFile(bytes, startB, endB, batchSize);
  // aTest.IGBinaryToFile(bytes, startB, endB, batchSize);
  
  
  /*
  aTest.config.step=256;
  aTest.featureSelectionToFile(1,1);//*/
  cout<<commonLib::exec("date");
  time (&end);
  double dif = difftime (end,start);
  printf ("Elasped time is %.2lf seconds.", dif );  
  int i;
  //cin>>i;
  return 0;
  switch(comm)
  {
    
    case 105://run Evaluate only 1 case
    {
      // cout<<"expected 5 input - n shift fs attrNum classifier"<<endl;//n,classifier,shift,fs,attrNum
      // cout<<"note that fs can be 0 to loop fs = 1-4"<<endl;
      // cout<<"note that attrNum can be 0 to loop attrNum = 32-4096"<<endl;
      int iN = strtol(argv[2],NULL,10);
      int iShift = strtol(argv[3],NULL,10);
      int iFs = strtol(argv[4],NULL,10);
      int iAttrNum = strtol(argv[5],NULL,10);
      int iClassifier = strtol(argv[6],NULL,10);
      string num = patch::to_string(iAttrNum);
      while(num.length()<4)
        num = "0"+num;
      string  arffOut = "./OutARFF/arffFiles";
              arffOut += "/FS_Top50000_n"+patch::to_string(iN)+"_s"+patch::to_string(iShift)+"_m"+patch::to_string(iFs);
              arffOut += "_"+num+".arff";
      {
        double d = aTest.arffToWeka(arffOut,iClassifier);
        cout<<" n = "<<iN
            <<" shift = "<<iShift
            <<" fs = "<<iFs
            <<" attrNum = "<<iAttrNum
            <<" classifier = "<<iClassifier;
        cout<<" d = "<<d<<endl;
        cout<<commonLib::exec("date");
      }
    }
    break;
    
    case 106://run Evaluate one classifier at a time
    {
      // cout<<"expected 2 input - n classifier"<<endl;//n,classifier,shift,fs,attrNum
      // cout<<"note that fs can be 0 to loop fs = 1-4"<<endl;
      // cout<<"note that attrNum can be 0 to loop attrNum = 32-4096"<<endl;
      int iN=0;
      int iClassifier=0;
      iN = strtol(argv[2],NULL,10);
      iClassifier = strtol(argv[3],NULL,10);
      // cin>>iN;
      // cin>>iClassifier;
      
      switch(iN)
      {
        case 1:
        {
          for(int iShift = 1;iShift<=1;iShift++)
          {
            for(int iAttrNum = 32;iAttrNum<=256;iAttrNum*=2)
            {
              for(int iFs=1;iFs<=4;iFs++)//fsmethod
              {
                string num = patch::to_string(iAttrNum);
                while(num.length()<4)
                  num = "0"+num;
                string  arffOut = "./OutARFF/arffFiles";
                        arffOut += "/FS_Top50000_n"+patch::to_string(iN)+"_s"+patch::to_string(iShift)+"_m"+patch::to_string(iFs);
                        arffOut += "_"+num+".arff";
                {
                  double d = aTest.arffToWeka(arffOut,iClassifier);
                  cout<<" n = "<<iN
                      <<" shift = "<<iShift
                      <<" fs = "<<iFs
                      <<" attrNum = "<<iAttrNum
                      <<" classifier = "<<iClassifier;
                  cout<<" d = "<<d<<endl;
                  cout<<commonLib::exec("date");
                }
              }
            }
          }
        }break;
        case 2:
        {
          for(int iShift = 1;iShift<=2;iShift++)
          {
            for(int iAttrNum = 32;iAttrNum<=4096;iAttrNum*=2)
            {
              for(int iFs=1;iFs<=4;iFs++)//fsmethod
              {
                string num = patch::to_string(iAttrNum);
                while(num.length()<4)
                  num = "0"+num;
                string  arffOut = "./OutARFF/arffFiles";
                        arffOut += "/FS_Top50000_n"+patch::to_string(iN)+"_s"+patch::to_string(iShift)+"_m"+patch::to_string(iFs);
                        arffOut += "_"+num+".arff";
                {
                  double d = aTest.arffToWeka(arffOut,iClassifier);
                  cout<<" n = "<<iN
                      <<" shift = "<<iShift
                      <<" fs = "<<iFs
                      <<" attrNum = "<<iAttrNum
                      <<" classifier = "<<iClassifier;
                  cout<<" d = "<<d<<endl;
                  cout<<commonLib::exec("date");
                }
              }
            }
          }
        }break;
        case 3:
        {
          for(int iShift = 1;iShift<=3;iShift++)
          {
            for(int iAttrNum = 32;iAttrNum<=4096;iAttrNum*=2)
            {
              for(int iFs=1;iFs<=4;iFs++)//fsmethod
              {
                string num = patch::to_string(iAttrNum);
                while(num.length()<4)
                  num = "0"+num;
                string  arffOut = "./OutARFF/arffFiles";
                        arffOut += "/FS_Top50000_n"+patch::to_string(iN)+"_s"+patch::to_string(iShift)+"_m"+patch::to_string(iFs);
                        arffOut += "_"+num+".arff";
                {
                  double d = aTest.arffToWeka(arffOut,iClassifier);
                  cout<<" n = "<<iN
                      <<" shift = "<<iShift
                      <<" fs = "<<iFs
                      <<" attrNum = "<<iAttrNum
                      <<" classifier = "<<iClassifier;
                  cout<<" d = "<<d<<endl;
                  cout<<commonLib::exec("date");
                }
              }
            }
          }
        }break;
        default:cout<<"wrong iN"<<endl;
      }
    }
    break;
    
    case 107://run Evaluate all case ?
    {
      
    }
    break;
    
    case 61://run Evaluate n = 1
    {//case for n = 1
      // for(int classifier = 1;classifier<=8;classifier++)
      {
        for(int attrNum = 32;attrNum<=256;attrNum*=2)
        {
          for(int fs=0;fs<4;fs++)//fsmethod
          {
            // string fsattr = "./OutARFF/FS_Top50000_n1_s1_m"+ patch::to_string(fs+1) +".fsattr";
            string num = patch::to_string(attrNum);
            while(num.length()<4)
              num = "0"+num;
            string arffOut = "./resultARFF/arff_n1/FS_Top50000_n1_s1_m"+patch::to_string(fs+1);
            arffOut += "_"+num+".arff";
            // aTest.fsattrToarff(fsattr,attrNum,1);
  // cout << float( clock () - begin_time ) / CLOCKS_PER_SEC<<endl;begin_time = clock();

            // for(int classifier = 1;classifier<=7;classifier++)
            {
              double d = aTest.arffToWeka(arffOut,classifier);
              cout<<" n = "<<1
                  <<" shift = "<<1
                  <<" fs = "<<(fs+1)
                  <<" attrNum = "<<attrNum
                  <<" classifier = "<<classifier;
              cout<<" d = "<<d<<endl;
    cout<<commonLib::exec("date");
  // cout << float( clock () - begin_time ) / CLOCKS_PER_SEC<<endl;begin_time = clock();
            }
          }
        }
      }
    }
    break;    

    
    case 62://run Evaluate n = 2
    {//case for n = 2
      // for(int classifier = 1;classifier<=8;classifier++)
      for(int shift = 1;shift<=2;shift++)
      {
        for(int attrNum = 32;attrNum<=4096;attrNum*=2)
        {
          for(int fs=0;fs<4;fs++)//fsmethod
          {
            // string fsattr = "./OutARFF/FS_Top50000_n2_s"+patch::to_string(shift)+"_m"+ patch::to_string(fs+1) +".fsattr";
          string num = patch::to_string(attrNum);
          while(num.length()<4)
            num = "0"+num;
          string arffOut = "./resultARFF/arff_n2/FS_Top50000_n2_s"+patch::to_string(shift)+"_m"+patch::to_string(fs+1);
          arffOut += "_"+num+".arff";
// cout << float( clock () - begin_time ) / CLOCKS_PER_SEC<<endl;begin_time = clock();

            // for(int classifier = 1;classifier<=7;classifier++)
            {
              double d = aTest.arffToWeka(arffOut,classifier);
              cout<<" n = "<<2
                  <<" shift = "<<shift
                  <<" fs = "<<(fs+1)
                  <<" attrNum = "<<attrNum
                  <<" classifier = "<<classifier;
              cout<<" d = "<<d<<endl;
  cout<<commonLib::exec("date");
// cout << float( clock () - begin_time ) / CLOCKS_PER_SEC<<endl;begin_time = clock();
            }
          }
        }
      }
    }
    break;

    
    case 63://run Evaluate More of classifier 3
    {
      int attrNum = 4096;
      int shift = 1;
      // int fs = 
      for(int fs=0;fs<4;fs++)//fsmethod
      {
        string num = patch::to_string(attrNum);
        while(num.length()<4)
          num = "0"+num;
        string arffOut = "./resultARFF/arff_n2/FS_Top50000_n2_s"+patch::to_string(shift)+"_m"+patch::to_string(fs+1);
        arffOut += "_"+num+".arff";
// cout << float( clock () - begin_time ) / CLOCKS_PER_SEC<<endl;begin_time = clock();

          // for(int classifier = 1;classifier<=7;classifier++)
          {
            double d = aTest.arffToWeka(arffOut,classifier);
            cout<<" n = "<<2
                <<" shift = "<<shift
                <<" fs = "<<(fs+1)
                <<" attrNum = "<<attrNum
                <<" classifier = "<<classifier;
            cout<<" d = "<<d<<endl;
            cout<<commonLib::exec("date");
            // cout << float( clock () - begin_time ) / CLOCKS_PER_SEC<<endl;begin_time = clock();
          }
      }
    }
    break;
    
    case 64://run Evaluate MoreMore of classifier 3
    {
      for(int shift = 2;shift<=2;shift++)
      {
        for(int attrNum = 32;attrNum<=4096;attrNum*=2)
        {
          for(int fs=0;fs<4;fs++)//fsmethod
          {
            // string fsattr = "./OutARFF/FS_Top50000_n2_s"+patch::to_string(shift)+"_m"+ patch::to_string(fs+1) +".fsattr";
          string num = patch::to_string(attrNum);
          while(num.length()<4)
            num = "0"+num;
          string arffOut = "./resultARFF/arff_n2/FS_Top50000_n2_s"+patch::to_string(shift)+"_m"+patch::to_string(fs+1);
          arffOut += "_"+num+".arff";
// cout << float( clock () - begin_time ) / CLOCKS_PER_SEC<<endl;begin_time = clock();

            // for(int classifier = 1;classifier<=7;classifier++)
            {
              double d = aTest.arffToWeka(arffOut,classifier);
              cout<<" n = "<<2
                  <<" shift = "<<shift
                  <<" fs = "<<(fs+1)
                  <<" attrNum = "<<attrNum
                  <<" classifier = "<<classifier;
              cout<<" d = "<<d<<endl;
  cout<<commonLib::exec("date");
// cout << float( clock () - begin_time ) / CLOCKS_PER_SEC<<endl;begin_time = clock();
            }
          }
        }
      }
    }
    break;

    case 50:
    {//case for n = 1
      for(int attrNum = 32;attrNum<=256;attrNum*=2)
      {
        for(int fs=1;fs<=4;fs++)//fsmethod
        {
          string fsattr = "./OutARFF/fsattrFiles/FS_Top50000_n1_s1_m"+ patch::to_string(fs) +".fsattr";
          string arffOut = aTest.fsattrToarff(fsattr,attrNum,1);
cout << float( clock () - begin_time ) / CLOCKS_PER_SEC<<endl;begin_time = clock();
        }
      }
    }
    break;    
    
    case 51:
    {//case for n = 2
      for(int shift = 1;shift<=2;shift++)
      {
        for(int attrNum = 32;attrNum<=4096;attrNum*=2)
        {
          for(int fs=1;fs<=4;fs++)//fsmethod
          {
            string fsattr = "./OutARFF/fsattrFiles/FS_Top50000_n2_s"+patch::to_string(shift)+"_m"+ patch::to_string(fs) +".fsattr";
            string arffOut = aTest.fsattrToarff(fsattr,attrNum,shift);
cout << float( clock () - begin_time ) / CLOCKS_PER_SEC<<endl;begin_time = clock();
          }
        }
      }
    }
    break;
  
    case 52:
    {//case for n = 3
      for(int shift = 1;shift<=3;shift++)
      {
        for(int attrNum = 32;attrNum<=4096;attrNum*=2)
        {
          for(int fs=1;fs<=4;fs++)//fsmethod
          {
            string fsattr = "./OutARFF/fsattrFiles/FS_Top50000_n3_s"+patch::to_string(shift)+"_m"+ patch::to_string(fs) +".fsattr";
            string arffOut = aTest.fsattrToarff(fsattr,attrNum,shift);
cout << float( clock () - begin_time ) / CLOCKS_PER_SEC<<endl;begin_time = clock();
          }
        }
      }
    }
    break;
  
    case 45:
    {
          cout<<"1"<<endl;
          aTest.config.step=256;
          for(int i=1;i<=1;i++){
            aTest.featureSelectionToFile(1,1);
            cout<<i<<"---";
          cout << float( clock () - begin_time ) / CLOCKS_PER_SEC<<endl;begin_time = clock();
          }
    }
    break;
    
    case 44:
    /*
          aTest.getFileList();
          cout<<"a01"<<endl;
          // aTest.fileToMemString(1,1);
          cout << float( clock () - begin_time ) / CLOCKS_PER_SEC<<endl;begin_time = clock();
          // aTest.fileToMemLong(2,1);
          cout << float( clock () - begin_time ) / CLOCKS_PER_SEC<<endl;begin_time = clock();
          cout<<"a02"<<endl;*/
    break;
    
    case 1://All file to hex, just a pre step.
    {
      aTest.getFileList();
      aTest.extractAllFileToHEX();
    }
    break;
    
    case 22:
    {
          cout<<"2"<<endl;
          // aTest.config.step=1024*64;
          aTest.config.step=32768;
          //testing
          /*
          for(int i=2;i<=3;i++){
            cout<<"step = "<<aTest.config.step<<endl;                
            aTest.featureSelectionToFile(i,1);
            // cout<<i<<"-"<<j<<"-"<<k<<"---";
            cout << float( clock () - begin_time ) / CLOCKS_PER_SEC<<endl;
          }
          break;
          */
          for(int i=2;i<=2;i++)
            for(int j=1;j<=i;j++)
            {
              aTest.featureSelectionToFile(i,j);
              cout<<i<<"-"<<j<<"---";
              cout << float( clock () - begin_time ) / CLOCKS_PER_SEC<<endl;begin_time = clock();
            }
          /*
          aTest.config.step=aTest.config.step*256;
          aTest.featureSelectionToFile(4,1,1);
          aTest.featureSelectionToFile(4,2,1);
          aTest.featureSelectionToFile(4,3,1);
          aTest.featureSelectionToFile(4,4,1);
          //*/

    }
    break;
    
    case 23:
    {
      aTest.config.step=32768;
      aTest.featureSelectionToFile(3,1);
      cout << float( clock () - begin_time ) / CLOCKS_PER_SEC<<endl;begin_time = clock();
    }
    break;
    
    default:cout<<"what?"<<endl;
  }
  // cout<< "memPeak = "<<getPeakRSS( )<<endl;
  
  cout<<"return"<<endl;
  return 0;
  /*
  cout<<commonLib::exec("date");
  const clock_t begin_time = clock();  
  aTest.filterSomeFiles();
  cout << float( clock () - begin_time ) / CLOCKS_PER_SEC<<endl;
  cout<<commonLib::exec("date");
  return 0;*/
  
  // const clock_t begin_time = clock();  
  
  //testing time
  
  // aTest.fsattrToarff("./OutARFF/FS_Top50000_n2_s1_m1.fsattr",32,2);
  // for(int i=1;i<=4;i++)

    // aTest.arffToWeka("./OutARFF/FS_Top50000_n2_s1_m1_0032.arff",7);
    /*
  for(int i=1;i<=1;i++)
  {
    // aTest.config.step*=2;
    // cout<<"step = "<<aTest.config.step<<" ";
    cout<<"method = "<<i<<" ";
    const clock_t begin_time = clock();  
    // aTest.featureSelectionToFile(n,shift,1);
    aTest.featureSelectionToFile(2,1,i);
    cout << float( clock () - begin_time ) / CLOCKS_PER_SEC<<endl;
  }*/
  
  int n_max = 3;
  aTest.config.step=1;//Max them, lol.
  for(int n=1;n<=n_max;n++)
  {
    aTest.config.step=aTest.config.step*256;
    /*
    if(n==1)
      aTest.config.step=256;//exceptional case1
    else
      aTest.config.step=65536;
    */
    for(int shift=1;shift<=n;shift++)
    {
      for(int fs_method = 1;fs_method<=4;fs_method++)
      {
        // cout<<"work on n="<<n<<" s="<<shift<<endl;
        cout<<commonLib::exec("date");
        const clock_t begin_time = clock();  
        // string fsattr = aTest.featureSelectionToFile(n,shift,fs_method);
        
        // aTest.featureSelectionToFile(2,1,1);
        /*
        for(int attrNum = 32;attrNum<=4096;attrNum*=2)
        {
          if(n==1&&attrNum>256)//exceptional case1
            continue;
          string arffOut = aTest.fsattrToarff(fsattr,attrNum,shift);
          
          for(int classifier = 1;classifier;classifier++)
          {
            double d = aTest.arffToWeka(arffOut,classifier);
            cout<<" n = "<<n
                <<" shift = "<<shift
                <<" fs = "<<fs_method
                <<" attrNum = "<<attrNum
                <<" classifier = "<<classifier;
            cout<<" d = "<<d<<endl;
                
          }
        }
        */
        cout << float( clock () - begin_time ) / CLOCKS_PER_SEC<<endl;
        cout<<commonLib::exec("date");
      }
    }
  }
  return 0;
}
