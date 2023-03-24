#include "equal-paths.h"
#include <iostream>
using namespace std;

// You may add any prototypes of helper functions here
int funcEqualPaths(Node* root);

bool equalPaths(Node * root)
{
    // Add your code below
    if (funcEqualPaths(root) <= -1) return false;
    return true;
}

int funcEqualPaths(Node* root) {
  if (root == NULL) return 0;

int leftside = root->left ? funcEqualPaths(root->left) + 1 : 0;
int rightside = root->right ? funcEqualPaths(root->right) + 1 : 0;

   if (leftside && rightside) {
    return (leftside == rightside) ? leftside : -1000;
}  
    else if (rightside == 0 && leftside != 0) return leftside;
    else if (leftside == 0 && rightside != 0) return rightside;
    else if (rightside == 0 && leftside == 0) return 0;


	return -1;
}