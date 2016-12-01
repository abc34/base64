//encoding blob to base64 string
//s    - input blob (byte array)
//slen - length s
//out  - output char array,
//       out length = int((slen+2)/3)*4 (without '\0' termination)
int Base64Enc(const unsigned char* s,int slen, unsigned char* out)
{
	const static unsigned char* codesym="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	unsigned int c,len=slen/3;
	if(slen<=0)return -1;
	while(len--)
	{
		c=*s++;c<<=8;c|=*s++;c<<=8;c|=*s++;*out++=codesym[(c>>18)&0x3F];*out++=codesym[(c>>12)&0x3F];*out++=codesym[(c>>6)&0x3F];*out++=codesym[(c)&0x3F];
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
	return 0;
}
//decoding base64 string to blob (byte array)
//s    - input base64 string
//slen - length s, divisible by 4
//out  - output byte array, out length = int(slen/4)*3,
//       out may be the same as s (inplace)
int Base64Dec(const unsigned char* s,int slen,unsigned char* out)
{
	const static unsigned char symdec[] = {
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,-1,-1,-1,63,
	52,53,54,55,56,57,58,59,60,61,-1,-1,-1,-1,-1,-1,
	-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,
	15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,
	-1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,
	41,42,43,44,45,46,47,48,49,50,51,-1,-1,-1,-1,-1};
	unsigned int c,len=slen/4-1;unsigned char a,b=0;
	if(slen<4 || slen&3)return -1;
	while(len--)
	{
		a=symdec[*s++];b|=a; c=a;c<<=6;
		a=symdec[*s++];b|=a;c|=a;c<<=6;
		a=symdec[*s++];b|=a;c|=a;c<<=6;
		a=symdec[*s++];b|=a;c|=a;
		if(b>63)return -1;
		*out++=c>>16;*out++=c>>8;*out++=c;
	}
	a=symdec[*s++];b|=a;c=a;c<<=6;
	a=symdec[*s++];b|=a;c|=a;
	c<<=6;if(s[0]!='='){a=symdec[s[0]];b|=a;c|=a;}
	c<<=6;if(s[1]!='='){a=symdec[s[1]];b|=a;c|=a;}
	if(b>63)return -1;
	*out++=c>>16;
	if(s[0]!='=')*out++=c>>8;
	if(s[1]!='=')*out=c;
	return 0;
}
