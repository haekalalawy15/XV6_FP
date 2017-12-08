#include "types.h"
#include "user.h"
#include "fcntl.h"
#include "stat.h"
#include "fs.h"

char buffer[512];
int cekext=0;

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void
cpstar(char *src,char *dest,char *starext)
{
  char buf[512], *p;
  int fd;
  int dfd;
  int lstar=strlen(starext);
  struct dirent de;
  struct stat st;
  stat(src,&st);
  fd=open(src,O_RDONLY);
  dfd=open(dest,O_WRONLY);
  if(dfd >= 0){
    printf(2, "cp -R: %s is a file or does not exist\n",dest);
    close(fd);
    close(dfd);
    return;
  }
  close(dfd);
  dfd=open(dest,O_RDONLY);
  if(st.type==T_FILE){
      char newdest[512];
      strcat(newdest,"/");
      strcat(newdest,dest);
      strcat(newdest,"/");
      strcat(newdest,src);
      int outs;
      outs=open(newdest,O_CREATE | O_RDWR);
      while(read(fd,buffer,sizeof(buffer))>0) write(outs,buffer,sizeof(buffer));
   }

  else if(st.type==T_DIR){
    strcpy(buf, src);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf,&st)<0){
	printf(1,"cp -R: file cannot be stats\n");
	continue;	
	}
      if(st.type==2){
        if(fmtname(buf)[0]=='.')continue;
	int a;        
	char newdest[512]; char newsrc[512];
        if(lstar==1){
		strcpy(newdest,dest);
        	strcat(newdest,"/");
        	strcat(newdest,p);
		strcpy(newsrc,src);
        	strcat(newsrc,"/");
		strcat(newsrc,p);
        	int in,out;
	//	printf(1,"file terkopi\n");
		in=open(newsrc,O_RDONLY);
		out=open(newdest,O_CREATE | O_RDWR);
        	while((a=read(in,buffer,in))>0) 
		{
			//printf(1,"%s\n",buffer);		
			write(out,buffer,in);
		}
		close(in);
		close(out);
		}
       else{
	  	char comp[512],comp2[512];
		strcpy(comp,starext+1);
		strcpy(comp2,p+(strlen(p)-strlen(comp)));
		if(strcmp(comp,comp2)==0){
			strcpy(newdest,dest);
        		strcat(newdest,"/");
        		strcat(newdest,p);
			strcpy(newsrc,src);
        		strcat(newsrc,"/");
			strcat(newsrc,p);
        		int in,out;
			in=open(newsrc,O_RDONLY);
			out=open(newdest,O_CREATE | O_RDWR);
        		while((a=read(in,buffer,in))>0) 
			{
			write(out,buffer,a);
			}
			close(in);
			close(out);
		}
	}		
	}
    }
  }
  close(fd);
  close(dfd);
}


void
cpr(char *src,char *dest)
{
  char buf[512], *p;
  int fd;
  int dfd;
  struct dirent de;
  struct stat st;
  stat(src,&st);
  fd=open(src,O_RDONLY);
  dfd=open(dest,O_WRONLY);
  if(dfd >= 0){
    printf(2, "cp -R: %s is a file or does not exist\n",dest);
    close(fd);
    close(dfd);
    return;
  }
  close(dfd);
  dfd=open(dest,O_RDONLY);
  if(st.type==T_FILE){
      char newdest[512];
      strcat(newdest,"/");
      strcat(newdest,dest);
      strcat(newdest,"/");
      strcat(newdest,src);
      int outs;
      outs=open(newdest,O_CREATE | O_RDWR);
      while(read(fd,buffer,sizeof(buffer))>0) write(outs,buffer,sizeof(buffer));
   }

  else if(st.type==T_DIR){
    strcpy(buf, src);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf,&st)<0){
	printf(1,"cp -R: file cannot be stats\n");
	continue;	
	}
      if(st.type==1){
        if(fmtname(buf)[0]=='.')continue;
	printf(1,"dir terjadi\n");
	char newsrc[512];
	char newdest[512];
	strcpy(newsrc,src);
	strcat(newsrc,"/");
	strcat(newsrc,p);
	strcpy(newdest,dest);
	strcat(newdest,"/");
	strcat(newdest,p);
	mkdir(newdest);
	cpr(newsrc,newdest);	
	}
       else{
	int a;        
	char newdest[512]; char newsrc[512];
        //strcpy(newdest,"/");
	strcpy(newdest,dest);
        strcat(newdest,"/");
        strcat(newdest,p);
	//strcpy(newsrc,"/");
        strcpy(newsrc,src);
        strcat(newsrc,"/");
	strcat(newsrc,p);
        int in,out;
//	printf(1,"file terkopi\n");
	//printf(1,"%s %s\n",newsrc,newdest);
        in=open(newsrc,O_RDONLY);
	out=open(newdest,O_CREATE | O_RDWR);
        while((a=read(in,buffer,out))>0) 
	{
		//printf(1,"%s\n",buffer);		
		write(out,buffer,in);
	}
	close(in);
	close(out);
	}
      //printf(1, "%s\n", fmtname(buf));
    }
  }
  close(fd);
  close(dfd);
}

void cp(char *src,char *dest){
	int fd0, fd1, n,isfile=1;
	char sec[512];
	
	fd0=open(src,O_RDWR);
	fd1=open(dest,O_RDWR);
	if(fd1>=0)isfile=0;
	close(fd1);
	fd1=open(dest,O_RDWR | O_CREATE);
	if(fd0<0 && fd1>=0){
		printf(1,"%s does not exist or is a directory\n",src);
		if(isfile)unlink(dest);
		close(fd0);
		close(fd1);
		exit();
	}
	else if(fd0<0 && fd1<0){
		printf(1,"Cannot copy directory to directory, use cp -R instead\n");
		close(fd0);
		close(fd1);
		exit();
	}
	else if(fd1<0 && fd0>=0){
		close(fd1);
		fd1=open(sec,O_CREATE | O_RDWR);
	}
//	char buffer[512];
	while((n=read(fd0,buffer,sizeof(buffer)))>0) write(fd1,buffer,sizeof(buffer));
	close(fd0);
	close(fd1);

}

int main(int argc, char *argv[]){
	char *src,*dest;
	if(argc<=2){
		printf(1," Usage: cp source destination\n");
		exit();
	}
	if(argc==3&&argv[1][0]!='*'){
	   	src=argv[1];
		dest=argv[2];
		cp(src,dest);
		exit();	
	}
	if(argc==4 && argv[1][0]=='-' && argv[1][1]=='R'){
		src=argv[2];dest=argv[3];		
		cpr(src,dest);
		exit();	
	}
	if(argv[1][0]=='*'){
		if(argc==3){
		dest=argv[2];	
		cpstar(".",dest,argv[1]);
		}
		else if(argc==4){
		src=argv[2];
		dest=argv[3];
		cpstar(src,dest,argv[1]);		
		}
		exit();	
	}
}
