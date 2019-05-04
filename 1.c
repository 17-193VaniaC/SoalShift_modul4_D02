#define FUSE_USE_VERSION 28
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

static int xmp_getattr(const char *path, struct stat *stbuf)
{
	printf("%s\n", path);
  	int res;
	char fpath[1000];
	sprintf(fpath,"%s%s",dirpath,path);
	res = lstat(fpath, stbuf);

	if (res == -1)
		return -errno;

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

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
  char fpath[1000];
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,path);
	int res = 0;

	DIR *dp;
	struct dirent *de;

	(void) offset;
	(void) fi;

	dp = opendir(fpath);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL) {
		struct stat st;
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;
		res = (filler(buf, de->d_name, &st, 0));
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

static int xmp_chmod(const char *path, mode_t mode){
	int res;
	char fpath[1000];
	sprintf(fpath, "%s%s",dirpath,path);
	if(strncmp(fpath,"/home/cikei/shift4/YOUTUBE",25)==0){
	        if(strstr(fpath, ".iz1") != NULL){
			perror("File berekstensi iz1 tidak boleh diubah permissionnya\n");
			return -errno;
		}
	else{
res = chmod(path, mode);
return 0;

}
	}

	res = chmod(path, mode);
	if(res == -1){
		return -errno;}

	return 0;
}

static int xmp_open(const char *path, struct fuse_file_info *fi)
{
	int res;

	res = open(path, fi->flags);
	if (res == -1)
		return -errno;

	close(res);
	return 0;
}

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
