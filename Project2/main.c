#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
int TABLESIZE=3;
int OLD_SIZE=0;
int *countNodes=0;
struct indications{//linkedlist represents the indications
    char ind[30];
    struct indications* nextNode;
};
int sizeTable=0;
struct letUsHash{
    char name[30];
    char active_ingredient[30];
    char category[30];
    struct indications* totalIndecations;
    char expiry_date[30];
    int count;
    char company[30];
};
//int *collision=0;
int sizeOfEnsertedMedicine=0;
typedef struct letUsHash* LETSHASH;
typedef struct indications* INDECATIONs;
#define Deleted (LETSHASH)(0xFFFFFFFFF000000)
struct  medicine {
    char  medicineName[30];
    char active_ingredient[30];
    char category[30];
    struct indications* totalIndecations;
    char expiry_date[30];
    int count;
    char company[30];
    struct medicine* left;
    struct medicine* right;
};
typedef struct medicine* MedicineData;
void DeleteList (INDECATIONs indicationslist){//if the list wasn't empty, delete all the nodes which inside of it;
    INDECATIONs Pointer,Temp;
    Pointer=indicationslist->nextNode;
    indicationslist->nextNode=NULL;

    while(Pointer!=NULL){
        Temp=Pointer->nextNode;
        free(Pointer);
        Pointer=Temp;
    }

}
LETSHASH *hashTable=NULL;

INDECATIONs MakeEmptyindication(INDECATIONs indicationslist){ //make the head to start reading the data of the medicines from Meds.txt

    if (indicationslist!=NULL)
        DeleteList(indicationslist);

    indicationslist=(INDECATIONs)malloc(sizeof(struct indications));


    if (indicationslist==NULL)
        printf("Out of memory");

    indicationslist->nextNode=NULL;
    return indicationslist;
};

void ReadFromFileToTable(FILE *input, LETSHASH hashTable){//read data from the file pharmacy.txt
    char cpy[200],read[200];
    char *cpyData;
    fgets(read,200,input);
    LETSHASH newMedicine=malloc(sizeof(LETSHASH));
    INDECATIONs newIndications=MakeEmptyindication(NULL);

    strcpy(newMedicine->name,strtok(read,":"));
    strcpy(newMedicine->active_ingredient,strtok(NULL,":"));
    strcpy(newMedicine->category,strtok(NULL,":"));
    strcpy(cpy,strtok(NULL,":"));
    strcpy(newMedicine->expiry_date,strtok(NULL,":"));
    newMedicine->count=atoi(strtok(NULL,":"));

    char *Data=strtok(NULL,"\n");
    strcpy(newMedicine->company,Data);
    cpyData=strtok(cpy,",");

    while( cpyData != NULL ) {//insert indications to linked list
        InsertIndication(newIndications,cpyData);
        cpyData=strtok(NULL,",");
    }
    newMedicine->totalIndecations=newIndications;
    insertNewCellToHashTable(hashTable,newMedicine);

}


 int hash (char *name,LETSHASH hashTable[]){//hash to reach empty spot
    int hashValue=0;

    for (int i=0;i<3;i++){
        hashValue+=(name[i]*(i*26+1))%TABLESIZE;;
    }
    return hashValue;
}





void InsertIndication (INDECATIONs indicationslist, char indicationName[30]){//insert new medicine to the table.
    INDECATIONs newNode=(INDECATIONs)malloc(sizeof(struct indications));
    strcpy(newNode->ind,indicationName);
    newNode->nextNode=indicationslist->nextNode;
    indicationslist->nextNode=newNode;
}

MedicineData MakeEmpty(MedicineData Node ){
    if( Node != NULL ){
        MakeEmpty( Node->left );
        MakeEmpty( Node->right );
        free(Node);
    }
    return NULL;
}

MedicineData Insert(MedicineData newNode,MedicineData medicineTree){//inert medicine to the tree.
    if (medicineTree==NULL){
        medicineTree=newNode;
    }

    else if (strcmp(medicineTree->medicineName,newNode->medicineName)>0){
        medicineTree->left=Insert(newNode,medicineTree->left);
    }

    else if (strcmp(medicineTree->medicineName,newNode->medicineName)<0){
        medicineTree->right=Insert(newNode,medicineTree->right);
    }

    return medicineTree;

}

MedicineData readTheFile(MedicineData medicineTree,FILE* input){//read data from file to insert it to the tree
    char read[200],bde[200],cpy[20];

    char *Data,*cpyData;

    while(!feof(input)){
        countNodes++;
        MedicineData NewNode=(MedicineData)malloc(sizeof (struct medicine));
        INDECATIONs newIndications=MakeEmptyindication(NULL);
        fgets(read,200,input);
        strcpy(NewNode->medicineName,strtok(read,":"));
        strcpy(NewNode->active_ingredient,strtok(NULL,":"));
        strcpy(NewNode->category,strtok(NULL,":"));
        Data=strtok(NULL,":");
        strcpy(bde,Data);
        strcpy(NewNode->expiry_date,strtok(NULL,":"));
        NewNode->count=atoi(strtok(NULL,":"));
        strcpy(cpy,strtok(NULL,":"));
        Data=strtok(cpy,"\n");
        strcpy(NewNode->company,Data);
        cpyData=strtok(bde,",");

        while( cpyData != NULL ) {
            InsertIndication(newIndications,cpyData);
            cpyData = strtok(NULL, ",");
        }
        NewNode->totalIndecations=newIndications;

        NewNode->left=NewNode->right=NULL;
        medicineTree=Insert(NewNode,medicineTree);

    }

    return medicineTree;
}

MedicineData findNode(char name[30],MedicineData MedicineNode){//find a node in the tree
    if (MedicineNode==NULL)
        return NULL;
    else if (strcmp(MedicineNode->medicineName,name)>0)
        return findNode(name,MedicineNode->left);
    else if (strcmp(MedicineNode->medicineName,name)<0)
        return findNode(name,MedicineNode->right);
    else if (strcmp(MedicineNode->medicineName,name)==0)
        return MedicineNode;
}

void addMedicine(MedicineData medicineTree){//add new medicine to the tree
    countNodes++;
    MedicineData newNode=(MedicineData)malloc(sizeof (struct medicine));
    INDECATIONs newIndications=MakeEmptyindication(NULL);
    char data[60];
    //take the data from the user
    printf("\nEnter the name of the medicine:");
    gets(data);gets(data);
    strcpy(newNode->medicineName,data);
    printf("\nEnter the active ingredient:");
    gets(data);
    strcpy(newNode->active_ingredient,data);
    printf("\nEnter the category:");
    gets(data);
    strcpy(newNode->category,data);
    int x=0;
    printf("\nEnter how many indication do we have for this medicine:");
    scanf("%d",&x);
    gets(data);
    int cot=1;
    while (x--){
        printf("Enter indication number %d:",cot);
        cot++;
        gets(data);
        InsertIndication(newIndications,data);
    }
    newNode->totalIndecations=newIndications;
    printf("\nEnter the expiry date:");
    gets(data);
    strcpy(newNode->expiry_date,data);
    printf("\nEnter the quantity that we have:");
    scanf("%d",&x);
    gets(data);
    newNode->count=x;
    printf("\nEnter the company:");
    gets(data);
    newNode->left=newNode->right=NULL;
    strcpy(newNode->company,data);
    medicineTree=Insert(newNode,medicineTree);
    printf("The medicine %s was inserted successfully\n",newNode->medicineName);

}

INDECATIONs FindPrevious(char data[30], INDECATIONs medicineNode){//This struct return a pointer struct pointing on the node which is the previous of the node which has points equal to the integer pointsValue.
    INDECATIONs PreNode, Node;
    PreNode=medicineNode;
    Node=PreNode->nextNode;
    while (Node!=NULL&&strcmp(data,Node->ind)!=0){
        PreNode=Node;
        Node=Node->nextNode;
    }

    return PreNode;
};

int IsTeamLast(INDECATIONs node){
    return node->nextNode==NULL;
}

void deleteNode (INDECATIONs Node, char data[30]){//to delete an indecation
    INDECATIONs Temp, Pointer;
    Pointer=Node;
    Pointer=FindPrevious(data,Pointer);//pointing in the node which is before the node which we want to delete to attach it to the node which is next to the node that we want to delete.

    if (!IsTeamLast(Pointer)){
        Temp=Pointer->nextNode;
        Pointer->nextNode=Temp->nextNode;
        free(Temp);
    }
}

void editNode(MedicineData MedicineNode){//to edit a medicine
    printf("Select what you want to edit:\n1-Medicine name\n2-Active ingredient\n3-Category\n4-Indication\n5-Expiry date\n6-The remaining quantity\n7-company\n8 To exit\n");
    int x,i,ind;
    char newData[100];
    scanf("%d",&x);//what you want to edit
    while (x!=8){
        if (x==1){//edit the name
            printf("\nEnter the new name of the medicine:");
            gets(newData);
            gets(newData);
            strcpy(MedicineNode->medicineName,newData);

        }
        if (x==2){//edit the active ingredeint
            printf("\nEnter the new active ingredient:");
            scanf("%s",newData);
            strcpy(MedicineNode->active_ingredient,newData);

        }
        if (x==3){//edit the category
            printf("\nEnter the new category:");
            scanf("%s",newData);
            strcpy(MedicineNode->category,newData);

        }
        if (x==4){//edit the indications

            printf("\nSelect what you want to do:\n1-Delete all indications.\n2-Delete a specific indication.\n3-Edit a specific indication.\n4-Add new indication.\n5-Enter 5 to end\n");
            int t;
            scanf("%d",&t);
            while (t!=5){
                if (t==1){
                    DeleteList(MedicineNode->totalIndecations);
                }
                if (t==2){
                    printf("\nWhich one you want to delete:");
                    INDECATIONs node=MedicineNode->totalIndecations->nextNode;
                    i=1;
                    while (node!=NULL){
                        printf("\n%d-%s",i,node->ind);
                        node=node->nextNode;
                        i++;
                    }
                    scanf("%d",&i);
                    node=MedicineNode->totalIndecations->nextNode;
                    while(i>0){
                        node=node->nextNode;
                        i--;
                    }
                    deleteNode(node,node->ind);
                }
                if (t==3){
                   printf("\nWhich one you want to Edit:");
                    INDECATIONs node=MedicineNode->totalIndecations->nextNode;
                    i=1;
                    while (node!=NULL){
                        printf("\n%d-%s",i,node->ind);
                        node=node->nextNode;
                        i++;
                    }
                    scanf("%d",&i);
                    node=MedicineNode->totalIndecations->nextNode;
                    while(i>0){
                        node=node->nextNode;
                        i--;
                    }
                    scanf("%s",newData);
                    strcpy(node->ind,newData);
                }
                if (t==4){
                    printf("\nEnter how many indications you want to add:");
                    scanf("%d",&ind);
                    i=1;
                    while (ind--){
                        printf("\nEnter indication number %d:",i);
                        scanf("%s",newData);
                        InsertIndication(MedicineNode->totalIndecations,newData);
                        i++;
                    }

                }
        printf("\nSelect what you want to do:\n1-Delete all indications.\n2-Delete a specific indication.\n3-Edit a specific indication.\n4-Add new indication.\n5-Enter 5 to end\n");
        scanf("%d",&t);
        }


    }
    if (x==5){//edit the date
        printf("\nEnter the new expiry date:");
        scanf("%s",newData);
        strcpy(MedicineNode->expiry_date,newData);

    }
    if (x==6){//edit the count
        printf("\nEnter the new remaining quantity :");
        int d;
        scanf("%d",&d);
        MedicineNode->count=d;
    }
    if (x==7){//edit the company.
        printf("\nEnter the new company:");
        scanf("%s",newData);
        strcpy(MedicineNode->company,newData);

    }
    printf("Select what you want to edit:\n1-Medicine name\n2-Active ingredient\n3-Category\n4-Indication\n5-Expiry date\n6-The remaining quantity\n7-company\n8 To exit\n");
    scanf("%d",&x);
    }
}

MedicineData FindMin (MedicineData neededNode){//find minimum node in the tree
    if (neededNode==NULL)
        return NULL;
    if (neededNode->left){
        return FindMin(neededNode->left);
    }
    else
        return neededNode;
}

MedicineData Delete(char medName[30],MedicineData node ){//delete a medicine from the tree.

    MedicineData TmpCell;
    if( node == NULL ){
        printf( "Element not found" );
        countNodes++;
    }
    else if( strcmp(medName , node->medicineName)<0 )
        node->left = Delete( medName, node->left );
    else if(strcmp(medName,node->medicineName)>0)
        node->right = Delete( medName, node->right );
    else
        if( node->left && node->right ){
            TmpCell=FindMin( node->right );
            strcpy(node->medicineName,TmpCell->medicineName);
            strcpy(node->active_ingredient,TmpCell->active_ingredient);
            strcpy(node->category,TmpCell->category);
            DeleteList(node->totalIndecations);
            node->totalIndecations=TmpCell->totalIndecations;
            TmpCell->totalIndecations=NULL;
            strcpy(node->expiry_date,TmpCell->expiry_date);
            node->count=TmpCell->count;
            strcpy(node->company,TmpCell->company);
            node->right = Delete( node->medicineName, node->right );
        }
        else{
            TmpCell= node;
            INDECATIONs tmp=TmpCell->totalIndecations;
            if (tmp!=NULL)
                DeleteList(tmp);
            free(tmp);
            if(node->left==NULL){
                node= node->right;
            }
            else if(node->right==NULL)
                node = node->left;
            free(TmpCell);
        }
        return node;



}

void DeleteAccordingToCompany (MedicineData parent,MedicineData child,MedicineData node,MedicineData wantedNode,char comp[30]){
    //parent here represent the parent of the node that will be deleted and the pointer child represent the node that will be deleted.
    if (strcmp(comp,wantedNode->company)==0){//if you found the node
        child=parent;
        wantedNode=Delete(wantedNode->medicineName,parent);
        countNodes--;

    }
    if (wantedNode==NULL)//if you didn't find anynode related to this company
        return ;


       if (wantedNode->left){
            parent=child;
            if (parent!=NULL)
            child=child->left;
            DeleteAccordingToCompany(parent,child,node,wantedNode->left,comp);
       }


        if (wantedNode!=NULL&&wantedNode->right){
            if (wantedNode->left){
                    child=parent;
                    child=child->right;
            }
            else{
            parent=child;
            if (parent!=NULL)
            child=child->right;
            }
            DeleteAccordingToCompany(parent,child,node,wantedNode->right,comp);
        }



}
int * count=0;
void writeToFile(MedicineData treeData,FILE* output){//write the data in the tree to pharmacy.txt file

    if (treeData==NULL){
        printf("No data to print");
    }
    else{
        count+=1;
        fprintf(output,"%s:",treeData->medicineName);
        fprintf(output,"%s:",treeData->active_ingredient);
        fprintf(output,"%s:",treeData->category);
        INDECATIONs temp=treeData->totalIndecations->nextNode;
        while (temp!=NULL){
            if (temp->nextNode!=NULL)
                fprintf(output,"%s,",temp->ind);
            else{
               fprintf(output,"%s:",temp->ind);
               break;
            }
            temp=temp->nextNode;
        }

        fprintf(output,"%s:",treeData->expiry_date);
        fprintf(output,"%d:",treeData->count);
        if (count==countNodes)
            fprintf(output,"%s",treeData->company);
        else
            fprintf(output,"%s\n",treeData->company);
        if (treeData->left){
            writeToFile(treeData->left,output);
        }
        if(treeData->right){
            writeToFile(treeData->right,output);
        }

    }
}

void print(MedicineData tree){//print the data to the screen.

    printf("%s:%s:%s:",tree->medicineName,tree->active_ingredient,tree->category);
    INDECATIONs temp=tree->totalIndecations->nextNode;
    while (temp!=NULL){
        if (temp->nextNode==NULL)
            printf("%s:",temp->ind);
        else
            printf("%s,",temp->ind);
        temp=temp->nextNode;
    }
    printf("%s:%d:%s",tree->expiry_date,tree->count,tree->company);
   printf("\n");
}

MedicineData inOrder(MedicineData treeNode){//to print the data in the tree in alphapetical order

    if (treeNode==NULL)
        return;
    inOrder(treeNode->left);
    print(treeNode);
    inOrder(treeNode->right);

}

int printSameCategory(MedicineData node,char cate[30]){//print the medicines that have the same category.
    int sum=0;
    if (node==NULL)
        return;
    if (!strcmp(cate,node->category)){
            sum++;
            print(node);
        }
     if (node->left){//go left
        sum+=printSameCategory(node->left,cate);
    }
     if (node->right){//go right
        sum+=printSameCategory(node->right,cate);
    }
    return sum;
}

int printSameDisease(MedicineData node,char disease[30]){//print the medicines that treat the same disease.
    int sum=0;
    INDECATIONs find=node->totalIndecations->nextNode;//first indication in the list
    if (node==NULL){
        return 0;///////////////
    }
    while (find!=NULL){
        if (!strcmp(disease,find->ind)){
            print(node);
            sum+=1;
            break;
        }
        find=find->nextNode;
    }
    if (node->left)//go to the left node
       sum+= printSameDisease(node->left,disease);
    if (node->right)//go right
       sum+= printSameDisease(node->right,disease);

    return sum;
}

LETSHASH reHash() {
    LETSHASH *tempTable=NULL;
    sizeTable = 0;// Resting entries size
    OLD_SIZE = TABLESIZE; // save the old size
    TABLESIZE = nextPrime(OLD_SIZE*2); // new size to the table

    tempTable=(LETSHASH *)malloc(TABLESIZE*sizeof(LETSHASH));//new table with new size.
    for (int i = 0; i < TABLESIZE; ++i) { //initializing table
        tempTable[i]=NULL;
    }

    for (int i = 0; i < OLD_SIZE; ++i) {
        if (hashTable[i]!=NULL&& hashTable[i]!=Deleted) {//if you found an empty spot.
           sizeTable++;//increase the size of the table.
           insertNewCellToHashTable(tempTable, hashTable[i]); //inserting nodes to temp table
        }
    }

    free(hashTable);// free the old table
    hashTable=tempTable;
    return tempTable;
}

double reload_factor(){//to know when to rehash.
    return ((double)sizeTable/TABLESIZE);
}

void insertNewCellToHashTable(LETSHASH table[],LETSHASH newMedicine){
    if (reload_factor()>0.75){//if the table is almost full
       table=reHash();sizeTable++;
    }

    char name[30];
    strcpy(name,newMedicine->name);//copy the name to the spot

    int cont;
    int value=hash(name,table);//to know where insert the node

    for (int i=0;i<TABLESIZE;i++){
        int reValue=(value+i*i)%TABLESIZE;//the function that used to hash
        if (table[reValue]==NULL){
            table[reValue]=newMedicine;//assigning the new medicine to the table.
            break;
        }
    }

}

int isPrime(int t) {
    if (t<=1)
        return 0;

    if (t<=3)
        return 1;

    for (int i= 2; i<=sqrt(t);i++){
        if (t%i==0)
            return 0;
    }

    return 1;
}

int nextPrime(int n) {
    if (n<=1)
        return 2;
    int primeNum=n;
    while (!isPrime(primeNum)) {
        primeNum++;
    }
    return primeNum;
}



void printFunction(){

    printf("\nhashValue+=(name[i]*(i*26+1))%TABLESIZE\n name[i] =ASCII Code and i =Length ,up to 3\n");

}

void printTable(LETSHASH* table){//to print the table on the screen.
    INDECATIONs temp;//will point on the indication list.
    printf("\n\nThe sequence of the printing process will be as follow:\nMedicine name - active ingredient - category - indication(s) - expiry date - remaining quantity - company\n\n");
    for (int i=0;i<TABLESIZE;i++){
        if (table[i]==NULL){//No data in the spot
            printf("%d-Empty Spot.\n",i+1);
        }

        else if (table[i]==Deleted){//If the medicine was deleted.
                printf("%d-Deleted spot.\n",i+1);
        }

        else {//if there's data in the spot.
            printf("%d-%s  -  %s  -  %s  -  ",i+1,table[i]->name,table[i]->active_ingredient,table[i]->category);//printing the data
            temp=table[i]->totalIndecations->nextNode;

            while (temp!=NULL){// printing the indications which is in the list.
                if (temp->nextNode!=NULL)
                    printf("%s,",temp->ind);

                else
                    printf("%s",temp->ind);
                temp=temp->nextNode;//next indication.
            }

            printf("  -  %s  -  %d  -  %s\n",table[i]->expiry_date,table[i]->count,table[i]->company);//printing the data
        }
    }
}

LETSHASH DeleteRecord(){//to delete medicine from the table
    char name[30];
    printf("Enter the medicine that you want to delete:");
    gets(name);gets(name);
    int index=hash(name,hashTable);//first spot on the table

    for (int i=0;i<TABLESIZE;i++){
        int newIndex=(index+i*i)%TABLESIZE;
        if(hashTable[newIndex]==NULL){//the medicine is not dound
            printf("The medicine %s is not found\n",name);
            return hashTable[newIndex];
        }

        else if (hashTable[newIndex]==Deleted)//if the spot was deleted before then go to the next one.
            continue;
        else if (!strcmp(hashTable[newIndex]->name,name)){//if you found the medicine
            INDECATIONs newOne=hashTable[newIndex]->totalIndecations;
            DeleteList(newOne);//delete the indications
            free(hashTable[newIndex]);//free the node.
            hashTable[newIndex]=Deleted;
            printf("The medicine was deleted successfully\n");
            return hashTable[newIndex];
        }
    }
    return NULL;
}

void searchMedicineInTable(LETSHASH *hashTable){//find a medicine in the table.
    printf ("Enter the name of the medicine that you want to find and -1 to end:");
    char name[60],temp[3],collisionn=0;
    gets(name);gets(name);
    while (strcmp(name,"-1")){

        int index;

        index=hash(name,hashTable);//first medicine in the table

        for (int i=0;i<TABLESIZE;i++){
            int newIndex=(index+i*i)%TABLESIZE;
            if(hashTable[newIndex]==NULL){//the medicine is not found
                printf("The medicine %s is not found\n",name);
                break;
            }

            else if (hashTable[newIndex]==Deleted)//if the spot has a deleted medicine
                continue;

            else if (!strcmp(hashTable[newIndex]->name,name)){//if you found the medicine
                if (i==0)//if no collision happened
                    printf("\nThe medicine %s was found with no collision to reach it\n",name);
                else
                    printf("\nThe medicine %s was found with number of collision to reach it equal to :%d\n",name,i);
                break;
            }
        }
        printf ("\nEnter the name of the medicine that you want to find and -1 to end:");
        gets(name);
    }
}

void insertNewSpot(){//to insert new medicine
    char name [200];
    int cont;
    INDECATIONs newOne=MakeEmptyindication(NULL);//new indication list
    LETSHASH newMedicine=(LETSHASH)malloc(sizeof(LETSHASH));
    printf("Here you're inserting a new record\n\n");
    //Taking the data from the user
    printf("Enter the name of the medicine:");gets(name);gets(name);
    strcpy(newMedicine->name,name);

    printf("Enter the active ingredient:");gets(name);
    strcpy(newMedicine->active_ingredient,name);

    printf("Enter the category:");gets(name);
    strcpy(newMedicine->category,name);

    printf("Enter the number if the indications that you want to ensert:");scanf("%d",&cont);gets(name);
    int i=1;
    while (cont--){
        printf("Enter the indication number %d:",i);
        i++;
        gets(name);
        InsertIndication(newOne,name);
    }
    newMedicine->totalIndecations=newOne;

    printf("Enter the expiry date:");gets(name);
    strcpy(newMedicine->expiry_date,name);

    printf("Enter the remaining quantity:");scanf("%d",&cont);gets(name);
    newMedicine->count=cont;

    printf("Enter the company that own this medicine:");gets(name);
    strcpy(newMedicine->company,name);

    insertNewCellToHashTable(hashTable,newMedicine);//inserting the data to the table
    printf("New record was inserted successfully.\n");

}

void SaveHashTableBackToPharmacytxt(int sizeTable){//print the table to the file pharmacy.txt
    FILE *output=fopen("Pharmacy.txt","w");
    int size=0;
    for (int i=0;i<TABLESIZE;i++){
        if (hashTable[i]!=NULL&&hashTable[i]!=Deleted){//if the spot has a medicine information
            size++;
            fprintf(output,"%s:%s:%s:",hashTable[i]->name,hashTable[i]->active_ingredient,hashTable[i]->category);
            INDECATIONs newone=hashTable[i]->totalIndecations->nextNode;

            while (newone!=NULL){//to print the indications
                if (newone->nextNode==NULL)
                    fprintf(output,"%s:",newone->ind);
                else
                    fprintf(output,"%s,",newone->ind);
                newone=newone->nextNode;
            }
            printf("%d",sizeTable);
            if (size==sizeTable){
                fprintf(output,"%s:%d:%s",hashTable[i]->expiry_date,hashTable[i]->count,hashTable[i]->company);
            }
            else
                fprintf(output,"%s:%d:%s\n",hashTable[i]->expiry_date,hashTable[i]->count,hashTable[i]->company);
        }
    }
    fclose(output);
}
int main()
{
    int q=0;
    hashTable=(LETSHASH *)malloc(TABLESIZE*sizeof(LETSHASH));

    for (int i=0;i<TABLESIZE;i++){
        hashTable[i]=NULL;
    }
    MedicineData MedicineNode=(MedicineData)malloc(sizeof (struct medicine));
    MedicineNode=MakeEmpty(NULL);

    int t;
    printf("\nchoose one\n1-Read the file Meds.txt that contains the medicines infornation.\n2-Insert a new medicine with all its associated information.\n3-Find a medicine and modify it\n4-print the medicine in alphabetical order.\n5-print all medicines that have the same category.\n6-print all medicines that treat the same disease .\n7-Delete a medicine.\n8-Delete all medicines that related to the same company.\n9-Save back all the data to pharmcay.txt file.\n11-print all the spots(including deleted, empty spot).\n12-print how many medicine in the table.\n13-insert new medicine to the table.\n14-print used function for hashing\n15-Delete a medicine from the table\n16-search a medicine in the table\n17-save back the data that in the table to pharmacy.txt file:");
    scanf("%d",&t);
    //char cpy[3],read[200];
    //char *Data;
    char name[30];
    while (1){

        if (t==1){// if the user want to read teams data from file.
           FILE *Input=fopen("Meds.txt","r");
           MedicineNode=readTheFile(MedicineNode,Input);
           if (MedicineNode==NULL){
               printf("Empty file\n");
           }
           else
            printf("The file was read successfully\n");
        }

        else if (t==2){//To add new medicine
            addMedicine(MedicineNode);
        }

        else if (t==3){//To edit medicine.
            printf("\nEnter the name of the medicine which you want to find:");
            gets(name);gets(name);

            MedicineData neededNode=findNode(name,MedicineNode);//First you need to find this medicine.
            if (neededNode!=NULL){//If the medicine is exist.
                printf("Do you want to edit it? Answer by YES or NO:");
                scanf("%s",name);

                if (!strcmp(name,"YES")){//If the user want to edit a medicine data.
                    editNode(neededNode);
                }
            }
            else {
                printf("The medicine %s is not found\n",name);//If the medicine is not exist.
            }

        }

        else if (t==4){//If the user want to print the medicine in alphabetical order.
            printf("\n\nThe sequence of the printing process will be as follow:\nMedicine name:active ingredient:category:indication(s):expiry date:remaining quantity:company\n\n");
            inOrder(MedicineNode);
        }

        else if (t==5){//print all medicines that have the same category.

            printf("Enter the category that you want to print its medicines:");
            gets(name);gets(name);
            printf("\n\nThe sequence of the printing process will be as follow:\nMedicine name:active ingredient:category:indication(s):expiry date:remaining quantity:company\n\n");
            int flag=printSameCategory(MedicineNode,name);//if flag>0, then there's at least one medicine has the category that the user entered.
            if (!flag){
                printf("\nNo medicine related to this category!\n");
            }
        }


        else if (t==6){//print all medicines that treat the disease that the user entered.
            printf("Enter the disease that you want to find medicines treat it:");
            gets(name);gets(name);
            printf("\n\nThe sequence of the printing process will be as follow:\nMedicine name:active ingredient:category:indication(s):expiry date:remaining quantity:company\n\n");
            int flag=printSameDisease(MedicineNode,name);//if flag>0, then there's at least one medicine has the disease that the user entered.
            if (!flag){
                printf("\nNo medicine related to this category!");
            }

        }

        else if (t==7){//Delete a medicine.
            printf("Enter the name of the medicine that you want to delete:");
            gets(name);gets(name);
            countNodes--;
            MedicineData deletee=Delete(name,MedicineNode);
        }

        else if (t==8){//Delete all medicines that related to the same company.
           printf("Enter the name of the company that you want to delete its medicine:");
           gets(name);gets(name);

           MedicineData parent=MedicineNode,child=MedicineNode;
           DeleteAccordingToCompany(parent,child,MedicineNode,MedicineNode,name);
        }

        else if (t==9){//Save back all the data to a file.
            FILE *output=fopen("Pharmacy.txt","w");
            writeToFile(MedicineNode,output);
            fclose(output);

        }

        if (t>=10&&!q){//To read the data from the file.
                q++;
            FILE *Input=fopen("pharmacy.txt","r");
            while(!feof(Input)){
                ++sizeTable;

                ReadFromFileToTable(Input,hashTable);
            }
        }

            if (t==11){//print all the spots(including deleted, empty spot).
                printTable(hashTable);
            }

            else if(t==12){//print how many medicine in the table.
                printf("size of the table =%d\n",sizeTable);
            }

            else if(t==13){//insert new medicine to the table.
                ++sizeTable;
                insertNewSpot();
            }

            else if (t==14){//print used function for hashing
               printFunction();
            }

            else if (t==15){//Delete a medicine from the table
                LETSHASH *temp =DeleteRecord();
                if (temp!=NULL){
                    sizeTable--;
                }
            }

            else if (t==16){//search a medicine in the table
                searchMedicineInTable(hashTable);
            }

            else if (t==17){//save back the data that in the table to pharmacy.txt file
                printf("%d",sizeTable);
                SaveHashTableBackToPharmacytxt(sizeTable);
            }
        printf("\nchoose one\n1-Read the file Meds.txt that contains the medicines infornation.\n2-Insert a new medicine with all its associated information.\n3-Find a medicine and modify it\n4-print the medicine in alphabetical order.\n5-print all medicines that have the same category.\n6-print all medicines that treat the same disease.\n7-Delete a medicine.\n8-Delete all medicines that related to the same company.\n9-Save back all the data to pharmcay.txt file.\n11-print all the spots(including deleted, empty spot).\n12-print how many medicine in the table.\n13-insert new medicine to the table.\n14-print used function for hashing\n15-Delete a medicine from the table\n16-search a medicine in the table\n17-save back the data that in the table to pharmacy.txt file:");
        scanf("\n%d",&t);
    }
    return 0;
}

