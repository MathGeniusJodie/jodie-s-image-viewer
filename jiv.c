/* usage
clang jiv.c -Os -o jiv_binary -nostartfiles -ffreestanding -nostdlib -fno-stack-protector -static -Wl,-n -fdata-sections -ffunction-sections -Wl,-gc-sections -s; ./jiv ~/Desktop/firefox.png 16
*/


__attribute__((naked)) void _start(){
	__asm__(
		"xorq %rbp,%rbp\n"    // zero
		"movq 0(%rsp),%rdi\n" // argc
		"leaq 8(%rsp),%rsi\n" // argv
		"lea 16(%rsp,%rdi,8), %rdx\n" // envp
		"andq $-16, %rsp\n"   // align to 16 bytes
		"call main\n"
		"movq %rax,%rdi\n"
		"movq $60,%rax\n"     // exit
		"syscall"
	);
}

#define size_t long unsigned int

size_t read(int fd, void* buf, size_t size) {
  	size_t result;
  	__asm__ volatile(
  		"syscall" :
  		"=a"(result) :
  		"0"(0), "D"(fd), "S"(buf), "d"(size) :
  		"rcx", "r11", "memory"
	);
  	return result;
}
size_t write(int fd, void* buf, size_t size) {
  	size_t result;
  	__asm__ volatile(
  		"syscall" :
  		"=a"(result) :
  		"0"(1), "D"(fd), "S"(buf), "d"(size) :
  		"rcx", "r11", "memory"
	);
  	return result;
}

void * memset(void *dest, int c, size_t n) {
	__asm__ volatile (
    	"rep stosb"
    	: "+D" (dest), "+c" (n) : "a" (c)
    	: "memory");
	return dest;
}

static char * strpcpy(char *restrict d, const char *restrict s){
	for (; (*d=*s); s++, d++);
	return d;
}

#define uint unsigned int 
#define uchar unsigned char

static size_t char_to_string(char * buffer,uchar i) {
	buffer[2] = (char)('0' + i % 10);
	i /= 10;
	buffer[1] = (char)('0' + i % 10);
	i /= 10;
	buffer[0] = (char)('0' + i);
	return 3;
}

typedef struct {
	uchar r, g, b, a;
}color;

static size_t color_to_string(char* a, color c){
	char_to_string(a,c.r);
	a[3] = ';';
	char_to_string(a+4,c.g);
	a[7] = ';';
	char_to_string(a+8,c.b);
	return 11;
}

static uint stringToUint(const char *str){
	uint r = 0;
	while(*str){
		r = r * 10u + (uint)(*str - '0');
		str += 1;
	}
	return r;
}

int main(int argc, char **argv){

	if(argc<=1) {
		write(1,"ERROR must enter the width of the image\n",40);
		return 0;
	}
	uint width = stringToUint(argv[1]);
	color byteLine[width*2];
	char terminalLine[45*width+1];
	
	while(1){
		memset(byteLine,0,sizeof(byteLine));
		size_t length = read(0,byteLine,sizeof(byteLine));
		if(!length) break;
		char* p = terminalLine;
		for (uint x=0;x<width;x+=1) {
			enum {none,top,bottom,both} state = (byteLine[x].a>128) | ((byteLine[width+x].a>128)*2);
			switch (state){
			    case none:
			        p = strpcpy(p," ");
			        break;
			    case top:
			        p = strpcpy(p,"\x1b[38;2;");
			        p += color_to_string(p,byteLine[x]);
			        p = strpcpy(p,"m▀\x1b[0m");
			        break;
			    case bottom:
			        p = strpcpy(p,"\x1b[38;2;");
			        p += color_to_string(p,byteLine[width+x]);
			        p = strpcpy(p,"m▄\x1b[0m");
			        break;
			    case both:
			        p = strpcpy(p,"\x1b[48;2;");
			        p += color_to_string(p,byteLine[width+x]);
			        p = strpcpy(p,"m\x1b[38;2;");
			        p += color_to_string(p,byteLine[x]);
			        p = strpcpy(p,"m▀\x1b[0m");
			}
		}
		p = strpcpy(p,"\n");
		write(1,terminalLine,p-terminalLine);
	}
	return 0;
}
