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
    std::vector<int> test_error1 = {9, 7, 2, 14, 11, 5, 4, 12, 13, 3, 6, 10, 8};         // Less elements
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

using timer = std::chrono::steady_clock;

struct Node {
    int value = -1;
    int idx = -1;
    std::vector<int> compVec;
    std::unordered_set<int> usedCompNum;
};

// struct Number {
//     int value = -1;  
//     Number* right = nullptr;
//     Number* left = nullptr;
//     std::vector<int> cmpVec = {};
//     std::unordered_set<int> cmpAlredyUsed = {};
//     int idx = -1;
// };

bool check(std::vector<int>& vec, int n);
std::vector<int> square_sums_row(int n);
void display(std::vector<int>& vec);
void displayComplementary(std::vector<Node>& compVec);

int main(int argc, char** argv ) {
    //const std::vector<int> setOfN = {25, 50, 75, 100, 250, 500, 750, 1000};
    const std::vector<int> setOfN = {50};
    // std::vector<int> setOfN;
    // for(int i = 2; i < 72; ++i) {
    //     setOfN.push_back(i);
    // }
    timer::time_point testStart = timer::now();
    for(int n : setOfN) {
        std::cout << "====== Test N(" << n << ") ======" << std::endl;
        timer::time_point start = timer::now();
        std::vector<int> vec = square_sums_row(n);
        timer::time_point end = timer::now();
        check(vec, n);
        //display(vec);
        std::chrono::duration<double> timeDelta = end-start;
        //std::cout << "Function took: " << std::chrono::duration<double, std::micro>(timeDelta).count() << " micro seconds" << std::endl;
        std::cout << "Function took: " << std::chrono::duration<double, std::milli>(timeDelta).count() << " ms" << std::endl;
    } 
    timer::time_point testEnd = timer::now();
    std::chrono::duration<double> testDelta = testEnd-testStart;
    std::cout << "==================" << std::endl << "Whole test took: " << (std::chrono::duration<double, std::milli>(testDelta).count() / 1000.0) << " s" << std::endl;
    return 0;
}   

std::vector<int> square_sums_row(int n) {
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
    int currentNode = 1;
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

        if(num != currentNode && compCount < nodeMatrix[currentNode-1].compVec.size()) {
            currentNode = num;
        }
    }
    // === Display complementary vector === 
    // displayComplementary(nodeMatrix);

    // === Finding result series ===
    nodeMatrix[currentNode-1].idx = 0;
    std::vector<int> resultSeries = {currentNode};
    resultSeries.reserve(n);
    int lastNode = currentNode;
    std::vector<int> junctionPoints;

    for(int index = 1; index < n; ++index) {
        int nextNodeSize = n;
        int nextNode = -1;
        
        Node& node = nodeMatrix[currentNode-1];
        bool junctionPoint = false;
        for(int cmpNum : node.compVec) {
            if(lastNode != cmpNum && nodeMatrix[cmpNum-1].idx == -1 && node.usedCompNum.count(cmpNum) == 0) {
                if(nextNode != -1 && !junctionPoint ) {
                    junctionPoint = true;
                    junctionPoints.push_back(node.value);
                }

                if(nodeMatrix[cmpNum-1].compVec.size() < nextNodeSize) {
                    nextNodeSize = nodeMatrix[cmpNum-1].compVec.size();
                    nextNode = cmpNum;
                }
            }
        }

        // Set new current node or brake 
        if(nextNode != -1) {
            node.usedCompNum.insert(nextNode);
            lastNode = currentNode;
            currentNode = nextNode;
            nodeMatrix[currentNode-1].idx = resultSeries.size();
            resultSeries.push_back(currentNode);
        }
        else {
            if(junctionPoints.size() > 0) {
                // display(resultSeries);
                
                // No next move, backtracking.
                --index;
                Node& junction = nodeMatrix[junctionPoints.back()-1];
                for(; index > junction.idx; --index) {
                    Node& nodeToDelete = nodeMatrix[resultSeries.back()-1];
                    nodeToDelete.idx = -1;
                    nodeToDelete.usedCompNum.clear();
                    resultSeries.pop_back();
                }
                currentNode = junction.value;
                lastNode = resultSeries[junction.idx - 1];
                junctionPoints.pop_back();
            }
            else {
                return {};
            }
        }
    }
    return resultSeries;

    return {};
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
