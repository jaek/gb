#include <iostream>
#include <string>
using namespace std;

class Registers
{
public:
    // 8 * 8 bit registers
    u_int8_t a = 0;
    u_int8_t b = 0;
    u_int8_t c = 0;
    u_int8_t d = 0;
    u_int8_t e = 0;
    u_int8_t f = 0;
    u_int8_t h = 0;
    u_int8_t l = 0;

    // There are four virtual 16 bit registers: af bc de hl
    // af
    u_int16_t get_af(){
        u_int16_t v = a;
        return (v << 8) | f;
    } 

    void set_af(u_int16_t v){
        a = (v & 0xFF00) >> 8;
        f = (v & 0xFF);
    }
    // bc
    u_int16_t get_bc(){
        u_int16_t v = b;
        return (v << 8) | c;
    } 

    void set_bc(u_int16_t v){
        b = (v & 0xFF00) >> 8;
        c = (v & 0xFF);
    }
    //de
    u_int16_t get_de(){
        u_int16_t v = d;
        return (v << 8) | e;
    } 

    void set_de(u_int16_t v){
        d = (v & 0xFF00) >> 8;
        e = (v & 0xFF);
    }
    //hl
    u_int16_t get_hl(){
        u_int16_t v = h;
        return (v << 8) | l;
    } 

    void set_hl(u_int16_t v){
        h = (v & 0xFF00) >> 8;
        l = (v & 0xFF);
    }
};

int main()
{
    Registers r;
    r.b = 10;
    r.c = 5;
    cout << r.get_bc();
    r.set_bc(15);
    cout << r.get_bc();
    return 0;
}