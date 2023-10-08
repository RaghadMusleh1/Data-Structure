#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct TEAM{
    char Name[10];
    char Code[3];
    int NumberOfWins;
    int NumberOfDraws;
    int GoalsDiffrance;
    int NumberOfLoses;
    int points;
    struct TEAM* NextNode;
    struct TEAM* LPlayers;//pointing on the list which has the elementary players
    struct TEAM* QPlayers;//pointing in the queue which has the spare players
};
struct PlayersPartOne{
    char PlayerName1[20];
    struct PlayersPartOne* Next;
};
struct PlayersPartTwo_QueueRecord{
    int capacity;
    int front;
    int rare;
    int size;
    char *array[23];
};
typedef struct TEAM *TeamsInformation;
typedef struct PlayersPartOne *ElementaryPlayer;
typedef struct PlayersPartTwo_QueueRecord *SparePlayers;

TeamsInformation MakeEmptyTeam(TeamsInformation teamData){ //make the head to start reading the data of the teams from teams.txt

    if (teamData!=NULL)
        DeleteListTeam(teamData);

    teamData=(TeamsInformation)malloc(sizeof(struct TEAM));


    if (teamData==NULL)
        printf("Out of memory");

    teamData->NextNode=NULL;
    return teamData;
};

void DeleteListTeam (TeamsInformation teamData){//if the list wasn't empty, delete all the nodes which inside of it;
    TeamsInformation Pointer,Temp;
    Pointer=teamData->NextNode;
    teamData->NextNode=NULL;

    while(Pointer!=NULL){
        Temp=Pointer->NextNode;
        free(Pointer);
        Pointer=Temp;
    }

}

void InsertTeam (char name[],char code1[],int NoOfWins,int NoOfDraws,int NoOfLoses,int GoalsDiffrance,struct TEAM* Pointer1,TeamsInformation list){
    TeamsInformation NewNode;// This  void function to insert the data which was read from teams.txt into the linked list.
    NewNode=(TeamsInformation)malloc(sizeof (struct TEAM));

    strcpy(NewNode->Name,name);
    strcpy(NewNode->Code,code1);
    NewNode->NumberOfWins=NoOfWins;
    NewNode->NumberOfDraws=NoOfDraws;
    NewNode->NumberOfLoses=NoOfLoses;
    NewNode->GoalsDiffrance=GoalsDiffrance;
    NewNode->points=NoOfWins*3+NoOfDraws;
    NewNode->NextNode=Pointer1->NextNode;
    Pointer1->NextNode=NewNode;
}

TeamsInformation FindLast(TeamsInformation TeamData){//this struct return a pointer struct pointing on the node which is located at the end of the list.
    TeamsInformation Last,PreLast;
    PreLast=TeamData;
    Last=TeamData->NextNode;

    while (Last!=NULL){
        PreLast=Last;
        Last=Last->NextNode;
    }
    return PreLast;
};

void PrintTeamsList(TeamsInformation TeamData){// this void function print all the data which is stored on the list.
    TeamsInformation pointer;
    pointer=TeamData;
    printf("Name\tCode\tWins\tDraws/tLoses\tGoals Differences\n\n");
    while (!IsTeamLast(pointer,TeamData)){
        pointer=pointer->NextNode;
        printf("%s\t%s\t%d\t%d\t%d\t%d\n",pointer->Name,pointer->Code,pointer->NumberOfWins,pointer->NumberOfDraws,pointer->NumberOfLoses,pointer->GoalsDiffrance);

    }
}

int IsTeamLast(TeamsInformation p, TeamsInformation TeamData){
    return p->NextNode==NULL;
}

TeamsInformation FindPrevious(int pointsValue, TeamsInformation DataTeam){//This struct return a pointer struct pointing on the node which is the previous of the node which has points equal to the integer pointsValue.
    TeamsInformation PreNode, Node;
    PreNode=DataTeam;
    Node=DataTeam->NextNode;
    while (Node!=NULL&&Node->points!=pointsValue){
        PreNode=Node;
        Node=Node->NextNode;
    }

    return PreNode;
};

void deleteNode (TeamsInformation TeamData, int PointsValue){//This void function delete a node which has points equal to the integer PointsValue
    TeamsInformation Temp, Pointer;
    Pointer=FindPrevious(PointsValue,TeamData);//pointing in the node which is before the node which we want to delete to attach it to the node which is next to the node that we want to delete.

    if (!IsTeamLast(Pointer,TeamData)){
        Temp=Pointer->NextNode;
        Pointer->NextNode=Temp->NextNode;
        free(Temp);
    }
}

int FindMax(TeamsInformation DataTeam){//this integer function return the number of the digits on the maximum number(and the number here is the pointsValue)
    TeamsInformation p1=DataTeam->NextNode,p2=p1->NextNode;
    int max=p1->points;
    while (p1!=NULL&&p2!=NULL){//to find the maximum number(the number here is the points for the teams).
        if (max<p2->points){
            max=p2->points;
        }
        p1=p2;
        p2=p2->NextNode;
    }

    int SizeOfRadixLoop=0;
    while (max!=0){//to calculate the number of the digits in the maximum number(points).
        max/=10;
        SizeOfRadixLoop++;
    }

    return SizeOfRadixLoop;
}



TeamsInformation NineDigits[10];


void RadixSortList (TeamsInformation TeamData){

    int NumOfNodes=0;//to calculate the number of the nodes which was inserted into the linked list.
    TeamsInformation pointerToNode=TeamData;
    pointerToNode=TeamData->NextNode;

    while (pointerToNode!=NULL){//this loop calculate the number of the nodes in the linked list(teams list).
        NumOfNodes++;
        pointerToNode=pointerToNode->NextNode;
    }

    for (int i=0;i<10;i++){//to make digit list empty:
        NineDigits[i]=MakeEmptyTeam(NULL);
    }

    int SizeOfRadixLoop=0;
    SizeOfRadixLoop=FindMax(TeamData);// to find the size of the loop:

    pointerToNode=TeamData->NextNode;
    int m=10,w=1,Digit;

    for (int i=0;i<NumOfNodes;i++){//this loop was created to copy the data to the 9 linked list according to the least significant digit.
            Digit=pointerToNode->points%m;
            Digit/=w;
            Copy(NineDigits[Digit],pointerToNode);// copy the node which is in team linked list to the 9 linked list according to the least significant digit
            pointerToNode=pointerToNode->NextNode;//go to the next node in the team linked list

    }

   TeamsInformation Temp=(struct TEAM*)malloc(sizeof(struct TEAM));
   TeamsInformation copy=(struct TEAM*)malloc(sizeof(struct TEAM));

    m*=10;w*=10;
    for (int j=0;j<SizeOfRadixLoop;j++){
        for (int r=0;r<10;r++){
            pointerToNode=NineDigits[r]->NextNode;//after the data was copied to the 9 linked list,here we will start sort every node.

            while (pointerToNode!=NULL){
                Digit=pointerToNode->points%m;
                Digit/=w;

                if (Digit!=r){//if the digit not equal to the number of that list which it's located at.
                copy=FindPrevious(pointerToNode->points,NineDigits[r]);//the struct copy is a pointer that points to a node which is before the node that we want to delete.

                strcpy(Temp->Name,pointerToNode->Name);//Start copying the data which is inside the node that we want to delete.
                strcpy(Temp->Code,pointerToNode->Code);
                Temp->NumberOfWins=pointerToNode->NumberOfWins;
                Temp->NumberOfDraws=pointerToNode->NumberOfDraws;
                Temp->NumberOfLoses=pointerToNode->NumberOfLoses;
                Temp->GoalsDiffrance=pointerToNode->GoalsDiffrance;
                Temp->points=pointerToNode->points;

                deleteNode(NineDigits[r] ,pointerToNode->points);//delete the node that we want to transfer to another linked list.
                Copy(NineDigits[Digit],Temp);//copy the list which was deleted from linked list to another one.
                pointerToNode=copy; //pointerToNode will point on the node which is before the node which was deleted
                }

                pointerToNode=pointerToNode->NextNode;
            }
        }
        m*=10,w*=10;
    }
    printf("The teams were sorted successfully \n");
}

void Copy(TeamsInformation Digits, TeamsInformation Data){
    TeamsInformation Temp, p1, p2;
    Temp=(struct TEAM*)malloc(sizeof(struct TEAM));

    strcpy(Temp->Name,Data->Name);//start copying the node from a node to temp node.
    strcpy(Temp->Code,Data->Code);
    Temp->NumberOfWins=Data->NumberOfWins;
    Temp->NumberOfDraws=Data->NumberOfDraws;
    Temp->NumberOfLoses=Data->NumberOfLoses;
    Temp->points=Data->points;
    Temp->GoalsDiffrance=Data->GoalsDiffrance;
    p1=Digits;// p= the head of the list that we want to insert the temp node to.
    p2=Digits->NextNode;//pp= the first node of the list that we want to insert the temp node to.
    int i=0;



         if (p1!=NULL&&p2!=NULL&&p2->points==Data->points){// If the points of the node that is in the list= the points of the node that we want to transfer then
            while (p2!=NULL&&p2->points==Data->points){//this while loop to compare the nodes which have the same points by their goals difference

                 if (p2->GoalsDiffrance>Data->GoalsDiffrance){//if the goal diff of the node that is in the list > goals diff of the node that we want to transfer to its right place then go to the next node in the list.
                    p1=p2;
                    p2=p2->NextNode;
                 }
                 else{//else if goal diff of the node that is in the list > or = goals diff of the node that we want to transfer to its right place then break the 2 loops because it's its right place.
                    i++;//i here to break from the first loop.
                    break;
                 }
            }
        }

    Temp->NextNode=p1->NextNode;
    p1->NextNode=Temp;
}

int IsQFull(SparePlayers player){// to check if the spare players list is full or not
    return player->size==player->capacity;
}

SparePlayers createQ( int MaxElement){// to create a spare players list

    SparePlayers player;
    player=(SparePlayers)malloc(sizeof(struct PlayersPartTwo_QueueRecord));

    if (player==NULL)
        printf("out of space\n");

    for (int i=0;i<23;i++)
        player->array[i]=(char*)malloc(sizeof(char)*MaxElement);

    player->capacity=MaxElement;
    makeQEmpty(player);

    return player;
};

void makeQEmpty(SparePlayers player){// to give the initial values for size, front and rare.
    player->size=0;
    player->front=1;
    player->rare=0;
}


int succ (int vlaue,SparePlayers player){//which keeps the queue circular.
    if (++vlaue==player->size)
        vlaue=0;

    return vlaue;
}


void Enqueue (char Name[10],SparePlayers player){// to enqueue a new node to the spare queue
    if (IsQFull(player))
        printf("Out of memory");

    else{
        player->size++;
        player->rare=succ(player->rare,player);
        strcpy(player->array[player->rare],Name);
    }
}

void DisposeQueue ( SparePlayers Q ){
    int i=0;
    if( Q != NULL ){
        free( Q->array[i] );
        free( Q );
        i++;
    }
}


void PrintTeamsToFile (TeamsInformation sortedTeams, FILE *output){//To print the data which was sorted (Radix sort) to file teamsInfo.txt
    TeamsInformation p=sortedTeams->NextNode;//pointer to each node in the NineDigits lists to print the data of the teams.
    while (p!=NULL){
        fprintf(output,"%s\t%s\t%d\t%d\t%d\t%d\n",p->Name,p->Code,p->NumberOfWins,p->NumberOfDraws,p->NumberOfLoses,p->GoalsDiffrance);
        p=p->NextNode;
    }
}

TeamsInformation FindPreviousUsingCode(TeamsInformation Teams,char code [4]){
    TeamsInformation preWanted, WantedNode;
    preWanted=Teams;
    WantedNode=preWanted->NextNode;//p1 points at the head of Teams linked list and p2 points on the next of the head.
    while(preWanted!=NULL&&WantedNode!=NULL){
        if (strcmp(WantedNode->Code,code)==0)
            break;
        preWanted=WantedNode;
        WantedNode=WantedNode->NextNode;
    }
    return preWanted;
}

void DeleteTeam(TeamsInformation List1){//Delete a team from the Teams linked list.
    printf("Enter the code of the team that you want to delete:");
    char code[4];
    scanf("%s",&code);

    TeamsInformation preWantedNode=FindPreviousUsingCode(List1,code);

    TeamsInformation temp;
    temp=preWantedNode->NextNode;//deleting the team.
    preWantedNode->NextNode=temp->NextNode;
    free(temp);
    printf("The team was deleted successfully\n");
}

void DeleteElemetaryplayers(ElementaryPlayer playerList){
    ElementaryPlayer Pointer,Temp;
    Pointer=playerList->Next;
    playerList->Next=NULL;

    while(Pointer!=NULL){
        Temp=Pointer->Next;
        free(Pointer);
        Pointer=Temp;
    }
}

void AddTeam(TeamsInformation list){//This void function to add a new team to teams linked list.
    char name[10],code[4],ch;
    int wins,draws,loses,diff;
    TeamsInformation p;
    printf("Enter the data of the team that you want to add: Name, code, NumberOfWins, NumberOfDraws, numberOfLoses, NumberOfGoalDifferences.\n");
    scanf("%c",&ch);
    scanf("%s",name);
    scanf("%s",code);
    scanf("%d%d%d%d",&wins,&draws,&loses,&diff);
    InsertTeam(name,code,wins,draws,loses,diff,list,list);//insert the new team to the list.
    printf("The team was added successfully\n");
}


void ModifyTeam (TeamsInformation Teams){//to modify a team that in the linked list.

    printf("Enter the code of the team that you want to modify:");
    char code[4];
    scanf("%s",code);//code of the team that you want to modify.

    TeamsInformation WantedToEdit;
    WantedToEdit=Teams->NextNode;//pointing on the team in teamsInfo linked list.

    while (WantedToEdit!=NULL&&strcmp(WantedToEdit->Code,code)!=0){//while the program didn't find the team that we want to modify its data.
        WantedToEdit=WantedToEdit->NextNode;//go to the next team.
    }

    if (WantedToEdit==NULL){
        printf("The code is not related to any team!\n");
        return 0;
    }
    printf ("Select what you want to modify\n1-Name\n2-Code\n3-Number of wins\n4-Number of draws\n5-Number of Draws\n6-Number of loses and enter 7 to end:");
    int whatToEdit;//to select what to edit
    scanf("%d",&whatToEdit);

    while (whatToEdit!=7){
        if (whatToEdit==1){ //Modify the name
            char name[10];
            printf("Enter the new name:");
            scanf("%s",name);
            strcpy(WantedToEdit->Name,name);
        }

        if (whatToEdit==2){//Modify the code
            char code[3];
            printf("Enter the new code:");
            scanf("%s",code);
            strcpy(WantedToEdit->Code,code);
        }

        if (whatToEdit==3){//Modify the Number of wins
            int wins;
            printf("Enter the new wins num:");
            scanf("%d",&wins);
            WantedToEdit->NumberOfWins=wins;
            WantedToEdit->points=3*WantedToEdit->NumberOfWins+WantedToEdit->NumberOfDraws;
        }

        if (whatToEdit==4){//Modify the number of draws
            int draws;
            printf("Enter the new draws num:");
            scanf("%d",&draws);
            WantedToEdit->NumberOfDraws=draws;
            WantedToEdit->points=3*WantedToEdit->NumberOfWins+WantedToEdit->NumberOfDraws;
        }

        if (whatToEdit==5){//Modify the number of loses
            int loses;
            printf("Enter the new loses num:");

            scanf("%d",&loses);
            WantedToEdit->NumberOfLoses=loses;
        }

        if (whatToEdit==6){//Modify the goals differences
            int diff;
            printf ("Enter the new goal difference num:");
            scanf("%d",&diff);
            WantedToEdit->GoalsDiffrance=diff;
        }
        printf ("Select what you want to modify\n1-Name\n2-Code\n3-Number of wins\n4-Number of draws\n5-Number of Draws\n6-Number of loses and enter 7 to end:");
        scanf("%d",&whatToEdit);
    }
    printf("The team was modified successfully\n");
}

void CreateMatch(TeamsInformation Teams){
    printf ("Enter the code of the two teams\n");
    char code1[7],code2[7];

    scanf("%s",code1);
    scanf("%s",code2);

    int flag1=0,flag2=0;
    TeamsInformation WantedTeam=Teams->NextNode;

    while (WantedTeam!=NULL){
        if (strcmp(WantedTeam->Code,code1)==0)//If the first code that the user entered is in teams linked list make flag 1 true
            flag1=1;
        if (strcmp(WantedTeam->Code,code2)==0)//If the second code that the user entered is in teams linked list make flag 2 true
            flag2=1;

        if (flag1==1&&flag2==1)// If the two codes were found in teams lined list then break.
            break;

        WantedTeam=WantedTeam->NextNode;//go to the next team in teams linked list.
    }

    if (flag1==1&&flag2==1)// If the two codes were found in teams lined list
        printf("The match was created\n");

    else
        printf("Error:one of these code or both are not found\n");
}

ElementaryPlayer findLastPlayerInLinkedList(ElementaryPlayer PlayerLinkedList){
    ElementaryPlayer Last, PreLast;
    PreLast=PlayerLinkedList;
    Last=PlayerLinkedList->Next;

    while (Last!=NULL){
        PreLast=Last;
        Last=Last->Next;
    }
    return PreLast;
};


ElementaryPlayer makeEmptyPlayerLinkedList(ElementaryPlayer list){//To make Player linked list empty.

    list=(ElementaryPlayer)malloc(sizeof(struct PlayersPartOne));
    list->Next=NULL;
};


ElementaryPlayer FindLastInPlayerList(ElementaryPlayer playerList){
    ElementaryPlayer pointer1=playerList,pointer2=pointer1->Next;
    while (pointer2!=NULL){
        pointer1=pointer2;
        pointer2=pointer2->Next;
    }
    return pointer1;
}
void ReadPlayers(TeamsInformation TeamsList, ElementaryPlayer playerL, SparePlayers playerQ, FILE* input){

    char chForAsterisk, code[6],Name[10];

    fscanf(input,"%c%s",&chForAsterisk,code);//read the asterisk in the char ch, and the code of the team in the string code.

    TeamsInformation PointerOnTeam;
    PointerOnTeam=TeamsList->NextNode;

    while (PointerOnTeam!=NULL){//This while loop to find the the team which was scanned its code to make its players pointer point on the list and the queue which we will insert the players into.

        if (strcmp(PointerOnTeam->Code,code)==0)//compare the code with the codes in teams linked list.
            break;// if the two codes were the same then its the team that we want.

        PointerOnTeam=PointerOnTeam->NextNode;// else go to the next team.
    }

    if (PointerOnTeam==NULL)//if the team wasn't found then return zero.
        return 0;

    else{// if the team was found then make it points to the list and the queue which will contain the names of the players.
        PointerOnTeam->LPlayers=playerL;
        PointerOnTeam->QPlayers=playerQ;
    }

    int num;//will take the number of the player.
    ElementaryPlayer Player, p;// Player here to take the name of  the player from and attach to player linked list.

    for (int i=0;i<11;i++){//This loop will read 11 players(elemanaty) from the file and insert their names to players linked list.
        fscanf(input,"%c",&chForAsterisk);//To scanf (\n) (which was entered before to start a new line).
        fscanf(input,"%d%c%s",&num,&chForAsterisk,Name);//To scanf the number of the player, the character '-' and the name of the player from the file.

        Player=(ElementaryPlayer)malloc(sizeof(struct PlayersPartOne));//creating new node.
        ElementaryPlayer Last=FindLastInPlayerList(playerL);
        strcpy(Player->PlayerName1,Name);//Copying the name of the player from the file to the created node.
        Player->Next=Last->Next;
        Last->Next=Player;
        //Player->Next=playerL->Next;//Inserting the node which was created in the front of the player linked list.
        //playerL->Next=Player;
    }

    for (int i=0;i<11;i++){//This loop will read 11 players(spare) from the file and insert their names to players Queue.
        fscanf(input,"%c",&chForAsterisk);
        fscanf(input,"%d%c%s",&num,&chForAsterisk,Name);//To scanf the number of the player, the character '-' and the name of the player from the file.
        Enqueue(Name,playerQ);//Enqueue the name of the player to the player Queue.
    }

    fscanf(input,"%c",&chForAsterisk);//To scanf (\n) (which was entered before starting a new line).

}

void IsQueueEmpty(SparePlayers SparePlayer){
    return SparePlayer->size==0;
}


void changePlayer(ElementaryPlayer playerList,SparePlayers playerQueue){//Add player from elementary list to spare queue.

    char PlayerNameInList[20],PlayerNameInqueue[20];
    printf("Enter the name of the player who you want to add to the spare:");
    scanf("%s",PlayerNameInList);

    ElementaryPlayer pointerToPlayerName=playerList->Next;

    while (pointerToPlayerName!=NULL){
        if (strcmp(PlayerNameInList,pointerToPlayerName->PlayerName1)==0){//if the name of the player which the user entered was found.
            //starting switching players between the queue and the list.

            strcpy(PlayerNameInqueue,playerQueue->array[0]);//take a copy of the first player in the queue and store it in the string (PlayerNameInqueue).
            strcpy(pointerToPlayerName->PlayerName1,PlayerNameInqueue);//The string (PlayerNameInqueue) will be the new value instead of the player which the user entered.

            strcpy(playerQueue->array[0],PlayerNameInList);//Store the player which was entered by the player in the queue.
            printf("The player was added to the spare\n");
            break;
        }

        else
            pointerToPlayerName=pointerToPlayerName->Next;//else if the name of the player which the user entered was found then go to the next player.
    }

    if (pointerToPlayerName==NULL)//if the player wasn't found in the whole elementary list.
        printf("The name is not found in the list\n");

}

void deletePlayer(TeamsInformation Teams, ElementaryPlayer playerList,char code [4]){
    TeamsInformation pointer=Teams->NextNode;
    char PlayerName[20];
    printf("Enter the name of the player who had the red card:");
    scanf("%s",PlayerName);
    ElementaryPlayer p1, p2;

    while(pointer!=NULL){//searching about the team which have the code that the user entered.
        if (strcmp(code,pointer->Code)==0){
            break;
        }
        pointer=pointer->NextNode;
    }

    if (pointer==NULL){//if the code is not found.
        printf("The code is not related to a team\n");
        return 0;
    }

    p1=pointer->LPlayers;p2=p1->Next;
    while (p1!=NULL&&p2!=NULL){

        if (strcmp(PlayerName,p2->PlayerName1)==0){
            break;
        }

        else{
            p1=p2;
            p2=p2->Next;
        }
    }
    if (p1==NULL||p2==NULL){
        printf("Player is not found\n");
        return 0;
    }
    p1->Next=p2->Next;
    printf("The player %s is out\n",p2->PlayerName1);
    free(p2);

}

void printPlayers(TeamsInformation Teams){
    char code[10];
    printf("Enter the code of the team that you want to print its player:");
    scanf("%s",code);

    TeamsInformation pointerToTeam=Teams->NextNode;
    while (pointerToTeam!=NULL){
        if (strcmp(code,pointerToTeam->Code)==0)
            break;
        pointerToTeam=pointerToTeam->NextNode;
    }
    int i=1;


        if(strcmp(pointerToTeam->Code,code)==0){//if the code that the user entered was found

            ElementaryPlayer elemntary=pointerToTeam->LPlayers;//pointing to the head of the list which contains the elementary players.
            elemntary=elemntary->Next;//go to the first player node.

            while(elemntary!=NULL) {//to print all the elementary players
                printf("%d-%s\n",i,elemntary->PlayerName1);
                elemntary=elemntary->Next;
                i++;//just a counter of the player numbers.
            }

            SparePlayers Spare=pointerToTeam->QPlayers;//pointing on the queue which has the spare players.

            for (int j=0;j<11;j++){//printing the spare players.
                printf("%d-%s\n",i,Spare->array[j]);
                i++;
            }

        }
        else
            pointerToTeam=pointerToTeam->NextNode;

    if (pointerToTeam==NULL)//if the code that the user entered wasn't found.
        printf("The code is not related to a team!");

}

void PrintPlayersToFile (SparePlayers SpareP, ElementaryPlayer Elementary, char code[4], FILE* out){

    fprintf(out,"*%s\n",code);//print the code of the team in the file.

    ElementaryPlayer pointerToElementaryName=Elementary->Next;//pointing on the first player in the elementary player list.
    int i=1;//to print the number of the player.

    while (pointerToElementaryName!=NULL){//while you didn't reach the last player in the elementary player list.
        fprintf(out,"%d-%s\n",i,pointerToElementaryName->PlayerName1);//print  the number and the name of the player.
        pointerToElementaryName=pointerToElementaryName->Next;//go to the next player.
        i++;//number of the next player.
    }

     for (int j=0;j<11;j++){
        fprintf(out,"%d-%s\n",i,SpareP->array[j]);
        i++;
     }
}

int CountTheTeams(TeamsInformation Teams){//to count the number of the teams.

    TeamsInformation pointerToTeam=Teams->NextNode;
    int counter=0;

    while (pointerToTeam!=NULL){

        counter++;
        pointerToTeam=pointerToTeam->NextNode;
    }

    return counter;
}

void ReadTeamsInfo (TeamsInformation teamsInfo,FILE* input){//Reading teams data from teams.txt file.

    char read[30],num[10],code1[10],name[10];//initializing the data of the team.
    int t=0,NoOfWins,NoOfDraws,NoOfLoses,GoalsDiffrance;//initializing the data of the team.

    while (!feof(input)){
        fgets(read,30,input);

        char *team;
        team=strtok(read,",");//Reading the name of the team from the file.
        strcpy(name,team);

        team=strtok(NULL,",");//Reading the code of the team from the file.
        strcpy(code1,team);

        team=strtok(NULL,",");//Reading the number of wins for the team from the file.
        strcpy(num,team);
        NoOfWins=atoi(num);

        team=strtok(NULL,",");//Reading the number of draws for the team from the file.
        strcpy(num,team);
        NoOfDraws=atoi(num);

        team=strtok(NULL,",");//Reading the number of loses for the team from the file.
        strcpy(num,team);
        NoOfLoses=atoi(num);

        team=strtok(NULL,",");//Reading the number of goals difference for the team from the file.
        strcpy(num,team);
        GoalsDiffrance=atoi(num);

        InsertTeam(name,code1,NoOfWins,NoOfDraws,NoOfLoses,GoalsDiffrance,teamsInfo,teamsInfo);//Insert these data to TeamsInfo linked list.
    }
}
ElementaryPlayer playerLinkedList[25];
SparePlayers playerQueue[25];
int main()
{
    TeamsInformation TeamsInfo=MakeEmptyTeam(NULL);
    TeamsInformation Pointer1;
    int t;
    printf("choose one\n1-Read the file teams.txt that contains the teams information.\n2-Sort the teams according to their points and save back the teams information (sorted) to the file teamsInfo.txt \n3-Add,modify,delete a team\n4-Print all teams with their relevant information .\n5-Save back the teams information (sorted) to the file teamsInfo.txt.\n6-Create a match.\n7-Read the teams’ players lists from the file players.txt.\n8- Change the players between elementary and spare.\n9-Give a red card to a player.\n10-Print all elementary players for a specific team.\n11-Save back the players information to the file playersInfo.txt.\n12-Exit\n");
    scanf("\n%d",&t);

    while (1){

        if (t==1){// if the user want to read teams data from file.
           FILE *Input=fopen("Teams.txt","r");
           ReadTeamsInfo(TeamsInfo,Input);
           int NumOfTeams=CountTheTeams(TeamsInfo);
        }

        else if (t==2){//sorting the teams according to their points and goals difference.
            if (TeamsInfo->NextNode==NULL){
                printf("Error: No teams in the list\n\n");
                printf("choose one\n1-Read the file teams.txt that contains the teams information.\n2-Sort the teams according to their points and save back the teams information (sorted) to the file teamsInfo.txt \n3-Add,modify,delete a team\n4-Print all teams with their relevant information .\n5-Save back the teams information (sorted) to the file teamsInfo.txt.\n6-Create a match.\n7-Read the teams’ players lists from the file players.txt.\n8- Change the players between elementary and spare.\n9-Give a red card to a player.\n10-Print all elementary players for a specific team.\n11-Save back the players information to the file playersInfo.txt.\n12-Exit\n");
                scanf("\n%d",&t);
                continue;
            }

            RadixSortList(TeamsInfo);
        }

        else if (t==3){//To modify a team, Add team or delete a team.

            if (TeamsInfo->NextNode==NULL){
                printf("Error: No teams in the list\n\n");
                printf("choose one\n1-Read the file teams.txt that contains the teams information.\n2-Sort the teams according to their points and save back the teams information (sorted) to the file teamsInfo.txt \n3-Add,modify,delete a team\n4-Print all teams with their relevant information .\n5-Save back the teams information (sorted) to the file teamsInfo.txt.\n6-Create a match.\n7-Read the teams’ players lists from the file players.txt.\n8- Change the players between elementary and spare.\n9-Give a red card to a player.\n10-Print all elementary players for a specific team.\n11-Save back the players information to the file playersInfo.txt.\n12-Exit\n");
                scanf("\n%d",&t);
                continue;
            }

            int WhatToDo;
            printf("If you want to delete team enter 1\n");
            printf("If you want to add team enter 2\n");
            printf("If you want to modify a team enter 3\n");
            scanf("%d",&WhatToDo);

            if (WhatToDo==1)
                DeleteTeam(TeamsInfo);

            else if (WhatToDo==2)
                AddTeam(TeamsInfo);

            else
                ModifyTeam(TeamsInfo);
        }

        else if (t==4){//to print teams information.

            if (TeamsInfo->NextNode==NULL){
                printf("Error: No teams in the list\n\n");
                printf("choose one\n1-Read the file teams.txt that contains the teams information.\n2-Sort the teams according to their points and save back the teams information (sorted) to the file teamsInfo.txt \n3-Add,modify,delete a team\n4-Print all teams with their relevant information .\n5-Save back the teams information (sorted) to the file teamsInfo.txt.\n6-Create a match.\n7-Read the teams’ players lists from the file players.txt.\n8- Change the players between elementary and spare.\n9-Give a red card to a player.\n10-Print all elementary players for a specific team.\n11-Save back the players information to the file playersInfo.txt.\n12-Exit\n");
                scanf("\n%d",&t);
                continue;
            }

            PrintTeamsList(TeamsInfo);
        }

        else if (t==5){

            if (TeamsInfo->NextNode==NULL){
                printf("Error: No teams in the list\n\n");
                printf("choose one\n1-Read the file teams.txt that contains the teams information.\n2-Sort the teams according to their points and save back the teams information (sorted) to the file teamsInfo.txt \n3-Add,modify,delete a team\n4-Print all teams with their relevant information .\n5-Save back the teams information (sorted) to the file teamsInfo.txt.\n6-Create a match.\n7-Read the teams’ players lists from the file players.txt.\n8- Change the players between elementary and spare.\n9-Give a red card to a player.\n10-Print all elementary players for a specific team.\n11-Save back the players information to the file playersInfo.txt.\n12-Exit\n");
                scanf("\n%d",&t);
                continue;
            }

            FILE* output=fopen("teamsInfo.txt","w");
            fprintf(output,"Name\t\tCode\tWins\tDraws\tLoses\tGoals Differences\t\n\n");

            for (int i=9;i>-1;i--){//printing data to the file decreasing.
                if (NineDigits[i]->NextNode!=NULL)
                    PrintTeamsToFile(NineDigits[i],output);
            }

            fclose(output);
        }

        else if (t==6){
            if (TeamsInfo->NextNode==NULL){
                printf("Error: No teams in the list\n\n");
                printf("choose one\n1-Read the file teams.txt that contains the teams information.\n2-Sort the teams according to their points and save back the teams information (sorted) to the file teamsInfo.txt \n3-Add,modify,delete a team\n4-Print all teams with their relevant information .\n5-Save back the teams information (sorted) to the file teamsInfo.txt.\n6-Create a match.\n7-Read the teams’ players lists from the file players.txt.\n8- Change the players between elementary and spare.\n9-Give a red card to a player.\n10-Print all elementary players for a specific team.\n11-Save back the players information to the file playersInfo.txt.\n12-Exit\n");
                scanf("\n%d",&t);
                continue;
            }

            CreateMatch(TeamsInfo);
        }

        else if (t==7){
            if (TeamsInfo->NextNode==NULL){
                printf("Error: No teams in the list\n\n");
                printf("choose one\n1-Read the file teams.txt that contains the teams information.\n2-Sort the teams according to their points and save back the teams information (sorted) to the file teamsInfo.txt \n3-Add,modify,delete a team\n4-Print all teams with their relevant information .\n5-Save back the teams information (sorted) to the file teamsInfo.txt.\n6-Create a match.\n7-Read the teams’ players lists from the file players.txt.\n8- Change the players between elementary and spare.\n9-Give a red card to a player.\n10-Print all elementary players for a specific team.\n11-Save back the players information to the file playersInfo.txt.\n12-Exit\n");
                scanf("\n%d",&t);
                continue;
            }

            int NumOfTeams=CountTheTeams(TeamsInfo);//the number of teams that we have in the list.

            for (int i=0;i<NumOfTeams;i++){//creating elementary players list and spare players queue.
                playerLinkedList[i]=makeEmptyPlayerLinkedList(NULL);
                playerQueue[i]=createQ(23);
            }

            FILE* input=fopen("players.txt","r");

            for (int i=0;i<NumOfTeams;i++){//reading the players names of each team.

                ReadPlayers(TeamsInfo,playerLinkedList[i],playerQueue[i],input);

                ElementaryPlayer p=playerLinkedList[i]->Next;
                SparePlayers p2=playerQueue[i];

            }
        }

        else if (t==8){//if you want to change between elementary players and spare players.
            if (TeamsInfo->NextNode==NULL|| playerLinkedList[0]==NULL){
                printf("Error: No teams in the list or no player in the list\n\n");
                printf("choose one\n1-Read the file teams.txt that contains the teams information.\n2-Sort the teams according to their points and save back the teams information (sorted) to the file teamsInfo.txt \n3-Add,modify,delete a team\n4-Print all teams with their relevant information .\n5-Save back the teams information (sorted) to the file teamsInfo.txt.\n6-Create a match.\n7-Read the teams’ players lists from the file players.txt.\n8- Change the players between elementary and spare.\n9-Give a red card to a player.\n10-Print all elementary players for a specific team.\n11-Save back the players information to the file playersInfo.txt.\n12-Exit\n");
                scanf("\n%d",&t);
                continue;
            }

            printf("Enter the code of the team:");
            char code1[4];
            scanf("%s",code1);

            Pointer1=TeamsInfo->NextNode;//pointing on the first team.

            while (Pointer1!=NULL){//while loop to find the team that we want to change its player by the code.

                if (strcmp(Pointer1->Code,code1)==0){
                   break;
                }

                Pointer1=Pointer1->NextNode;
            }
            if (Pointer1==NULL)
            {
                printf("The code is not related to team\n");
                printf("choose one\n1-Read the file teams.txt that contains the teams information.\n2-Sort the teams according to their points and save back the teams information (sorted) to the file teamsInfo.txt \n3-Add,modify,delete a team\n4-Print all teams with their relevant information .\n5-Save back the teams information (sorted) to the file teamsInfo.txt.\n6-Create a match.\n7-Read the teams’ players lists from the file players.txt.\n8- Change the players between elementary and spare.\n9-Give a red card to a player.\n10-Print all elementary players for a specific team.\n11-Save back the players information to the file playersInfo.txt.\n12-Exit\n");
                scanf("\n%d",&t);
                continue;
            }

            changePlayer(Pointer1->LPlayers,Pointer1->QPlayers);
        }

        else if (t==9){//to delete a player
            if (TeamsInfo->NextNode==NULL|| playerLinkedList[0]==NULL){
                printf("Error: No teams in the list or no player in the list\n\n");
                printf("choose one\n1-Read the file teams.txt that contains the teams information.\n2-Sort the teams according to their points and save back the teams information (sorted) to the file teamsInfo.txt \n3-Add,modify,delete a team\n4-Print all teams with their relevant information .\n5-Save back the teams information (sorted) to the file teamsInfo.txt.\n6-Create a match.\n7-Read the teams’ players lists from the file players.txt.\n8- Change the players between elementary and spare.\n9-Give a red card to a player.\n10-Print all elementary players for a specific team.\n11-Save back the players information to the file playersInfo.txt.\n12-Exit\n");
                scanf("\n%d",&t);
                continue;
            }
            printf("Enter the code of the team that you want to give its player the red card:");
            char code[4];
            scanf("%s",code);
            deletePlayer(TeamsInfo,playerLinkedList,code);
        }

        else if (t==10){
            if (TeamsInfo->NextNode==NULL|| playerLinkedList[0]==NULL){
                printf("Error: No teams in the list or no player in the list\n\n");
                printf("choose one\n1-Read the file teams.txt that contains the teams information.\n2-Sort the teams according to their points and save back the teams information (sorted) to the file teamsInfo.txt \n3-Add,modify,delete a team\n4-Print all teams with their relevant information .\n5-Save back the teams information (sorted) to the file teamsInfo.txt.\n6-Create a match.\n7-Read the teams’ players lists from the file players.txt.\n8- Change the players between elementary and spare.\n9-Give a red card to a player.\n10-Print all elementary players for a specific team.\n11-Save back the players information to the file playersInfo.txt.\n12-Exit\n");
                scanf("\n%d",&t);
                continue;
            }
            printPlayers(TeamsInfo);
        }
        else if (t==11){
            if (TeamsInfo->NextNode==NULL|| playerLinkedList[0]==NULL){
                printf("Error: No teams in the list or no player in the list\n\n");
                printf("choose one\n1-Read the file teams.txt that contains the teams information.\n2-Sort the teams according to their points and save back the teams information (sorted) to the file teamsInfo.txt \n3-Add,modify,delete a team\n4-Print all teams with their relevant information .\n5-Save back the teams information (sorted) to the file teamsInfo.txt.\n6-Create a match.\n7-Read the teams’ players lists from the file players.txt.\n8- Change the players between elementary and spare.\n9-Give a red card to a player.\n10-Print all elementary players for a specific team.\n11-Save back the players information to the file playersInfo.txt.\n12-Exit\n");
                scanf("\n%d",&t);
                continue;
            }

            TeamsInformation PointerToTeam=TeamsInfo->NextNode;//pointing at the first team of the in teams linked list.
            ElementaryPlayer Elementary=PointerToTeam->LPlayers;//pointing on the player linked list for the team.
            SparePlayers Spare=PointerToTeam->QPlayers;//pointing on the player queue for the team

            char code[4];
            strcpy(code,PointerToTeam->Code);//taking the code of the team.

            FILE* out=fopen("playersInfo.txt","w");
            int NumberOfTeams=CountTheTeams(TeamsInfo);
            for (int i=0;i<NumberOfTeams;i++){
                PrintPlayersToFile(Spare,Elementary,code,out);
                PointerToTeam=PointerToTeam->NextNode;//Go to the next team.

                if(PointerToTeam!=NULL){//while you didin't reach the end of teams linked list.
                strcpy(code,PointerToTeam->Code);//copying the code of the next team.
                Spare=PointerToTeam->QPlayers;//pointing on the player linked list for the next team.
                Elementary=PointerToTeam->LPlayers;//pointing on the player queue for the next team.
                }
            }
           fclose(out);
           printf("The data of the teams were printed successfully\n");
        }

        if (t==12){
            int NumofTeams=CountTheTeams(TeamsInfo);
            for (int i=0;i<10;i++){
                if (NineDigits[i]!=NULL)
                DeleteListTeam(NineDigits[i]);
            }

            for (int j=0;j<NumofTeams;j++){
                DeleteElemetaryplayers(playerLinkedList[j]);
            }
            DeleteListTeam(TeamsInfo);
            for (int i=0;i<NumofTeams;i++){
                DisposeQueue(playerQueue[i]);
            }
            printf("\nAhlen\n");
            return 0;
        }
        printf("choose one\n1-Read the file teams.txt that contains the teams information.\n2-Sort the teams according to their points and save back the teams information (sorted) to the file teamsInfo.txt \n3-Add,modify,delete a team\n4-Print all teams with their relevant information .\n5-Save back the teams information (sorted) to the file teamsInfo.txt.\n6-Create a match.\n7-Read the teams’ players lists from the file players.txt.\n8- Change the players between elementary and spare.\n9-Give a red card to a player.\n10-Print all elementary players for a specific team.\n11-Save back the players information to the file playersInfo.txt.\n12-Exit\n");
        scanf("\n%d",&t);

    }

        return 0;
}








