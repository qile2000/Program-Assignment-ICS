#include "klib.h"

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  assert(s != NULL);
  size_t count = 0; 
  while (*(s++)){  //*s++改的是指针s，与const修饰s不冲突  
    count++; 
  }
  return count;
}

char *strcpy(char* dst,const char* src) {
  return strncpy(dst, src, strlen(src));
}

char* strncpy(char* dst, const char* src, size_t n) {
  
  char *address = dst;
  assert((dst!=NULL)&&(src!=NULL));
  if ((dst != src) && (0 < n) ){
	  while( n-- && '\0'!=( *dst++ = *src++ ) ){
		   ;// do nothing
	  }
  }
  *dst='\0';
  return address;
}

char* strcat(char* dst, const char* src) {
  char *result = dst;
	while(*dst){
    dst++;
  } 
	strcpy(dst, src);
	return result;
}

int strcmp(const char* s1, const char* s2) {
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
}

int strncmp(const char* s1, const char* s2, size_t n) {
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
}

void* memset(void* v,int c,size_t n) {
  void* ret = v;
  while(n--){
    *(char*)v = (char)c;
    v = (char*)v + 1; //移动一个字节
  }
  return ret;
}

void* memcpy(void* out, const void* in, size_t n) {
  /*
  const char *s = (char*)in;
  char *d = (char*)out;

  while (n--){
    *d++ = *s++;
  }
  */
  char *s1;
	const char *s2;
	for(s1=out,s2=in;0<n;++s2,--n)
		*s1=*s2;	
  return out;
}

int memcmp(const void* s1, const void* s2, size_t n){
  const unsigned char *str1 = (unsigned char *)s1;
  const unsigned char *str2 = (unsigned char *)s2;
  while (n-- > 0){
    if (*str1++ != *str2++){
      return str1[-1] < str2[-1] ? -1 : 1;
    }
  }
  return 0;
}

#endif
