#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h> //use gcc sic.c -lm

int pass1(){
  	char buffer[64],label[12],mnemonic[8],operand[12],mnem[8],op[2],symbol[12];
	int start=0X0, locctr=0X0, ret, flag=0, address=0X0, j, program_length=0X0, count=0X0;
	FILE *fProg,*fSymtab, *fOptab, *fIntermediate, *fLength;
	
	fProg = fopen("SIC_program.txt","r");
	if(fProg == NULL)
	{
		printf("Source file missing!!");
		return 0;
	}
	
	fSymtab = fopen("SYMTAB.txt","w+");
	
	fOptab = fopen("OPTAB.txt","r");
	if(fOptab == NULL)
	{
		printf("OPTAB missing!!");
		return 0;
	}

		
	fIntermediate = fopen("Intermediate_File.txt","w");
	
	fgets(buffer,64,fProg);
	sscanf(buffer,"%s %s %s",label,mnemonic,operand);
	printf(buffer);
	if(strcmp(mnemonic,"START")==0)
	{
		locctr = atoi(operand);	//save operand as starting address, converts string to int
		while(locctr > 0)
		{
			start = start + (locctr%10)*pow(16,count);  //converting in hex value
			locctr = locctr/10;
			count++;
		}
		locctr = start;
        fprintf(fIntermediate,"%X\t%s\t%s\t%s\n",start,label,mnemonic,operand);
	}
	else
	{
		locctr = 0X0;
	}
	
	while(!feof(fProg))
	{
		fgets(buffer,64,fProg);
		ret = sscanf(buffer,"%s%s%s",label,mnemonic,operand);

		if(label[0] != ';' && label[0] != '.')	//not a comment line
		{	
			if(ret == 1)				//rsub
			{
				strcpy(mnemonic,label);
				fprintf(fIntermediate,"%04X\t\t%s\n",locctr,mnemonic);
			}
			if(ret == 2)
			{
				strcpy(operand,mnemonic);
				strcpy(mnemonic,label);
				fprintf(fIntermediate,"%X\t\t%s\t%s\n",locctr,mnemonic,operand);
			}
			if(ret == 3) //there is a symbol in the Label field
			{
				rewind(fSymtab);		
				while(!feof(fSymtab))
				{
					flag = 0;
					fscanf(fSymtab,"%s%X",symbol,&address);
					if(strcmp(label,symbol)==0)
					{
						flag = 1;	//duplicate symbol found
						printf("\nDuplicate LABEL found: %s",label);
						return 0;
					}
				}					
				
				if(flag == 0)	//no duplicate symbol
				{
					fprintf(fSymtab,"%s\t%X\n",label,locctr);
					fprintf(fIntermediate,"%X\t%s\t%s\t%s\n",locctr,label,mnemonic,operand);
				}
			}
			
			rewind(fOptab);
			while(!feof(fOptab))	//search optab for OPCODE
			{
				fscanf(fOptab,"%s",mnem);
				//printf("%s %s",mnem,op);
				if(strcmp(mnemonic,mnem)==0)
				{
					locctr += 3;
					flag = 0;
					break;
				}
				else if(strcmp(mnemonic,"WORD")==0 || strcmp(mnemonic,"word")==0)
				{	
					locctr += 3;
					flag = 0;
					break;
				}
				else if((strcmp(mnemonic,"RESW")==0) || (strcmp(mnemonic,"resw")==0))
				{	
					locctr += 3*atoi(operand);
					flag = 0;
					break;
				}
				else if(strcmp(mnemonic,"RESB")==0 || strcmp(mnemonic,"resb")==0)
				{	
					locctr += atoi(operand);
					flag = 0;
					break;
				}
				else if(strcmp(mnemonic,"BYTE")==0 || strcmp(mnemonic,"byte")==0)
				{
					j = strlen(operand);
					if(operand[0] !='C' && operand[0] != 'X')
					{	
						locctr += 1;
						flag = 0;
						break;
					}
					else if(strcmp(mnemonic,"BYTE")==0 && operand[0] =='C')
					{
						locctr += j-3;	//-3 is done to account for C' '
						flag = 0;
						break;
					}
					else if(strcmp(mnemonic,"BYTE")==0 && operand[0] =='X')
					{	
						if((j-3)%2 != 0)
							locctr += (j-3)/2 + 1;
						else
							locctr += (j-3)/2 ;
						flag = 0;
						break;
					}
					else
					{
						flag = 1;
					}
				}
				if(flag == 1)
				{
					printf("\n%s not present in OPTAB!",mnemonic);
					printf("\nExiting ...");
					return 0;
				}
			}
		}
		if(strcmp(mnemonic,"END")==0)
		{
			break;
		}
	}
	printf("\nSYMTAB generated...\n");
	
	fLength = fopen("Program Length.txt","w");
	program_length = locctr - start;
	fprintf(fLength,"%X",program_length);
	
	fclose(fProg);
	fclose(fSymtab);
	fclose(fOptab);
	fclose(fIntermediate);
	fclose(fLength);
}


void main(){
  pass1();
  
}
