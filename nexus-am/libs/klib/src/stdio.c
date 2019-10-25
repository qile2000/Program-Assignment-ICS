#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  return 0;
}

int sprintf(char *out, const char *fmt, ...) {
  char *traverse; 
  int i; 
	char *s;
   
//Initializing sprintf's arguments 
  va_list arg; 
	va_start(arg, fmt);
 	for(traverse = fmt; *traverse != '\0'; traverse++) { 
  	while( *traverse != '%' ) { 
  		*out = *traverse;				//putchar(*traverse);
			if(*traverse == '\0'){break;}
			out++;
  		traverse++; 
  	} 
    //out++;

  	if(*traverse == '\0'){break;} 
    traverse++;

  	//Module 2: Fetching and executing arguments
		switch(*traverse) { 
          /*
    			  case 'c' : i = va_arg(arg,int);    //Fetch char argument
        			    *out = i;			     //putchar(i);  if writing printf
        			    break; 
           */ 
    			  case 'd' : i = va_arg(arg,int);     //Fetch Decimal/Integer argument
        			    if(i<0) { 
        			      i = -i;
        			      *out = '-';	      //putchar('-'); if writing printf
				            out++;
        			    } 
        			    s = convert(i,10);	      //puts(convert(i,10));if writing printf
				          while(*s != '\0'){ 
					          *out = *s; s++; out++; 
				          }
				    //*buf = '\0';
        			    break; 
            /*
      			case 'o': i = va_arg(arg,unsigned int); //Fetch Octal representation
     				  s = convert(i,8);		//puts(convert(i,8));if writing printf
				      while(*s != '\0'){ 
					      *out = *s; s++; out++; 
				      }
				    //*buf = '\0';
        		  break; 
            */
			      case 's': s = va_arg(arg,char *);     //Fetch string
        			    			      //puts(s); if writing printf
				          while(*s != '\0'){ 
					          *out = *s; s++;out++; 
				          }
				    //*buf = '\0';
        			    break; 
            /*
		        case 'x': i = va_arg(arg,unsigned int); //Fetch Hexadecimal representation
        			  s = convert(i,16);		//  puts(convert(i,16));if writing printf
				        while(*s != '\0'){ 
					        *out = *s; s++; out++; 
				        }
				    //*buf = '\0';
            		break;
            */
			      default: break; 
    		}  
  } 
  *out = '\0';
  	//Closing argument list to necessary clean-up
	va_end(arg); 
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif
