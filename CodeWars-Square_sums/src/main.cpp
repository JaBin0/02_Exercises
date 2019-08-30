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
*/
#include <iostream>
#include <chrono>
#include <vector>
#include <math.h>
#include <unordered_set>
#include <unordered_map>

using timer = std::chrono::steady_clock;

struct Number {
  int value = -1;  
  Number* right = nullptr;
  Number* left = nullptr;
  std::vector<int> cmpVec = {};
  std::unordered_set<int> cmpAlredyUsed = {};
  int idx = -1;
};

bool check(std::vector<int>& vec, int n);

std::vector<int> square_sums_row(int n);

int main(int argc, char** argv ) {
    // Proof of works
    /*
    std::vector<int> test_passed = {9, 7, 2, 14, 11, 5, 4, 12, 13, 3, 6, 10, 15, 1, 8};
    std::vector<int> test_error1 = {9, 7, 2, 14, 11, 5, 4, 12, 13, 3, 6, 10, 8};         // Less elements
    std::vector<int> test_error2 = {9, 7, 2, 14, 11, 5, 4, 12, 13, 10, 6, 10, 15, 1, 8}; // number used 2 times
    std::vector<int> test_error3 = {9, 7, 2, 14, 11, 4, 5, 12, 13, 3, 6, 10, 1, 15, 8};  // Not a perfect square
    check(test_passed, 15);
    check(test_error1, 15);
    check(test_error2, 15);
    check(test_error3, 15);
    */
    //const std::vector<int> setOfN = {25, 50, 75, 100, 250, 500, 750, 1000};
    const std::vector<int> setOfN = {25, 50, 75, 500};
    timer::time_point testStart = timer::now();

    for(int n : setOfN) {
        std::cout << "====== Test N(" << n << ") ======" << std::endl;
        timer::time_point start = timer::now();
        std::vector<int> vec = square_sums_row(n);
        timer::time_point end = timer::now();
        check(vec, n);
        std::chrono::duration<double> timeDelta = end-start;
        std::cout << "Function took: " << std::chrono::duration<double, std::milli>(timeDelta).count() << " ms" << std::endl;
    } 
    timer::time_point testEnd = timer::now();
    std::chrono::duration<double> testDelta = testEnd-testStart;
    std::cout << "==================" << std::endl << "Whole test took: " << (std::chrono::duration<double, std::milli>(testDelta).count() / 1000.0) << " s" << std::endl;
}

std::vector<int> square_sums_row(int n) {
  if(n < 15) {
    return {};
  }
  
  // Vector of squared numbers
  int base = sqrt(n + (n - 1));
  std::vector<int> squaredVec;
  squaredVec.reserve(base);
  for (; base > 0; base--) {
    squaredVec.push_back(base * base);
  }
  
  // Find complementary numbers
  std::unordered_map<int, Number> cmpMap;
  for(int idx = 1; idx <= n; ++idx) {
    cmpMap[idx].value = idx;
  }
  
  // Number which start the series, by default it is 1 and during search it
  // should be set to number which has only one complementary value if such exist,
  // otherwise default stay unchange.
  int currentNum = 1;
  // Initial value of the number of numbers which can have only one complementary
  int numberWithOneComp = 2;
  
  for (int idx = 1; idx <= n; ++idx) {
    // Amount of complementary number finded for the num
    int cmpCount = 0;
    Number& num = cmpMap[idx];
    
    for(auto squared : squaredVec) {
      int cmpNum = squared - num.value;
      if(cmpNum > 0 && cmpNum <= n && cmpNum != num.value) {
        cmpCount++;
        num.cmpVec.push_back(cmpNum);
      }
    }
    
    // Check if only one complementary 
    if(cmpCount < 2 ) {
      currentNum = num.value;
      if(--numberWithOneComp < 0) {
        std::cout << "Series for " << n << ", more then two numbers have only" 
                << "one complementary number" << std::endl;
        return {};
      }
    }
    else if(cmpCount == 2) {
      Number& firstCmp = cmpMap[num.cmpVec.at(0)];
      Number& secondCmp = cmpMap[num.cmpVec.at(1)];
      bool firstUsed = false;
      bool secondUsed = false;
      
      if(num.left == nullptr) {
        if(firstCmp.right == nullptr) {
          num.left = &firstCmp;
          firstCmp.right = &num;
          firstUsed = true;
        }
        else if(secondCmp.right == nullptr) {
          num.left = &secondCmp;
          secondCmp.right = &num;
          secondUsed = true;
        }
        else {
          std::cout << "Assert - M0" << std::endl;
        }
      }
      
      if(num.right == nullptr) {
        if(firstCmp.left == nullptr && !firstUsed ) {
          num.right = &firstCmp;
          firstCmp.left = &num;
        }
        else if(secondCmp.left == nullptr && !secondUsed) {
          num.right = &secondCmp;
          secondCmp.left = &num;
        }
        else {
          std::cout << "Assert - M1" << std::endl;
        }
      }
    
      //std::cout << num.left->value << " << " << num.value << " >> " << num.right->value << std::endl;
    }
  }
  
//   std::cout << std::endl;
//   std::cout << "==========================" << std::endl;
//   std::cout << std::endl;
  
  // Show complementary
//   for(auto elem : cmpMap) {
//     std::cout << "[" <<elem.first << "]: ";
//     for(auto i : elem.second.cmpVec) {
//       std::cout << i << ", ";
//     }
//     std::cout << std::endl;
//   }
  
//   std::cout << std::endl;
//   std::cout << "==========================" << std::endl;
//   std::cout << std::endl;
  
  std::vector<int> resultSeries;
  resultSeries.reserve(n);
  
  // Test
  resultSeries.push_back(currentNum);
  cmpMap.at(currentNum).idx = 0;
  
  std::unordered_set<int> alredyUsed = {currentNum};
  
  int lastNum = 0;
  std::vector<int> multiplePosibilites = {};
  
  for(int idx = 1; idx < n; ++idx) {
    int tmpNum = -1;
    bool nextValueFound = false;
    for(int num : cmpMap.at(currentNum).cmpVec) {
      
     // std::cout << num <<", ";
      if(num != lastNum && alredyUsed.count(num) == 0 && cmpMap.at(currentNum).cmpAlredyUsed.count(num) == 0) {
        if (cmpMap[num].left == nullptr || cmpMap[num].right == nullptr || 
            cmpMap[num].left->value == currentNum || cmpMap[num].right->value == currentNum) { 
            if(tmpNum == -1) {
              tmpNum = num;
            }
            else {
              bool elemFind = false;
              for (auto i : multiplePosibilites) {
                if(currentNum == i) {
                  elemFind =true;
                  break;
                }
              }
              
              if(!elemFind) {
                multiplePosibilites.push_back(currentNum);
              }
            }
        }
      }
    }
    
    if(tmpNum != -1) {
        lastNum = currentNum;
        cmpMap.at(currentNum).cmpAlredyUsed.insert(tmpNum);
        currentNum = tmpNum;
        // Pass index of new element in vector
        cmpMap.at(currentNum).idx = resultSeries.size();
        resultSeries.push_back(currentNum);
        
        alredyUsed.insert(currentNum);  
      }
      else {
        // For test print result
//         std::cout << "Faild Result (" << numberResult++ << "): ";
//         for (auto num : resultSeries) {
//           std::cout << num << ", ";
//         }
//         std::cout << std::endl;
        // Delete last 
        
        // Find index of the last multipossible element
        if(multiplePosibilites.size() > 0) {
          int lastMulti = multiplePosibilites.back();
          int lastIdx = 0;
          for(auto i : resultSeries) {
            if(lastMulti == i) {
              break;
            }
            lastIdx++;
          }
          
          idx--;
          for (; idx > lastIdx; idx--) {
            int elem = resultSeries.back();
            cmpMap.at(elem).idx = -1;
            cmpMap.at(elem).cmpAlredyUsed.clear();
            alredyUsed.erase(elem);
            resultSeries.pop_back();
            
          }
          currentNum = lastMulti;
          lastNum = resultSeries.at(cmpMap[lastMulti].idx - 1);
          
//           std::cout << "Test: " << currentNum << " << " << lastNum << std::endl; 
          multiplePosibilites.pop_back();
        }
        else {
          // No possible result - in teorii
        }
      }
//     if(lastNum == currentNum) {
//       std::cout << "Assert M3" << std::endl;
//       return {};
//     }
  }
  // Test
  
//   for (auto num : resultSeries) {
//     std::cout << num << ", ";
//   }
//   std::cout << std::endl;
  
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


    


    // std::chrono::steady_clock::time_point time_0 = std::chrono::steady_clock::now();
    // std::cout << "Printing out 1000 stars...\n";
    // for(int i=0; i<1000; ++i) {
    //     std::cout << "*";
    // }
    // std::cout << std::endl;
    // std::chrono::steady_clock::time_point time_1 = std::chrono::steady_clock::now();
    // std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(time_0 - time_1);

    // std::cout << "It took: " << time_span.count() << " seconds." << std::endl;


// #include <iostream>
// #include <chrono>


// int main(int argc, char** argv ) {
//     std::cout << "Configuration of the clocks: "<< std::endl; 
//     std::cout << "system_clock" << std::endl
//               << "<num, den>: <" << std::chrono::system_clock::period::num << ", "
//               << std::chrono::system_clock::period::den << ">" << std::endl
//               << "Stedy = " << std::boolalpha << std::chrono::system_clock::is_steady << std::endl;
//     std::cout << std::endl;
//     std::cout << "high_resolution_clock" << std::endl
//               << "<num, den>: <" << std::chrono::high_resolution_clock::period::num << ", "
//               << std::chrono::high_resolution_clock::period::den << ">" << std::endl
//               << "Stedy = " << std::boolalpha << std::chrono::high_resolution_clock::is_steady << std::endl;
//     std::cout << std::endl;
//     std::cout << "steady_clock" << std::endl
//               << "<num, den>: <" << std::chrono::steady_clock::period::num << ", "
//               << std::chrono::steady_clock::period::den << ">" << std::endl
//               << "Steady = " << std::boolalpha << std::chrono::steady_clock::is_steady << std::endl;
//     std::cout << std::endl 
//               << "Time of the simple code execiution: " << std::endl;
//     // For performence use steady timer as theay are not adjusted by the system
//     // Hight resolution or system also can be steady, it depends on the machine
//     std::chrono::steady_clock::time_point time_0 = std::chrono::steady_clock::now();
//     std::cout << "Printing out 1000 stars...\n";
//     for(int i=0; i<1000; ++i) {
//         std::cout << "*";
//     }
//     std::cout << std::endl;
//     std::chrono::steady_clock::time_point time_1 = std::chrono::steady_clock::now();

//     // Result
//     std::chrono::duration<double> diff = time_1 - time_0;
//     std::cout << "This code took: " << std::chrono::duration<double, std::milli>(diff).count() << "ms" << std::endl;

//     // Other posiible conversion (std::ratio)   <num, den>
//     // std::chrono::duration<double, std::pico>  <1, 1000000000000>
//     // std::chrono::duration<double, std::nano>  <1, 1000000000>
//     // std::chrono::duration<double, std::micro> <1, 1000000>
//     // std::chrono::duration<double, std::milli> <1, 1000>
//     // std::chrono::duration<double, std::centi> <1, 100>
//     // std::chrono::duration<double, std::deci>  <1, 10>
//     // std::chrono::duration<double, std::deca>  <10, 1>



/*



std::vector<int> square_sums_row(int n)
{
 

 */