#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


int letterValues[] = {1, 3, 3, 2, 1, 4, 2,
                      4, 1, 8, 5, 1, 3, 1, 1,
                      3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};
const int LETTERINDEXOFFSET = 65;

const char DOUBLEWORD = '!';
const char TRIPLEWORD = '#';

void printArray(int arraySize, int array[arraySize]){
    printf("{");
    for (int i = 0; i < arraySize; i++) {
        printf("%d,",array[i]);
    }
    printf("}\n");
}

void printArrayCharacters(int arraySize, char array[arraySize]){
    printf("{");
    for (int i = 0; i < arraySize; i++) {
        printf("%c,",array[i]);
    }
    printf("}\n");
}

struct Player{
    char name[16];
    int doubles;
    int triples;
    int score;
} players[2];

char generateLetters(char array[11]) {
    for (int i = 0; i < 10; i++) {
        int randomNumber = rand() % 26;

        randomNumber += LETTERINDEXOFFSET;
        char randomCharacter = (char) randomNumber;
        array[i] = randomCharacter;
    }
    array[10] = '\0';
}
int stringToScore(char word[10], int *doubles, int *triples) {
    char currentChar = word[0];
    int i = 0;

    int score = 0;
    int multiplier = 1;

    if (currentChar == DOUBLEWORD) {
        if (*doubles > 0) {
            *doubles -=1 ;
            multiplier = 2;
            i=1;
        }else{
            printf("You have no more doubles");
        }

    } else if (currentChar == TRIPLEWORD) {
        if (*triples > 0) {
            *triples -=1;
            multiplier = 3;
            i = 1;
        }
        }else{
            printf("You have no more triples\n");
        }

    printf("multiplier %d\n", multiplier);

    while (currentChar != '\0') {
        currentChar = word[i];
        //printf("The current char is %c\n",currentChar);
        //printf("The current char is %c\n",currentChar);
        int letterIndex = (int)currentChar - LETTERINDEXOFFSET;
        //printf("The letter value index is %d\n",letterIndex);

        score += letterValues[letterIndex];

        i++;

    }

    return (score * multiplier);
}

int isSubset(char baseArray[11], char cmpArray[9]) {
    int baseFrequency[26];
    int cmpFrequency[26];

    for (int i = 0; i < 26; ++i) {
        baseFrequency[i] = 0;
        cmpFrequency[i] = 0;
    }


    //printArray(26,baseFrequency);
    //printArray(26,cmpFrequency);

    for (int i = 0; i < 10; i++) {
        char currentChar = baseArray[i];
        int charIndex = (int)currentChar - LETTERINDEXOFFSET;
        baseFrequency[charIndex] += 1;
        //printf("INDEX : %d CURRENTCHAR :%c CHARINDEX : %d\n",i,currentChar,charIndex);
        //printArray(26,baseFrequency);
    }
    //printf("HAND HASHED!");

    int index = 0; //don't like this name
    char currentChar = cmpArray[index];
    while (currentChar != '\0') {
        currentChar = cmpArray[index];
        if (currentChar == '!' || currentChar == '#') {
            index++;
            //printf("char is multiplier");
            continue;
        }

        int charIndex = (int) currentChar - LETTERINDEXOFFSET;
        /*
        printf("The word is : %s\n",cmpArray);
        printf("The letter of the word is : %c\n",currentChar);
        printf("The index of the letter is: %d\n", charIndex);
        printf("The index of the word compared is %d\n", index);*/
        cmpFrequency[charIndex] += 1;
        index++;
    }


/*
    printf("BASEFREQUENCY :");
    printArray(26,baseFrequency);
    printf("COMPFREQUENCY :");
    printArray(26,cmpFrequency);*/

    for (int i = 0; i < 26; i++) {
        if (baseFrequency[i] - cmpFrequency[i] < 0) {
            return 0;
        }
    }
    return 1;
}

void stringUpper(char string[10]){
    int i=0;
    if(string[i] == '!' || string[i] == '#'){
        i=1;
    }
    while(string[i] !='\0'){
        //printf("The letter converting to upper is : %c\n", string[i]);
        string[i] = (char) toupper(string[i]);
        i++;

    }
}

struct ListNode {
    char name[16];
    int score;
    struct ListNode* nextNode;
};

void readScoreLog(FILE* fp) {
    char name[128];
    char score[128];
    char truncatedScore[128];
    char divisionBuffer[128];
    int scoreInt;

    struct ListNode* previousNode = NULL;

    while (fgets(name,sizeof(name),fp) != NULL) {
        fgets(score,sizeof(score),fp);
        fgets(divisionBuffer, sizeof(divisionBuffer), fp);

        printf("%s",name);
        printf("%s",score);
        strncpy(truncatedScore, score+7, 100);
        printf("Length of truncated: %d\n", strlen(truncatedScore));
        scoreInt = atoi(truncatedScore);
        printf("Truncated score: %s",truncatedScore);
        printf("Final product: %d\n\n", scoreInt);

        struct ListNode currentNode;
        strcpy(currentNode.name,name);
        currentNode.score = scoreInt;

        if(previousNode != NULL){
            previousNode->nextNode = &currentNode;
        }
        previousNode = &currentNode;
    }
}


int main() {
    srand(time(NULL));
    char hand[10];
    char inputWord[10];
    int playerTurn = 0;

    FILE *scoreFile;

    scoreFile= fopen("scores.txt","r");
    readScoreLog(scoreFile);

    for (int i = 0; i < 2; ++i) {
        players[i].doubles = 2;
        players[i].triples = 1;
        players[i].score = 0;
        printf("Enter player %d's name :", i+1);
        scanf("%s", players[i].name);
    }

    int roundCount=0;

    while(roundCount < 2){

        playerTurn = playerTurn % 2;

        printf("%s's turn\n",players[playerTurn].name);

        generateLetters(hand);
        printArrayCharacters(10, hand);
        do {
            printf("Enter Word (max 8 letters) :");
            fflush(stdin);
            gets(inputWord);
            stringUpper(inputWord);
        }while( !isSubset(hand, inputWord));

        players[playerTurn].score += stringToScore(inputWord, &players[playerTurn].doubles, &players[playerTurn].triples);
        printf("%s's score is now : %d, they have %d doubles and %d triples\n",
               players[playerTurn].name, players[playerTurn].score, players[playerTurn].doubles, players[playerTurn].triples);

        roundCount++;
        playerTurn++;
    }

    if(players[0].score > players[1].score){
        printf("The winner is %s!", players[0].name);
    } else if(players[0].score == players[1].score){
        printf("It's a draw!");
    } else{
        printf("The winner is %s!", players[1].name);
    }

    FILE *fp;

    fp= fopen("scores.txt","w");
    for (int i= 0; i < 2; i++) {
        fprintf(fp, "Name: %s\nScore: %d\n--------\n", players[i].name, players[i].score);
    }

    fclose(fp);
    return 0;
}
