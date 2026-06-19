#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>
#include "DynamicArray.h"
#include "Student.h"
#include "ClassInfo.h"
#include "Attendance.h"

using namespace std;

class FileManager {
 public:
   static void docLopHoc(const string& filename, DynamicArray<LopHoc>& ds);
  static void ghiLopHoc(const string& filename, const DynamicArray<LopHoc>& ds);

 static void docSinhVien(const string& filename, DynamicArray<SinhVien>& ds);
    static void ghiSinhVien(const string& filename, const DynamicArray<SinhVien>& ds);

 static void docDiemDanh(const string& filename, DynamicArray<DiemDanh>& ds);
 static void ghiDiemDanh(const string& filename, const DynamicArray<DiemDanh>& ds);

   static DynamicArray<string> split(const string& s, char delimiter);
 static string trim(const string& s);
};

#endif
