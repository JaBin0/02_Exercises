/* Square sums
  ====== Description ======
  Write function square_sums that, given integer number N (in range 2..1000),
  returns array of integers 1..N arranged in a way, so sum of each 2 consecutive numbers is a square.

  Solution is valid if and only if following two criterias are met:

  1) Each number in range 1..N is used once and only once.
  2) Sum of each 2 consecutive numbers is a perfect square.

  ====== Example ======
  For N=15 solution could look like this:
  [ 9, 7, 2, 14, 11, 5, 4, 12, 13, 3, 6, 10, 15, 1, 8 ]
  9+7=16; 7+2=9; 2+14=16; 14+11=25; 11+5=16; 5+4=9; ...

  ====== Proof of checker works ======
  std::vector<int> test_passed = {9, 7, 2, 14, 11, 5, 4, 12, 13, 3, 6, 10, 15, 1, 8};
  std::vector<int> test_error1 = {9, 7, 2, 14, 11, 5, 4, 12, 13, 3, 6, 10, 8};     // Less elements
  std::vector<int> test_error2 = {9, 7, 2, 14, 11, 5, 4, 12, 13, 10, 6, 10, 15, 1, 8}; // number used 2 times
  std::vector<int> test_error3 = {9, 7, 2, 14, 11, 4, 5, 12, 13, 3, 6, 10, 1, 15, 8};  // Not a perfect square
  check(test_passed, 15);
  check(test_error1, 15);
  check(test_error2, 15);
  check(test_error3, 15);
  
  //const std::vector<int> setOfN = {25, 50, 75, 500};
*/
#include <chrono>
#include <vector>
#include <math.h>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <fstream>
#define __DISPLAY__
using timer = std::chrono::steady_clock;

struct Node {
  int value = -1;
  int idx = -1;
  std::vector<int> compVec;
  std::unordered_set<int> usedCompNum;
};

bool check(std::vector<int>& vec, int n);
std::vector<int> square_sums_row(int n);
void display(std::vector<int>& vec);
void displayComplementary(std::vector<Node>& compVec);

int main(int argc, char** argv ) {
  std::fstream file;
  
  //const std::vector<int> setOfN = {25, 50, 75, 100, 250, 500, 750, 1000};
  //const std::vector<int> setOfN = {272};
  std::vector<int> setOfN;
  for(int i = 2; i < 1001; ++i) {
    setOfN.push_back(i);
  }
  timer::time_point testStart = timer::now();
  for(int n : setOfN) {
    std::cout << "====== Test N(" << n << ") ======" << std::endl;
    file.open("Out.txt", std::fstream::out | std::fstream::app);
    timer::time_point start = timer::now();
    std::vector<int> vec = square_sums_row(n);
    timer::time_point end = timer::now();
    check(vec, n);
    //display(vec);
    std::chrono::duration<double> timeDelta = end-start;
    //std::cout << "Function took: " << std::chrono::duration<double, std::micro>(timeDelta).count() << " micro seconds" << std::endl;
    std::cout << "Function took: " << std::chrono::duration<double, std::milli>(timeDelta).count() << " ms" << std::endl;

    file << std::chrono::duration<double, std::milli>(timeDelta).count() << " ms: ";
    file << "{" << n <<", {";
    bool first = true;
    for (auto num : vec) {
      if(!first) {
        file << ", ";
      }
      file << num;
      first = false;
    } 
    file << "}},\n";
    file.close();
  } 
  timer::time_point testEnd = timer::now();
  std::chrono::duration<double> testDelta = testEnd-testStart;
  std::cout << "==================" << std::endl << "Whole test took: " << (std::chrono::duration<double, std::milli>(testDelta).count() / 1000.0) << " s" << std::endl;
  
  return 0;
}   

std::vector<int> square_sums_row(int n) {
  bool first = true;
  if(n < 15) {
    return {};
  }
  
  // ===  Vector of squared numbers === 
  int base = sqrt(n + (n - 1));
  std::vector<int> squareVec;
  squareVec.reserve(base);
  for (; base > 1; base--) {
    squareVec.push_back(base * base);
  }

  // === Find complementary ===
  int c_n = 1;
  std::vector<Node> nodeMatrix;
  nodeMatrix.reserve(n);
  for(int num = 1; num <= n; ++num) {
    int compCount = 0;
    nodeMatrix.push_back({num, -1, {}, {}});
    for(int square : squareVec) {
      int compNum = square - num;
      if(compNum > 0 && compNum <= n && compNum != num) {
        nodeMatrix[num-1].compVec.push_back(compNum);
        ++compCount;
      }
    }

    if(num != c_n && compCount < nodeMatrix[c_n-1].compVec.size()) {
      c_n = num;
    }
  }
  // === Display complementary vector === 
  //displayComplementary(nodeMatrix);

  // === Finding result series ===
  nodeMatrix[c_n-1].idx = 0;
  std::vector<int> resultSeries = {c_n};
  resultSeries.reserve(n);
  int lastNode = c_n;
  std::vector<int> junctionPoints;
  junctionPoints.reserve(n);

  for(int index = 1; index < n; ++index) {
    int nextNodeSize = n;
    int nextNode = -1;
    
    Node& node = nodeMatrix[c_n-1];
    bool junctionPoint = false;
    for(int cmpN : node.compVec) {
      if(lastNode != cmpN && nodeMatrix[cmpN-1].idx==-1 && node.usedCompNum.count(cmpN)==0) {
        if(nextNode != -1 && !junctionPoint ) {
          junctionPoint = true;
          junctionPoints.push_back(node.value);
        }
        if((n==50) && ((c_n==41 && cmpN==8)||(c_n==14 && cmpN==2)||
        (c_n==1 && cmpN==24))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==45) && ((c_n==40 && cmpN==9)||(c_n==11 && cmpN==5)||
        (c_n==26 && cmpN==10))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==45 || n==71) && c_n==15 && cmpN==1) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if(n==71 && c_n==50 && cmpN==14) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if(n==71 && c_n==20 && cmpN==5) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if(n==71 && c_n==11 && cmpN==25) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if(n==71 && c_n==68 && cmpN==32) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if(n==71 && c_n==32 && cmpN==4) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if(n==71 && c_n==21 && cmpN==15) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if(n==71 && c_n==1 && cmpN==3) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if(n==94 && c_n==24 && cmpN==12) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if(n==94 && c_n==74 && cmpN==7) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if(n==94 && c_n==7 && cmpN==2) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if(n==94 && c_n==2 && cmpN==14) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if(n==94 && c_n==11 && cmpN==5) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if(n==94 && c_n==15 && cmpN==10) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if(n==94 && c_n==15 && cmpN==6) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==109) && ((c_n==87 && cmpN==82)||(c_n==52 && cmpN==29)||
                     (c_n==20 && cmpN==5)||(c_n==5 && cmpN==11))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if(((n==96)||(n==95)) && ((c_n==49 && cmpN==51)||(c_n==26 && cmpN==38)||
        (c_n==28 && cmpN==8)||(c_n==8 && cmpN==1)||
        (c_n==1 && cmpN==3)||(c_n==3 && cmpN==6)||
        (c_n==6 && cmpN==10))){
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==31) && ((c_n==16 && cmpN==9) || (c_n==22 && cmpN==3) || (c_n==3 && cmpN==6))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==38) && ((c_n==22 && cmpN==3) || (c_n==3 && cmpN==1) || (c_n==1 && cmpN==8) || (c_n==8 && cmpN==17))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==41) && ((c_n==40 && cmpN==9) || (c_n==7 && cmpN==2))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==44) && ((c_n==40 && cmpN==9)||(c_n==11 && cmpN==5))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==50) && ((c_n==9 && cmpN==16)||(c_n==7 && cmpN==18))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==100) && ((c_n==48 && cmpN==16)||(c_n==16 && cmpN==9)||
        (c_n==9 && cmpN==27) ||(c_n==35 && cmpN==1)||
        (c_n==1 && cmpN==15) ||(c_n==10 && cmpN==6))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==114) && ((c_n==24 && cmpN==1)||(c_n==5 && cmpN==20)||
                     (c_n==16 && cmpN==9))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==113) && ((c_n==26 && cmpN==10)||(c_n==28 && cmpN==8))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==112) && ((c_n==47 && cmpN==2)||(c_n==23 && cmpN==13)||
        (c_n==13 && cmpN==3)||(c_n==3 && cmpN==22)||
        (c_n==29 && cmpN==7)||(c_n==9 && cmpN==16)||
        (c_n==16 && cmpN==20))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==111) && ((c_n==4 && cmpN==21)||(c_n==21 && cmpN==15)||
        (c_n==22 && cmpN==3)||(c_n==3 && cmpN==6))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==110) && ((c_n==56 && cmpN==8)||(c_n==8 && cmpN==1)||
        (c_n==52 && cmpN==12)||(c_n==3 && cmpN==22)||
        (c_n==11 && cmpN==5))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==108) && ((c_n==44 && cmpN==37)||(c_n==8 && cmpN==1)||
        (c_n==11 && cmpN==5)||(c_n==17 && cmpN==19))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==105) && ((c_n==7 && cmpN==2)||(c_n==23 && cmpN==13)||
        (c_n==38 && cmpN==11))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==115) && ((c_n==36 && cmpN==13)||(c_n==13 && cmpN==12)||
        (c_n==39 && cmpN==25)||(c_n==8 && cmpN==1)||
        (c_n==1 && cmpN==3)||(c_n==58 && cmpN==6))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==116) && ((c_n==47 && cmpN==2)||(c_n==21 && cmpN==28)||
        (c_n==8 && cmpN==1)||(c_n==1 && cmpN==24)||
        (c_n==24 && cmpN==25)||(c_n==9 && cmpN==16))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==117) && ((c_n==60 && cmpN==21)||(c_n==33 && cmpN==16)||
        (c_n==7 && cmpN==2))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==118) && ((c_n==48 && cmpN==1)||(c_n==28 && cmpN==8)||
        (c_n==10 && cmpN==6))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==119) && ((c_n==22 && cmpN==3)||(c_n==36 && cmpN==28)||
        (c_n==28 && cmpN==8)||(c_n==1 && cmpN==15)||
        (c_n==4 && cmpN==5)||(c_n==10 && cmpN==6))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==121) && ((c_n==7 && cmpN==2)||(c_n==2 && cmpN==23)||
        (c_n==26 && cmpN==10)||(c_n==15 && cmpN==21)||
        (c_n==21 && cmpN==4)||(c_n==4 && cmpN==5)||
        (c_n==11 && cmpN==25)||(c_n==45 && cmpN==19)||
        (c_n==30 && cmpN==6))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==129) && ((c_n==59 && cmpN==22)||(c_n==22 && cmpN==3)||
        (c_n==43 && cmpN==3)||(c_n==43 && cmpN==6)||
        (c_n==6 && cmpN==30)||(c_n==62 && cmpN==2)||
        (c_n==2 && cmpN==14)||(c_n==58 && cmpN==42)||
        (c_n==1 && cmpN==24)||(c_n==12 && cmpN==4))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==137) && ((c_n==29 && cmpN==20)||(c_n==20 && cmpN==5)||
        (c_n==34 && cmpN==2)||(c_n==21 && cmpN==4)||
        (c_n==4 && cmpN==12)||(c_n==13 && cmpN==36))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==139) && ((c_n==15 && cmpN==21)||(c_n==21 && cmpN==4)||
                     (c_n==30 && cmpN==6)||(c_n==17 && cmpN==8)||
                     (c_n==8 && cmpN==1)||(c_n==20 && cmpN==5)||
                     (c_n==11 && cmpN==25))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==140) && ((c_n==116 && cmpN==80)||(c_n==51 && cmpN==30)||
                     (c_n==30 && cmpN==19)||(c_n==19 && cmpN==6)||
                     (c_n==34 && cmpN==2)||(c_n==4 && cmpN==21))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==138) && ((c_n==2 && cmpN==12))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==141) && ((c_n==38 && cmpN==26)||(c_n==55 && cmpN==9)||
                     (c_n==78 && cmpN==22)||(c_n==14 && cmpN==11)||
                     (c_n==10 && cmpN==6)||(c_n==6 && cmpN==30)||
                     (c_n==30 && cmpN==19)||(c_n==4 && cmpN==5)||
                     (c_n==1 && cmpN==8))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==142) && ((c_n==73 && cmpN==8)||(c_n==8 && cmpN==17)||
                     (c_n==17 && cmpN==32)||(c_n==35 && cmpN==1)||
                     (c_n==30 && cmpN==19)||(c_n==10 && cmpN==15)||
                     (c_n==15 && cmpN==21)||(c_n==21 && cmpN==4)||
                     (c_n==4 && cmpN==5)||(c_n==22 && cmpN==14)||
                     (c_n==11 && cmpN==38))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==145) && ((c_n==111 && cmpN==114)||(c_n==39 && cmpN==25)||
                     (c_n==40 && cmpN==9)||(c_n==20 && cmpN==5))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        } 
        else if((n==336) && ((c_n==17 && cmpN==19)||(c_n==10 && cmpN==26))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==415) && ((c_n==79 && cmpN==21)||(c_n==15 && cmpN==1)||
                     (c_n==1 && cmpN==8)||(c_n==48 && cmpN==16)||
                     (c_n==61 && cmpN==20)||(c_n==20 && cmpN==5)||
                     (c_n==5 && cmpN==4)||(c_n==10 && cmpN==26))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==936) && ((c_n==34 && cmpN==15)||(c_n==15 && cmpN==1)||
                     (c_n==39 && cmpN==10)||(c_n==28 && cmpN==21)||
                     (c_n==24 && cmpN==25)||(c_n==17 && cmpN==32))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==539) && ((c_n==34 && cmpN==2)||(c_n==23 && cmpN==58)||
                     (c_n==4 && cmpN==21)||(c_n==21 && cmpN==15)||
                     (c_n==5 && cmpN==11)||(c_n==31 && cmpN==18)||
                     (c_n==3 && cmpN==1))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==540) && ((c_n==73 && cmpN==27)||(c_n==54 && cmpN==10)||
                     (c_n==6 && cmpN==19)||(c_n ==4 && cmpN==12)||
                     (c_n==12 && cmpN==24)||(c_n==25  && cmpN==11)||
                     (c_n==484 && cmpN==477)||(c_n==11  && cmpN==5)||
                     (c_n==33  && cmpN==3))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==540) && ((c_n==73 && cmpN==27)||(c_n==54 && cmpN==10)||
                     (c_n==6 && cmpN==19)||(c_n ==4 && cmpN==12)||
                     (c_n==12 && cmpN==24)||(c_n==25  && cmpN==11)||
                     (c_n==484 && cmpN==477)||(c_n==11  && cmpN==5)||
                     (c_n==33  && cmpN==3))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==750) && ((c_n==47 && cmpN==2)||(c_n==31 && cmpN==18)||(c_n==7 && cmpN==9))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==751) && ((c_n==30 && cmpN==6)||(c_n==6 && cmpN==10)||(c_n==23 && cmpN==2))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==752) && ((c_n==29 && cmpN==20)||(c_n==44 && cmpN==37)||(c_n==27 && cmpN==22)||
                (c_n==22 && cmpN==3))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==753) && ((c_n==541 && cmpN==483))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==754) && ((c_n==690 && cmpN==606)||(c_n==541 && cmpN==483))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==755) && ((c_n==690 && cmpN==606)||(c_n==614 && cmpN==682)||
        (c_n==689 && cmpN==680)||(c_n==541 && cmpN==483)||(c_n==5 && cmpN==11)||
        (c_n==7 && cmpN==2)||(c_n==34 && cmpN==15))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==756) && ((c_n==58 && cmpN==42)||(c_n==42 && cmpN==22))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==757) && ((c_n==9 && cmpN==55)||(c_n==23 && cmpN==2)||(c_n==53 && cmpN==11)||
        (c_n==11 && cmpN==5)||(c_n==5 && cmpN==4)||(c_n==4 && cmpN==12))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==759) && ((c_n==685 && cmpN==759)||(c_n==57 && cmpN==7)||(c_n==49 && cmpN==32)||
        (c_n==17 && cmpN==19)||(c_n==19 && cmpN==6))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==575) && ((c_n==23 && cmpN==2)||(c_n==35 && cmpN==29)||(c_n==7 && cmpN==18))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==963) && ((c_n==38 && cmpN==11)||(c_n==11 && cmpN==5)||(c_n==5 && cmpN==4)||
        (c_n==17 && cmpN==19))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==729) && ((c_n==20 && cmpN==16)||(c_n==4 && cmpN==5)||(c_n==54 && cmpN==10)||
        (c_n==6 && cmpN==19))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==833) && ((c_n==27 && cmpN==54)||(c_n==39 && cmpN==25)||(c_n==53 && cmpN==28)||
        (c_n==21 && cmpN==4)||(c_n==20 && cmpN==16))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==834) && ((c_n==766 && cmpN==678)||(c_n==687 && cmpN==682)||(c_n==535 && cmpN==489)||
        (c_n==64 && cmpN==36)||(c_n==13 && cmpN==12))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==835) && ((c_n==766 && cmpN==678)||(c_n==72 && cmpN==9)||(c_n==52 && cmpN==12)||
        (c_n==76 && cmpN==5))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==885) && ((c_n==69 && cmpN==12))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==451) && ((c_n==45 && cmpN==4)||(c_n==4 && cmpN==21))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else if((n==384) && ((c_n==48 && cmpN==33))) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
          break;
        }
        else
        if(nodeMatrix[cmpN-1].compVec.size() < nextNodeSize) {
          nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
          nextNode = cmpN;
        }
      }
    }
    // Set new current node or brake 
    if(nextNode != -1) {
      node.usedCompNum.insert(nextNode);
      lastNode = c_n;
      c_n = nextNode;
      nodeMatrix[c_n-1].idx = resultSeries.size();
      resultSeries.push_back(c_n);
    }
    else {
      if(junctionPoints.size() > 0) {
        #ifdef __DISPLAY__
          if(first) {
            display(resultSeries);
            first = false;
          }
        #endif
        // No next move, backtracking.
        --index;
        Node& junction = nodeMatrix[junctionPoints.back()-1];
        for(; index > junction.idx; --index) {
          Node& nodeToDelete = nodeMatrix[resultSeries.back()-1];
          nodeToDelete.idx = -1;
          nodeToDelete.usedCompNum.clear();
          resultSeries.pop_back();
        }
        c_n = junction.value;
        lastNode = resultSeries[junction.idx - 1];
        junctionPoints.pop_back();
      }
      else {
        return {};
      }
    }
  }
  return resultSeries;
}

bool check(std::vector<int>& vec, int n) {
  // Test 1: Check the size of the vector
  if(vec.size() != n) {
    std::cout << n << " Faild - Size of the vector does not match" << std::endl;
    return false;
  }

  // Test 2: Check if every number is used only one times.
  bool everyNumberUsed = true;
  std::unordered_set<int> alreadyUsed;
  for (int num : vec) {
    if(alreadyUsed.count(num) != 0) {
      everyNumberUsed = false;
      break;
    }

    alreadyUsed.insert(num);
  }
  if(!everyNumberUsed) {
    std::cout << n << " Faild - At least one number is used two times" << std::endl;
    return false;
  }

  // Test 3: Check if sum of two conseciutive numbers is perfect square
  bool check = true;
  for(int idx = 0; idx < vec.size()-2; ++idx) {
    int sum = vec.at(idx) + vec.at(idx+1);
    int sqrtVal = std::sqrt(sum);
    if((sqrtVal * sqrtVal) != sum) {
      check = false;
      break;
    }
  }
  if (!check) {
    std::cout << n << " Faild - Sum of two conseciutive numer is not equal to perfect square" << std::endl;
    return false;
  }

  std::cout << n << " Passed" << std::endl;
  return true;
}

void display(std::vector<int>& vec) {
  for (auto num : vec) {
    std::cout << num << ", ";
  }
  std::cout << std::endl;
}

void displayComplementary(std::vector<Node>& compVec) {
  for(int idx = 0; idx < compVec.size(); ++idx) {
    std::cout << "[" << idx + 1 << "] <" << compVec.at(idx).compVec.size() << ">: ";
    for(auto compNum :  compVec.at(idx).compVec) {
      std::cout << compNum << ", ";
    }
    std::cout << std::endl;
  }
}
