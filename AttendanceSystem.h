#ifndef ATTENDANCE_SYSTEM_H
#define ATTENDANCE_SYSTEM_H

#include <string>
using namespace std;

#include "Attendance.h"
#include "ClassInfo.h"
#include "DynamicArray.h"
#include "FileManager.h"
#include "SortSearch.h"
#include "Student.h"

class HeThongDiemDanh {
private:
  DynamicArray<LopHoc> dsLopHoc;
  DynamicArray<SinhVien> dsSinhVien;
  DynamicArray<DiemDanh> dsDiemDanh;

  string fileLopHoc;
  string fileSinhVien;
  string fileDiemDanh;

  int timLopHoc(const string &maLop);
  int timSinhVien(const string &maSV);
  DynamicArray<int> laySVTheoLop(const string &maLop);
  DynamicArray<string> layNgayDiemDanh(const string &maLop);
  bool daTonTai(const DynamicArray<string> &ds, const string &val);
  void luuTatCa();
  void docTatCa();

  void veDuongKe(int doDai);
  void hienThiTieuDe(const string &tieuDe);

public:
  HeThongDiemDanh();

  void menuQuanLyLop();
  void menuQuanLySV();
  void menuDiemDanh();
  void menuTimKiem();
  void menuBaoCao();

  void themLopHoc();
  void suaLopHoc();
  void xoaLopHoc();
  void hienThiDanhSachLop();

  void themSinhVien();
  void suaSinhVien();
  void xoaSinhVien();
  void hienThiDanhSachSV();

  void diemDanhTheoLop();
  void suaDiemDanh();

  void timKiemTheoNgay();
  void timKiemTheoMaSV();

  void tinhTyLeVang();
  void canhBaoCamThi();
  void thongKeSiSo();
  void danhSachVangNhieuNhat();
};

#endif
