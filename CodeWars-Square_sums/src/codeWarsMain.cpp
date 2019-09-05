#include <vector>
#include <math.h>
#include <unordered_set>
std::vector<std::vector<std::pair<int, int>>>rules={
{},
{},
{},
{},
{},
{},
{},
{},
{},
{},
{},
{},
{},
{},
{},
{},
{},
{},
{},
{},
{},
{},
{},
{},
{},
{},
{},
{},
{},
{},
{{16,9},{22,3},{3,6}},
{},
{{14,2}},
{},
{},
{},
{},
{{22,3},{3,1},{1,8},{8,17}},
{},
{},
{{40,9},{7,2}},
{},
{},
{{40,9},{11,5}},
{{40,9},{11,5},{26,10},{15,1}},
{},
{},
{},
{},
{{41,8},{14,2},{1,24},{9,16},{7,18}},
{},
{{28,8}},
{},
{},
{},
{},
{},
{{32,4}},
{},
{},
{{6,10}},
{},
{{28,21}},
{},
{},
{},
{},
{},
{{20,16}},
{{20,16}},
{{11,25},{20,5},{50,14},{15,1},{68,32},{32,4},{21,15},{1,3}},
{{56,8},{19,6},{30,51},{24,1}},
{{37,12},{12,4},{2,14}},
{{16,9}},
{{12,24}},
{{56,8}},
{},
{{22,3}},
{{15,21},{17,8},{33,3}},
{{14,2}},
{{30,6},{23,2}},
{},
{},
{{14,2}},
{{15,21},{19,6},{14,22}},
{},
{{15,10}},
{{20,5}},
{{20,5}},
{{8,1}},
{},
{{25,39}},
{},
{{24,12},{74,7},{7,2},{2,14},{11,5},{15,10}},
{{49,51},{26,38},{28,8},{8,1},{1,3},{3,6},{6,10}},
{{49,51},{26,38},{28,8},{8,1},{1,3},{3,6},{6,10}},
{},
{{37,12}},
{},
{{48,16},{16,9},{9,27},{35,1},{1,15},{10,6}},
{},
{},
{{15,21}},
{{34,2}},
{{7,2},{23,13},{38,11}},
{},
{},
{{44,37},{8,1},{11,5},{17,19}},
{{87,82},{52,29},{20,5},{5,11}},
{{56,8},{8,1},{52,12},{3,22},{11,5}},
{{4,21},{21,15},{22,3},{3,6}},
{{47,2},{23,13},{13,3},{3,22},{29,7},{9,16},{16,20}},
{{26,10},{28,8}},
{{24,1},{5,20},{16,9}},
{{36,13},{13,12},{39,25},{8,1},{1,3},{58,6}},
{{47,2},{21,28},{8,1},{1,24},{24,25},{9,16}},
{{60,21},{33,16},{7,2}},
{{48,1},{28,8},{10,6}},
{{22,3},{36,28},{28,8},{1,15},{4,5},{10,6}},
{},
{{7,2},{2,23},{26,10},{15,21},{21,4},{4,5},{11,25},{45,19},{30,6}},
{},
{},
{},
{},
{{6,3}},
{},
{{62,19},{43,38}},
{{59,22},{22,3},{43,3},{43,6},{6,30},{62,2},{2,14},{58,42},{1,24},{12,4}},
{{21,15}},
{},
{},
{},
{{28,36},{55,9},{32,4}},
{{20,5},{31,18}},
{{31,5}},
{{29,20},{20,5},{34,2},{21,4},{4,12},{13,36}},
{{2,12},{52,12},{17,8}},
{{15,21},{21,4},{30,6},{17,8},{8,1},{20,5},{11,25}},
{{116,80},{51,30},{30,19},{19,6},{34,2},{4,21}},
{{38,26},{55,9},{78,22},{14,11},{10,6},{6,30},{30,19},{4,5},{1,8}},
{{73,8},{8,17},{17,32},{35,1},{30,19},{10,15},{15,21},{21,4},{4,5},{22,14},{11,38}},
{{40,9}},
{{47,2}},
{{111,114},{39,25},{40,9},{20,5}},};
struct Node{int value=-1;int idx=-1;std::vector<int> compVec; std::unordered_set<int> usedCompNum;};
std::vector<int> square_sums_row(int n) {
if(n<15){return {};}  
std::vector<std::pair<int, int>>& rulesVec = rules[n];
int base = sqrt(n + (n - 1));std::vector<int> squareVec;squareVec.reserve(base);
for (;base>1;base--){squareVec.push_back(base * base);}
int c_n = 1;std::vector<Node> nodeMatrix;nodeMatrix.reserve(n);
for(int num = 1;num<=n;++num) {
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
  }
  return resultSeries;
}