#ifndef mashy_lib_integer64_H_included
#define mashy_lib_integer64_H_included

#if 0
	for testing see
"X:\12d\MACROS\TESTING SNIPPETS\integer64.4dm"
#endif

#define LARGE_POSITIVE 9223372036854775807

//#define BIT33 4294967296

#define BIT1 1
#define BIT2 2
#define BIT3 4
#define BIT4 8
#define BIT5 16
#define BIT6 32
#define BIT7 64
#define BIT8 128
#define BIT9 256
#define BIT10 512
#define BIT11 1024
#define BIT12 2048
#define BIT13 4096
#define BIT14 8192
#define BIT15 16384
#define BIT16 32768
#define BIT17 65536
#define BIT18 131072
#define BIT19 262144
#define BIT20 524288
#define BIT21 1048576
#define BIT22 2097152
#define BIT23 4194304
#define BIT24 8388608
#define BIT25 16777216
#define BIT26 33554432
#define BIT27 67108864
#define BIT28 134217728
#define BIT29 268435456
#define BIT30 536870912
#define BIT31 1073741824
#define BIT32 2147483648LL
#define BIT33 4294967296LL
#define BIT34 8589934592LL
#define BIT35 17179869184LL
#define BIT36 34359738368LL
#define BIT37 68719476736LL
#define BIT38 137438953472LL
#define BIT39 274877906944LL
#define BIT40 549755813888LL
#define BIT41 1099511627776LL
#define BIT42 2199023255552LL
#define BIT43 4398046511104LL
#define BIT44 8796093022208LL
#define BIT45 17592186044416LL
#define BIT46 35184372088832LL
#define BIT47 70368744177664LL
#define BIT48 140737488355328LL
#define BIT49 281474976710656LL
#define BIT50 562949953421312LL
#define BIT51 1125899906842624LL
#define BIT52 2251799813685248LL
#define BIT53 4503599627370496LL
#define BIT54 9007199254740992LL
#define BIT55 18014398509481984LL
#define BIT56 36028797018963968LL
#define BIT57 72057594037927936LL
#define BIT58 144115188075855872LL
#define BIT59 288230376151711744LL
#define BIT60 576460752303423488LL
#define BIT61 1152921504606846976LL
#define BIT62 2305843009213693952LL
#define BIT63 4611686018427387904LL
#define BIT64 -9223372036854775808LL

#define FULL32BITS 0xFFFFFFFF

void set_i64_to_i32i32(Integer64 &i, Integer &x, Integer &y){
	Integer64 result64 = i / BIT33;
	Integer64 remainder64 = i - (result64 * BIT33);
	y = remainder64;
	x = result64;
}

void set_i32i32_to_i64(Integer &x, Integer &y, Integer64 &i){
	Integer64 yy = y;
	Integer64 b=x;
	i = b * BIT33 + yy;
}

void make_bits(){
	Integer64 b = 1;
	for(Integer i=1;i<=64;i++){
		Print("#define BIT"+To_text(i)+" "+To_text(b)+"\n");
		b+=b;
	}
}

Text get_char_i_to_h(Integer c){
	return Get_subtext("0123456789ABCDEF",c+1,c+1);
}

Text i32_to_h(Integer v){
	Text t = "";
	Integer b =1;
	Integer swap = 1;
	for(Integer i=1;i<=32;i+=4){
		if(swap){
			t=" "+t;
			swap=0;
		}else{
			swap=1;
		}
		Integer c=0;
		if(b&v)c+=1;
		b+=b;
		if(b&v)c+=2;
		b+=b;
		if(b&v)c+=4;
		b+=b;
		if(b&v)c+=8;
		b+=b;
		t=get_char_i_to_h(c)+t;
	}
	return t;
}

Text i64_to_h(Integer64 &in){
	Integer a,b;
	set_i64_to_i32i32(in,a,b);
	return (i32_to_h(a)+" "+i32_to_h(b));
}

void test_i64_to_h(){
	Print("\nSTART - Looking at Integer64 in hex"+"\n");
	Integer64 b =1;
	for(Integer i=1;i<=64;i++){
		Text t="";
		if(i<10)t="0";
		Print("["+t+To_text(i)+"] "+i64_to_h(b)+"\n");
		b+=b;
	}
	Print("END - Looking at Integer64 in hex"+"\n\n");
}

Text i32_to_b(Integer v){
	Text t = "";
	Integer b =1;
	for(Integer i=1;i<=32;i++){
		if(!Mod(i-1,4)){
			t=" "+t;
		}
		t = ((b&v)? "1" : "0" )+ t;
		b+=b;

	}
	return t;
}


Text i64_to_b(Integer64 &i){
	Integer64 result64 = i / BIT33;
	Integer result = result64;
	Integer64 remainder64 = i -(result64 * BIT33);
	Integer remainder = remainder64;
	return (i32_to_b(result)+" "+i32_to_b(remainder));
}

void test_i64_to_b(){
	Print("\nSTART - Looking at Integer64 in binary"+"\n");
	Integer64 b =1;
	for(Integer i=1;i<=64;i++){
		Text t="";
		if(i<10)t="0";
		Print("["+t+To_text(i)+"] "+i64_to_b(b)+"\n");
		b+=b;
	}
	Print("END - Looking at Integer64 in binary"+"\n\n");
}


void test_i64_to_b2(){
	Print("\nSTART - Looking at Integer64 in binary"+"\n");
	Integer64 b =BIT32-40;
	for(Integer i=1;i<=50;i++){
		Text t="";
		Print("["+t+To_text(b)+"] "+i64_to_b(b)+"\n");
		Print("["+t+To_text(b)+"] "+i64_to_h(b)+"\n");
		Print("\n");
		b++;
	}
	Print("END - Looking at Integer64 in binary"+"\n\n");
}


void test_bitwise_output(Integer64 &a,Integer64 &b,Integer64 &c){
	Print("\n");
	Print("IN1  : "+i64_to_b(a)+"\n");
	Print("IN2  : "+i64_to_b(b)+"\n");
	Print("OUT: "+i64_to_b(c)+"\n");
//	Print("\n");
//	Print("IN1  : "+i64_to_h(a)+"\n");
//	Print("IN2  : "+i64_to_h(b)+"\n");
//	Print("OUT: "+i64_to_h(c)+"\n");
}

void test_bitwise_output(Integer64 &a, Integer64 &c){
	Print("\n");
	Print("IN1  : "+i64_to_b(a)+"\n");
	Print("OUT: "+i64_to_b(c)+"\n");
//	Print("\n");
//	Print("IN1  : "+i64_to_h(a)+"\n");
//	Print("OUT: "+i64_to_h(c)+"\n");


}

Integer64 bitwise_and(Integer64 &a, Integer64 &b){ // true if a _AND_ b
	Integer a1,a2,b1,b2;
	set_i64_to_i32i32(a,a1,a2);
	set_i64_to_i32i32(b,b1,b2);
	Integer c1 = (a1 & b1);
	Integer c2 = (a2 & b2);
	Integer64 i;
	set_i32i32_to_i64(c1,c2,i);
	return i;
}

void test_bitwise_and(Integer64 a, Integer64 b){
	Integer64 c = bitwise_and(a,b);
	test_bitwise_output(a, b, c);
}

Integer64 bitwise_inclusive_or(Integer64 &a, Integer64 &b){// true if a _OR_ b
	Integer a1,a2,b1,b2;
	set_i64_to_i32i32(a,a1,a2);
	set_i64_to_i32i32(b,b1,b2);
	Integer c1 = (a1 | b1);
	Integer c2 = (a2 | b2);
	Integer64 i;
	set_i32i32_to_i64(c1,c2,i);
	return i;
}

void test_bitwise_inclusive_or(Integer64 a, Integer64 b){
	Integer64 c = bitwise_inclusive_or(a,b);
	test_bitwise_output(a, b, c);
}


Integer64 bitwise_exclusive_or(Integer64 &a, Integer64 &b){ // true for _ONLY_ a OR b
	Integer a1,a2,b1,b2;
	set_i64_to_i32i32(a,a1,a2);
	set_i64_to_i32i32(b,b1,b2);
	Integer c1 = (a1 ^ b1);
	Integer c2 = (a2 ^ b2);
	Integer64 i;
	set_i32i32_to_i64(c1,c2,i);
	return i;
}

void test_bitwise_exclusive_or(Integer64 a, Integer64 b){
	Integer64 c = bitwise_exclusive_or(a,b);
	test_bitwise_output(a, b, c);
}

Integer64 bitwise_unary_complement(Integer64 &a){ // _FLIP_ the bits of a  ////// PROBELMO HERE
	Integer a1,a2;
	set_i64_to_i32i32(a,a1,a2);
//	Integer c1 = (~a1);
//	Integer c2 = (~a2);

	Integer c1 = (a1 ^ (FULL32BITS));
	Integer c2 = (a2 ^ (FULL32BITS));

	Integer64 i;
	set_i32i32_to_i64(c1,c2,i);
	return i;
}

void test_bitwise_unary_complement(Integer64 a){
	Integer64 c = bitwise_unary_complement(a);
	test_bitwise_output(a, c);
}

void test_bitwise(){
	Print("\n[test_bitwise &]"+"\n");
	test_bitwise_and(LARGE_POSITIVE,55);
	test_bitwise_and(BIT55,BIT55);
	test_bitwise_and(1,2);
	test_bitwise_and(BIT56,BIT56+1);
	test_bitwise_and(BIT64,BIT64);	//ISSUES HERE
	Print("\n[test_bitwise_inclusive_or |]"+"\n");
	test_bitwise_inclusive_or(LARGE_POSITIVE,55);
	test_bitwise_inclusive_or(BIT55,BIT55);
	test_bitwise_inclusive_or(1,2);
	test_bitwise_inclusive_or(BIT56,BIT56+1);
	test_bitwise_inclusive_or(BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7|BIT8|BIT9|BIT10|BIT11|BIT12|BIT13|BIT14|BIT15,BIT56+1);
	test_bitwise_inclusive_or(BIT64,BIT56);	//ISSUES HERE
	Print("\n[test_bitwise_exclusive_or |]"+"\n");
	test_bitwise_exclusive_or(LARGE_POSITIVE,55);
	test_bitwise_exclusive_or(BIT55,BIT55);
	test_bitwise_exclusive_or(1,2);
	test_bitwise_exclusive_or(BIT56,BIT56+1);
	test_bitwise_exclusive_or(BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7|BIT8|BIT9|BIT10|BIT11|BIT12|BIT13|BIT14|BIT15,BIT56+1);
	test_bitwise_exclusive_or(BIT64,BIT56);	//ISSUES HERE
	Print("\n[test_bitwise_unary_complement |]"+"\n");
	test_bitwise_unary_complement(LARGE_POSITIVE);
	test_bitwise_unary_complement(0);
	test_bitwise_unary_complement(BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7|BIT8|BIT9|BIT10|BIT11|BIT12|BIT13|BIT14|BIT15);
	test_bitwise_unary_complement(10);

	test_bitwise_unary_complement(BIT64);	//ISSUES HERE... 64th and 32nd bits
}


#endif