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
mvstar(char *src,char *dest)
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
      while(read(fd,buffer,sizeof(buffer))>0){
	 write(outs,buffer,sizeof(buffer));
	}
	unlink(src);
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
	mvstar(newsrc,newdest);	
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
        in=open(newsrc,O_RDONLY);
	out=open(newdest,O_CREATE | O_RDWR);
        while((a=read(in,buffer,sizeof(buffer)))>0) 
	{
		write(in,buffer,sizeof(buffer));
	}
	unlink(newsrc);
	close(in);
	close(out);
	}
      //printf(1, "%s\n", fmtname(buf));
    }
  }
  close(fd);
  close(dfd);
}

void mv(char *src,char *dest){
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
		printf(1,"use mv * instead\n");
		close(fd0);
		close(fd1);
		exit();
	}
	else if(fd1<0 && fd0>=0){
		close(fd1);
		fd1=open(sec,O_CREATE | O_RDWR);
	}
	while((n=read(fd0,buffer,sizeof(buffer)))>0) write(fd1,buffer,sizeof(buffer));
	unlink(src);
	close(fd0);
	close(fd1);

}

int main(int argc, char *argv[]){
	char *src,*dest;
	if(argc<=2){
		printf(1," Usage: mv source destination\n");
		exit();
	}
	if(argc==3&&argv[1][0]!='*'){
	   	src=argv[1];
		dest=argv[2];
		mv(src,dest);
		exit();	
	}
	if(argc==3 && argv[1][0]=='*'){
		dest=argv[2];
		mvstar(".",dest);
		exit();	
	}
}
