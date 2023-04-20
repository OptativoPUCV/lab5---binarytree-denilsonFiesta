#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {

  TreeMap* new = (TreeMap*)calloc(1, sizeof(TreeMap));
    
  new->lower_than = lower_than;
  return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
  //holi UwU

  TreeNode* it = tree->root;
  TreeNode* parent;
  
  if(it==NULL){  
    TreeNode* new =  createTreeNode(key, value);
    tree->root = new;
    return ;
  }
  
  while(1){

    if(is_equal(tree, it->pair->key, key)) return;
    parent = it;
    if( ! tree->lower_than(it->pair->key,key) ){
      
      it = it->left;
      
      if(it == NULL){
        TreeNode* new =  createTreeNode(key, value);
        parent->left = new;
        new->parent = parent;
        tree->current = new;
        return;
      }
    }else{
      it = it->right;
      
      if(it == NULL){
        TreeNode* new =  createTreeNode(key, value);
        parent->right = new;
        new->parent = parent;
        tree->current = new;
        return;
      }
    } 
  }

  
}

TreeNode * minimum(TreeNode * x){
  if(x->left == NULL) return x;
  else return minimum(x->left);
}


void removeNode(TreeMap * tree, TreeNode* node) {

  if(node == NULL) return;

  if(node->left == NULL && node->right == NULL) {

    if(node->parent->right->pair->key == node->pair->key){
        node->parent->right =  NULL;
    }else node->parent->left = NULL;
    
    free(node);
    
  }else  if(node->left != NULL && node->right != NULL){
    
    TreeNode* iyou = minimum(node->right);
    node->pair = iyou->pair;        
    free(iyou);
    
  }else if(node->left != NULL){

    node->pair = node->left->pair;
    free(node->left);
  }else{
    node->pair = node->right->pair;
    free(node->right);
  }
  
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {

  TreeNode* it = tree->root;

  while(it!= NULL){
    if(is_equal(tree, it->pair->key, key)){
      tree->current = it;
      return it->pair;
    }

    if( ! tree->lower_than(it->pair->key,key) ){
        it = it->left; 
    }else it = it->right;
  
  }
  
  return NULL;
}

TreeNode * firstUp(TreeMap* tree, TreeNode * node, TreeNode* parent){
  if(parent == NULL) return NULL;
  if(!tree->lower_than(node->pair->key ,parent->pair->key )) return firstUp(tree, node, parent->parent);
  else return parent;
}

Pair * upperBound(TreeMap * tree, void* key) {

  TreeNode* it = tree->root;

  while(it != NULL){

    if(is_equal(tree, it->pair->key, key)){
      tree->current = it;
      return it->pair;
    }
    
    if( ! tree->lower_than(it->pair->key,key) ){

      if(it->left == NULL) return it->pair;
      
      it = it->left; 
    
    }else{

      if(it->right == NULL){
        
        if(firstUp(tree, it, it->parent)==NULL){
          
          return it->pair;
        
        }else{
          printf("llegue aca\n");
          return (firstUp(tree, it, it->parent) ) ->pair;
        }

      }
      
      it = it->right;
    }
  }
  
  return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
  TreeNode* it = tree->root;
  if(it == NULL) NULL; 
  tree->current = minimum(it);
  return tree->current->pair;
}



Pair * nextTreeMap(TreeMap * tree) {
  if(tree->current->right != NULL){
    tree->current = minimum(tree->current->right);
  }else{
    tree->current = firstUp(tree, tree->current, tree->current->parent);  
  }
  if(tree->current == NULL) return NULL;
  else return tree->current->pair;
}
