define FUSE_USE_VERSION 28
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
//static const char *dirpath2 ="/home/cikei/shift4";

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                       off_t offset, struct fuse_file_info *fi)
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

tatic int xmp_mkdir(const char *path,mode_t mode)
{
    int res;
    char fpath[1000];
    sprintf(fpath,"%s%s",dirpath,path);
	if(strncmp(path,"/home/cikei/shift4/YOUTUBE",25)==0){//nama folder dari file system
		res=mkdir(path, 0750);
	}
	else{
		res = mkdir (fpath,mode);
	}
    if(res == -1)
        return -errno;

    
    return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,struct fuse_file_info *fi){
	int res;

	(void) fi;
	
	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	return res;
}
static int xmp_chmod(const char *path, mode_t mode){
        int res;
        if(strncmp(path,"/home/cikei/shift4/YOUTUBE",25)==0){//nama folder dari file system
                if(strstr(fpath, ".iz1") != NULL) == 0){
                printf("File berekstensi iz1 tidak boleh diubah permissionnya\n");
                        return -errno;
        }
        else{
                res = chmod(fpath,mode);
        }
        res = chmod(path, mode);
        if (res == -1)
                return -errno;

        return 0;
}



static struct fuse_operations xmp_oper = {
        .getattr        = xmp_getattr,
        .mkdir          = xmp_mkdir,
        .readdir        = xmp_readdir,
        .read           = xmp_read,
	.chmod		= xmp_chmod
};

int main(int argc, char *argv[])
{
        umask(0);
        return fuse_main(argc, argv, &xmp_oper, NULL);
}
