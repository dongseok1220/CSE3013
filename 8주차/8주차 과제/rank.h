#include "tetris.h"

/* rank.txt���� ��ŷ���� �а�, ��ŷ����� ���� �� */
void createRankList() {
	FILE* fp;
	int i, j;

	fp = fopen("rank.txt", "r");
	if (!fp) {
		fp = fopen("rank.txt", "w");
		return;
	}
	int num; // ��ũ ����
	fscanf(fp, "%d", &num);
	count = num; // count �� ���������μ� ���Ͽ� ����ִ� ����� ����

	if (!num) {
		printf("Empty");
		return;
	}

	int i_temp[count];
	char c_temp[NAMELEN]; // �ӽú��� 

	/* �⺻ ���̵��
	1. count��ŭ ������ �ߺ����� �ʴ� ���ڷ� ���� tree ����  -> �۴ٸ� ���� ũ�ٸ� ������ (rank�� ��Ÿ���� ������)
	2. ���� ������ ������ 1~count ���� �Ҵ�
	*/

	for (i = 0; i < count; i++)
		i_temp[i] = i + 1; // �迭�� 1~ count ���� �Ҵ� 

	int ran;

	if (root == NULL) {
		root = malloc(sizeof(NODE));
		root->left = NULL; root->right = NULL;
		root->rank = i_temp[num / 2];
		swap(i_temp[num - 1], i_temp[num / 2]);
		num--;
	}
	for (i = 0; i < count - 1; i++)
	{
		ran = rand() % num; // 0~num-1 ����  
		Insert_NODE(root, i_temp[ran]);
		swap(i_temp[num - 1], i_temp[ran]);
		// ���ĵ� �迭���� 
		num--;
	}	// 1~count ���� Ʈ������

	// ������ Ʈ���� ������ �� �Ҵ�
	int t_data;

	for (i = 1; i <= count; i++)
	{
		fscanf(fp, "%s %d", c_temp, &t_data);
		Insert_FILE(root, t_data, c_temp, i);
	}

	fclose(fp);
}

NODE* Insert_NODE(NODE * node, int data) // struct ���� rank������ �״� -> ó�� ����Ʈ�� ���鶧����� ���� ���x
{
	if (node == NULL) {
		NODE* temp = malloc(sizeof(NODE));
		temp->rank = data;
		temp->left = NULL; temp->right = NULL;
		return temp;
	} // ������ ����

	if (data < node->rank)
		node->left = Insert_NODE(node->left, data);
	else if (data > node->rank)
		node->right = Insert_NODE(node->right, data);
	return node;
}

NODE* Insert_FILE(NODE * node, int data, char* name, int rank)
{
	if (rank == node->rank) {
		node->data = data;
		strcpy(node->name, name);
	}
	else if (rank < node->rank) // rank �� �� ���� -> ��������
		node->left = Insert_FILE(node->left, data, name, rank);
	else if (rank > node->rank)  // rank�� �� �۴� -> ������
		node->right = Insert_FILE(node->right, data, name, rank);

	return node;
}

void rank() {
	int X = 1, Y = 0, ch, i, j;
	clear();
	noecho();
	printw("1. list ranks form X to Y\n");
	printw("2. list ranks by a specific name\n");
	printw("3. delete a specific rank\n");
	ch = getch();
	echo();
	int check = 1;

	if (ch == '1') {
		printw("X: ");
		int check_x = scanw("%d\n", &X);
		if (check_x != 1) X = 1;
		printw("Y: ");
		int check_y = scanw("%d\n", &Y);
		if (check_y != 1) Y = count; // Ȥ�� count���� 
		printw("\tname\t|   score\n");
		printw("----------------------------\n");
		if (X > Y) {
			printw("\nsearch failure: no rank in the list\n");
		}

		NODE* curr = root;
		/*	for (i=X-1; i<Y; i++)
			{
			if (curr->right !=NULL) printw("%-16s|%-d\n", curr->right->name,curr->right->data);
			printw("%-16s|%-d\n",curr->name,curr->data);
			curr= curr->left;
			} */
		print_list(curr, X, Y, &check);
	}
	else if (ch == '2') {
		int ex = 0; // �����ϴ��� üũ�ϴ� ���� 
		printw("Input the name: ");
		char name[NAMELEN]; // �̸� �Է¹��� ���� ���� 
		int check_name = scanw("%s", name);
		printw("\tname\t|   score\n");
		printw("----------------------------\n");
		print_list_2(root, name, &ex);
		if (!ex) printw("\nsearch failure: no name in the list\n");
	}
	else if (ch == '3') {
		printw("Input the rank: ");
		int d_rank; // ������ rank
		int c_rank = scanw("%d", &d_rank);
		if (c_rank != 1) {
			printw("Nothing deleted\n");
		}
		else if (d_rank > count || d_rank <= 0)
			printw("\nsearch failure: the rank not in the list");
		else {
			root = Delete(root, d_rank);
			count--;
			printw("\nresult: the rank deleted\n");
			//printw("%s %d",root->name, root->data);	
			writeRankFile(root, 1); // ���� �ݿ� 
		}
	}
	check = 1;
	getch();
}

void writeRankFile(NODE * root, int first) {
	if (first == 1) {
		FILE* remove = fopen("rank.txt", "w+");
		fprintf(remove, "%d\n", count);
		fclose(remove);
	}
	FILE* fp = fopen("rank.txt", "a+");

	echo();

	if (root != NULL) {
		writeRankFile(root->left, first + 1);
		//	printw("%s %d\n", root->name, root->data);	
		fprintf(fp, "%s %d\n", root->name, root->data);
		fclose(fp);
		writeRankFile(root->right, first + 1);
	}
}

void newRank(int score) {
	clear();
	move(0, 0);
	// char str[NAMELEN+1];
	int i, j;
	echo();
	printw("your name: ");
	NODE* newNODE = (NODE*)malloc(sizeof(NODE));
	// scanf("%s",newNODE->name);
	getstr(newNODE->name);
	newNODE->data = score;
	newNODE->left = NULL; newNODE->right = NULL;
	count++;

	if (root == NULL) {
		root = newNODE;
	}
	else {
		NODE* curr = root;
		for (i = 0; i < count; i++)
		{
			if (curr->data >= newNODE->data) {
				if (curr->right == NULL) {
					curr->right = newNODE;
					break;
				}
				else curr = curr->right;
			}
			else if (curr->data < newNODE->data) {
				if (curr->left == NULL) {
					//printw("INSERT in %d next %d",i,curr->data);
					curr->left = newNODE;
					break;
				}
				else curr = curr->left;
			}
		}

	}
	writeRankFile(root, 1);
}

void print_list(NODE * root, int X, int Y, int* check)
{
	if (root == NULL) return;
	if (root != NULL) {
		print_list(root->left, X, Y, check);
		if (*check >= X && *check <= Y)
			printw("%-16s| %-d\n", root->name, root->data);
		(*check)++;
		print_list(root->right, X, Y, check);
	}
}

void print_list_2(NODE * root, char* name, int* check)
{
	if (root == NULL) return;
	if (root != NULL) {
		print_list_2(root->left, name, check);
		if (!strcmp(name, root->name)) {
			printw("%-16s| %-d\n", root->name, root->data);
			*check = 1;  // ������ ��� 
		}
		print_list_2(root->right, name, check);
	}
}

NODE* Delete_NODE(NODE * r_node, int d_rank, int* check) // �ش� rank�� �ּ� ��ȯ 
{
	NODE temp_node;  // �����ϰ� ���� ����� ���� ���� 
	NODE* d_node = root; // ���� ������ �������� ������ ��� ���� 
	NODE* p = NULL; // �� ����� �θ��� 
	NODE* child; // �ڽĳ�尡 ���� �� ����� ���� 

	find_rank(root, d_rank, check, &temp_node);
	*check = 1;

	while (d_node != NULL)
	{
		if (strcmp(d_node->name, temp_node.name) == 0 && temp_node.data == d_node->data) break;
		p = d_node;
		if (d_node->data < temp_node.data) {
			d_node = d_node->left;
		}
		else if (d_node->data >= temp_node.data) {
			d_node = d_node->right;
		}
	}

	//	printw("%s %d\n", p->name, p->data);
	//	printw("%s %d\n", d_node->name, d_node->data);

	if (d_node == NULL) return root;

	if (d_node->left == NULL && d_node->right == NULL) // �ܸ����
	{
		if (p != NULL) {
			if (p->left == d_node) p->left = NULL;
			else p->right = NULL;
		}
		else root = NULL;
	}
	else if (d_node->left == NULL || d_node->right == NULL) // �ڽĳ�尡 �Ѱ� 
	{
		if (d_node->left != NULL) child = d_node->left;
		else child = d_node->right;

		if (p != NULL) {
			if (p->left == d_node) p->left = child;
			else p->right = child;
		}
		else root = child;
	}
	else { // �ڽĳ�尡 �ΰ�
		p = d_node;
		child = d_node->left;

		while (child->right != NULL)
		{
			p = child;
			child = child->right;
		}
		if (p->left == child) p->left = child->left;
		else p->right = child->left;

		d_node->data = child->data;
		strcpy(d_node->name, child->name);
		d_node = child;
	}

	free(d_node); // �޸����� 


	return root;

}
NODE* Delete(NODE * root, int d_rank)
{
	int check = 1;
	return Delete_NODE(root, d_rank, &check);
}

NODE* find_rank(NODE * root, int d_rank, int* check, NODE * temp)
{
	if (root == NULL) {
	}
	if (root != NULL) {
		find_rank(root->left, d_rank, check, temp);
		if (*check == d_rank) {
			(*check)++;
			strcpy(temp->name, root->name);
			temp->data = root->data;
		}
		(*check)++;
		find_rank(root->right, d_rank, check, temp);
	}
}

NODE* Free_NODE(NODE * root)
{
	if (root != NULL) {
		Free_NODE(root->left);
		Free_NODE(root->right);
		free(root);
	}
}
