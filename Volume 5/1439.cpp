#include<iostream>
#include<algorithm>

using namespace std;

/*
    It is a solution for the Timus Volume 5 problem "Battle with you know who."
    Here I have implemented a modified segmented tree that has self balancing
    features of the AVL tree.
    It took 0.468 sec time and 1028KB space.
*/

struct Node{
    int val;
    int start;
    int end;
    Node *left,*right;
    int destroyedRooms;
    int height;

    Node(int s,int e)
    {
        start = s;
        end = e;
        val = e-s+1;
        left = right = NULL;
        destroyedRooms=0;
        height=1;
    }

    void update(int s,int e)
    {
        start = s;
        end = e;
        val = e-s+1;
    }
};

Node *rightRotate(Node *a)
{
    Node *b = a->left;
    Node *T2 = b->right;

    // Perform rotation
    b->right = a;
    a->left = T2;

    // Update heights
    a->height = max(a->left->height,
                    a->right->height) + 1;
    b->height = max(b->left->height,
                    b->right->height) + 1;

    // Update destroyedRooms
    a->destroyedRooms = a->left->destroyedRooms +
                        a->right->destroyedRooms + 1;
    b->destroyedRooms = b->left->destroyedRooms +
                        b->right->destroyedRooms + 1;

    a->update(a->left->start,a->right->end);
    b->update(b->left->start,b->right->end);

    // Return new root
    return b;
}


Node *leftRotate(Node *a)
{
    Node *b = a->right;
    Node *T2 = b->left;

    // Perform rotation
    b->left = a;
    a->right = T2;

    // Update heights
    a->height = max(a->left->height,
                    a->right->height) + 1;
    b->height = max(b->left->height,
                    b->right->height) + 1;

    // Update destroyedRooms
    a->destroyedRooms = a->left->destroyedRooms +
                        a->right->destroyedRooms + 1;
    b->destroyedRooms = b->left->destroyedRooms +
                        b->right->destroyedRooms + 1;

    a->update(a->left->start,a->right->end);
    b->update(b->left->start,b->right->end);

    // Return new root
    return b;
}

int getBalance(Node *N)
{
    if (N == NULL)
        return 0;
    return N->left->height - N->right->height;
}

int search(Node *root,int r)
{
    if((root->left==NULL)&&(root->right==NULL)){
        return root->start+r-1;
    }else{
        Node* lNode = root->left;
        int leftRooms = lNode->val-lNode->destroyedRooms;
        if(leftRooms>=r)
            return search(root->left,r);
        else
            return search(root->right,r-leftRooms);
    }
}

Node* split(Node *root,int r)
{
    root->destroyedRooms++;

    if((root->left==NULL)&&(root->right==NULL)){
        int splitPoint = root->start+r-1;
        root->left = new Node(root->start,splitPoint-1);
        root->right = new Node(splitPoint+1,root->end);
        root->height++;
    }else{

        Node* lNode = root->left;
        int leftRooms = lNode->val-lNode->destroyedRooms;
        if(leftRooms>=r)
            root->left = split(root->left,r);
        else
            root->right = split(root->right,r-leftRooms);

        root->height = 1 + max(root->left->height,
                        root->right->height);

        int balance = getBalance(root);

        //Went to right case
        if(balance<-1){
            Node *lc = root->right->left;
            Node *rc = root->right->right;
            //right left
            if(lc->height>rc->height){
                root->right = rightRotate(root->right);
                return leftRotate(root);
            }
            //right right
            else{
                return leftRotate(root);
            }
        }
        //Went to left case
        if(balance>1){
            Node *lc = root->left->left;
            Node *rc = root->left->right;
            //left left
            if(lc->height>rc->height){
                return rightRotate(root);
            }
            //left right
            else{
                root->left = leftRotate(root->left);
                return rightRotate(root);
            }
        }
    }
    return root;
}

void printTree(Node *root)
{
    if(root){
        if(root->left==NULL&&root->right==NULL)
            cout<<"("<<root->start<<","<<root->end<<") height:"<<root->height<<endl;
        printTree(root->left);
        printTree(root->right);
    }
}

int main()
{
    int n,m;
    cin>>n>>m;

    Node *root = new Node(1,n);

    while(m--)
    {
        char c;
        int r;
        cin>>c>>r;

        if(c=='L')
            cout<<search(root,r)<<endl;
        else
            root = split(root,r);

        //printTree(root);
        //cout<<endl;
    }

    return 0;
}
