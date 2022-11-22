// Group project C: Tyler McKenzie, Ian Chartier
// 11/10/2022
void printString(char*);
void printChar(char*);
void readString(char*);
void readSector(char*,int);
void readFile(char*,char*,int*);
void writeSector(char*,int);
void deleteFile(char*);
void directory();
void terminate();
void handleInterrupt21(int,int,int,int);
main(){
	/*
	interrupt 21 for readsector
	makeInterrupt21();
	interrupt(0x21,2,buffer,30,0);
	interrupt(0x21,0,buffer,0,0);

	while(1){
		printString("\nEnter a Line: \0");
		makeInterrupt21();
		interrupt(0x21,1,count,0,0);
		interrupt(0x21,0,count,0,0);
	}


	char buffer[13312];
	int sectorsRead;
	makeInterrupt21();
	interrupt(0x21, 3, "messag", buffer, &sectorsRead);
	if (sectorsRead>0)
		interrupt(0x21, 0, buffer, 0, 0);   print out the file
	else
		interrupt(0x21, 0, "messag not found\r\n", 0, 0);
	while(1); */

	makeInterrupt21();
	interrupt(0x21,4,"shell\0",0,0);
	while(1);
}

void printChar(char letter){
	int ah=0xe;
        int al=letter;
        interrupt(0x10,ah*0x100+al,0,0,0);
}

void printString(char* charstring){
	int counter;
	int ah = 0xe;
	interrupt(0x10,0xe*0x100+0xd,0,0,0);
	for (counter = 0; charstring[counter] != '\0'; counter++){
		int al = charstring[counter];
		interrupt(0x10,ah*256+al,0,0,0);
	}
}
void readString(char* reader){
	int counter = 0;
	while(1){
		char x = interrupt(0x16,0,0,0,0);
        	if (x == 0xd){
			printChar('\n');
			printChar('\b');
			reader[counter] = '\0';
			break;
		}else if (x == 0x8 && counter != 0){
			printChar('\b');
			printChar(' ');
			printChar('\b');
			counter--;
		}else if (x != 0x8 && counter < 80){
			printChar(x);
			reader[counter++] = x;
		}
	}
}
void readSector(char* buffer, int sector){
	interrupt(0x13,2*256+1,buffer,0*256+sector+1,0*256+0x80);
}
void writeSector(char* buffer, int sector){
	interrupt(0x13,3*256+1,buffer,0*256+sector+1,0*256+0x80);
}
void readFile(char* name, char* buffer, int* sectorsRead){
	char entryfile[512];
	char dir[512];
	int i,j,entry,confirm;
	readSector(dir,2);
	for(i = 0; i < 512; i++){
		if(dir[i] != name[confirm]){
			i+=31 - confirm;
			confirm = 0;
			*sectorsRead = *sectorsRead +1;
		}else if (confirm == 5){
			for(j = 0; j < 26; j++){
				if(dir[j+i+1] == 0x0){
					buffer[j*512] = 0x0;
					break;
				}else{
					readSector(entryfile,dir[j+i+1]);
					for(entry = 0; entry < 512; entry++){
						buffer[(j* 512)+entry]=entryfile[entry];
					}
				}
			}
			break;
		}else{
			confirm++;
		}
	}
	if(confirm==0){
		*sectorsRead=0;
	}

}
void deleteFile(char* name){
	char dir[512];
	char map[512];
	int  i, del, entry;
	//load dir and map
	readSector(map, 1);
	readSector(dir, 2);
	//loop through dir for filename
	for(entry = 0;entry<512;entry+=32){
		for(i=0;i<6;i++){
			//if its not the filename break
			if(name[i]!=dir[i+entry]){
				break;
			//if it is
			}else if (i == 5){
				//set first byte to '\0'
				dir[entry]=0x0;
				//set the map sectors to 0
				for(i=6;i<32;i++){
					if(dir[entry+i]!=0){
						del=dir[entry+i];
						map[del]=0;
					}else if(dir[entry+i]==0 && dir[entry+i+1]==0){
						break;
					}
				}
				//write back to the sectors
				writeSector(dir,2);
				writeSector(map,1);
				break;
			}
		}
	}
}
void writeFile(char* filename,char*buffer, int sectsWrote){
	char dir[512];
	char map[512];
	int entry,i,text,sect,file;
	readSector(map,1);
	readSector(dir,2);
	//loop through directory for free entry
	for(entry=0; entry < 512; entry+=32){
		if(dir[entry]==0x0){
                	for(i=0;i < 6; i++){
                                dir[6]=0x0;
				dir[entry+i]=filename[i];
                        }
			for(sect=0;sect<32;sect++){
				if(map[sect==0]){
					map[sect]-0xff;
					sect++;
				}
			}
			for(i=0;i<sectsWrote;i++){
				for(text=0;text<32;text++){
					if(dir[entry+text]==0){
						dir[entry+text]=sect - sectsWrote-i;
					}
				}
				writeSector(buffer,sect-sectsWrote-i);
				if(map[sect]==0xff){
					for(text=sect;text<32;text++){
						if(map[text]==0){
							sect=text;
						}
					}
				}
			}
			writeSector(map,1);
			writeSector(dir,2);
			break;
		}
	}
}
void executeProgram(char* name)
{
	int i;
	char buffer[13312];
	int segment = 0x2000;
	readFile(name, buffer);
	for(i = 0; i < 13312; i++)
	{
		putInMemory(segment, i, buffer[i]);
	}
	launchProgram(segment);
}
void terminate(){
	char shell[6];
	shell[0] = 's';
	shell[1] = 'h';
	shell[2] = 'e';
	shell[3] = 'l';
	shell[4] = 'l';
	shell[5] = '\0';
	executeProgram(shell,0x2000);
}
void directory(){
	char dir[512];
	char buffer[512];
	int entry,i,j;
	j = 0;
	for(entry = 0; entry < 512; entry++){
		dir[entry] = buffer[entry] = 0x0;
	}
	readSector(dir,2);
	for(entry = 0; entry < 16; entry++){
		if(dir[32*entry] != '\0'){
			for(i = 0; i < 6; i++){
				buffer[j] = dir[entry*32+i];
				j++;
			}
			buffer[j] = '\r';
			buffer[j+1] = '\n';
			j += 2;
		}
	}
	printString("\n\r");
	for(entry = 0; entry < 512; entry++){
		if(buffer[entry] != 0x0){
			printChar(buffer[entry]);
		}
	}
}
void handleInterrupt21(int ax, int bx, int cx, int dx){
	if (ax == 0){
		printString(bx);
	}else if (ax == 1){
		readString(bx);
	}else if (ax == 2){
		readSector(bx,cx);
	}else if (ax == 3){
		readFile(bx,cx,dx);
	}else if (ax == 4){
		executeProgram(bx);
	}else if (ax == 5){
		terminate();
	}else if (ax == 6){
		writeSector(bx,cx);
	}else if (ax == 7){
		directory();
	}else if (ax == 8){
		deleteFile(bx);
	}else if(ax==9){
		writeFile(bx,cx,dx);
	}else{
		printString("ax should be between 0 and 7");
	}
}
