#define FUSE_USE_VERSION 28
#define HAVE_SETXATTR

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef linux
/* For pread()/pwrite() */
#define _XOPEN_SOURCE 500
#endif

#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <libgen.h>

static const char *dirpath = "/home/cikei/shift4";
char charlist[] = {"qE1~ YMUR2\"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0"};

void encrypt(char paths[])
{
    int i, j;
    char huruf;
    for(i=0; i<strlen(paths); i++)
    {
        huruf = paths[i];
        if(huruf == '/') continue;

        for(j=0; j<strlen(charlist); j++)
        {
            if(huruf==charlist[j]) break;
        }
        j=j+17;
        if(i>93)
        {
            j=j-strlen(charlist);
        }
        
        paths[i]=charlist[j];
    }
}

void decrypt(char paths[])
{
    int i, j;
    char huruf;
    for(i=0; i<strlen(paths); i++)
    {
        huruf=paths[i];
        if(huruf == '/') continue;

        for(j=0; j<strlen(charlist); j++)
        {
            if(huruf==charlist[j]) break;
        }
        j=j-17;
        if(j<0)
        {
            j=j+strlen(charlist);
        } 
        paths[i]=charlist[j];
    }
}

static int xmp_getattr(const char *path, struct stat *stbuf)
{
    int res;
    char fpath[1000], path_enc[1000];

    strcpy(path_enc, path);
    if(strcmp(path_enc,".")!=0 || strcmp(path_enc,"..")!=0) encrypt(path_enc);

    sprintf(fpath,"%s%s", dirpath, path_enc);
    res = lstat(fpath, stbuf);
    if (res == -1)
        return -errno;

    return 0;
}

static int xmp_open(const char *path, struct fuse_file_info *fi)
{
        int res;
char fpath[1000];
        if(strcmp(path,"/") == 0)
        {
                path=dirpath;
                sprintf(fpath,"%s",path);
        }
        else sprintf(fpath, "%s%s",dirpath,path);
        res = open(fpath, fi->flags);
        if (res == -1)
                return -errno;

        close(res);
        return 0;
}


static int xmp_mkdir(const char *path,mode_t mode){
    int res;
    char fpath[1000];
    sprintf(fpath,"%s%s",dirpath,path);
    if(strncmp(fpath,"/home/cikei/shift4/YOUTUBER/",20) == 0){
	res = mkdir(fpath, 0750);
    	}
    else{
    res = mkdir(fpath,mode);
    }
    if(res == -1)
        return -errno;
    return 0;
}

	return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                       off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000], path_dec[1000], path_enc[1000];
    if(strcmp(path,"/") == 0)
    {
        path=dirpath;
        sprintf(fpath,"%s",path);
    }
    else
    {
        strcpy(path_enc, path);
        if(strcmp(path_enc, ".") != 0 && strcmp(path_enc, "..") != 0)encrypt(path_enc);
        sprintf(fpath, "%s%s", dirpath, path_enc);
    }
    int res = 0;

    DIR *dp;
    struct dirent *de;

    (void) offset;
    (void) fi;

    dp = opendir(fpath);
    if (dp == NULL) return -errno;

    while ((de = readdir(dp)) != NULL)
    {
        struct stat st;
        memset(&st, 0, sizeof(st));

        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;

        strcpy(path_dec, de->d_name);
        decrypt(path_dec);
        res = (filler(buf, path_dec, &st, 0));
        if(res!=0) break;
    }

    closedir(dp);
    return 0;
}

static int xmp_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi){
	int fd;
	int res;
	char fpath[1000];
	sprintf(fpath, "%s%s",dirpath,path);

	(void) fi;
	fd = open(path, O_WRONLY);
	if (fd == -1)
		return -errno;
	if(strncmp(fpath,"/home/cikei/shift4/YOUTUBE",25)==0){
            if(strstr(fpath, ".iz1") == NULL){
		res = pwrite(fd, buf, size, offset);
		}
	    else{ return -errno;}
	}
	else{
		res = pwrite(fd, buf, size, offset); 
}
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}
/*
static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
  	char fpath[1000];
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,path);
	if(strstr(fpath, ".pdf") != NULL || strstr(fpath, ".doc") != NULL || strstr(fpath, ".txt") != NULL)
	{
		printf("Terjadi kesalahan! File berisi konten berbahaya.\n");
		char end[1000];
		char ay[1000];
		strcat(ay, dirpath);
		strcat(ay, "/rahasia");
		int result2 = mkdir(ay, ACCESSPERMS);
		if(result2 == -1)
		{
			printf("Direktori sudah ada\n");
		}
		strcat(end, fpath);
		strcat(end, ".ditandai");
		int result = rename(fpath, end);
		char cmd[250];
		if(result == -1)
			return -errno;
		strcat(cmd, "mv ");
		strcat(cmd, fpath);
		strcat(cmd, ".ditandai ");
		strcat(cmd, ay);
		system(cmd);
		char *name = basename(fpath);
		char filenya[200];
		strcat(filenya, ay);
		strcat(filenya, "/");
		strcat(filenya, name);
		strcat(filenya, ".ditandai");
		int result3 = chmod(filenya, 0000);
		if(result3 == -1)
			return -errno;
		return 0;
	}
	int res = 0;
  	int fd = 0 ;

	(void) fi;
	fd = open(fpath, O_RDONLY);
	if (fd == -1)
		return -errno;

	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}
*/
static int xmp_chmod(const char *path, mode_t mode){
	int res;
	char fpath[1000];
	sprintf(fpath, "%s%s",dirpath,path);
	if(strncmp(fpath,"/home/cikei/shift4/YOUTUBER",25)==0){
	        if(strstr(fpath, ".iz1") != NULL){
			printf("\nFile berekstensi iz1 tidak boleh diubah permissionnya\n");
			return 0;
		}
	else{
		res = chmod(fpath, mode);
		printf("bisa terchmod");
          	return 0;}
	}

	res = chmod(fpath, mode);
	if(res == -1){
		return -errno;}

	return 0;
}

static int xmp_read(const char *path, char *st, size_t size, off_t offset,
                    struct fuse_file_info *fi)
{
    char fpath[1000], path_enc[1000];
    strcpy(path_enc, path);
    if(strcmp(path_enc, ".") != 0 && strcmp(path_enc, "..") != 0)encrypt(path_enc);

    if(strcmp(path,"/") == 0)
    {
        path=dirpath;
        sprintf(fpath,"%s",path);
    }
    else sprintf(fpath,"%s%s", dirpath, path_enc);
    int res = 0;
    int fd = 0 ;

    (void) fi;
    fd = open(fpath, O_RDONLY);
    if (fd == -1) return -errno;

    res = pread(fd, st, size, offset);
    if (res == -1)
        res = -errno;

    close(fd);
    return res;
}

/*

static int xmp_open(const char *path, struct fuse_file_info *fi)
{
	int res;

	res = open(path, fi->flags);
	if (res == -1)
		return -errno;

	close(res);
	return 0;
}
*/
static struct fuse_operations xmp_oper = {
	.getattr	= xmp_getattr,
	.readdir	= xmp_readdir,
	.read		= xmp_read,
	.mkdir		= xmp_mkdir,
	.open		= xmp_open,
	.write		= xmp_write,
	.chmod		= xmp_chmod

};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}
