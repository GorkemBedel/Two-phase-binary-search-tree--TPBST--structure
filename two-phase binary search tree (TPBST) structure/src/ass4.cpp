//============================================================================
// Name        : ass4.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <bits/stdc++.h>

using namespace std;


vector<vector<string> > read_input_file(string s);



#define SPACE 10



/*struct AVL_TreeNode {
	string secondary_key; //name
	AVL_TreeNode * left;
	AVL_TreeNode * right;
	int data; //price
};*/

class AVL_TreeNode {
public:
	string secondary_key; //name
	AVL_TreeNode * left;
	AVL_TreeNode * right;
	int data; //price

	AVL_TreeNode(string& secondary_keyy, int& data) : secondary_key(secondary_keyy), left(NULL), right(NULL), data(data) {}



};

class AVLTree {
  public:
    AVL_TreeNode * root;
  AVLTree() {
    root = NULL;
  }
  bool isTreeEmpty() {
    if (root == NULL) {
      return true;
    } else {
      return false;
    }
  }

  // Get Height
  int height(AVL_TreeNode * r) {
    if (r == NULL)
      return -1;
    else {
      /* compute the height of each subtree */
      int lheight = height(r -> left);
      int rheight = height(r -> right);

      /* use the larger one */
      if (lheight > rheight)
        return (lheight + 1);
      else return (rheight + 1);
    }
  }

  // Get Balance factor of node N
  int getBalanceFactor(AVL_TreeNode * n) {
    if (n == NULL)
      return -1;
    return height(n -> left) - height(n -> right);
  }

  AVL_TreeNode * rightRotate(AVL_TreeNode * y) {
    AVL_TreeNode * x = y -> left;
    AVL_TreeNode * T2 = x -> right;

    // Perform rotation
    x -> right = y;
    y -> left = T2;

    return x;
  }

  AVL_TreeNode * leftRotate(AVL_TreeNode * x) {
    AVL_TreeNode * y = x -> right;
    AVL_TreeNode * T2 = y -> left;

    // Perform rotation
    y -> left = x;
    x -> right = T2;

    return y;
  }

  AVL_TreeNode * insert(AVL_TreeNode * r, AVL_TreeNode * new_node) {

    if (r == NULL) {


      r = new_node;
      //cout << new_node->secondary_key<<" secondary_key inserted successfully" << endl;
      return r;
    }


    if (lexicographical_compare(r->secondary_key.begin(), r->secondary_key.end(), new_node->secondary_key.begin(), new_node->secondary_key.end()) == 1 ){
    	r -> left = insert(r -> left, new_node);
    } else if (lexicographical_compare(r->secondary_key.begin(), r->secondary_key.end(), new_node->secondary_key.begin(), new_node->secondary_key.end()) == 0) {
      r -> right = insert(r -> right, new_node);
    } else {
      cout << "No duplicate values allowed!" << endl;
      return r;
    }
    int bf = getBalanceFactor(r);



    // Left Left Case
    if (bf > 1 && (lexicographical_compare(r->left->secondary_key.begin(), r->left->secondary_key.end(), new_node->secondary_key.begin(), new_node->secondary_key.end()) == 1)){
    	return rightRotate(r);


  	}
    // Right Right Case
    else if (bf < -1 && (lexicographical_compare(r->right->secondary_key.begin(), r->right->secondary_key.end(), new_node->secondary_key.begin(), new_node->secondary_key.end()) == 0)){
    	return  leftRotate(r);
    }
    // Left Right Case
    else if (bf > 1 && (lexicographical_compare(r->left->secondary_key.begin(), r->left->secondary_key.end(), new_node->secondary_key.begin(), new_node->secondary_key.end()) == 0) ){
    	r -> left = leftRotate(r -> left);
    	return rightRotate(r);
    }
    // Right Left Case
    else if (bf < -1 && (lexicographical_compare(r->right->secondary_key.begin(), r->right->secondary_key.end(), new_node->secondary_key.begin(), new_node->secondary_key.end()) == 1) ){
    	r -> right = rightRotate(r -> right);
    	return leftRotate(r);
    }

    /* return the (unchanged) node pointer */
    return r;
  }

  AVL_TreeNode * recursiveSearch(AVL_TreeNode* r, string val) {
     if (r == NULL || r -> secondary_key == val){
       return r;

     }else if (lexicographical_compare(r->secondary_key.begin(), r->secondary_key.end(),val.begin(), val.end()) == 1 ){
       return recursiveSearch(r -> left, val);
     }else{
       return recursiveSearch(r -> right, val);
  	  }
   }

  AVL_TreeNode * minValueNode(AVL_TreeNode * node) {
    AVL_TreeNode * current = node;
    /* loop down to find the leftmost leaf */
    while (current -> left != NULL) {
      current = current -> left;
    }
    return current;
  }

  AVL_TreeNode * deleteNode(AVL_TreeNode * r, string v) {
    // base case
    if (r == NULL) {
      return NULL;
    }
    // If the key to be deleted is smaller than the root's key,
    // then it lies in left subtree
    else if (v < r -> secondary_key) {

      r -> left = deleteNode(r -> left, v);
    }
    // If the key to be deleted is greater than the root's key,
    // then it lies in right subtree
    else if (v > r -> secondary_key) {

      r -> right = deleteNode(r -> right, v);
    }
    // if key is same as root's key, then This is the node to be deleted
    else {

      // node with only one child or no child
      if (r -> left == NULL) {
        AVL_TreeNode * temp = r -> right;
        delete r;
        r->left = NULL;
        r->right = NULL;
        r = NULL;
        return temp;
      } else if (r -> right == NULL) {
        AVL_TreeNode * temp = r -> left;
        delete r;
        r->left = NULL;
        r->right = NULL;
        r = NULL;
        return temp;
      } else {
        // node with two children: Get the inorder successor (smallest
        // in the right subtree)
        AVL_TreeNode * temp = minValueNode(r -> right);
        // Copy the inorder successor's content to this node
        r -> secondary_key = temp -> secondary_key;
        // Delete the inorder successor
        r -> right = deleteNode(r -> right, temp -> secondary_key);
        //deleteNode(r->right, temp->secondary_key);
      }
    }


    int bf = getBalanceFactor(r);
    // Left Left Imbalance/Case or Right rotation
    if (bf == 2 && getBalanceFactor(r -> left) >= 0)
      return rightRotate(r);
    // Left Right Imbalance/Case or LR rotation
    else if (bf == 2 && getBalanceFactor(r -> left) == -1) {
      r -> left = leftRotate(r -> left);
      return rightRotate(r);
    }
    // Right Right Imbalance/Case or Left rotation
    else if (bf == -2 && getBalanceFactor(r -> right) <= -0)
      return leftRotate(r);
    // Right Left Imbalance/Case or RL rotation
    else if (bf == -2 && getBalanceFactor(r -> right) == 1) {
      r -> right = rightRotate(r -> right);
      return leftRotate(r);
    }

    return r;
  }

  void print2D(AVL_TreeNode * r, int space) {
    if (r == NULL) // Base case  1
      return;
    space += SPACE; // Increase distance between levels   2
    print2D(r -> right, space); // Process right child first 3
    cout << endl;
    for (int i = SPACE; i < space; i++) // 5
      cout << " "; // 5.1
    cout << r -> secondary_key << "\n"; // 6
    print2D(r -> left, space); // Process left child  7
  }
  void printPreorder(AVL_TreeNode * r) //(current node, Left, Right)
  {
    if (r == NULL)
      return;
    /* first print data of node */
    cout << r -> secondary_key << " ";
    /* then recur on left sutree */
    printPreorder(r -> left);
    /* now recur on right subtree */
    printPreorder(r -> right);
  }

  void printInorder(AVL_TreeNode * r) //  (Left, current node, Right)
  {
    if (r == NULL)
      return;
    /* first recur on left child */
    printInorder(r -> left);
    /* then print the data of node */
    cout << r -> secondary_key << " ";
    /* now recur on right child */
    printInorder(r -> right);
  }
  void printPostorder(AVL_TreeNode * r) //(Left, Right, Root)
  {
    if (r == NULL)
      return;
    // first recur on left subtree
    printPostorder(r -> left);
    // then recur on right subtree
    printPostorder(r -> right);
    // now deal with the node
    cout << r -> secondary_key << " ";
  }

  /* Print nodes at a given level */
  void printGivenLevel(AVL_TreeNode * r, int level,fstream &outputfile ) {
    if (r == NULL)
      return;
    else if (level == 0){

    	//cout<<"\t"<<"\""<<r->secondary_key<<"\":"<<"\""<<r->data<<"\""<<",";
    	outputfile<<"\t"<<"\""<<r->secondary_key<<"\":"<<"\""<<r->data<<"\""<<",";


    }else{ // level > 0

      printGivenLevel(r -> right, level - 1,outputfile);
      printGivenLevel(r -> left, level - 1,outputfile);
    }
  }

  void printLevelOrderBFS(AVL_TreeNode * r,fstream &outputfile ) {
    int h = height(r);
    for (int i = 0; i <= h; i++){
    	printGivenLevel(r, i,outputfile);
    	//cout<<endl;
    	outputfile<<endl;

    }
  }

  /*AVL_TreeNode * iterativeSearch(int v) {
    if (root == NULL) {
      return root;
    } else {
      AVL_TreeNode * temp = root;
      while (temp != NULL) {
        if (v == temp -> secondary_key) {
          return temp;
        } else if (v < temp -> secondary_key) {
          temp = temp -> left;
        } else {
          temp = temp -> right;
        }
      }
      return NULL;
    }
  }

  AVL_TreeNode * recursiveSearch(AVL_TreeNode * r, string val) {
    if (r == NULL || r -> secondary_key == val){
      return r;
    }else if (val < r -> secondary_key){
    	return recursiveSearch(r -> left, val);
    }else{
	  return recursiveSearch(r -> right, val);
  }
  }*/

};

class Node {
 public:
  string category;
  Node* left;
  Node* right;
  AVL_TreeNode* AVL_ptr;
  AVLTree* avl_tree;

  Node(const string& category) : category(category), left(NULL), right(NULL), AVL_ptr(NULL), avl_tree(NULL) {}
};

class BinarySearchTree {
 public:
  Node* root;

  BinarySearchTree() : root(NULL) {}

  void insert(BinarySearchTree* tree,const string& category, string& secondary_key, int price) {
    if (root == NULL) {
      root = new Node(category);
      root->avl_tree = new AVLTree;
      AVL_TreeNode* avl_root_node = new AVL_TreeNode(secondary_key, price);
      //avl_root_node->secondary_key = secondary_key;
      //avl_root_node->data = price;
      root->avl_tree->root = avl_root_node;
    } else {
      insertHelper(tree,root, category,secondary_key,price);
    }
  }


  void print() {
      printHelper(root);
    }

  Node* search(const std::string& data) {
      return searchHelper(root, data);
    }


 private:
  void insertHelper(BinarySearchTree* tree,Node* node, const string& category, string& secondary_key, int price) {

	Node* nodeptr = tree->search(category);
	if(nodeptr == NULL ){
    if (category < node->category) {
      if (node->left == NULL) {
    	  	node->left = new Node(category);
        	node->left->avl_tree = new AVLTree;
        	AVL_TreeNode* avl_root_node = new AVL_TreeNode(secondary_key,price);
        	avl_root_node->secondary_key = secondary_key;
        	avl_root_node->data = price;
        	node->left->avl_tree->root = avl_root_node;


      } else {
        insertHelper(tree,node->left,category,secondary_key,price);
      }
    }
    else {
      if (node->right == NULL) {
    	  	node->right = new Node(category);
        	node->right->avl_tree = new AVLTree;
        	AVL_TreeNode* avl_root_node = new AVL_TreeNode(secondary_key, price);
        	avl_root_node->secondary_key = secondary_key;
        	avl_root_node->data = price;
        	node->right->avl_tree->root= avl_root_node;

      }
      else {
        insertHelper(tree,node->right, category,secondary_key,price);
      }
    }
	}else if(nodeptr != NULL){

    	//AVL_TreeNode* avl_node = new AVL_TreeNode;
    	//avl_node->secondary_key = secondary_key;
    	//avl_node->data = price;
    	nodeptr->avl_tree->root =  nodeptr->avl_tree->insert(nodeptr->avl_tree->root,new AVL_TreeNode(secondary_key,price));






	}


  }

  void printHelper(Node* node) {
      if (node == NULL) {
        return;
      }
      std::cout << node->category << " ";
      printHelper(node->left);
      printHelper(node->right);
    }

  Node* searchHelper(Node* node, const std::string& data) {
     if (node == NULL) {
       return NULL;
     }
     if (node->category == data) {
       return node;
     }
     if (data < node->category) {
       return searchHelper(node->left, data);
     } else {
       return searchHelper(node->right, data);
     }
   }

};


void updateData(BinarySearchTree* &tree, vector<vector<string> >& vect,int i );
void printItem(BinarySearchTree* &tree,vector<vector<string> >& vect,int i,fstream &outputfile );
void find(BinarySearchTree* &tree,vector<vector<string> >& vect,int i ,fstream &outputfile);
void printAllItemsInCategory(BinarySearchTree* &tree, vector<vector<string> > &vect,int i,fstream &outputfile);
void remove(BinarySearchTree* &tree, vector<vector<string> > &vect,int i);
void insert(BinarySearchTree* &tree, vector<vector<string> > &vect,int i);
void printAllItemsInKnownCategory(BinarySearchTree* &tree, vector<vector<string> > &vect,Node* knownNode,fstream &outputfile);


int main(int argc, char** argv) {

	string input_file = argv[1];
	string output_txt_1 = argv[2];
	string output_txt_2 = argv[3];

	BinarySearchTree* tree = new BinarySearchTree;
	vector<string> insert_vect;
	fstream myFile;
	myFile.open(output_txt_1.c_str(), ios::out);

	vector<vector<string> > vect = read_input_file(input_file);
	int size_of_input = vect.size();


	for(int i = 0; i < size_of_input; i++){



		if(vect[i][0] == "insert"){
			bool x = false;
			int size = insert_vect.size();
			for(int j = 0; j < size; j++){
				if(insert_vect[j] == vect[i][1]){
					x = true;
				}
			}
			if(x == false ){
				insert_vect.push_back(vect[i][1]);
			}
			insert(tree, vect, i);
		}
		if(vect[i][0] == "remove"){
			string primary_key = vect[i][1];
			string secondary_key = vect[i][2];
			Node* removed_main_node = tree->search(primary_key);
			AVL_TreeNode* removed_secondery_node =  removed_main_node->avl_tree->recursiveSearch(removed_main_node->avl_tree->root, secondary_key);
			removed_main_node->avl_tree->root = removed_main_node->avl_tree->deleteNode(removed_main_node->avl_tree->root, removed_secondery_node->secondary_key);
		}
		if(vect[i][0] == "printAllItems"){
				//cout<<"command:printAllItems"<<endl;
				myFile<<"command:printAllItems"<<endl;

				//cout<<"{"<<endl;
				myFile<<"{"<<endl;

				int size = insert_vect.size();
				for(int j = 0; j < size; j++){
					Node* found_main_node = tree->search(insert_vect.at(j));
					printAllItemsInKnownCategory(tree, vect, found_main_node,myFile);
				}
				//cout<<"}"<<endl;
				myFile<<"}"<<endl;


		}
		if(vect[i][0] == "printAllItemsInCategory"){
			printAllItemsInCategory(tree, vect, i,myFile);
		}
		if(vect[i][0] == "printItem"){
			printItem(tree, vect, i,myFile);
		}
		if(vect[i][0] == "find"){
			find(tree, vect, i,myFile);
		}
		if(vect[i][0] == "updateData"){
			updateData(tree, vect, i);
		}
	}



	cout<<endl<<"görkem";

	myFile.close();
	return 0;
}

vector<vector<string> > read_input_file(string s){
	fstream MyReadFile;
	string lines;
	vector<vector<string> > mainvect;
	vector<string> vect;
	MyReadFile.open(s.c_str(), ios::in);
	if(MyReadFile.is_open()){
		while(getline(MyReadFile,lines)){
			vector<string> vect;
			stringstream iss(lines);
			string columns;
			while(getline(iss,columns,'\t')){
				vect.push_back(columns);
			}
		mainvect.push_back(vect);

	}
	}

	return mainvect;

}

void printItem(BinarySearchTree* &tree,vector<vector<string> >& vect,int i,fstream &outputfile){
	string primary_key = vect[i][1];
	string secondary_key = vect[i][2];
	//cout<<"command:printItem->"<<primary_key<<"->"<<secondary_key<<endl;
	outputfile<<"command:printItem->"<<primary_key<<"->"<<secondary_key<<endl;

	Node* found_main_node = tree->search(primary_key);
	AVL_TreeNode* found_secondery_node =  found_main_node->avl_tree->recursiveSearch(found_main_node->avl_tree->root, secondary_key);
	//cout<<"{"<<endl;
	outputfile<<"{"<<endl;

	//cout<<"\""<<found_main_node->category<<"\":"<<endl;
	outputfile<<"\""<<found_main_node->category<<"\":"<<endl;


	//cout<<"\t"<<"\""<<found_secondery_node->secondary_key<<"\":"<<" \""<<found_secondery_node->data<<" \""<<endl;
	outputfile<<"\t"<<"\""<<found_secondery_node->secondary_key<<"\":"<<" \""<<found_secondery_node->data<<" \""<<endl;

	//cout<<"}"<<endl;
	outputfile<<"}"<<endl;

	//cout<<endl;
	outputfile<<endl;

}

void find(BinarySearchTree* &tree,vector<vector<string> >& vect,int i,fstream &outputfile ){
	string primary_key = vect[i][1];
	string secondary_key = vect[i][2];
	//cout<<"command:find->"<<primary_key<<"->"<<secondary_key<<endl;
	outputfile<<"command:find->"<<primary_key<<"->"<<secondary_key<<endl;



	Node* found_main_node = tree->search(primary_key);
	if(found_main_node == NULL){
			outputfile<<"{}"<<endl;
			return;
		}
	AVL_TreeNode* found_secondery_node =  found_main_node->avl_tree->recursiveSearch(found_main_node->avl_tree->root, secondary_key);

	if(found_secondery_node == NULL){
		//cout<<"{}"<<endl;
		outputfile<<"{}"<<endl;
		return;

	}else{

		//cout<<"{"<<endl;
		outputfile<<"{"<<endl;

		//cout<<"\""<<found_main_node->category<<"\":"<<endl;
		outputfile<<"\""<<found_main_node->category<<"\":"<<endl;

		//cout<<"\t"<<"\""<<found_secondery_node->secondary_key<<"\":"<<" \""<<found_secondery_node->data<<" \""<<endl;
		outputfile<<"\t"<<"\""<<found_secondery_node->secondary_key<<"\":"<<" \""<<found_secondery_node->data<<" \""<<endl;

		//cout<<"}"<<endl;
		outputfile<<"}"<<endl;

	}
	//cout<<endl;
	outputfile<<endl;


}

void updateData(BinarySearchTree* &tree, vector<vector<string> > &vect,int i ){
	string primary_key = vect[i][1];
	string secondary_key = vect[i][2];
	string string_price = vect[i][3];
	int price;
	stringstream converter(string_price);
	converter >> price;
	Node* found_main_node = tree->search(primary_key);
	AVL_TreeNode* found_secondery_node =  found_main_node->avl_tree->recursiveSearch(found_main_node->avl_tree->root, secondary_key);
	found_secondery_node->data = price;
}

void printAllItemsInCategory(BinarySearchTree* &tree, vector<vector<string> > &vect,int i,fstream &outputfile ){
	string primary_key = vect[i][1];
	Node* found_main_node = tree->search(primary_key);
	//cout<<"command:printAllItemsInCategory->"<<primary_key<<endl;
	outputfile<<"command:printAllItemsInCategory->"<<primary_key<<endl;

	//cout<<"{"<<endl;
	outputfile<<"{"<<endl;

	if(found_main_node->avl_tree->root != NULL){
		//cout<<"\""<<primary_key<<"\":"<<endl;
		outputfile<<"\""<<primary_key<<"\":"<<endl;

		found_main_node->avl_tree->printLevelOrderBFS(found_main_node->avl_tree->root,outputfile);
	}else{
		//cout<<"\""<<primary_key<<"\":";
		outputfile<<"\""<<primary_key<<"\":";

		//cout<<"{}"<<endl;
		outputfile<<"{}"<<endl;

	}
	//cout<<"}"<<endl;
	outputfile<<"}"<<endl;

	//cout<<endl;
	outputfile<<endl;

}

void printAllItemsInKnownCategory(BinarySearchTree* &tree, vector<vector<string> > &vect,Node* knownNode,fstream &outputfile){
	if(knownNode->avl_tree->root != NULL){
		//cout<<"\""<<knownNode->category<<"\":"<<endl;
		outputfile<<"\""<<knownNode->category<<"\":"<<endl;

		knownNode->avl_tree->printLevelOrderBFS(knownNode->avl_tree->root,outputfile);
	}else{
		//cout<<"\""<<knownNode->category<<"\":";
		outputfile<<"\""<<knownNode->category<<"\":";

		//cout<<"{}"<<endl;
		outputfile<<"{}"<<endl;

	}
	//cout<<"}"<<endl;
	outputfile<<"}"<<endl;

	//cout<<endl;
	outputfile<<endl;

}

void remove(BinarySearchTree* &tree, vector<vector<string> > &vect,int i ){
	string primary_key = vect[i][1];
	string secondary_key = vect[i][2];

	Node* removed_main_node = tree->search(primary_key);
	AVL_TreeNode* removed_secondery_node =  removed_main_node->avl_tree->recursiveSearch(removed_main_node->avl_tree->root, secondary_key);
	removed_main_node->avl_tree->root = removed_main_node->avl_tree->deleteNode(removed_main_node->avl_tree->root, removed_secondery_node->secondary_key);
	removed_secondery_node->data = 0;
	removed_secondery_node->secondary_key = "";
}

void insert(BinarySearchTree* &tree, vector<vector<string> > &vect,int i){
	string primary_key = vect[i][1];
	string secondary_key = vect[i][2];
	string string_price = vect[i][3];
	int price;
	stringstream converter(string_price);
	converter >> price;
	tree->insert(tree,primary_key, secondary_key, price);
}
