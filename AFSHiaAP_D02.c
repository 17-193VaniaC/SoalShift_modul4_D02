#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <libgen.h>

static const char *dirpath = "/home/alifi/Shift4";
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

static int xmp_mkdir(const char *path,mode_t mode)
{
    int res;
    char fpath[1000];

    sprintf(fpath,"%s%s",dirpath,path);
    if(strncmp(fpath,"/home/cikei/shift4/YOUTUBE",20) == 0)
    {
        res = mkdir(fpath, 0750);
    }
    else
    {
        res = mkdir(fpath, mode);
    }

    if(res == -1) return -errno;
    
    return 0;
}

static int xmp_chmod(const char *path, mode_t mode)
{
    int res;

    if(strncmp(path,"/home/cikei/shift4/YOUTUBE",25)==0) //nama folder dari file system
    {
        if(strstr(path, ".iz1") != NULL)
        {
            printf("File berekstensi iz1 tidak boleh diubah permissionnya\n");
            return -errno;
        }
        else
        {
            res = chmod(path,mode);
        }
    }
    else
    {
        res = chmod(path, mode);
    }
    if (res == -1) return -errno;

    return 0;
}

static struct fuse_operations xmp_oper =
{
    .getattr	= xmp_getattr,
    .readdir	= xmp_readdir,
    .read	= xmp_read,
    .mkdir      = xmp_mkdir,
    .chmod      = xmp_chmod
};

int main(int argc, char *argv[])
{
    umask(0);
    return fuse_main(argc, argv, &xmp_oper, NULL);
}
