#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int opcode, rx, ry, rz;
} Instruction;
Instruction IM[125];

unsigned int mem[500];
int i = 0;
int PC = 0;
int MAR1 = 0;
int RF[8];
int DM[16];
int IFID[4];
int IDEX[4];
int EXDM[4];
int DMWB[4];
int v1, v2, v3 = 0;
int idcu, cu, excu, dmcu, wbcu = 0;
int idx, exx, dmx, wbx = 0;
int idy, exy, dmy, wby = 0;
int idz, exz, dmz, wbz = 0;

void main(int argc, char *argv[]){

    FILE *file;
    file = fopen(argv[1], "r");
    if(file != NULL){
        while(!feof(file)){
            fscanf(file, "%d ", &mem[i]);   //transcribing from input file to array
            i++;
        }
    }
    else{
        printf("Cannot read file.");
    }

    int l = 0;
    for(int j = 0; j < 125; j++){ //moving Instruction Memory from array to struct
        IM[j].opcode = mem[l];
        l++;
        IM[j].rx = mem[l];
        l++;
        IM[j].ry = mem[l];
        l++;
        IM[j].rz = mem[l];
        l++;
    }

    for(int k = 0; k < 125; k++){       //Printing code in assembly language
        switch(IM[k].opcode){
            case 1:
                 printf("1 %d %d %d     lw   %d %d %d\n", IM[k].rx, IM[k].ry, IM[k].rz, IM[k].rx, IM[k].ry, IM[k].rz);
                 break;
            case 2:
                 printf("2 %d %d %d     add  %d %d %d\n", IM[k].rx, IM[k].ry, IM[k].rz, IM[k].rx, IM[k].ry, IM[k].rz);
                 break;
            case 3:
                 printf("3 %d %d %d     sw   %d %d %d\n", IM[k].rx, IM[k].ry, IM[k].rz, IM[k].rx, IM[k].ry, IM[k].rz);
                 break;
            case 4:
                 printf("4 %d %d %d     sub  %d %d %d\n", IM[k].rx, IM[k].ry, IM[k].rz, IM[k].rx, IM[k].ry, IM[k].rz);
                 break;
            case 5:
                 printf("5 %d %d %d     sio1 %d %d %d\n", IM[k].rx, IM[k].ry, IM[k].rz, IM[k].rx, IM[k].ry, IM[k].rz);
                 break;
            case 6:
                 printf("6 %d %d %d     sio2 %d %d %d\n", IM[k].rx, IM[k].ry, IM[k].rz, IM[k].rx, IM[k].ry, IM[k].rz);
                 break;
            case 7:
                 printf("7 %d %d %d     halt\n", IM[k].rx, IM[k].ry, IM[k].rz);
                 break;
            case 8:
                 printf("8 %d %d %d     jmp  %d %d %d\n", IM[k].rx, IM[k].ry, IM[k].rz, IM[k].rx, IM[k].ry, IM[k].rz);
                 break;
            case 9:
                 printf("9 %d %d %d     beq  %d %d %d\n", IM[k].rx, IM[k].ry, IM[k].rz, IM[k].rx, IM[k].ry, IM[k].rz);
                 break;
        }
    }

    while(PC <= 500){ //while loop which controls execution

        printf("[[[PC = %d]]]\n", PC);      //shows current Program Clock


        PC = PC + 4;                        //Increment by 4 after each execution

        IFID[0] = IM[MAR1].opcode;          //Moving information from struct of interest to first buffer IFID
        IFID[1] = IM[MAR1].rx;
        IFID[2] = IM[MAR1].ry;
        IFID[3] = IM[MAR1].rz;

        MAR1++;   //Memory Address Register, controls which instruction will be used, can be manipulateed by jump and beqz


        /*------------------------Instruction decode / Op. fetch------------------------*/
        idcu = IFID[0];     //moving buffer to variables
        idx = IFID[1];
        idy = IFID[2];
        idz = IFID[3];

        switch(idcu){ //Switch statement to determine which operation i want to perform based on idcu
            case 1: //load word
                IDEX[0] = idcu;
                IDEX[1] = idx;  //RF[idx] or Rx
                IDEX[2] = 0;
                IDEX[3] = idz; //address
                break;
            case 2: //add
                v1 = RF[idy];
                v2 = RF[idz];

                IDEX[0] = idcu;
                IDEX[1] = idx;
                IDEX[2] = v1;
                IDEX[3] = v2;
                break;

            case 3: //store word

                IDEX[0] = idcu;
                IDEX[1] = RF[idx];
                IDEX[2] = idy;
                IDEX[3] = idz; //address
                break;
            case 4: //subtract
                v1 = RF[idy];
                v2 = RF[idz];

                IDEX[0] = idcu;
                IDEX[1] = idx;
                IDEX[2] = v1;
                IDEX[3] = v2;
                break;
            case 5: //input
                IDEX[0] = idcu;
                IDEX[1] = idx;  //Rx
                IDEX[2] = idy;
                IDEX[3] = idz;
                break;
            case 6: //output
                IDEX[0] = idcu;
                IDEX[1] = idx;  //Rx
                IDEX[2] = idy;
                IDEX[3] = idz;
                break;
            case 7: //E.O.P
                IDEX[0] = idcu;
                IDEX[1] = idx;
                IDEX[2] = idy;
                IDEX[3] = idz;
                break;
            case 8: //Jump
                IDEX[0] = idcu;
                IDEX[1] = idx;
                IDEX[2] = idy;
                IDEX[3] = idz;;
                break;
            case 9: //beqz
                IDEX[0] = idcu;
                IDEX[1] = idx;
                IDEX[2] = idy;
                IDEX[3] = idz;
                break;
        }

        /*------------------------Execute------------------------*/
        excu = IDEX[0];     //moving buffer to variables
        exx = IDEX[1];
        exy = IDEX[2];
        exz = IDEX[3];

        switch(excu){   //Switch statement to determine which operation i want to perform based on excu
            case 1: //load word
                EXDM[0] = excu;
                EXDM[1] = exx; //Rx
                EXDM[2] = exy;
                EXDM[3] = exz; //address
                break;

            case 2: //add
                v3 = exy + exz;

                EXDM[0] = excu;
                EXDM[1] = exx;
                EXDM[2] = 0;
                EXDM[3] = v3;
                break;

            case 3: //store word
                EXDM[0] = excu;
                EXDM[1] = exx;  //num. i want to store
                EXDM[2] = 0;
                EXDM[3] = exz;  //address
                break;

            case 4: //subtract
                v3 = exy - exz; //performs subtraction

                EXDM[0] = excu;
                EXDM[1] = exx;
                EXDM[2] = 0;
                EXDM[3] = v3;
                break;

            case 5: //input
                printf("Input number: ");   //takes user input
                scanf("%d", &exz);
                EXDM[0] = excu;
                EXDM[1] = exx;
                EXDM[2] = exy;
                EXDM[3] = exz;
                break;

            case 6: //output
                EXDM[0] = excu;
                EXDM[1] = exx;
                EXDM[2] = exy;
                EXDM[3] = RF[exx];
                break;

            case 7: //E.O.P
                EXDM[0] = excu;
                EXDM[1] = exx;
                EXDM[2] = exy;
                EXDM[3] = exz;
                printf("Done.\n");
                break;
            case 8: //Jmp
                PC = exz;
                MAR1 = exz/4;
                EXDM[0] = excu;
                EXDM[1] = exx;
                EXDM[2] = exy;
                EXDM[3] = exz;
                break;
            case 9: //beqz
                if(RF[exx] == 0){   //used to skip
                    PC = PC + 4;
                    MAR1 = MAR1 + 2;    //+2 so that i can "jump" over the jmp operator
                }
                EXDM[0] = excu;
                EXDM[1] = exx;
                EXDM[2] = exy;
                EXDM[3] = exz;
                break;
        }

        /*------------------------Data Memory------------------------*/
        dmcu = EXDM[0];     //moving buffer to variables
        dmx = EXDM[1];
        dmy = EXDM[2];
        dmz = EXDM[3];

        switch(dmcu){   //Switch statement to determine which operation i want to perform based on dmcu
            case 1: //load word
                DMWB[0] = dmcu;
                DMWB[1] = dmx;
                DMWB[2] = dmy;
                DMWB[3] = dmz;
                break;

            case 2: //add
                DMWB[0] = dmcu;
                DMWB[1] = dmx;
                DMWB[2] = dmy;
                DMWB[3] = dmz;
                break;

            case 3: //store word
                DM[dmz] = dmx;

                DMWB[0] = dmcu;
                DMWB[1] = dmx;
                DMWB[2] = dmy;
                DMWB[3] = dmz;
                break;

            case 4: //subtract
                DMWB[0] = dmcu;
                DMWB[1] = dmx;
                DMWB[2] = dmy;
                DMWB[3] = dmz;
                break;

            case 5: //input
                DMWB[0] = dmcu;
                DMWB[1] = dmx;
                DMWB[2] = dmy;
                DMWB[3] = dmz;
                break;

            case 6: //output
                DMWB[0] = dmcu;
                DMWB[1] = dmx;
                DMWB[2] = dmy;
                DMWB[3] = dmz;
                break;

            case 7: //E.O.P
                DMWB[0] = dmcu;
                DMWB[1] = dmx;
                DMWB[2] = dmy;
                DMWB[3] = dmz;
                break;
            case 8: //Jump
                DMWB[0] = dmcu;
                DMWB[1] = dmx;
                DMWB[2] = dmy;
                DMWB[3] = dmz;

                break;
            case 9: //beqz
                DMWB[0] = dmcu;
                DMWB[1] = dmx;
                DMWB[2] = dmy;
                DMWB[3] = dmz;
                break;
        }

        /*------------------------Write Back------------------------*/
        wbcu = DMWB[0];     //moving buffer to variables
        wbx = DMWB[1];
        wby = DMWB[2];
        wbz = DMWB[3];

        switch(wbcu){   //Switch statement to determine which operation i want to perform based on wbcu
            case 1: //load word
                RF[wbx] = DM[wbz];
                break;

            case 2: //add
                RF[wbx] = wbz;  //stores variable
                break;

            case 3: //store word
                break;

            case 4: //subtract
                RF[wbx] = wbz;  //stores variable
                break;

            case 5: //input
                RF[0] = wbz;
                break;

            case 6: //output
                printf("OUTPUT -> R0 = %d\n", wbz); //shows output
                break;

            case 7: //E.O.P
                //printf("Done.\n");
                return;

            case 8: //Jump
                break;

            case 9: //beqz
                break;
        }
        printf("RF = ");                //VV shows status updates VV
        for(int m = 0; m < 8; m++){
            printf("%d ", RF[m]);
        }

        printf("\nDM = ");
        for(int m = 0; m < 16; m++){
            printf("%d ", DM[m]);
        }

        printf("\nIFID = ");
        for(int m = 0; m < 4; m++){
            printf("%d ", IFID[m]);
        }

        printf("\nIDEX = ");
        for(int m = 0; m < 4; m++){
            printf("%d ", IDEX[m]);
        }

        printf("\nEXDM = ");
        for(int m = 0; m < 4; m++){
            printf("%d ", EXDM[m]);
        }

        printf("\nDMWB = ");
        for(int m = 0; m < 4; m++){
            printf("%d ", DMWB[m]);
        }
        printf("\n\n");
    }
    printf("\nDone.\n");
    return;
}
