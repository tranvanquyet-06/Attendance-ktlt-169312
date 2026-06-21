#ifndef ATTENDANCE_H
#define ATTENDANCE_H

#include <string>
using namespace std;

const int CO_MAT = 0;
const int VANG_CO_PHEP = 1;
const int VANG_KHONG_PHEP = 2;

struct DiemDanh {
    string maLop;
    string maSV;
    string ngay;
    int trangThai;
};

inline string trangThaiToString(int tt) {
    switch (tt) {
        case CO_MAT:          return "Co mat";
        case VANG_CO_PHEP:    return "Vang co phep";
        case VANG_KHONG_PHEP: return "Vang khong phep";
        default:              return "Khong xac dinh";
    }
}

inline string trangThaiVietTat(int tt) {
    switch (tt) {
        case CO_MAT:          return "CM";
        case VANG_CO_PHEP:    return "VCP";
        case VANG_KHONG_PHEP: return "VKP";
        default:              return "???";
    }
}

#endif
