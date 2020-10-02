//
//
// Fixpoint Klasse Version 2.0
//
// Martin Steen 2001-2008

#ifndef CFixpointT_H
#define CFixpointT_H

template<int Tbits>
class CFixpointT
{
#define FP_FAKT			(1<<Tbits)
#define FP_MASK			(FP_FAKT-1)
#define FP_PI			(M_PI * FP_FAKT)
#define FP_0POINT5		(1<<(Tbits-1)) // 0.5


    protected:

        int value;

    public:

        // int Operators
        CFixpointT operator+(const int z)  { CFixpointT<Tbits> v; v.value = value + (z << Tbits); return v; }
        CFixpointT operator+=(const int z) { value += (z << Tbits); return *this; }
        CFixpointT operator-(const int z)  { CFixpointT<Tbits> v; v.value = value -  (z << Tbits); return v; }
        CFixpointT operator-=(const int z) { value -= (z << Tbits); return *this; }
        CFixpointT operator*(const int z)  { CFixpointT<Tbits> v; v.value = value *  z; return v; }
        CFixpointT operator*=(const int z) { value *= z; return *this; }
        CFixpointT operator/(const int z)  { CFixpointT<Tbits> v; v.value = value /  z; return v; }
        CFixpointT operator/=(const int z) { value /= z; return *this; }
        CFixpointT operator<<(const int n)  { CFixpointT<Tbits> v; v.value = value << n; return v; }
        CFixpointT operator>>(const int n)  { CFixpointT<Tbits> v; v.value = value >> n; return v; }
        CFixpointT operator<<=(const int n) { value <<= n; return *this; }
        CFixpointT operator>>=(const int n) { value >>= n; return *this; }

        bool operator==(const int z) { return value == (z << Tbits); }
        bool operator>(const int z)  { return value > (z << Tbits); }
        bool operator<(const int z)  { return value < (z << Tbits); }
        bool operator>=(const int z) { return value >= (z << Tbits); }
        bool operator<=(const int z) { return value <= (z << Tbits); }
        bool operator!=(const int z) { return value != (z << Tbits); }

        // unsigned int Operators
        CFixpointT operator+(const unsigned int z)  { CFixpointT<Tbits> v; v.value = value + (z << Tbits); return v; }
        CFixpointT operator+=(const unsigned int z) { value += (z << Tbits); return *this; }
        CFixpointT operator-(const unsigned int z)  { CFixpointT<Tbits> v; v.value = value -  (z << Tbits); return v; }
        CFixpointT operator-=(const unsigned int z) { value -= (z << Tbits); return *this; }
        CFixpointT operator*(const unsigned int z)  { CFixpointT<Tbits> v; v.value = value *  z; return v; }
        CFixpointT operator*=(const unsigned int z) { value *= z; return *this; }
        CFixpointT operator/(const unsigned int z)  { CFixpointT<Tbits> v; v.value = value /  z; return v; }
        CFixpointT operator/=(const unsigned int z) { value /= z; return *this; }
        CFixpointT operator<<(const unsigned int n)  { CFixpointT<Tbits> v; v.value = value << n; return v; }
        CFixpointT operator>>(const unsigned int n)  { CFixpointT<Tbits> v; v.value = value >> n; return v; }
        CFixpointT operator<<=(const unsigned int n) { value <<= n; return *this; }
        CFixpointT operator>>=(const unsigned int n) { value >>= n; return *this; }

        bool operator==(const unsigned int z) { return value == (z << Tbits); }
        bool operator>(const unsigned int z)  { return value > (z << Tbits); }
        bool operator<(const unsigned int z)  { return value < (z << Tbits); }
        bool operator>=(const unsigned int z) { return value >= (z << Tbits); }
        bool operator<=(const unsigned int z) { return value <= (z << Tbits); }
        bool operator!=(const unsigned int z) { return value != (z << Tbits); }

        // double Operators
        CFixpointT operator+(const double z)  { CFixpointT<Tbits> v; v.value = value + int(z * FP_FAKT); return v; }
        CFixpointT operator+=(const double z) { value += int(z * FP_FAKT); return *this; }
        CFixpointT operator-(const double z)  { CFixpointT<Tbits> v; v.value = value - int(z * FP_FAKT); return v; }
        CFixpointT operator-=(const double z) { value -= int(z * FP_FAKT); return *this; }
        CFixpointT operator*(const double z)  { CFixpointT<Tbits> v; v.value = int(value * z); return v; }
        CFixpointT operator*=(const double z) { value = int(value * z); return *this; }
        CFixpointT operator/(const double z)  { CFixpointT<Tbits> v; v.value = int(value / z); return v; }
        CFixpointT operator/=(const double z) { value = int(value / z); return *this; }

        bool operator==(const double z) { return value == (z * FP_FAKT); }
        bool operator>(const double z)  { return value > (z * FP_FAKT); }
        bool operator<(const double z)  { return value < (z * FP_FAKT); }
        bool operator>=(const double z) { return value >= (z * FP_FAKT); }
        bool operator<=(const double z) { return value <= (z * FP_FAKT); }
        bool operator!=(const double z) { return value != (z * FP_FAKT); }

        // Fixpoint Operators

        CFixpointT operator+(const CFixpointT<Tbits> z)  { CFixpointT<Tbits> v; v.value = value + z.value; return v; }
        CFixpointT& operator+=(const CFixpointT<Tbits> z) { value += z.value; return *this; }
        CFixpointT operator-(const CFixpointT<Tbits> z)  { CFixpointT<Tbits> v; v.value = value - z.value; return v; }
        CFixpointT& operator-=(const CFixpointT<Tbits> z) { value -= z.value; return *this; }
        CFixpointT operator*(const CFixpointT<Tbits> z);
        CFixpointT& operator*=(const CFixpointT<Tbits> z);
        CFixpointT operator/(const CFixpointT<Tbits> z);
        CFixpointT& operator/=(const CFixpointT<Tbits> z);

        CFixpointT operator-()  { CFixpointT v; v.value = -value; return v; }

        bool operator==(const CFixpointT z) const { return value == z.value; }
        bool operator>(const CFixpointT z)  const { return value > z.value; }
        bool operator<(const CFixpointT z)  const { return value < z.value; }
        bool operator>=(const CFixpointT z) const { return value >= z.value; }
        bool operator<=(const CFixpointT z) const { return value <= z.value; }
        bool operator!=(const CFixpointT z) const { return value != z.value; }

        // Cast operators
        operator char() const { return value / Tbits; }
        operator int()  const { return value / FP_FAKT; }
        operator double()  const { return GetDouble(); }
        operator unsigned char() const { return value >> Tbits; }
        operator unsigned int() const { return value >> Tbits; }

        // Get Accessory Methods:


        int Get()       { return value; }
        double GetDouble() const { return ((double)value) / FP_FAKT; }
        double GetDouble(const int fpbits) { return ((double)value) / (1<<fpbits); }
        int GetInt()    { return value / FP_FAKT; }
        int GetIntU()   { return value >> Tbits; }
        int RoundIntU()
        {
            return ((value & FP_MASK) >= FP_0POINT5) ? GetIntU()+1 : GetIntU();
        }


        int RoundInt()
        {
            if (value < 0)
            {
                return ((-value & FP_MASK) >= FP_0POINT5) ? GetInt()-1 : GetInt();
            }
            return ((value & FP_MASK) >= FP_0POINT5) ? GetInt()+1 : GetInt();
        }


        // Set Accessory Methods:

        void Set(CFixpointT z)     { value = z.value; }
        void SetDouble(double z)  { value = int(z * FP_FAKT); }
        void SetInt(int z)        { value = z << Tbits; }
        void SetInt(int z, int d) { value = (z << Tbits) / d; }


        void operator=(const CFixpointT z) { value = z.value; }
        void operator=(const int z)         { value = z << Tbits; }
        void operator=(const double z)      { value = int(z * FP_FAKT); }

        // Konstruktors:

        CFixpointT(const double z) { value = int(z * FP_FAKT); }
        CFixpointT(const int z)    { value = z << Tbits; }
        CFixpointT(const int z, const int d) { value = (z << Tbits) / d; }
        CFixpointT() { }

        // String Konversion:

        void ToString(char* tmp, int fpbits);
        char* ToString(int fpbits = Tbits);

        // Mathematische Funktionen

        CFixpointT Reziprok()
        {
            CFixpointT<Tbits> v = 1;

            return v / *this;
        }


        CFixpointT abs()
        {
            CFixpointT v;

            if (value >= 0) { v.value = value; }
            else{ v.value = -value; }

            return v;
        }


        // Nachkomma-Anteil der Fixpoint-Zahl
        CFixpointT GetFract() const
        {
            CFixpointT<Tbits> v = *this;

            if (v.value < 0)
            {
                v.value = ((-v.value) & FP_MASK);
            }
            else
            {
                v.value &= FP_MASK;
            }
            return v;
        }


        // 1 - Nachkomma-Anteil
        CFixpointT GetFract2() const
        {
            CFixpointT<Tbits> v;

            v.value = FP_FAKT - (value & FP_MASK);
            return v;
        }
};

#include <CFixpointT.hpp>

#endif
