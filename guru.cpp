#include <iostream>
#include <string>
#include <iomanip>
#include <cctype>
using namespace std;

class Guru{
    private:
        string nip;
        
        bool validasiNIP(){
            if(nip == "Belum Terdaftar") return false;
            else if (nip.length()!=18) return false;
            for (int i=0 ; i<nip.length() ; i++){
                if(!isdigit(nip[i])) return false;
            }
            return true;
        }
    protected:
        string nama;
    public :
        string namaSekolah;

        Guru (){
            nip = "Belum Terdaftar";
            nama = "Belum Terdaftar";
            namaSekolah = "Belum Terdaftar";
        }

        Guru(string a, string b, string c)
        : nama(a), nip(b), namaSekolah(c) {}

        void tampilkanProfil(){
            cout << left << setw(15) << "Nama" << ": " << nama << endl
                 << left << setw(15) << "NIP" << ": " << nip << endl
                 << left << setw(15) << "Nama Sekolah" << ": " << namaSekolah << endl
                 << "\nKETERANGAN NIP (Valid/Tidak) : ";
                 if(validasiNIP()==true) cout << "Valid";
                 else cout << "Tidak Valid";
        }
};

int main(){
    string x, y, z;
    cout << "Masukkan nama lengkap   : ";
    getline(cin, x);
    cout << "Masukkan NIP (18 digit) : ";
    getline(cin, y);
    cout << "Masukkan Nama Sekolah   : ";
    getline(cin, z);
    
    Guru guru1(x, y, z);
    cout << "\nDATA INPUT\n";
    guru1.tampilkanProfil();
    
    return 0;
}