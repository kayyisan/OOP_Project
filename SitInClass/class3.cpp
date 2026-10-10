//enkapsullasi = mau melindungi data yang menjaganya agar tidak bisa diubah dan melenceng dari ppengaturan yang ada (ex : ga boleh minus)
//friend function = akses data private dari external class (agak melanggar peraturan harus hati hati) ex : minjemin tapping ke temen fkg


#include<iostream>
#include<string>
using namespace std;

class AkunBank{
    private:
        string norek;
        double saldo;
        string nama;
        bool terblokir;

        //helper function
        bool isJumlahPositive(double jumlah){
            if(jumlah<100){
                cout << "negatif bro";
                return true;
            }
            return false;
        }
        bool isSaldoCukup(double jumlah){
            if(jumlah>saldo){
                cout << "kurang duitnya";
                return true;
            }
            return false;
        }

    public:
        AkunBank (string r, double s, double n, ) :
            norek(r), saldo(s), nama(n), terblokir(false){
                cout << "selamat! anda telah berhasil membuat akun baru!!!\n";
            }

        void setNama (string n){
            nama = n;
        }

        //kita membatasi agar saldo ga bisa melanggar aturan jadi minus => enkapsulasi agar akses tetap valid (melindugi)
        void setSaldo(double s){
            if(s<0) {
                cout << "HEEE GABOLEH SALDO 0";
                return;
            }
            saldo = s;
        }

        public tarikTunai (double jumlah){
            if (!isJumlahPositive(jumlah)) return;
            if (!isSaldoCukup(jumlah)) return;
            if (jumlah < 0){
                cout << "saldo tidak mencukupi";
                return;
            }
            cout << "saldo awal : " << saldo;
            saldo-=jumlah;
            cout << ", saldo setelah ditarik : " << saldo;
        }

        void setorTunai (double jumlah){
            saldo += jumlah;
        }

        void transfer(AkunBank& akunB, double jumlah){
            if (jumlah < 0){
                cout << "saldo tidak mencukupi";
                return;
            }

            saldo -= jumlah;
            akunB.saldo += jumlah;
        }

        //friend function
        friend void bandingkanSaldo (AkunBank& A, AkunBank& B);
        friend ostream& operator<<(ostream& out, const AkunBank& akun);
        friend bank; //bisa memberi akses ke 1 class (bisa liat + ganti data), kalo tadi itu ke 1 fungsi (ambil data doang)
};

class bank{
    string nama;

    public : 
        void blokirAkun(AkunBank& akun){ //terblokir itu class private yang ada di AkunBank
            akun.terblokir = true;

        }
}

void bandingkanSaldo (AkunBank& A, AkunBank& B){
    if(A.saldo > B.saldo){
        cout << "Rekening " << A.nama << "lebih kaya dibanding " << B.nama;
    }
    else {
        cout << "Rekening " << B.nama << "lebih kaya dibanding " << A.nama;
    }
}

    ostream operator<<(ostream& out, const AkunBank& akun){
        out << "Nama pemilik rekening : " << akun.nama << endl;
        out << " saldonya adalah sebesar " << akun.saldo << "dollar \n";
        return  
    }

int main(){
    AkunBank akunKu("1234567890", 10000000, "Kayyisa Nur lathifa");
    cout << "Nama pemilik akun pertama adalah" << akunKu.getNama() << endl;
    AkunBank AkunMu("3826432438", 300000, "Regina Titian Pinasti");
    akunKu.transfer(akunMu, 1000);
    akunKu.tarikTunai(167);
    akunKu.setSaldo(-1000);

    bandingkanSaldo(akunKu, akunMu);
    cout << "Info rekening" << akunKu.getNama() << endl; 
}