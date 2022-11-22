char buffer[13312];
char filename[6];
char filememory[512];
char copymemory[512];
char newfile[512];
int sectorsRead,i,j,filewrite;

main(){
	while(1){
		syscall(0,"SHELL>");
		syscall(1,buffer);
		if(buffer[0]=='t'&&buffer[1]=='y'&&buffer[2]=='p'&&buffer[3]=='e'){
			if(buffer[5] != 0x0){
				for(i = 0; i < 6; i++){
					filename[i] = buffer[5 + i];
				}
				syscall(3,filename,filememory,&sectorsRead);
				if(sectorsRead>0){
					syscall(0,"\n\r");
					syscall(0,filememory);
					syscall(0,"\n\r");
					break;
				}else{
					syscall(0,"Error: No File Found.\n\r");
					break;
				}
			}else{
				syscall(0,"Enter a File Name after type.\n\r");
				break;
			}
		}else if(buffer[0]=='e'&&buffer[1]=='x'&&buffer[2]=='e'&&buffer[3]=='c'){
			if(buffer[5] != 0x0){
				syscall(4,buffer+5);
			}else{
				syscall(0,"Enter a Program Name after exec.\n\r");
				break;
			}
		}else if(buffer[0]=='d'&&buffer[1]=='i'&&buffer[2]=='r'){
			syscall(7);
		}else if(buffer[0]=='d'&&buffer[1]=='e'&&buffer[2]=='l'){
			for(i = 0; i < 6; i++){
				filename[i] = buffer[4 + i];
			}
				filename[6] = 0x0;
				syscall(8,filename);
				syscall(0,"File has been Deleted.\n\r");
		}else if(buffer[0]=='c'&&buffer[1]=='o'&&buffer[2]=='p'&&buffer[3]=='y'){
			syscall(0,"\n\r File copied\n\r");
			for(i=0;i<6;i++){
				filename[i] = buffer[7 + i];
			}
			filename[6]=0x0;
			for(i=7;i<13;i++){
				newfile[j]= buffer[5+i];
				j++;
			}
			newfile[6]=0x0;
			syscall(3,filename,copymemory,&sectorsRead);
			syscall(9,newfile,copymemory,sectorsRead);
			break;
		}else if(buffer[0]=='c'&&buffer[1]=='r'&&buffer[2]=='e'&&buffer[3]=='a'&&buffer[4]=='t'&&buffer[5]=='e'){
                        syscall(0,"\n\rFile Created\n\r");
                        for(i = 0; i < 6; i++){
                                filename[i] = buffer[7 + i];
                        }
                        syscall(0,filename);
                        syscall(0,"\n\r");
                        while(1){
                                syscall(0,"text:");
                                syscall(1,newfile,0,0);
                                if(newfile[2] == 0x0){
                                        syscall(0,"DONE\n\r");
                                        break;
                                }else{
                                        for(i = 0; i < 512; i++){
                                                filememory[i] = newfile[i];
                                        }
                                        sectorsRead++;
                                }

                        }
                        syscall(9,filename,filememory,sectorsRead);
                        break;
		}else{
			syscall(0,"Bad Command!\n\r");
		}
	}
}
