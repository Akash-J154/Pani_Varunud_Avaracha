#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h> //use gcc sic.c -lm


  
void pass2(){
  int locctr=0X0, start=0X0, sa=0X0, program_length=0X0, ret, op_status = 0, address=0X0, target=0X0, ascii=0X0, temp1=0X0, j = 0, k, count=0X0, record_len=0X0;
  char label[12], mnemonic[9], operand[12], buffer[64], mnem[9], op[3], symbol[12], opcode[3], cons[8], text_rec[70];
  long int aseek,bseek;
  FILE *fIntrm, *fSymtab, *fOptab, *fLength, *fobj;
	
  fIntrm = fopen("Intermediate_File.txt","r");
  if(fIntrm == NULL)
  {
	printf("\nIntermediate file missing!"); 
	return;
	}
    fSymtab=fopen("SYMTAB.txt","r");
    if(fSymtab == NULL)
    {
    	printf("\nSYMTAB missing!"); 
    	return;
    }
    fOptab=fopen("OPTAB.txt","r");
    if(fOptab == NULL)
    {
    	printf("\nOPTAB missing!"); 
    	return;
    }
    fLength=fopen("Program Length.txt","r");
    if(fLength == NULL)
    {
    	printf("\nProgram_length file missing!"); 
    	return;
    }
    fobj = fopen("Object_Program.txt","w");
    fscanf(fIntrm,"%X\t%s\t%s\t%s\n",&locctr,label,mnemonic,operand);
	if(strcmp(mnemonic,"START")==0)
    	{
        	start = (int)strtol(operand,NULL,16); 
        	fscanf(fLength,"%X",&program_length);
        	fprintf(fobj,"H^%6s^%06X^%06X",label,start,program_length);
    		fprintf(fobj,"\nT^%06X^00^",start);
    		bseek = ftell(fobj);
	}
	    
    while(!feof(fIntrm))
	{
		fgets(buffer,64,fIntrm);
		ret = sscanf(buffer,"%X\t%s\t%s\t%s",&locctr,label,mnemonic,operand);
		printf("%X\t%s\t%s\t%s\t%d\n",locctr,label,mnemonic,operand,ret);
		if(ret == 2) //no label
		{
			strcpy(mnemonic,label);
		}
		else if(ret == 3)
		{
			strcpy(operand,mnemonic);
			strcpy(mnemonic,label);
		}
		else
		{}

		if(count >= 0X3C || strcmp(mnemonic,"RESB")==0 || strcmp(mnemonic,"RESW")==0 || strcmp(mnemonic,"END")==0)	//0X3C is hex equivalent of 60
		{
			aseek = ftell(fobj);
			fseek(fobj,-(aseek-bseek)-3L,1);
			record_len = count/0X2;
			fprintf(fobj,"%02X^",record_len);
			fseek(fobj,0L,2);
			if(strcmp(mnemonic,"END")==0)
			{
				break;
			}
			sa = locctr;
			if(strcmp(mnemonic,"RESW")!=0)
			{
				fprintf(fobj,"\nT^%06X^00^",sa);
			}
			bseek = ftell(fobj);
			count = 0X0;
		}
		
		rewind(fOptab);
		op_status = 0;
		while(!feof(fOptab)) //scanning optab
		{	
			fscanf(fOptab,"%s %s",mnem,op);
			if(strcmp(mnemonic,mnem)==0)
			{
				strcpy(opcode,op);
				op_status = 1;
				break;
			}
		}

		printf("op_status= %d \t mnemonic= %s\n", op_status,opcode);
		j = strlen(operand);
		if(op_status == 1 && operand[j-1]=='X' && operand[j-2]==',')
		{
			operand[j-2] = '\0';
			rewind(fSymtab);
			while(!feof(fSymtab))
			{
				fscanf(fSymtab,"%s%X",symbol,&address);
				if(strcmp(operand,symbol)==0)
				{
					target = address;
					target += 0X8000;
					break;
				}
			}
			fprintf(fobj,"%2s%04X^",opcode,target);
			count = count+0X6;
			continue;
		}
		
		else if (op_status == 1 && strcmp(mnemonic,"RSUB")!=0)
		{
			// printf("Not RSUB");
			rewind(fSymtab);
			while(!feof(fSymtab))
			{
				fscanf(fSymtab,"%s%X",symbol,&address);
				if(strcmp(operand,symbol)==0)
				{
					target = address;
					break;
				}
			}
			printf("\nopcode=%s\ttarget=%X\n",opcode,target);
			fprintf(fobj,"%2s%04X^",op,target);
			count = count+0X6;
			continue;
		}
		else if (op_status == 1 && strcmp(mnemonic,"RSUB")==0)
		{
			fprintf(fobj,"%s0000^",opcode);
			count = count+0X6;
			continue;
		}
		else
		{
			if((strcmp(mnemonic,"BYTE")==0) || ((strcmp(mnemonic,"byte")==0)))
			{
				if(operand[0] == 'C')
				{
					for(k=0;k<strlen(operand)-3;k++)
					{
						temp1=0x0;
						temp1=temp1+(int)operand[k+2];
						ascii=ascii* 0x100 + temp1;
					}			
					fprintf(fobj,"%6X^",ascii);
					count = count+strlen(operand)-0X3;
				}
				else	
				{
					for(k=0;k<strlen(operand)-3;k++)
					{
						cons[k]=operand[k+2];
					}   
					cons[k]='\0';
					fprintf(fobj,"%s^",cons);
					count = count + (strlen(cons)+0X0);
				}
				continue;
			}
			else if((strcmp(mnemonic,"WORD")==0) || (strcmp(mnemonic,"word")==0))
			{
				temp1 = (int)strtol(operand,NULL,10);
				fprintf(fobj,"%06X^",temp1);
				count = count+0X6;
				continue;
			}
			else
			{
				continue;
			}
		}
	}
	fprintf(fobj,"\nE^%06X",start);
	printf("\nObject Program written successfully!\n");
	fclose(fIntrm);
	fclose(fSymtab);
	fclose(fOptab);
	fclose(fLength);
	fclose(fobj);
}

void main(){
  
  pass2();
  printf("\n \n");
}
