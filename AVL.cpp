#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define COUNT 10

struct Node
{
    char nim[10];
    char name[90];
    struct Node *left;
    struct Node *right;
    int height;
};

int height(struct Node *N)
{
    if (N == NULL){
        return 0;
    }
    else{
        return N->height;
    }
}

int max(int a, int b)
{
    if(a>b){
        return a;
    }
    else{
        return b;
    }
}

struct Node * minValueNode(struct Node* node)
{
    struct Node* current = node;

    while (current->left != NULL){
        current = current->left;
    }
    return current;
}

struct Node* newNode(char * nim, char * name)
{
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    strcpy(node->name, name);
    fflush(stdin);
    strcpy(node->nim, nim);
    node->left   = NULL;
    node->right  = NULL;
    node->height = 1;
    return(node);
}

struct Node *rightRotate(struct Node *y)
{
    struct Node *x = y->left;
    struct Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right))+1;
    x->height = max(height(x->left), height(x->right))+1;

    return x;
}


struct Node *leftRotate(struct Node *x)
{
    struct Node *y = x->right;
    struct Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right))+1;
    y->height = max(height(y->left), height(y->right))+1;

    return y;
}

int getBalance(struct Node *N)
{
    if (N == NULL){
        return 0;
    }
    else{
        return height(N->left) - height(N->right);
    }

}

struct Node* insert(struct Node* node, char * nim, char * name)
{
    if (node == NULL){
        return(newNode(nim, name));
    }
    if (strcmp(nim, node->nim)<0){
        node->left  = insert(node->left, nim, name);
    }
    if (strcmp(nim, node->nim)>0){
        node->right = insert(node->right, nim, name);
    }
    else{
        return node;
    }



    int balance = getBalance(node);

    if (balance > 1 && strcmp(nim, node->left->nim)<0){
        return rightRotate(node);
    }
    if (balance < -1 && strcmp(nim, node->right->nim)>=0){
        return leftRotate(node);
    }

    if (balance > 1 && strcmp(nim, node->left->nim)>=0)
    {
        node->left =  leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && strcmp(nim,  node->right->nim)<0)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    node->height = 1 + max(height(node->left), height(node->right));

    return node;
}

struct Node* deleteNode(struct Node* root, char * nim)
{
    if (root == NULL){
        printf("No data\n");
        return root;
    }
    if (strcmp(nim, root->nim)<0){
        root->left = deleteNode(root->left, nim);
    }
    else if( strcmp(nim, root->nim)>0 ){
        root->right = deleteNode(root->right, nim);
    }
    else
    {
        if( (root->left == NULL) || (root->right == NULL) )
        {
            struct Node *temp = root->left ? root->left : root->right;

            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else{
                *root = *temp;
            }
            free(temp);
        }
        else
        {
            struct Node* temp = minValueNode(root->right);
            strcpy(temp->nim, root->nim);
            root->right = deleteNode(root->right, temp->nim);
        }
    }

    if (root == NULL){
        return root;
    }

    root->height = 1 + max(height(root->left), height(root->right));

    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    if (balance > 1 && getBalance(root->left) < 0)
    {
        root->left =  leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    if (balance < -1 && getBalance(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

void print2DUtil(struct Node *root, int space)
{
    if (root == NULL)
        return;

    space += COUNT;

    print2DUtil(root->right, space);

    printf("\n");
    for(int i = COUNT; i < space; i++)
        printf(" ");
    printf("%s: %s\n", root->nim, root->name);

    print2DUtil(root->left, space);
}

void print2D(struct Node *root)
{
   print2DUtil(root, 0);
}


bool validateNim(char * nim){
    if(strlen(nim)==10){
        return true;
    }else{
        return false;
    }
}

bool validateName(char * name){
    if(strlen(name)>=5 && strlen(name)<=90){
        return true;
    }else{
        return false;
    }
}

int main()
{

    struct Node *root = NULL;
    bool repeat = true;
    int choice;
    char inputNim[10] = "";
    char inputName[90] ="";

    while(repeat){
        fflush(stdin);
        system("cls");
        printf("========================\n");
        printf("1. Insert\n");
        printf("2. View\n");
        printf("3. Delete\n");
        printf("4. Exit\n");
        printf(">>");
        scanf("%d", &choice);
        switch(choice){
            case 1: do{
                        fflush(stdin);
                        printf("Input name (5-90 characters)");
                        gets(inputName);
                        fflush(stdin);
                    }while(validateName(inputName)==false);
                    do{
                        fflush(stdin);
                        printf("\nInput NIM (10 characters)");
                        gets(inputNim);
                    }while(validateNim(inputNim)==false);
                    root = insert(root, inputNim, inputName);
                    printf("Success Insert!\n");
                    system("pause");
                    break;
            case 2: print2D(root);
                    system("pause");
                    break;
            case 3: if(root==NULL){
                        printf("\nNo Data\n");
                    }
                    else{
                        do{
                            fflush(stdin);
                            printf("\nInput NIM (10 characters)");
                            gets(inputNim);
                        }while(validateNim(inputNim)==false);
                        root = deleteNode(root, inputNim);
                    }
                    break;
            case 4: repeat = false;
                    printf("\nSee you!\n");
                    system("pause");
                    break;
            default: printf("\nSee you!");
        }

    }



    return 0;
}
