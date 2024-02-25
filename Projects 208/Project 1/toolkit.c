/*****
 * Project 01: Binary toolkit
 * COSC 208, Introduction to Computer Systems, Fall 2023
 *****/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

 int MAX_BINARY_LEN = 32;

int BtoD(char binary[], char s){ //Stands for Binary to Decimal, does that with needed print statements
    int dec = 0; //var that keeps track of final value
    int base = 1; //base used to left shift on each loop;
   int leng = strlen(binary);
      //printf("This is the length: %d", leng);
   int cap; // copy of base used to go through loop again to show descending
   for (int i = leng-1; i >= 2; i--){
        if (binary[i] == '1'){
            dec += base; //adds base val to dec on each binary spot
           
        }
       // printf("This is the dec: %d \n", dec);
         base <<= 1;
        // printf("This is the base2: %d \n", base2);
    }
    printf("\n");
    //printf("dec is equal to: %u \n", dec);
    //printf("base is equal to: %d \n", base);
     if (binary[2] == '1' && s == 's'){ //if number signed, flips biggest num
           dec -= base;
           //printf("dec is equal to: %d \n", dec);
       }
    base >>= 1;
    cap = base;
  for (int i = 2; i < leng; i++){ //prints out the powers
        if(binary[i] == '1'){
            if (i == 2 && s == 's'){//checks if signed & flips if true
                printf("-2^%d ", leng - i -1);
            }
            else{
                  printf("2^%d ", leng - i -1);
            }
         
        }
        base >>= 1; //shifts base down
  }
  printf("\n");
   for (int i = 2; i < leng; i++){ //prints out the numbers
        if(binary[i] == '1'){
            if (i == 2 && s == 's'){ //checks if signed & flips if true
                printf("%d ", -cap);
            }
            else{
            printf("%d ", cap);
            }
        }
        cap >>= 1; //shifts the base down
  }
    printf("\n");
    printf("%d \n", dec);
    return dec;
   
}

char* DtoB(int decimal, int num_Bits) //stands for Decimal to Binary, does that with requested print statements
{
    int sum1 = 0;
    int sum2 = 0;
    char mult = decimal < 0 ? '-' : ' ';
    char *bin_String = (char *)malloc(num_Bits +1); //allocate enough mem include /0
    bin_String[num_Bits + 2] = '\0'; //add null string terminator

    /*
    char *exp= (char*) malloc(sizeof(char)*3);
    exp[0] = '2';
    exp[1] = '^';
    exp[2] = '1';
    exp[3] = '\0';
    */

   
    if(num_Bits >= MAX_BINARY_LEN)
    {
        bin_String = "INVALID BIT INPUT";
        return bin_String;
    }
   
    int single_Bit = 1 << (num_Bits -1); //create single_Bit to extract bit by bit
    //printf("%c \n", single_Bit);

    char currBit = num_Bits-1 + 'O';
    int counter = num_Bits -1;
    int binTimes = 0;

    for (int i=0; i < num_Bits; i++)
    {
        //exp[3] = currBit;
        bin_String[i] = (decimal & single_Bit) ? '1' : '0'; //check to either add 0 or 1
        if(bin_String[i] == '1')
        {
            if(decimal > 0)
            {
                sum2 = sum1 + single_Bit;
                printf("%d + 2^%d = %d \n", sum1, counter, sum2);
                counter--;
                sum1 = sum2;
            }
            else if(decimal < 0 && binTimes == 0)
            {
                int neg_Bit = -1 * single_Bit;
                sum2 = sum1 + neg_Bit;
                printf("%d + 2^%d = %d \n", sum1, counter, sum2);
                counter--;
                sum1 = sum2;
                binTimes++;
            }
            else{
                sum2 = sum1 + single_Bit;
                printf("%d + 2^%d = %d \n", sum1, counter, sum2);
                counter--;
                sum1 = sum2;
            }
        }

        if(bin_String[i] == '0')
        {
            if(decimal > 0 && binTimes == 0)
            {
                printf("-2^%d does not fit \n", counter);
                binTimes++;
                counter--;
            }

            else if(decimal < 0 && binTimes == 0)
            {
                printf("-2^%d does not fit \n", counter);
                binTimes++;
                counter--;
            }
            else{
                printf("2^%d does not fit \n", counter);
                counter--;
            }
        }

        //single bit is what to add
        //printf("DECIMAL %d SINGLE_BIT %d \n", decimal, single_Bit);
        //printf("BIN_STRING %c \n", bin_String[i]);
        single_Bit >>= 1; //shift bit we are checking by one decimal place
        currBit--;
    }

    //create new array for 0b
    char *final_Arr = (char *)malloc(num_Bits +3); //allocate enough mem include /0 and 0b
    final_Arr[0] = '0';
    final_Arr[1] = 'b';
    int count = 2;
   
    for(int i=0; i <num_Bits; i++)
    {  
        final_Arr[count] = bin_String[i];
        count++;
    }

    printf("%s \n", final_Arr);
    return final_Arr;
}

int NoPBtoD(char binary[], char s){ //Stands for No Print Binary to Decimal, just doesn't include the print statements
     int dec = 0; //var that keeps track of final value
    int base = 1; //base used to left shift on each loop;
   int leng = strlen(binary);
   for (int i = leng-1; i >= 2; i--){
        if (binary[i] == '1'){
            dec += base; //adds base val to dec on each binary spot
           
        }
       // printf("This is the dec: %d \n", dec);
         base <<= 1;
         //printf("This is the base2: %d \n", base2);
    }
    //printf("dec is equal to: %u \n", dec);
    //printf("base is equal to: %d \n", base);
     if (binary[2] == '1' && s == 's'){ //if number signed, flips biggest num
           dec -= base;
           //printf("dec is equal to: %d \n", dec);
       }
   
    return dec;
}

int ArithChecker(char bin1[], char symbol, char bin2[]){ //function that turns binary into dec, does math, and returns, to check work
    printf("\n\nTHIS IS ARITHCHECKER");
    printf("\n  %s \n", bin1);
    printf("%c %s \n", symbol, bin2);
    printf("----------------------");
    int dec1 = NoPBtoD(bin1, 's');
    int dec2 = NoPBtoD(bin2, 's');
    printf("\nnum 1 = %d \n", dec1);
    printf("num 2 = %d \n", dec2);
    int ans;
    if (symbol == '+'){
        ans = dec1 + dec2;
    }
    else{
       ans = dec1-dec2;
    }
    char* bans = DtoB(ans, strlen(bin1));
    printf("Binary ans is %s \n", bans);
    return ans;
}


void NegSwitcher(char bin[]){ //switches the binary sequence value from pos/neg and vice versa
    int len = strlen(bin);
    for (int i = 2; i < len; i++){
        if (bin[i] == '0'){
            bin[i] = '1';
        }
        else if (bin[i] == '1'){
            bin[i] = '0';
        }
    }
    if (bin[len -1] == '0'){
        bin[len -1] = '1';
    }
    else{
        int currPlace = len -1;
        while (bin[currPlace] == '1'){
            bin[currPlace] = '0';
            currPlace--;
        }
        bin[currPlace] = '1';
    }
    return;
}

void checkflow(int flow){ //prints if its an over/underflow
    if (flow > 0){
         printf("\n    Overflow");
     }
     else if (flow < 0){
         printf("\n    Underflow");
     }
     return;
}


void PrintStatements(int dec1, int dec2, int ans, char symbol, char carriedNums[], char bin1[], int flow, char bin2[]){
     printf("\n  %s", carriedNums); 
     printf("\n  %s \n", bin1); 
     printf("+ %s \n", bin2); 
     printf("----------------------\n");
     printf("  %s", bin1);
     checkflow(flow);
     printf("\n %d + %c", dec1, symbol);
     printf("%d = %d \n", dec2, ans);
}



void Arith(char bin1[], char symbol, char bin2[]){
    //FOR PAIGE: CREATE STORAGE WITH MALLOC
    //CHANGE ALL CHANGES FROM BIN 1 TO NEW BIN
    int len1 = strlen(bin1);
    int len1clone = len1;
    int len2 = strlen(bin2);
    char *clonebin1 = (char*) malloc(sizeof(char)*len1);
    char *clonebin2 = (char*) malloc(sizeof(char)*len2);

     for(int i = 0; i < len1; i++)
    {
        clonebin1[i] = bin1[i];
    }

    for(int i = 0; i < len1; i++)
    {
        clonebin2[i] = bin2[i];
    }

    int dec1 = NoPBtoD(bin1, 's'); //You can move these to bottom when
    int dec2 = NoPBtoD(bin2, 's');// new array is made
    int ans;
    int carry = 0;
    int flow = 0;
   
    char *carriedNums = (char*)malloc(sizeof(char)* (len1+1));
    for (int i = 0; i < len1; i++){
        carriedNums[i] = '1';
    }
   
    carriedNums[len1] = '\0';
    carriedNums[len1-1] = ' ';
   
     if (symbol == '-'){
       // printf("Neg was hit yurr");
        NegSwitcher(clonebin2); //have this modify the bin2 clone
    }
       //   printf("\n  %s \n", bin1); 
   //  printf("+ %s \n", bin2); 
   
    
   
    for (int i = len2 -1; i >= 2; i--){
        len1clone--;
       // printf("this is len1clone size: %d \n", len1clone);
       // printf("this is i size for each loop: %d \n", i);
       // printf("This is clonebin1 value: %d and this is clonebin2 value: %d\n", clonebin1[len1clone], clonebin2[i]);
        if (clonebin1[len1clone] == '1' && clonebin2[i] == '1'){
           printf("i was hit at level %d \n", i);
            if (carry){
                carriedNums[len1clone-1] = '1';
                clonebin1[len1clone] = '1';
                if ((i+ (len1 - len2))  == 3) flow = 1;
            }
            else{
                 
                clonebin1[len1clone] = '0';
                carriedNums[len1clone] = ' ';
                carry = 1;
                if ((i+ (len1 - len2)) == 2 && flow == 0) flow = -1;
            }
        }
        else if (clonebin1[len1clone] == '1' || clonebin2[i] == '1'){
            if (carry){
                clonebin1[len1clone] = '0';
                carriedNums[len1clone-1] = '1';
                if (i == 3 && clonebin1[len1clone-1] == 0) flow = 1;
            }
            else{
                clonebin1[len1clone] = '1';
                carriedNums[len1clone - 1] = ' ';
            }
        }
        else {
           // printf("second rung hit too at level %d\n", i);
            if (carry){
                
                clonebin1[len1clone] = '1';
                carriedNums[len1clone-1] = '1';
                carry = 0;
            }
            else{
                clonebin1[len1clone] = '0';
                carriedNums[len1clone - 1] = ' ';
            }
        }
    }
    for (int i = 0; i < len1clone; i++){
        carriedNums[i] = ' ';
    }
    ans = NoPBtoD(clonebin1, 's');
    PrintStatements(dec1, dec2, ans, symbol, carriedNums, clonebin1, flow, clonebin2);
    free(clonebin1);
    free(clonebin2);
    return;
}

void ArithMain(char bin1[], char symbol, char bin2[]){ //main function to see which binary sequence is larger. Returns if either is zero
    if (strlen(bin1) == 0 || strlen(bin2) == 0){
        return;
    }
    if (strlen(bin1) >= strlen(bin2)){
        Arith(bin1, symbol, bin2);
    }
    else{
        Arith(bin2, symbol, bin1);
    }
}

int main(int argc, char *argv[]) {
    // Write C code here
    if (argc == 4){
        char* bin = argv[1];
        char* bin2 = argv[3];
        char opp = *argv[2];
        ArithMain(bin, opp, bin2);
        }
    if (argc == 3){
        if (argv[1][0] == '0' && argv[1][1] == 'b'){
            char* bin = argv[1];
            BtoD(bin, *argv[2]);
        }
        else{
            int dec = atoi(argv[1]);
            int len = atoi(argv[2]);
            DtoB(dec, len);
        }
    }


    return 0;
}
