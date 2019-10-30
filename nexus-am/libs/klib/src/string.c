#include "klib.h"

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  assert(s != NULL);
  size_t count = 0; 
  while (*(s++))
  {  //*s++改的是指针s，与const修饰s不冲突  
    count++; 
  }
  return count;
}

char *strcpy(char* dst,const char* src) {
  /*
  if(dst==src){//考虑到源字符串和目的字符串有重叠，也即地址相同
    return dst;
  }
  char* tmp=dst;//备份目的字符串的首地址，由于后面的操作会修改dest值
  while((*dst++=*src++)!='\0');
  return tmp;//将目的地址返回，获得函数返回值；
  */
  return strncpy(dst, src, strlen(src));
}

char* strncpy(char* dst, const char* src, size_t n) {
  /*
  char *address = dst;
  assert((dst!=NULL)&&(src!=NULL));
  if ((dst != src) && (0 < n) ){
	  while( n-- && '\0'!=( *dst++ = *src++ ) ){
		   ;// do nothing
	  }
  }
  *dst='\0';
  return address;
  */
  size_t size_src = strlen(src);
	if( n > size_src) return strncpy(dst, src, size_src);
	else // n <= size_src
	{
		// printf("strncpy1 %s %s %d\n", dst, src, (int)n);
		// 注意前后覆盖的问题
		char ch[n+1];
		char *head = ch;
		size_t i = 0;
		while( i < n ) {
			*(head+i) = *(src+i);
			i++;
		} 
		ch[n] = '\0';
		head = ch;
		char *result = dst;
		while((*(dst++) = *(head++))) ;
		// printf("strncpy2 %s\n", dst);
		return result;
	}
}

char* strcat(char* dst, const char* src) {
  /*
  size_t i,j;
  assert((dst!=NULL)&&(src!=NULL));
  for (i = 0; dst[i] != '\0'; i++){
    ;
  }
  for (j = 0; src[j] != '\0'; j++){
    dst[i+j] = src[j];
  }
  dst[i+j] = '\0';
  return dst;
  */
  char *result = dst;
	while(*dst) dst++;
	strcpy(dst, src);
	// printf("strcat2 %s\n", dst);
	return result;
}

int strcmp(const char* s1, const char* s2) {
  /*
  assert((s1!=NULL)&&(s2!=NULL));
	while(*s1!='\0'&&*s2!='\0'){
		if(*s1-*s2>0)
			return 1;
		if(*s1-*s2<0)
			return -1;
		s1++;
		s2++;
	}
	if(*s1=='\0'&&*s2!='\0'){
		return -1;
  }
	if(*s2=='\0'&&*s1!='\0'){
    return 1;
  }
	return 0; 
  */
  int a = 0;
	while( (a = (*s1 - *s2)) == 0 && *s1 && *s2) {
		s1++;
		s2++;
	}
	// printf("strcmp2 %d\n", a);
	return a;
}

int strncmp(const char* s1, const char* s2, size_t n) {
  /*
  assert((s1!=NULL)&&(s2!=NULL));
	while(*s1!='\0'&&*s2!='\0'&&(n>0)){
		if(*s1-*s2>0)
			return 1;
		if(*s1-*s2<0)
			return -1;
		s1++;
		s2++;
    n--;
	}
	if(*s1=='\0'&&*s2!='\0'){
		return -1;
  }
	if(*s2=='\0'&&*s1!='\0'){
    return 1;
  }
	return 0; 
  */
  assert((s1 != NULL) && (s2 != NULL));
	int a = 0;
	while( (a = (*s1 - *s2)) == 0 && *s1 && *s2 && n--) {
		s1++;
		s2++;
	}
	return a;
}

void* memset(void* v,int c,size_t n) {
  /*
  int *ptr = (int*)v;

  while (n--)
  {
    *ptr++ = c;
  }

  return v;
  */
 void* ret = v;
    while(n--)
    {
        *(char*)v = (char)c;
        v = (char*)v + 1; //移动一个字节
    }
    return ret;
}

void* memcpy(void* out, const void* in, size_t n) {
  /*
  const char *s = (char*)in;
  char *d = (char*)out;

  while (n--)
  {
    *d++ = *s++;
  }

  return out;
  */
 void *ret = out; //可能会出现覆盖的问题
	char *begin1 = (char *)out;
	const char *begin2 = (const char *)in;
	while(n--)
	{
		*(begin1+n-1) = *(begin2+n-1);
	}
	return ret;
}

int memcmp(const void* s1, const void* s2, size_t n){
  const unsigned char *str1 = (unsigned char *)s1;
  const unsigned char *str2 = (unsigned char *)s2;
  while (n-- > 0)
  {
    if (*str1++ != *str2++)
    {
      return str1[-1] < str2[-1] ? -1 : 1;
    }
  }
  return 0;
}

#endif
