//encoding blob to base64 character array
//s    - input blob (byte array),
//slen - length of s,
//out  - output character array (without terminating character '\0'),
//       output length = int((slen+2)/3)*4
//returns
//      the number of characters actually written to the output array
//      or -1 on error.
int Base64Enc(const unsigned char* s,int slen, unsigned char* out)
{
	const static unsigned char* codesym="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	unsigned int c,len=slen/3;
	if(slen<=0)return-1;
	while(len--)
	{
		c=*s++;c<<=8;c|=*s++;c<<=8;c|=*s++;
		*out++=codesym[(c>>18)&0x3F];
		*out++=codesym[(c>>12)&0x3F];
		*out++=codesym[(c>>6)&0x3F];
		*out++=codesym[(c)&0x3F];
	}
	if(slen%3 == 2)
	{
		c=*s++;c<<=8;c|=*s++;c<<=8;
		*out++=codesym[(c>>18)&0x3F];
		*out++=codesym[(c>>12)&0x3F];
		*out++=codesym[(c>>6)&0x3F];
		*out='=';
	}
	else if(slen%3 == 1)
	{
		c=*s;c<<=16;
		*out++=codesym[(c>>18)&0x3F];
		*out++=codesym[(c>>12)&0x3F];
		*out++='=';
		*out='=';
	}
	return (slen+2)/3*4;
}
//decoding base64 character array to blob (byte array)
//s    - input base64 character array (can include '\r','\n',' '),
//slen - length of s,
//out  - output byte array,
//       output length = s_len_without_spaces/4*3-num_eq,
//         s_len_without_spaces - length s without space characters, divisible by 4,
//         num_eq - the number of tail symbols '=',
//       out may be the same as s (inplace),
//returns
//      the number of characters actually written to the output array
//      or -1 on error.
int Base64Dec(const unsigned char* s,int slen,unsigned char* out,int outlen)
{
	const static unsigned char symdec[] = {
	127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,
	127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,
	127,127,127,127,127,127,127,127,127,127,127, 62,127,127,127, 63,
	 52, 53, 54, 55, 56, 57, 58, 59, 60, 61,127,127,127,  0,127,127,
	127,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
	 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,127,127,127,127,127,
	127, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
	 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,127,127,127,127,127};
	unsigned int c,len=0;unsigned char a,b,a0,a1;
	unsigned char* s_end=(unsigned char*)s+slen;
	if(slen<=0 || outlen<=0)return -1;
	while(1)
	{
		while(s<s_end && (*s=='\r' || *s=='\n' || *s==' '))s++;if(s==s_end)break;
		a=*s++;if(a>127)return-1;b=symdec[a];if(b>63)return-1;c=b;c<<=6;
		while(s<s_end && (*s=='\r' || *s=='\n' || *s==' '))s++;if(s==s_end)return-1;
		a=*s++;if(a>127)return-1;b=symdec[a];if(b>63)return-1;c|=b;c<<=6;
		while(s<s_end && (*s=='\r' || *s=='\n' || *s==' '))s++;if(s==s_end)return-1;
		a=*s++;if(a>127)return-1;b=symdec[a];if(b>63)return-1;c|=b;c<<=6;a0=a;
		while(s<s_end && (*s=='\r' || *s=='\n' || *s==' '))s++;if(s==s_end)return-1;
		a=*s++;if(a>127)return-1;b=symdec[a];if(b>63)return-1;c|=b;a1=a;

		if(a0=='=' && a1!='=')return-1;
		if(a0=='='||a1=='=')
		{
						
                        *out++=c>>16;len++;
			if(a0!='='){*out++=c>>8;len++;}
			while(s<s_end && (*s=='\r' || *s=='\n' || *s==' '))s++;if(s==s_end)break;
			return-1;
		}
		else
		{
			if(len+3>outlen)return-1;			
                        *out++=c>>16;
			*out++=c>>8;
			*out++=c;
			len+=3;
		}
	}
	return len;
}
