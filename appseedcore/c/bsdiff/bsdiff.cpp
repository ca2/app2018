#include "StdAfx.h"
#include "bzlib.h"
#include "bsdiff.h"
#include "bsdiff_private.h"


typedef unsigned char u_char;
typedef long pid_t;


static void split(off_t *I,off_t *V,off_t start,off_t len,off_t h)
{
   off_t i,j,k,x,tmp,jj,kk;

   if(len<16) {
      for(k=start;k<start+len;k+=j) {
         j=1;x=V[I[k]+h];
         for(i=1;k+i<start+len;i++) {
            if(V[I[k+i]+h]<x) {
               x=V[I[k+i]+h];
               j=0;
            };
            if(V[I[k+i]+h]==x) {
               tmp=I[k+j];I[k+j]=I[k+i];I[k+i]=tmp;
               j++;
            };
         };
         for(i=0;i<j;i++) V[I[k+i]]=k+j-1;
         if(j==1) I[k]=-1;
      };
      return;
   };

   x=V[I[start+len/2]+h];
   jj=0;kk=0;
   for(i=start;i<start+len;i++) {
      if(V[I[i]+h]<x) jj++;
      if(V[I[i]+h]==x) kk++;
   };
   jj+=start;kk+=jj;

   i=start;j=0;k=0;
   while(i<jj) {
      if(V[I[i]+h]<x) {
         i++;
      } else if(V[I[i]+h]==x) {
         tmp=I[i];I[i]=I[jj+j];I[jj+j]=tmp;
         j++;
      } else {
         tmp=I[i];I[i]=I[kk+k];I[kk+k]=tmp;
         k++;
      };
   };

   while(jj+j<kk) {
      if(V[I[jj+j]+h]==x) {
         j++;
      } else {
         tmp=I[jj+j];I[jj+j]=I[kk+k];I[kk+k]=tmp;
         k++;
      };
   };

   if(jj>start) split(I,V,start,jj-start,h);

   for(i=0;i<kk-jj;i++) V[I[jj+i]]=kk-1;
   if(jj==kk-1) I[jj]=-1;

   if(start+len>kk) split(I,V,kk,start+len-kk,h);
}

static void qsufsort(off_t *I,off_t *V,u_char *old,off_t oldsize)
{
   off_t buckets[256];
   off_t i,h,len;

   for(i=0;i<256;i++) buckets[i]=0;
   for(i=0;i<oldsize;i++) buckets[old[i]]++;
   for(i=1;i<256;i++) buckets[i]+=buckets[i-1];
   for(i=255;i>0;i--) buckets[i]=buckets[i-1];
   buckets[0]=0;

   for(i=0;i<oldsize;i++) I[++buckets[old[i]]]=i;
   I[0]=oldsize;
   for(i=0;i<oldsize;i++) V[i]=buckets[old[i]];
   V[oldsize]=0;
   for(i=1;i<256;i++) if(buckets[i]==buckets[i-1]+1) I[buckets[i]]=-1;
   I[0]=-1;

   for(h=1;I[0]!=-(oldsize+1);h+=h) {
      len=0;
      for(i=0;i<oldsize+1;) {
         if(I[i]<0) {
            len-=I[i];
            i-=I[i];
         } else {
            if(len) I[i-len]=-len;
            len=V[I[i]]+1-i;
            split(I,V,i,len,h);
            i+=len;
            len=0;
         };
      };
      if(len) I[i-len]=-len;
   };

   for(i=0;i<oldsize+1;i++) I[V[i]]=i;
}

static off_t matchlen(u_char *old,off_t oldsize,u_char *_new,off_t newsize)
{
   off_t i;

   for(i=0;(i<oldsize)&&(i<newsize);i++)
      if(old[i]!=_new[i]) break;

   return i;
}

static off_t search(off_t *I,u_char *old,off_t oldsize,
      u_char *_new,off_t newsize,off_t st,off_t en,off_t *pos)
{
   off_t x,y;

   if(en-st<2) {
      x=matchlen(old+I[st],oldsize-I[st],_new,newsize);
      y=matchlen(old+I[en],oldsize-I[en],_new,newsize);

      if(x>y) {
         *pos=I[st];
         return x;
      } else {
         *pos=I[en];
         return y;
      }
   };

   x=st+(en-st)/2;
   if(memcmp_dup(old+I[x],_new,min(oldsize-I[x],newsize))<0) {
      return search(I,old,oldsize,_new,newsize,x,en,pos);
   } else {
      return search(I,old,oldsize,_new,newsize,st,x,pos);
   };
}

static void offtout(off_t x,u_char *buf)
{
   off_t y;

   if(x<0) y=-x; else y=x;

      buf[0]=y%256;y-=buf[0];
   y=y/256;buf[1]=y%256;y-=buf[1];
   y=y/256;buf[2]=y%256;y-=buf[2];
   y=y/256;buf[3]=y%256;y-=buf[3];
   y=y/256;buf[4]=y%256;y-=buf[4];
   y=y/256;buf[5]=y%256;y-=buf[5];
   y=y/256;buf[6]=y%256;y-=buf[6];
   y=y/256;buf[7]=y%256;

   if(x<0) buf[7]|=0x80;
}

int bsdiff(const char * oldfile, const char * newfile, const char * patchfile)
{
   _FILE * fd = 0;
   u_char *old = NULL;
   u_char *_new = NULL;
   off_t oldsize,newsize;
   off_t * I = NULL;
   off_t * V = NULL;
   off_t scan,pos,len;
   off_t lastscan,lastpos,lastoffset;
   off_t oldscore,scsc;
   off_t s,Sf,lenf,Sb,lenb;
   off_t overlap,Ss,lens;
   off_t i;
   off_t dblen,eblen;
   u_char *db = NULL;
   u_char *eb = NULL;
   u_char buf[8];
   u_char header[32];
   _FILE * pf = NULL;
   BZFILE * pfbz2 = NULL;
   int bz2err;

   /* allocate oldsize+1 bytes instead of oldsize bytes to ensure
      that we never try to ca2_alloc(0) and get a NULL pointer */
   //org:
   //if(((fd=_open(oldfile,O_RDONLY,0))<0) ||
   //   ((oldsize=fseek_dup(fd,0,SEEK_END))==-1) ||
   //   ((old=ca2_alloc(oldsize+1))==NULL) ||
   //   (fseek_dup(fd,0,SEEK_SET)!=0) ||
   //   (fread_dup(fd,old,oldsize)!=oldsize) ||
   //   (fclose_dup(fd)==-1)) return err(1,"%s",oldfile);
   //new:
   //fread_dup in chunks, don't rely on fread_dup always returns full data!
   if(((fd=fopen_dup(oldfile,"rb")) ==0) ||
      ((oldsize=fseek_dup(fd,0,SEEK_END))==-1) ||
      ((old=(u_char*)ca2_alloc(oldsize+1))==NULL) ||
      (fseek_dup(fd,0,SEEK_SET)!=0))
   {
      if(fd != 0)
      {
         fclose_dup(fd);
      }
      if(old != NULL)
      {
         ca2_free(old);
      }
      return err(1,"%s",oldfile);
   }
   int r=oldsize;
   while (r>0 && (i = fread_dup(old + oldsize - r, 1, r, fd))>0) r-=i;
   if (r>0 || fclose_dup(fd)>0)
   {
      ca2_free(old);
      return err(1,"%s",oldfile);
   }


   if(((I=(off_t*)ca2_alloc((oldsize+1)*sizeof(off_t)))==NULL) ||
      ((V=(off_t*)ca2_alloc((oldsize+1)*sizeof(off_t)))==NULL))
   {
      ca2_free(old);
      if(I != NULL)
      {
         ca2_free(I);
      }
      if(V != NULL)
      {
         ca2_free(V);
      }
      return err(1,NULL);
   }

   qsufsort(I,V,old,oldsize);

   ca2_free(V);

   /* allocate newsize+1 bytes instead of newsize bytes to ensure
      that we never try to ca2_alloc(0) and get a NULL pointer */
   //org:
   //if(((fd=_open(newfile,O_RDONLY,0))<0) ||
   //   ((newsize=fseek_dup(fd,0,SEEK_END))==-1) ||
   //   ((_new=ca2_alloc(newsize+1))==NULL) ||
   //   (fseek_dup(fd,0,SEEK_SET)!=0) ||
   //   (fread_dup(fd,_new,newsize)!=newsize) ||
   //   (fclose_dup(fd)==-1)) return err(1,"%s",newfile);
   //new:
   //fread_dup in chunks, don't rely on fread_dup always returns full data!
   if(((fd=fopen_dup(newfile,"rb"))== NULL) ||
      ((newsize=fseek_dup(fd,0,SEEK_END))==-1) ||
      ((_new=(u_char*)ca2_alloc(newsize+1))==NULL) ||
      (fseek_dup(fd,0,SEEK_SET)!=0))
   {
      if(fd != 0)
      {
         fclose_dup(fd);
      }
      ca2_free(old);
      ca2_free(I);
      return err(1,"%s",newfile);
   }

   r=newsize;
   while (r>0 && (i=fread_dup(_new + newsize - r, 1, r, fd))>0) r-=i;
   if (r>0 || fclose_dup(fd)> 0)
   {
      ca2_free(old);
      ca2_free(_new);
      ca2_free(I);
      return err(1,"%s",oldfile);
   }

   if(((db=(u_char*)ca2_alloc(newsize+1))==NULL) ||
      ((eb=(u_char*)ca2_alloc(newsize+1))==NULL))
   {
      ca2_free(old);
      ca2_free(_new);
      ca2_free(I);
      if(db != NULL)
      {
         ca2_free(db);
      }
      if(eb != NULL)
      {
         ca2_free(eb);
      }
      return err(1,NULL);
   }
   dblen=0;
   eblen=0;

   /* Create the patch file */
   //org:
   //if ((pf = fopen(patchfile, "w")) == NULL)
   //new:
   //if((fd=_open(patchfile,O_CREAT|O_TRUNC|O_WRONLY|O_BINARY|O_NOINHERIT,0666))<0)
   if ((pf = fopen_dup(patchfile, "wb")) == NULL)
   {
      ca2_free(old);
      ca2_free(_new);
      ca2_free(I);
      ca2_free(db);
      ca2_free(eb);
      return err(1,"%s",patchfile);
   }

   /* Header is
      0   8    "BSDIFF40"
      8   8   length of bzip2ed ctrl block
      16   8   length of bzip2ed diff block
      24   8   length of new file */
   /* File is
      0   32   Header
      32   ??   Bzip2ed ctrl block
      ??   ??   Bzip2ed diff block
      ??   ??   Bzip2ed extra block */
   memcpy_dup(header,"BSDIFF40",8);
   offtout(0, header + 8);
   offtout(0, header + 16);
   offtout(newsize, header + 24);
   if (fwrite_dup(header, 32, 1, pf) != 1)
   {
      ca2_free(old);
      ca2_free(_new);
      ca2_free(I);
      ca2_free(db);
      ca2_free(eb);
      fclose_dup(pf);
      return err(1, "fwrite_dup(%s)", patchfile);
   }

   /* Compute the differences, writing ctrl as we go */
   if ((pfbz2 = BZ2_bzWriteOpen(&bz2err, pf, 9, 0, 0)) == NULL)
   {
      ca2_free(old);
      ca2_free(_new);
      ca2_free(I);
      ca2_free(db);
      ca2_free(eb);
      fclose_dup(pf);
      return errx(1, "BZ2_bzWriteOpen, bz2err = %d", bz2err);
   }
   scan=0;len=0;
   lastscan=0;lastpos=0;lastoffset=0;
   while(scan<newsize) {
      oldscore=0;

      for(scsc=scan+=len;scan<newsize;scan++) {
         len=search(I,old,oldsize,_new+scan,newsize-scan,
               0,oldsize,&pos);

         for(;scsc<scan+len;scsc++)
         if((scsc+lastoffset<oldsize) &&
            (old[scsc+lastoffset] == _new[scsc]))
            oldscore++;

         if(((len==oldscore) && (len!=0)) || 
            (len>oldscore+8)) break;

         if((scan+lastoffset<oldsize) &&
            (old[scan+lastoffset] == _new[scan]))
            oldscore--;
      };

      if((len!=oldscore) || (scan==newsize)) {
         s=0;Sf=0;lenf=0;
         for(i=0;(lastscan+i<scan)&&(lastpos+i<oldsize);) {
            if(old[lastpos+i]==_new[lastscan+i]) s++;
            i++;
            if(s*2-i>Sf*2-lenf) { Sf=s; lenf=i; };
         };

         lenb=0;
         if(scan<newsize) {
            s=0;Sb=0;
            for(i=1;(scan>=lastscan+i)&&(pos>=i);i++) {
               if(old[pos-i]==_new[scan-i]) s++;
               if(s*2-i>Sb*2-lenb) { Sb=s; lenb=i; };
            };
         };

         if(lastscan+lenf>scan-lenb) {
            overlap=(lastscan+lenf)-(scan-lenb);
            s=0;Ss=0;lens=0;
            for(i=0;i<overlap;i++) {
               if(_new[lastscan+lenf-overlap+i]==
                  old[lastpos+lenf-overlap+i]) s++;
               if(_new[scan-lenb+i]==
                  old[pos-lenb+i]) s--;
               if(s>Ss) { Ss=s; lens=i+1; };
            };

            lenf+=lens-overlap;
            lenb-=lens;
         };

         for(i=0;i<lenf;i++)
            db[dblen+i]=_new[lastscan+i]-old[lastpos+i];
         for(i=0;i<(scan-lenb)-(lastscan+lenf);i++)
            eb[eblen+i]=_new[lastscan+lenf+i];

         dblen+=lenf;
         eblen+=(scan-lenb)-(lastscan+lenf);

         offtout(lenf,buf);
         BZ2_bzWrite(&bz2err, pfbz2, buf, 8);
         if (bz2err != BZ_OK)
         {
            ca2_free(old);
            ca2_free(_new);
            ca2_free(I);
            ca2_free(db);
            ca2_free(eb);
            fclose_dup(pf);
            return errx(1, "BZ2_bzWrite, bz2err = %d", bz2err);
         }

         offtout((scan-lenb)-(lastscan+lenf),buf);
         BZ2_bzWrite(&bz2err, pfbz2, buf, 8);
         if (bz2err != BZ_OK)
         {
            ca2_free(old);
            ca2_free(_new);
            ca2_free(I);
            ca2_free(db);
            ca2_free(eb);
            fclose_dup(pf);
            return errx(1, "BZ2_bzWrite, bz2err = %d", bz2err);
         }

         offtout((pos-lenb)-(lastpos+lenf),buf);
         BZ2_bzWrite(&bz2err, pfbz2, buf, 8);
         if (bz2err != BZ_OK)
         {
            ca2_free(old);
            ca2_free(_new);
            ca2_free(I);
            ca2_free(db);
            ca2_free(eb);
            fclose_dup(pf);
            return errx(1, "BZ2_bzWrite, bz2err = %d", bz2err);
         }

         lastscan=scan-lenb;
         lastpos=pos-lenb;
         lastoffset=pos-scan;
      };
   };
   BZ2_bzWriteClose(&bz2err, pfbz2, 0, NULL, NULL);
   if (bz2err != BZ_OK)
   {
      ca2_free(old);
      ca2_free(_new);
      ca2_free(I);
      ca2_free(db);
      ca2_free(eb);
      fclose_dup(pf);
      return errx(1, "BZ2_bzWriteClose, bz2err = %d", bz2err);
   }

   /* Compute size of compressed ctrl data */
   if ((len = ftell_dup(pf)) == -1)
   {
      ca2_free(old);
      ca2_free(_new);
      ca2_free(I);
      ca2_free(db);
      ca2_free(eb);
      fclose_dup(pf);
      return err(1, "ftello");
   }
   offtout(len-32, header + 8);

   /* _write compressed diff data */
   if ((pfbz2 = BZ2_bzWriteOpen(&bz2err, pf, 9, 0, 0)) == NULL)
   {
      ca2_free(old);
      ca2_free(_new);
      ca2_free(I);
      ca2_free(db);
      ca2_free(eb);
      fclose_dup(pf);
      return errx(1, "BZ2_bzWriteOpen, bz2err = %d", bz2err);
   }
   BZ2_bzWrite(&bz2err, pfbz2, db, dblen);
   if (bz2err != BZ_OK)
   {
      ca2_free(old);
      ca2_free(_new);
      ca2_free(I);
      ca2_free(db);
      ca2_free(eb);
      fclose_dup(pf);
      return errx(1, "BZ2_bzWrite, bz2err = %d", bz2err);
   }
   BZ2_bzWriteClose(&bz2err, pfbz2, 0, NULL, NULL);
   if (bz2err != BZ_OK)
   {
      ca2_free(old);
      ca2_free(_new);
      ca2_free(I);
      ca2_free(db);
      ca2_free(eb);
      fclose_dup(pf);
      return errx(1, "BZ2_bzWriteClose, bz2err = %d", bz2err);
   }

   /* Compute size of compressed diff data */
   if ((newsize = ftell_dup(pf)) == -1)
   {
      ca2_free(old);
      ca2_free(_new);
      ca2_free(I);
      ca2_free(db);
      ca2_free(eb);
      fclose_dup(pf);
      return err(1, "ftello");
   }
   offtout(newsize - len, header + 16);

   /* _write compressed extra data */
   if ((pfbz2 = BZ2_bzWriteOpen(&bz2err, pf, 9, 0, 0)) == NULL)
   {
      ca2_free(old);
      ca2_free(_new);
      ca2_free(I);
      ca2_free(db);
      ca2_free(eb);
      fclose_dup(pf);
      return errx(1, "BZ2_bzWriteOpen, bz2err = %d", bz2err);
   }
   BZ2_bzWrite(&bz2err, pfbz2, eb, eblen);
   if (bz2err != BZ_OK)
   {
      ca2_free(old);
      ca2_free(_new);
      ca2_free(I);
      ca2_free(db);
      ca2_free(eb);
      fclose_dup(pf);
      return errx(1, "BZ2_bzWrite, bz2err = %d", bz2err);
   }
   BZ2_bzWriteClose(&bz2err, pfbz2, 0, NULL, NULL);
   if (bz2err != BZ_OK)
   {
      ca2_free(old);
      ca2_free(_new);
      ca2_free(I);
      ca2_free(db);
      ca2_free(eb);
      fclose_dup(pf);
      return errx(1, "BZ2_bzWriteClose, bz2err = %d", bz2err);
   }

   /* seek to the beginning, _write the header, and fclose_dup the file */
   if (fseek_dup(pf, 0, SEEK_SET))
   {
      ca2_free(old);
      ca2_free(_new);
      ca2_free(I);
      ca2_free(db);
      ca2_free(eb);
      fclose_dup(pf);
      return err(1, "fseeko");
   }
   if (fwrite_dup(header, 32, 1, pf) != 1)
   {
      ca2_free(old);
      ca2_free(_new);
      ca2_free(I);
      ca2_free(db);
      ca2_free(eb);
      fclose_dup(pf);
      return err(1, "fwrite_dup(%s)", patchfile);
   }
   if (fclose_dup(pf))
   {
      fclose_dup(pf);
      return err(1, "fclose_dup");
   }

   /* Free the primitive::memory we used */
   ca2_free(db);
   ca2_free(eb);
   ca2_free(I);
   ca2_free(old);
   ca2_free(_new);

   return 0;
}
