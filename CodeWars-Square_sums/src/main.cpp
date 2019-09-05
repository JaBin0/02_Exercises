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
#include <algorithm>
#define __DISPLAY__
using timer = std::chrono::steady_clock;
std::unordered_map<int, std::vector<std::pair<int, int>>> rules = {};
std::unordered_map<int, std::vector<std::pair<int, int>>> filtred = {};
struct Node {
  int value = -1;
  int idx = -1;
  std::vector<int> compVec;
  std::unordered_set<int> usedCompNum;
};

bool check(std::vector<int>& vec, int n);
std::vector<int> square_sums_row(int n, std::vector<int>& firstErr, bool& timeOut);
void display(std::vector<int>& vec);
void displayComplementary(std::vector<Node>& compVec);
void loadRules(std::unordered_map<int, std::vector<std::pair<int, int>>>& rules);
void selectRules(std::unordered_map<int, std::vector<std::pair<int, int>>>& rules, std::unordered_map<int, std::vector<std::pair<int, int>>>& filtred);
void optimize(std::vector<int> vec, std::vector<int> err, std::unordered_map<int, std::vector<std::pair<int, int>>>& rules, int n);
void saveOptimizeMap( std::unordered_map<int, std::vector<std::pair<int, int>>>& rules);
void saveRulesVector( std::unordered_map<int, std::vector<std::pair<int, int>>>& rules);
void printToFile(std::fstream& file, double time, int n, std::vector<int>& vec);

int main(int argc, char** argv ) {
  //const std::vector<int> setOfN = {25, 50, 75, 100, 250, 500, 750, 1000};
  //const std::vector<int> setOfN = {574};
  std::vector<int> setOfN;
  for(int i = 2; i < 1001; ++i) {
    setOfN.push_back(i);
  }
  double threshold = 80; // 100 ms;
  loadRules(rules);
  //selectRules(rules, filtred);
  saveRulesVector(rules);
  // std::fstream file;
  // std::vector<int> prevN = {};
  // timer::time_point testStart = timer::now();
  // //for(int n : setOfN) {
  // for (int i=0; i < setOfN.size(); ++i) {
  //   int n = setOfN[i];
  //   std::cout << "====== Test N(" << n << ") ======" << std::endl;
  //   file.open("Out.txt", std::fstream::out | std::fstream::app);
  //   std::vector<int> firstErr{};
  //   bool timeOut = false;
  //   timer::time_point start = timer::now();
  //   std::vector<int> vec = square_sums_row(n, firstErr, timeOut);
  //   timer::time_point end = timer::now();
  //   check(vec, n);
  //   if(timeOut) {
  //     std::cout << "TIME OUT" << std::endl;
  //     // Check with previus success;
  //     vec = prevN;
  //   }

  //   //display(vec);
  //   std::chrono::duration<double, std::milli> timeDelta = end-start;
  //   if(timeDelta.count() >= threshold) {
  //     optimize(vec, firstErr, rules, n);
  //     saveOptimizeMap(rules);
  //     --i;
  //   }
  //   std::cout << "Function took: " << timeDelta.count() << " ms" << std::endl;
  //   printToFile(file, timeDelta.count(), n, vec);
  //   file.close();
  //   prevN = vec;
  // } 
  // timer::time_point testEnd = timer::now();
  // std::chrono::duration<double> testDelta = testEnd-testStart;
  // std::cout << "==================" << std::endl << "Whole test took: " << (std::chrono::duration<double, std::milli>(testDelta).count() / 1000.0) << " s" << std::endl;
  
  return 0;
}   

std::vector<int> square_sums_row(int n, std::vector<int>& firstErr, bool& timeOut) {
  timer::time_point start = timer::now();
  double timeOut_treshold = 100000;
  bool first = true;
  if(n<15) {
    return {};
  }
  
  // === Get rules for the n ===
  std::vector<std::pair<int, int>> rulesVec = rules[n];

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
  for (auto rule : rulesVec) {
    if(rule.first == -1) {
      c_n=rule.second;
    }
  }

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
        bool found = false;
        for (auto rule : rulesVec) {
          if(c_n == rule.first && cmpN == rule.second) {
              nextNodeSize = nodeMatrix[cmpN-1].compVec.size();
              nextNode = cmpN;
              found = true;
              break;
          }
        }
        if(found) {
          break;
        }

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
            firstErr = resultSeries;
            //display(resultSeries);
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
    timer::time_point midPoint = timer::now();
    std::chrono::duration<double, std::milli> checkTimeOut = midPoint-start;
    if(checkTimeOut.count() > timeOut_treshold) {
      timeOut = true;
      return {};
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
void loadRules(std::unordered_map<int, std::vector<std::pair<int, int>>>& rules) {
  std::fstream file;
  file.open("Rules.txt", std::fstream::in);
  if(file.is_open()) {
    std::string line;
    while(!file.eof()) {
      std::getline(file, line);
      if(!line.empty()) {
        auto semi = line.find(',', 0);
        int nvalue = std::atoi(line.substr(1,semi-1).data());
        rules[nvalue] = {};
        std::vector<std::pair<int, int>>& ruleVec = rules[nvalue];
        int st = line.find('{', semi+2);
        while(st != -1) {
          auto semi = line.find(',', st);
          auto end = line.find('}',st);
          int prev = std::atoi(line.substr(st+1, semi-st-1).data());
          int next = std::atoi(line.substr(semi+1, end-semi-1).data());
          ruleVec.push_back({prev,next});
          st = line.find('{', end+1);
        };
      }
    }
    file.close();
    // for (auto item : rules) {
    //   std::cout << "["<<item.first<<"]:";
    //   for (auto p : item.second) {
    //     std::cout << "{"<<p.first<<","<<p.second<<"},";
    //   }
    //   std::cout << std::endl;
    // }
  }
  else {
    std::cout << "FAILD" << std::endl;
  }
}
void optimize(std::vector<int> vec, std::vector<int> err, std::unordered_map<int, std::vector<std::pair<int, int>>>& rules, int n) {
  for(int idx = 0; idx < err.size(); ++idx) {
    if(err[idx] != vec[idx]) {
      if(rules.count(n) == 0) {
        rules[n] = {};
      }
      std::vector<std::pair<int, int>>& ruleLine = rules[n];
      if(idx != 0) {
        ruleLine.push_back({vec[idx-1], vec[idx]});
      }
      else {
        ruleLine.push_back({-1, vec[idx]});
      }
      
      break;
    }
  }
}
void saveOptimizeMap( std::unordered_map<int, std::vector<std::pair<int, int>>>& rules) {
  std::fstream file;
  file.open("Rules.txt", std::fstream::out | std::fstream::trunc);
  if(file.is_open()) {
    bool notFirstRow = false;
    for(auto item : rules) {
      if(item.second.size() != 0) {
        if(notFirstRow) {
          file << "\n";
        }
        file << "{" << item.first << ",{";
        bool notFirstPair = false;
        for(auto pair : item.second) {
          if(notFirstPair) {
            file << ",";
          }
          file << "{" << pair.first << "," << pair.second << "}";
          notFirstPair = true;
        }
        file << "}},";
        notFirstRow = true;
      }
    }
  }
  else {
    std::cout << "Add to optimize map - file is not open" << std::endl;
  }
  file.close();
}
void printToFile(std::fstream& file, double time, int n, std::vector<int>& vec) {
  if(file.is_open()) {
    file << time << " ms: ";
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
  }
}

void saveRulesVector( std::unordered_map<int, std::vector<std::pair<int, int>>>& rules) {
  std::fstream file;
  file.open("RulesVec.txt", std::fstream::out | std::fstream::trunc);
  if(file.is_open()) {
    for(int i=1; i < 1001; ++i) {
      auto vec = rules[i];
      if(i!=1) {
          //file << "\n";
      }
      file << "{";
      bool notFirstPair = false;
      for(auto pair : vec) {
        if(notFirstPair) {
          file << ",";
        }
        //file << "{" << pair.first << "," << pair.second << "}";
        file << pair.first << "," << pair.second;
        notFirstPair = true;
      }
        file << "},";
      }
    }
  else {
    std::cout << "Add to optimize map - file is not open" << std::endl;
  }
  file.close();
}

void selectRules(std::unordered_map<int, std::vector<std::pair<int, int>>>& rules, std::unordered_map<int, std::vector<std::pair<int, int>>>& filtred) {
  std::fstream file;
  file.open("input.txt", std::fstream::in);
  if(file.is_open()) {
    std::string line;
    while(!file.eof()) {
      std::getline(file,line);
      int number = std::atoi(line.data());
      filtred[number] = rules[number];
    }
    file.close();
    for(int i=1;i<146;++i) {
      filtred[i] = rules[i];
    }
    saveRulesVector(filtred);
  }
}