#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーからほとんど使用されていない部分を除外する
#include "Windows.h"                    // Windows API の機能定義
#include <stdlib.h>

#include "../include/lib_func.h"


// 2分木の初期化
void initialize(tree* t)
{
	if (t == NULL) return;

	t->root = NULL;
}

static void release_recursive(node* n)
{
	if (n == NULL) return;

	if (n->right) {
		release_recursive(n->right);
		n->right = NULL;
	}

	if (n->left) {
		release_recursive(n->left);
		n->left = NULL;
	}

	free(n);
}

// 使用メモリの全解放
void finalize(tree* t)
{
	if (t == NULL) return;

	release_recursive(t->root);
	t->root = NULL;
}


static node* generate(int key, const char* value)
{
	node* p = (node*)malloc(sizeof(node));

	if (p == NULL) return NULL;

	p->key = key;
	int n = (int)strlen(value);
	memcpy(p->value, value, strlen(value)+1);

	p->left = p->right = NULL;

	return p;
}

// keyの値を見てノードを追加する
bool add(tree* t, int key, const char* value)
{
	if (t == NULL) return false;

	node* p = generate(key, value);
	if (p == NULL) return false;// メモリ確保できなかった。

	if (t->root == NULL) {
		t->root = p;
		return true;
	}

	node* com = t->root;

	while (true)
	{
		if (com == NULL)
		{
			free(p);
			return false;
		}

		if (key == com->key)
		{
			memcpy(com->value, value, strlen(value) + 1);
			free(p);
			return true;
		}

		if (key < com->key)
		{
			if (com->left == NULL)
			{
				com->left = p;
				return true;
			}
			com = com->left;
		}
		else
		{
			if (com->right == NULL)
			{
				com->right = p;
				return true;
			}
			com = com->right;
		}
	}
}

// keyの値を見てノードを検索して、値を取得する
const char* find(const tree* t, int key)
{
	if (t == NULL) return NULL;

	node* com = t->root;

	while (com != NULL)
	{
		if (key == com->key)
			return com->value;

		if (key < com->key)
			com = com->left;
		else
			com = com->right;
	}
	return NULL;
}

static void walk(const node* n, void (*func)(const node* p))
{
	if (n == NULL)
		return;

	walk(n->left, func);

	func(n);

	walk(n->right, func);
}

// keyの小さな順にコールバック関数funcを呼び出す
void search(const tree* t, void (*func)(const node* p))
{
	if (t == NULL || func == NULL) return;

	walk(t->root, func);
}