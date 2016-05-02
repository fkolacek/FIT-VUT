#include "btree.h"

void BT_Init(struct_BTree* tree){
	if(!tree)
		return;
	
	tree->root = NULL;
	tree->last = NULL;
}

void BT_Free(struct_BTree* tree){
	if(!tree)
		return;

	if(!tree->root)
		return;
	
	recursive_Node_Delete(tree->root);
	
	BT_Init(tree);
}

void recursive_Node_Delete(struct_BTree_Node node){
	if(!node)
		return;
	
	if(node->left != NULL)
		recursive_Node_Delete(node->left);
	if(node->right != NULL)
		recursive_Node_Delete(node->right);
	
	gcFree(node->data);
	gcFree(node);
}


struct_BTree_Node BT_Search(struct_BTree* tree, STRING_PTR key)
{
	return recursive_Node_Search(tree->root, key);
}

struct_BTree_Node recursive_Node_Search(struct_BTree_Node node, STRING_PTR key)
{
	if(node == NULL)
        return NULL;
	else if(strCompareStrings(*node->key,*key))
		return node;
	else if (strSize(node->key) > strSize(key))
		return recursive_Node_Search(node->left, key);
	else
		return recursive_Node_Search(node->right, key);
}


ERROR BT_Insert(struct_BTree* tree, STRING_PTR key, void* data)
{
	if (tree->root == NULL)
	{
	    tree->root = gcMalloc(sizeof(struct struct_BTree_Node));
		tree->root->key = key;
		tree->root->left = NULL;
		tree->root->right = NULL;
		tree->root->data = data;
		tree->last = tree->root;
		return E_OK;
	}

    struct_BTree_Node temp_node = tree->root;

	while(temp_node != NULL)
	{
        if ( strSize(key) < strSize(temp_node->key))
		{
            if(temp_node->left == NULL)
			{
				temp_node->left = gcMalloc(sizeof(struct struct_BTree_Node));
				temp_node->left->key = key;
				temp_node->left->left = NULL;
				temp_node->left->right = NULL;
				temp_node->left->data = data;
				tree->last = temp_node->left;
				return E_OK;
            }
            else temp_node=temp_node->left;
        }
        else if (strSize(key) >= strSize(temp_node->key))
		{
            if (temp_node->right == NULL)
			{
				temp_node->right = gcMalloc(sizeof(struct struct_BTree_Node));
				temp_node->right->key = key;
				temp_node->right->left = NULL;
				temp_node->right->right = NULL;
				temp_node->right->data = data;
				tree->last = temp_node->right;
				return E_OK;
            }
            else temp_node = temp_node->right;
        }
        else
		{
			tree->last = temp_node;
			return E_OK;
		}
    }
    return E_OK;
}
