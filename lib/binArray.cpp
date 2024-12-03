#include "binArray.hpp"
#include <iostream>
#include <algorithm>
#include <vector>
unsigned long long const ed = (unsigned long long)-1; 
std::string BitArray::to_bin (unsigned long long n) const{
    std::string buffer;
    for (int i = 0; i < 64; i++, n /= 2)
        buffer = char('0' + n%2) + buffer;
    std::reverse(buffer.begin(), buffer.end());
    return buffer;        
}

BitArray::BitArray():size(0), last(-1), array({}){}//
BitArray::~BitArray(){
    this->array.erase(this->array.begin(), this->array.end());
}//
BitArray::BitArray(int num_bits, unsigned long long value):size(num_bits/64 + (num_bits%64 > 0 ? 1 : 0)), last(num_bits - 1) {
    if (num_bits == 0) {
        BitArray res;
        *this = res;
        return;
    }
    for(int i = 0; i < this->size - 1; i++){
        this->array.push_back(value);
    }
    this->array.push_back(value & ((unsigned long long) -1 >> (63 - this->last%64))); 
}//

BitArray::BitArray(const BitArray& b){*this = b;}//

void BitArray::swap(BitArray& b){
    BitArray c;
    c = b;
    b = *this;
    *this = c;
}//

BitArray& BitArray::operator=(const BitArray& b){
    this->size = b.size;
    this->last = b.last;
    this->array.resize(this->size);
    std::copy(b.array.begin(), b.array.end(), this->array.begin());
    return *this;
}//

void BitArray::resize(int num_bits, bool value){
    if((this->last == -1) || (num_bits == 0)){
        BitArray a (num_bits, (value ? (unsigned long long) -1 : 0));
        *this = a;
        return;
    }
    int new_size = (num_bits/64 + (num_bits%64 > 0 ? 1 : 0)); 
    int new_last = (num_bits - 1);
    this->array.resize(std::min(new_size, this->size));
    unsigned long long new_start = ed >> 63 - new_last%64;
    unsigned long long old_end = (ed << this->last%64 << 1);
    unsigned long long Uval = value ? ed : 0;
    if (this->size > new_size){
        this->array[new_size - 1] &= new_start;
    }
    if (this->size <= new_size){
        if (this->size > 0)
            this->array[this->size - 1] |= Uval & old_end;
        if (this->size < new_size)
            this->array.resize(new_size, Uval);
        this->array[new_size-1] = this->array[new_size-1] & new_start;
    }
    this->size = new_size;
    this->last = new_last;
}//
//Очищает массив.
void BitArray::clear(){
    this->size = 0;
    this->last = -1;
    this->array.clear();
}//
//Добавляет новый бит в конец массива. В случае необходимости 
//происходит перераспределение памяти.
void BitArray::push_back(bool bit){
    this->last += 1;
    int bias = this->last % 64;
    if (bias == 0)
        this->resize(this->last + 1);
    this->set(this->last, bit);
}//


//Битовые операции над массивами.
//Работают только на массивах одинакового размера.
//Обоснование реакции на параметр неверного размера входит в задачу.
void testing (const BitArray a, const BitArray b) {
    if (a.get_last() != b.get_last()){
        std::cerr << "Operation fault, size of BitArray are defferent\nCheck get_last()\n";
        exit (-1);
    }
}
BitArray& BitArray::operator&=(const BitArray& b){
    testing(*this, b);
    for (int i = 0; i < this->size; i++)
        this->array[i] &= b.array[i];
    return *this;
}//
BitArray& BitArray::operator|=(const BitArray& b){
    testing(*this, b);
    for (int i = 0; i < this->size; i++)
        this->array[i] |= b.array[i];
    return *this;
}//
BitArray& BitArray::operator^=(const BitArray& b){
    testing(*this, b);
    for (int i = 0; i < this->size; i++)
        this->array[i] ^= b.array[i];
    return *this;
}//

//Битовый сдвиг с заполнением нулями.
BitArray& BitArray::operator<<=(int n){
    int window = n / 64;
    int shift = n % 64;
    for (int i = 0; i < this->size - window - 1; i++)
        this->array[i] = (this->array[i + window] >> shift) | (this->array[i + window + 1] << (64 - shift));
    this->array[this->size - window - 1] = this->array[this->size - 1] >> shift;
    for(int i = this->size - window; i < this->size; i++){
        this->array[i] = 0;
    }
    return *this;
}//
BitArray& BitArray::operator>>=(int n){
    int window = n / 64;
    int shift = n % 64;
    for (int i = this->size-1; i >= window + 1; i--){
        this->array[i] = (this->array[i - window] << shift) | (this->array[i - window - 1] >> (64 - shift));
    }
    this->array[window] = this->array[window] << shift;
    for (int i = window-1; i >= 0; i--){
        this->array[i] = 0;
    }
    return *this;
}//
BitArray BitArray::operator<<(int n) const{
    BitArray a(*this);
    a <<= n;
    return a;
}//
BitArray BitArray::operator>>(int n) const{
    BitArray a(*this);
    a >>= n;
    return a;
}//

//0000000000000000000000000000011111111111111111111111111111111000
//Устанавливает бит с индексом n в значение val.
BitArray& BitArray::set(int n, bool val){
    if (n > this->last){
        std::cerr << "Size Error set(): last element < element in function set !\n";
        exit(-1);
    }
    int place = n / 64;
    n %= 64;
    unsigned long long a = (unsigned long long)1 << n;
    this->array[place] = val ? this->array[place] | a : this->array[place] & (ed ^ a); 
    return *this;
}//
//Заполняет массив истиной.
BitArray& BitArray::set(){
    for (int i = 0; i < this->size; i++)
        this->array[i] = ed;
    this->array[this->size - 1] = ed ^ (ed << this->last%64 << 1);
    return *this;
}//

//Устанавливает бит с индексом n в значение false.
BitArray& BitArray::reset(int n){
    this->set(n, false);
    return *this;
}//
//Заполняет массив ложью.
BitArray& BitArray::reset(){
    for (int i = 0; i < this->size; i++)
        this->array[i] = 0;
    return *this;
}//

//true, если массив содержит истинный бит.
bool BitArray::any() const{
    for (int i = 0; i < this->size; i++)
        if ((ed & this->array[i]) > 0)
            return true;
    return false;
}//
//true, если все биты массива ложПравила выставления оценкины.
bool BitArray::none() const{return true ^ this->any();}//
//Битовая инверсия
BitArray BitArray::operator~() const{
    BitArray a(*this);
    for(int i = 0; i < this->size; i++)
        a.array[i] ^= ed;
    return a;
}//
//Подсчитывает количество единичных бит.
int BitArray::count() const{
    int count = 0;
    for (int i = 0; i < this->size; i++)
        for (unsigned long long q = (unsigned long long)1 << 63; q > 0; q = q >> 1)
            count += ((q & this->array[i]) > 0 ? 1 : 0);
    return count;
}//

//Возвращает значение бита по индексу i.
bool BitArray::operator[](int i) const{return (this->array[i / 64] & ((unsigned long long) 1 << i%64)) > 0 ? 1 : 0;}//

int BitArray::get_size() const{return this->size;}
int BitArray::get_last() const{return this->last;}
unsigned long long BitArray::get_block(int i) const{return this->array[i];}
bool BitArray::empty() const{return this->size == 0 ? 1 : 0;}

std::string BitArray::to_string()const{
    std::string buffer;
    for(int i = 0; i < this->size; i++){
        std::string intermediate = to_bin(this->array[i]);
        buffer += intermediate + (i + 1 == this->size ? "" : "\n");
    }
    return buffer;
}//

bool operator==(const BitArray & a, const BitArray & b){
    if (a.get_last() != b.get_last())
        return false;
    for(int i = 0; i < a.get_size(); i++)
        if(a.get_block(i) != b.get_block(i))
            return false;
    return true;
}//
bool operator!=(const BitArray & a, const BitArray & b){return a == b ? false : true;}

BitArray operator&(const BitArray& b1, const BitArray& b2){
    BitArray a(b1);
    a &= b2;
    return a;
}
BitArray operator|(const BitArray& b1, const BitArray& b2){
    BitArray a(b1);
    a |= b2;
    return a;
}
BitArray operator^(const BitArray& b1, const BitArray& b2){
    BitArray a(b1);
    a ^= b2;
    return a;
}