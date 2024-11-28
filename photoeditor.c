/*Vasiliki Kounadi
AM: 2022202000102
dit20102@uop.gr*/


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include "bmp.h"

typedef struct
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
}pixel;

pixel **image;
FILE *fp1, *fp2;
int height, width,readwrong=0;
int kept ,realnum;
int zero=0;
struct bmp_header *header;
struct bmp_info *info;

void input(FILE* fp, char* name);
void output(FILE* fp, char* name);
void attributes();
void blackwhite(char *colour, int num);
//int** recursive(int x, int y, int** flag);
void Hflip();
void Vflip();
void palette(int num);

int main (int argc, char *argv[]) //NOT VOID PUT ARGUMENTS
{
    srand(time(NULL));

    char *start, *command, *strin, *strout; 
    char **bw, *colour;
    char *p,*hi;
    int num;
    
    //give names to arguments
    start=strdup(argv[0]);
    if(argc>=2)
        command=strdup(argv[1]);
    else 
        command=NULL;
    if (argc>=3)
        strin=strdup(argv[2]);
    else 
        strin=NULL;
    if (argc==4)
        strout=strdup(argv[3]);
    else 
        strout=NULL;
    
    //find command
    if (strcmp(command,"-a") == 0) 
    {
        if(strin==NULL || strout!=NULL)
            printf("\n\n\n\n\n**INVALID INPUT, PROBLEM WITH INPUT FILE**\n\n");
        else
        {
            input(fp1,strin);
            if(readwrong==0)
                attributes();
        }       
    }
        
    else if (strcmp(command,"-fv") == 0)
    {
        if(strin==NULL || strout==NULL)
            printf("\n\n\n\n\n**INVALID INPUT, PROBLEM WITH INPUT OR OUTPUT FILE**\n\n");
        else
        {
            input(fp1,strin);
            if(readwrong==0)
            {
                Vflip(); 
                output(fp2,strout);
                printf("\n\n\n\n\nThe output image \"%s.bmp\" is the input image \"%s.bmp\" flipped vertically\n\n", strout,strin);
            }         
        }
    }

    else if (strcmp(command,"-fh") == 0) 
    {
        if(strin==NULL || strout==NULL)
            printf("\n\n\n\n\n**INVALID INPUT, PROBLEM WITH INPUT OR OUTPUT FILE**\n\n");
        else
        {
                input(fp1,strin);
                if(readwrong==0)
                {
                    Hflip(); 
                    output(fp2,strout);
                    printf("\n\n\n\n\nThe output image \"%s.bmp\" is the input image \"%s.bmp\" flipped horizontally\n\n", strout,strin);
                }    
        }
    }

    else if (strstr(command,"-bw_") != NULL)//ERASE DEBUG
    {
        colour=(char*)malloc(sizeof(char));
        if(colour == NULL)
        {
            printf("\n\n**MEMORY ERROR**\n\n");
            exit(1);
        }
        
        if(strin==NULL || strout==NULL)
            printf("\n\n\n\n\n**INVALID INPUT, PROBLEM WITH INPUT OR OUTPUT FILE**\n\n");
        else
        {
            bw = (char**)malloc(3 * sizeof(char*));
            if(bw==NULL)
            {
                printf("\n\n**MEMORY ERROR**\n\n");
                exit(1);
            }
            
            bw[0]= strtok(command, "_");
            bw[1]= strtok(NULL, "_");
            bw[2]= strtok(NULL, " ");
            
            if(bw[1]!=NULL)
                strcpy(colour,bw[1]);
            
            if(bw[2]!=NULL)
                num=atoi(bw[2]);
            
            if(bw[0]==NULL || bw[1]==NULL || bw[2]==NULL)
            {
                printf("\n\n\n\n\n**INVALID INPUT, FORM \"bw_number_color\" WAS NOT USED**\n\n");
            } 
            else if(strcmp(bw[0],"-bw")!=0)
            {
                printf("\n\n\n\n\n**INVALID INPUT, NON EXISTANT COMMAND**\n\n");
            }
            else if(strcmp(bw[1],"red")!=0 && strcmp(bw[1],"lime")!=0 && strcmp(bw[1],"blue")!=0 && strcmp(bw[1],"yellow")!=0 && strcmp(bw[1],"cyan")!=0 && strcmp(bw[1],"aqua")!=0 && strcmp(bw[1],"magenta")!=0 && strcmp(bw[1],"fuchsia")!=0 && strcmp(bw[1],"maroon")!=0 && strcmp(bw[1],"olive")!=0 && strcmp(bw[1],"green")!=0 && strcmp(bw[1],"purple")!=0 && strcmp(bw[1],"teal")!=0 && strcmp(bw[1],"navy")!=0)
            {
                printf("\n\n\n\n\n**INVALID INPUT, COLOR NOT SUPPORTED**\n\n");
            }
            else if(num<0 || num>100)
            {
                printf("\n\n\n\n\n**INVALID INPUT, PERCENTAGE NUMBER MUST BE BETWEEN 0 AND 100**\n\n");
            }
            else
            {
                input(fp1,strin);
                if(readwrong==0)
                {
                    blackwhite(colour, num);
                    output(fp2,strout);
                    printf("\n\n\n\n\nThe output image \"%s.bmp\" is the grayscale versio.bmpn of the input image \"%s\" while keeping the %d %% of %s pixels\n\n", strout,strin,num, colour);
                }
                    
            }
            
            free(colour);
            free(bw);
        }
            
    }

    else if (strstr(command,"-p") != NULL) //order important cause "PurPle"
    {
        if(strin==NULL || strout==NULL)
            printf("\n\n\n\n\n**INVALID INPUT, PROBLEM WITH INPUT OR OUTPUT FILE**\n\n");
        else
        {
            strtok(command, "p");
            p = strtok(NULL, "\0");
            num=atoi(p);

            if(num<0 || num%2!=0 || num>256)
                printf("\n\n\n\n\n**INVALID INPUT, FORM \"pnumber\" WAS NOT USED OR NUMBER IS NOT MULTIPLE OF 2**\n\n");
            else
            {
                input(fp1,strin);
                if(readwrong==0)
                { 
                    printf("\n");
                    palette(num);
                   
                    output(fp2,strout);
                    printf("\n\n\n\nThe output image \"%s.bmp\" is the input image \"%s.bmp\" with its palette reduced by %d\n\n", strout,strin,num);
                }      
            }  
        }  
    }

    else
        printf("\n\n\n\n\n**INVALID INPUT, NON EXISTANT COMMAND**\n\n");

    printf("\n\n~~ THANK YOU FOR USING ~~\n\n");

    return 0;
}

void input(FILE* fp, char* name)//use fp1
{
    char *file;
    int i, j;

    file = strdup(name);
    strcat(file, ".bmp");

    fp = fopen(file, "r");
    if (fp == NULL)
    {
        printf("\n\n**ERROR OPENING FILE \"%s\"**\n\n", file);
        readwrong=1;
        return;
    }

    //readHeader(FILE *fp, struct bmp_header *header);
    //readInfo(FILE *fp, struct bmp_info_header *header);
    
    header = (struct bmp_header*)malloc(sizeof(struct bmp_header));
    if(header == NULL)
    {
        printf("\n\n**MEMORY ERROR**\n\n");
        exit(1);
    }

    info = (struct bmp_info*)malloc(sizeof(struct bmp_info));
    if(info == NULL)
    {
        printf("\n\n**MEMORY ERROR**\n\n");
        exit(1);
    }

    readHeader(fp, header);
    readInfo(fp, info);
    
    height=info->bmiHeader.biHeight;
    width=info->bmiHeader.biWidth;


    //multiple of 4
    while(((width*3)+zero)%4 !=0)
        zero++;
        

    image = (pixel**)malloc(height * sizeof(pixel*));
    if(image == NULL)
    {
        printf("\n\n**MEMORY ERROR**\n\n");
        exit(1);
    }
    for(i=0;i<height;i++)
    {
        image[i] = (pixel*)malloc(width * sizeof(pixel));
        if(image[i] == NULL)
        {
            printf("\n\n**MEMORY ERROR**\n\n");
            exit(1);
        }
    }


    for (i=height-1; i>0; i--) 
    {
        for(j=0; j<width; j++) 
        {
                fread(&(image[i][j].blue), 1, 1, fp);
                fread(&(image[i][j].green), 1, 1, fp);
                fread(&(image[i][j].red), 1, 1, fp);  
        }
        fseek(fp, zero,SEEK_CUR);
    }

    fclose(fp);
}

void output(FILE* fp, char* name)//use fp2
{
    char *file;
    int i, j, check=0;

    file = strdup(name);
    strcat(file, ".bmp");

    fp = fopen(file, "w");
    if (fp == NULL)
    {
        printf("\n\n**ERROR CREATING FILE \"%s\"**\n\n", file);
        free(header);
        free(info);
        for(i=0;i<height;i++)
            free(image[i]);
        free(image);
        return;
    }

    //writeHeader(FILE *fp, struct bmp_header *header);
    //writeInfo(FILE *fp, struct bmp_info_header *header);

    writeHeader(fp, info);
    writeInfo(fp, info);


    for (i=height-1; i>=0; i--) 
    {
        for(j=0; j<width; j++) 
        {
            fwrite(&(image[i][j].blue), 1, 1, fp);
            fwrite(&(image[i][j].green), 1, 1, fp);
            fwrite(&(image[i][j].red), 1, 1, fp);
        }
        for(int z=0; z<zero; z++)
        {
            image[i][j].blue=0;
            fwrite(&(image[i][j].blue), 1, 1, fp);      
        }
    }


    fclose(fp);

    free(header);
    free(info);
    for(i=0;i<height;i++)
      free(image[i]);
    free(image);

}

void attributes()
{
    printf("\n\nINPUT IMAGE ATTRIBUTES\n-----------------\n");

    printf("Magic number for file: %u\n",header->bfType);
    printf("Size of file: %u\n",header->bfSize);
    printf("Reversed: %u\n",header->bfReserved1);
    printf("Also reversed: %u\n",header->bfReserved2);
    printf("Offset to bitmap data: %u\n",header->bfOffBits);

    printf("Size of info header: %u\n", info->bmiHeader.biSize);
    printf("Width of image: %d\n", info->bmiHeader.biWidth);
    printf("Height of image: %d\n", info->bmiHeader.biHeight);
    printf("Number of colour planes: %u\n", info->bmiHeader.biPlanes);
    printf("Number of bits per pixel: %u\n", info->bmiHeader.biBitCount);
    printf("Type of compression used: %u\n", info->bmiHeader.biCompression);
    printf("Size of image data: %u\n", info->bmiHeader.biSizeImage);
    printf("X pixels per meter: %d\n", info->bmiHeader.biXPelsPerMeter);
    printf("Y pixels per meter: %d\n", info->bmiHeader.biYPelsPerMeter);
    printf("Number of colours used: %u\n", info->bmiHeader.biClrUsed);
    printf("Number of important colours: %u\n\n\n", info->bmiHeader.biClrImportant);
}

void blackwhite(char *colour, int num)
{
    int **flag;//spot the pixels with the colour
    int i,j,x,y;
    int targetcolour=0;
    unsigned int r,g,b,val;

    /*flag = (int**)malloc(height * sizeof(int*));
    if(flag == NULL)
    {
        printf("\n\n**MEMORY ERROR**\n\n");
        exit(1);
    }
    for(i=0;i<height;i++)
    {
        flag[i] = (int*)malloc( width* sizeof(int));
        if(flag[i] == NULL)
        {
            printf("\n\n**MEMORY ERROR**\n\n");
            exit(1);
        }
    }
    

    for (i=0; i<height; i++) 
    {
        for(j=0; j<width; j++) 
        {
            r=image[i][j].red;
            g=image[i][j].green;
            b=image[i][j].blue;

            if(strcmp(colour,"red") ==0)
            { 
                if(r>=200 && g<=40 && b<=40)
                {
                    flag[i][j]=1; printf("bitches\n");
                }int count;
               count++;
               printf("%d  ",count);
                          
            }

            else if(strcmp(colour,"lime") ==0)
            {
                if(r<=40 && g>=200 && b<=40)
                    flag[i][j]=1;
            }

            else if(strcmp(colour,"blue") ==0)
            {
                if(r<=40 && g<=40 && b>=200)
                    flag[i][j]=1;
            }

            else if(strcmp(colour,"yellow") ==0)
            {
                if(r>=200 && g>=200 && b<=40)
                    flag[i][j]=1;
            }

            else if(strcmp(colour,"cyan") ==0 || strcmp(colour,"aqua") ==0 )
            {
                if(r<=40 && g>=200 && b>=200)
                    flag[i][j]=1;
            }

            else if(strcmp(colour,"magenta") ==0 || strcmp(colour,"fuchsia") ==0 )
            {
                if(r>=200 && g<=40 && b>=200)
                    flag[i][j]=1;
            }

            else if(strcmp(colour,"maroon") ==0)
            {
                if(r>=80 && r<=140 && g>=20 && b<=20)
                    flag[i][j]=1;
            }

            else if(strcmp(colour,"olive") ==0)
            {
                if(r>=80 && r<=140 && g>=80 && g<=140 && b<=20)
                    flag[i][j]=1;
            }

            else if(strcmp(colour,"green") ==0)
            {
                if(r<=20 && g>=80 && g<=140 && b<=20)
                    flag[i][j]=1;
            }

            else if(strcmp(colour,"purple") ==0)
            {
                if(r>=80 && r<=140 && g<=20 && b>=80 && b<=140)
                    flag[i][j]=1;
            }

            else if(strcmp(colour,"teal") ==0)
            {
                if(r<=20 && g>=80 && g<=140 && b>=80 && b<=140)
                    flag[i][j]=1;
            }

            else if(strcmp(colour,"navy") ==0)
            {
                if(r<=20 && g<=20 && b>=80 && b<=140)
                    flag[i][j]=1;
            }

        }
    
    }

    for (i=0; i<height; i++)
    {
        for(j=0; j<width; j++) 
        {
            if(flag[i][j]==1)
                targetcolour++;//there are targetcolour red pixels
        }
    }
    printf("%d\n", targetcolour);
    realnum=(num*targetcolour)/100;//not 100% i want to keep realnum pixels of red


    do
    {
        kept=0;
        for (i=0; i<height; i++) //percentage kept
        {
            for(j=0; j<width; j++) 
            {
                if(flag[i][j]==2)
                    kept++;
            }
        }
        printf("%d,%d\n", kept,realnum);
        x=rand()%width;
        y=rand()%height;
        flag=recursive(x,y,flag);
    }while(kept<realnum);*/


    for (i=0; i<height; i++) 
    {
        for(j=0; j<width; j++) 
        {
            r=image[i][j].red;
            g=image[i][j].green;
            b=image[i][j].blue;
            //if(flag[i][j]==0 || flag[i][j]==1)
            //{
                val=(r+g+b)/3;
                image[i][j].red=image[i][j].green=image[i][j].blue=val;
            //}          
        }
    }

    /*for(i=0;i<height;i++)
      free(flag[i]);
    free(flag);*/
    
}

/*int** recursive(int x, int y, int** flag)
{

    if(kept<realnum)
    {
        if(flag[x][y]==1)
        {
            flag[x][y]=2;//color will be kept
            if(flag[x][y]==2) //checks if current position containes a dot for the recursion
            {
                if (x > 0)
                {
                    if (y > 0)
                    {
                        if (flag[x-1][y-1]==1)
                        recursive(x - 1, y - 1, flag);
                    } 
                    if (y < height-1)
                    {
                        if (flag[x-1][y+1]==1)
                            recursive(x - 1, y + 1, flag);
                    }
                    if (flag[x - 1][y]==1)
                        recursive(x - 1, y, flag);
                }
                if (y > 0)
                {
                    if (flag[x][y - 1]==1)
                        recursive(x, y - 1, flag);
                }
                if (y < height-1)
                {
                    if (flag[x][y + 1]==1)
                        recursive(x, y + 1, flag);
                }
                if (x < width-1) 
                {
                    if (y > 0)
                    {
                        if (flag[x + 1][y - 1]==1)
                            recursive(x + 1, y - 1, flag);
                    }
                    if (y < height-1)
                    {
                        if (flag[x + 1][y + 1]==1)
                            recursive(x + 1, y + 1, flag);
                    }
                        if (flag[x + 1][y]==1)
                            recursive(x + 1, y, flag);
                }
            }
        }
    }
    return flag;
}*/

void Vflip()
{
    pixel temp;
    for (int i = 0; i < height/2; i++)
    {
         for (int j = 0; j < width; j++)
        {
            temp = image[height-1-i][j];
            image[height-1-i][j] = image[i][j];
            image[i][j] = temp;
        }
    }

}   
void Hflip()
{
    pixel temp;
    for (int i = 0; i < height; i++)
    {
         for (int j = 0; j < width/2; j++)
        {
            temp = image[i][width-1-j];
            image[i][width-1-j] = image[i][j];
            image[i][j] = temp;
        }
    }
}

void palette(int num)
{
    int i, j, z, lower;
    int* limit, bima;

    bima = 256/num; //times pediou

    limit=(int*)malloc(num * sizeof(int));
    lower = 0;  //xamilo orio tou array limit    
    for(i=0; i<num; i++)
    {
        limit[i] = lower;
        lower = lower + bima;
    }

    for(i=0; i<height; i++)
    {
        for(j=0; j<width; j++)
        {
             for(z=num-1; z>=0; z--)
             {
                if(image[i][j].red>limit[z])
                {
                    image[i][j].red = limit[z];
                    break;
                }
             }

             for(z=num-1; z>=0; z--)
             {
                if(image[i][j].green>limit[z])
                {
                    image[i][j].green = limit[z];
                    break;
                }
             }

             for(z=num-1; z>=0; z--)
             {
                 if(image[i][j].blue>limit[z])
                {
                    image[i][j].blue = limit[z];
                    break;
                }
             }
        }
    }
    free(limit);
}