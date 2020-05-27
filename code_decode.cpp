#include<vector>
#include<cstdint>
#include<cstdio>
#include<iostream>
#include<string>
class hamming{
	private:
		std::vector<uint8_t> counter;
		std::vector<uint8_t> decode;
		std::vector<uint16_t> code;
		void inline make_two_byte(uint16_t& n, uint8_t& o, uint8_t& r0, uint8_t& r1, uint8_t& r2, uint8_t& r3);
		void inline correct_mistake(uint16_t& tb, uint8_t& s0, uint8_t& s1, uint8_t& s2, uint8_t& s3);
		void inline make_byte(uint16_t& tb, uint8_t& ob);
	public:
		hamming();
		void encode(std::vector<uint8_t> vect);
		void decoding(std::vector<uint16_t> vect);
		std::vector<uint16_t> en_get();
		std::vector<uint8_t> de_get();
};
hamming::hamming(){
	decode.resize(0);
	code.resize(0);
	counter.resize(12);
	for(uint8_t i = 0; i<12; i++)
		counter[i] = i+1;
}
std::vector<uint16_t> hamming::en_get(){
	return code;
}
std::vector<uint8_t> hamming::de_get(){
	return decode;
}
void inline hamming::make_two_byte(uint16_t& n, uint8_t& o, uint8_t& r0, uint8_t& r1, uint8_t& r2, uint8_t& r3){
	n = n | (o>>4);
	n = (n<<1) | r3;
	n = (n<<3) | (((o<<4) & 0xff) >> 5);
	n = (n<<1) | r2;
	n = (n<<1) | (o & 0x1);
	n = (n<<1) | r1;
	n = (n<<1) | r0;
}

void hamming::encode(std::vector<uint8_t> vect){
	decode = vect;
	code.erase(code.begin(), code.end());
	code.resize(decode.size());
	uint8_t f1,f2,f3,f4;
	for(size_t i = 0; i<code.size();i++){
		f1 = 0;
		f2 = 0;
		f3 = 0;
		f4 = 0;
		for(size_t j = 0, s = 0; j<counter.size(); j++){
			if( j!=0 && j!=1 && j!=3 && j!=7){
				f1^=( ((decode[i]>>s) & 0x1) * (counter[j] & 0x1));
				f2^=( ((decode[i]>>s) & 0x1) * ((counter[j] >> 1) & 0x1));
				f3^=( ((decode[i]>>s) & 0x1) * ((counter[j] >> 2) & 0x1));
				f4^=( ((decode[i]>>s) & 0x1) * ((counter[j] >> 3) & 0x1));
				s++;
			}
		}
		make_two_byte(code[i], decode[i], f1, f2, f3, f4);
	}
}
void inline hamming::correct_mistake(uint16_t& tb, uint8_t& s0, uint8_t& s1, uint8_t& s2, uint8_t& s3){
	uint8_t kontrol = (s3<<3) | (s2<<2) | (s1<<1) | s0;
	tb = (((tb>>kontrol) & 0xffff)<<kontrol) | ((!((tb>>(kontrol-1))&0x1))<<(kontrol-1)) | (((tb<<(16-kontrol+1))&0xffff)>>(16-kontrol+1));
}
void inline hamming::make_byte(uint16_t& tb, uint8_t& ob){
	ob = ((tb >> 2)&0x1) | ((((tb>>4)&0xf)<<1)&0xf) | (((tb >> 8) & 0xffff) << 4);
}
void hamming::decoding(std::vector<uint16_t> vect){
	code = vect;
	decode.erase(decode.begin(), decode.end());
	decode.resize(code.size());
	uint8_t s1,s2,s3,s4;
	for(size_t i = 0; i<code.size();i++){
		s1 = 0;
		s2 = 0;
		s3 = 0;
		s4 = 0;
		for(size_t j = 0; j<counter.size(); j++){
			s1^=( ((code[i]>>j) & 0x1) * (counter[j] & 0x1));
			s2^=( ((code[i]>>j) & 0x1) * ((counter[j] >> 1) & 0x1));
			s3^=( ((code[i]>>j) & 0x1) * ((counter[j] >> 2) & 0x1));
			s4^=( ((code[i]>>j) & 0x1) * ((counter[j] >> 3) & 0x1));
		}
		if(s1 != 0 || s2 != 0 || s3 != 0 || s4 != 0)
			correct_mistake(code[i],s1,s2,s3,s4);
		make_byte(code[i],decode[i]);
	}
}
int main(){
	std::vector<uint8_t> kek;
	std::string vou = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
	for(auto i:vou)
		kek.push_back(i);
	hamming kuk;
	kuk.encode(kek);
	auto c = kuk.en_get();
	kuk.decoding(c);
	auto l = kuk.de_get();
	for(auto i:l)
		printf("%c ", i);
	return 0;
}
