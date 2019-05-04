# SoalShift_modul4_D02

#### Anggota Kelompok D02
1. Alifiannisa A. W.    (05111740000011)
2. Vania Cikanindi      (05111740000193)
---

### Soal 1


Semua nama file dan folder harus terenkripsi
Enkripsi yang Atta inginkan sangat sederhana, yaitu Caesar cipher. Namun, Kusuma mengatakan enkripsi tersebut sangat mudah dipecahkan. Dia menyarankan untuk character list diekspansi,tidak hanya alfabet, dan diacak. Berikut character list yang dipakai:
```
qE1~ YMUR2"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\8s;g<{3.u*W-0
```
Misalkan ada file bernama “halo” di dalam folder “INI_FOLDER”, dan key yang dipakai adalah 17, maka:
“INI_FOLDER/halo” saat belum di-mount maka akan bernama “n,nsbZ]wio/QBE#”, saat telah di-mount maka akan otomatis terdekripsi kembali menjadi “INI_FOLDER/halo” .
Perhatian: Karakter ‘/’ adalah karakter ilegal dalam penamaan file atau folder dalam *NIX, maka dari itu dapat diabaikan

** Solusi **

Soal ini dikerjakan dengan membuat fungsi untuk mengenkripsi dan mendekripsi. Fungsi `encrypt` dibuat dengan menggeser setiap huruf ci paths sebanyak 17 karakter pada character list. Saat program menemukan karakter '/' pada paths, maka program akan mengabaikan karakter tersebut. Sama dengan fungsi `encrypt`, fungsi `decrypt` hanya mengubah arah geser huruf ke kiri.  

``` c
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
```

---

### Soal 2
  
Semua file video yang tersimpan secara terpecah-pecah (splitted) harus secara otomatis tergabung (joined) dan diletakkan dalam folder “Videos”
Urutan operasi dari kebutuhan ini adalah:
1. Tepat saat sebelum file system di-mount
   * Secara otomatis folder “Videos” terbuat di root directory file system
   * Misal ada sekumpulan file pecahan video bernama “computer.mkv.000”, “computer.mkv.001”, “computer.mkv.002”, dst. Maka secara otomatis file pecahan tersebut akan di-join menjadi file video “computer.mkv”
     Untuk mempermudah kalian, dipastikan hanya video file saja yang terpecah menjadi beberapa file. File pecahan tersebut dijamin terletak di root folder fuse
   * Karena mungkin file video sangat banyak sehingga mungkin saja saat menggabungkan file video, file system akan membutuhkan waktu yang lama untuk sukses ter-mount. Maka pastikan saat akan menggabungkan file pecahan video, file system akan membuat 1 thread/proses(fork) baru yang dikhususkan untuk menggabungkan file video tersebut
   * Pindahkan seluruh file video yang sudah ter-join ke dalam folder “Videos”
   * Jangan tampilkan file pecahan di direktori manapun
2. Tepat saat file system akan di-unmount
   * Hapus semua file video yang berada di folder “Videos”, tapi jangan hapus file pecahan yang terdapat di root directory file system
   * Hapus folder “Videos”
   
** Solusi **

---

### Soal 3

Sebelum diterapkannya file system ini, Atta pernah diserang oleh hacker LAPTOP_RUSAK yang menanamkan user bernama “chipset” dan “ic_controller” serta group “rusak” yang tidak bisa dihapus. Karena paranoid, Atta menerapkan aturan pada file system ini untuk menghapus “file bahaya” yang memiliki spesifikasi:
- Owner Name     : ‘chipset’ atau ‘ic_controller’
- Group Name    : ‘rusak’
- Tidak dapat dibaca
Jika ditemukan file dengan spesifikasi tersebut ketika membuka direktori, Atta akan menyimpan nama file, group ID, owner ID, dan waktu terakhir diakses dalam file “filemiris.txt” (format waktu bebas, namun harus memiliki jam menit detik dan tanggal) lalu menghapus “file bahaya” tersebut untuk mencegah serangan lanjutan dari LAPTOP_RUSAK.

** Solusi **

---

### Soal 4
Pada folder YOUTUBER, setiap membuat folder permission foldernya akan otomatis menjadi 750. Juga ketika membuat file permissionnya akan otomatis menjadi 640 dan ekstensi filenya akan bertambah “.iz1”. File berekstensi “.iz1” tidak bisa diubah permissionnya dan memunculkan error bertuliskan “File ekstensi iz1 tidak boleh diubah permissionnya.”

** Solusi **

Karena ingin membuat folder dengan permission yang berada di folder YOUTUBE, maka yang dilakukan adalah membandingkan folder path yang akan dibuat. sehingga, jika folder tersebut berada di dalam folder YOUTUBE, maka permissionnya akan menjado 750.
```c
static int xmp_mkdir(const char *path,mode_t mode)
{
    int res;
    char fpath[1000];

    sprintf(fpath,"%s%s",dirpath,path);
    if(strncmp(fpath,"/home/cikei/shift4/YOUTUBER",20) == 0){  //jika alamat dari folder yang dibuat berada di folder YOUTUBE
                res = mkdir(fpath, 0750);
        }
        else{
                res = mkdir(fpath, mode);
        }

        if(res == -1)
        return -errno;


    return 0;
}
```
Supaya file berekstensi iz1 dalam folder YOUTUBER tidak bisa diganti permissionnya, maka dilakukan perubahan dalam fungsi xmp_chmod seperti berikut.
```
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
                return 0;}
        }

        res = chmod(fpath, mode);
        if(res == -1){
                return -errno;}

        return 0;
}

```
Selain itu, agar file yang dibuat memiliki permission 640, maka fungsi yang dibutuhkan adalah sebagai berikut.


---

### Soal  5

Ketika mengedit suatu file dan melakukan save, maka akan terbuat folder baru bernama Backup kemudian hasil dari save tersebut akan disimpan pada backup dengan nama namafile_[timestamp].ekstensi. Dan ketika file asli dihapus, maka akan dibuat folder bernama RecycleBin, kemudian file yang dihapus beserta semua backup dari file yang dihapus tersebut (jika ada) di zip dengan nama namafile_deleted_[timestamp].zip dan ditaruh kedalam folder RecycleBin (file asli dan backup terhapus). Dengan format [timestamp] adalah yyyy-MM-dd_HH:mm:ss

** Solusi **

---
