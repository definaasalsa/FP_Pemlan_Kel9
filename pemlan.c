#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_MAKANAN 100

typedef struct {
    char nama[50];
    int rating;
} Makanan;

// Fungsi untuk membersihkan layar
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Fungsi untuk menampilkan teks di tengah
void printCentered(const char *text) {
    int terminalWidth = 80;  // Misalkan lebar terminal 80 karakter
    int textLength = strlen(text);
    int padding = (terminalWidth - textLength) / 2;  // Menghitung banyak spasi sebelum teks

    // Menambahkan padding untuk menempatkan teks di tengah
    int i;
    for (i = 0; i < padding; i++) {
        printf(" ");
    }
    printf("%s\n", text);
}

// Fungsi untuk menampilkan daftar makanan dengan perbaikan
void tampilkanMakanan(Makanan makanan[], int jumlah) {
    if (jumlah == 0) {
        printf("\nBelum ada makanan yang dimasukkan.\n");
        return;
    }

    // Deklarasikan variabel di luar loop
    int i;
    int maxNamaLength = 0;

    // Cari panjang nama makanan terpanjang untuk mengatur lebar kolom
    for (i = 0; i < jumlah; i++) {  // Deklarasi i di luar loop
        maxNamaLength = fmax(maxNamaLength, strlen(makanan[i].nama));
    }

    // Set lebar kolom Nama Makanan berdasarkan panjang nama terpanjang
    int lebarNama = (maxNamaLength > 30) ? maxNamaLength : 30; // Minimal 30 karakter

    // Menampilkan tabel dengan garis vertikal yang tepat
    printf("\nDaftar Makanan:\n");
    printf("--------------------------------------------------\n");
    printf("| %-4s | %-*s | %-6s |\n", "No", lebarNama, "Nama Makanan", "Rating");
    printf("---------------------------------------------------\n");

    for (i = 0; i < jumlah; i++) {  // Deklarasi i di luar loop
        printf("| %-4d | %-*s | %-6d |\n", i + 1, lebarNama, makanan[i].nama, makanan[i].rating);
    }

    printf("-------------------------------------------------\n");
}

// Fungsi untuk mengubah string menjadi huruf kecil (lowercase)
void toLowerCase(char str[]) {
	int i;
    for (i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// Fungsi untuk menghapus spasi di awal dan akhir string
void trim(char str[]) {
    int i, start = 0, end = strlen(str) - 1;

    // Hapus spasi di awal
    while (str[start] == ' ') {
        start++;
    }

    // Hapus spasi di akhir
    while (end >= start && str[end] == ' ') {
        end--;
    }

    // Pindahkan string ke posisi yang benar
    for (i = 0; start <= end; i++, start++) {
        str[i] = str[start];
    }
    str[i] = '\0';
}

// Bubble Sort untuk mengurutkan makanan berdasarkan rating
void bubbleSort(Makanan makanan[], int jumlah) {
    int i, j;  // Deklarasikan i, j di luar loop
    for (i = 0; i < jumlah - 1; i++) {
        for (j = 0; j < jumlah - i - 1; j++) {
            if (makanan[j].rating < makanan[j + 1].rating) {
                Makanan temp = makanan[j];
                makanan[j] = makanan[j + 1];
                makanan[j + 1] = temp;
            }
        }
    }
}

// Fungsi untuk melakukan Jump Search berdasarkan nama makanan
int jumpSearch(Makanan makanan[], int jumlah, char target[]) {
    int step = sqrt(jumlah);
    int prev = 0;

    trim(target);
    toLowerCase(target);

    while (prev < jumlah && strcmp(makanan[step - 1].nama, target) < 0) {
        prev = step;
        step += sqrt(jumlah);
        if (prev >= jumlah) {
            return -1;
        }
    }
	 
	 int i;
    for (i = prev; i < fmin(step, jumlah); i++) {
        char tempNama[50];
        strcpy(tempNama, makanan[i].nama);
        trim(tempNama);
        toLowerCase(tempNama);

        if (strcmp(tempNama, target) == 0) {
            return i;
        }
    }
    return -1;
}

// Fungsi untuk menambah makanan
void tambahMakanan(Makanan makanan[], int *jumlah) {
    if (*jumlah >= MAX_MAKANAN) {
        printf("\nDaftar makanan sudah penuh!\n");
        return;
    }

    printf("\nMasukkan nama makanan (gunakan spasi jika perlu): ");
    fgets(makanan[*jumlah].nama, sizeof(makanan[*jumlah].nama), stdin);
    makanan[*jumlah].nama[strcspn(makanan[*jumlah].nama, "\n")] = '\0';
    trim(makanan[*jumlah].nama);
    toLowerCase(makanan[*jumlah].nama);

    do {
        printf("Masukkan rating makanan (1-5): ");
        scanf("%d", &makanan[*jumlah].rating);
        getchar(); // Membersihkan buffer

        if (makanan[*jumlah].rating < 1 || makanan[*jumlah].rating > 5) {
            printf("Rating tidak valid. Harap masukkan rating antara 1 dan 5.\n");
        }
    } while (makanan[*jumlah].rating < 1 || makanan[*jumlah].rating > 5);

    (*jumlah)++;
    printf("\nMakanan berhasil ditambahkan!\n");
}

// Fungsi untuk menghapus makanan
void hapusMakanan(Makanan makanan[], int *jumlah, int index) {
	int i;
    for (i = index; i < *jumlah - 1; i++) {
        makanan[i] = makanan[i + 1];
    }
    (*jumlah)--;

    printf("\nMakanan berhasil dihapus!\n");
}

// Fungsi utama
int main() {
    Makanan makanan[MAX_MAKANAN];
    int jumlah = 0; // Awalnya tidak ada makanan
    char pilihan;

    // Menampilkan pesan selamat datang dengan judul di tengah
    clearScreen();
    printCentered("=== Selamat Datang di Program Pengurutan Makanan Favorit ===");
    printCentered("Program ini memungkinkan Anda untuk menambah, mencari, dan mengurutkan makanan favorit berdasarkan rating.");
    printf("\n===================================================================================================================\n");
    printf("\nTekan Enter untuk melanjutkan...");
    getchar();

    do {
        clearScreen();
        printf("=== Menu ===\n");
        printf("1. Tambahkan Makanan\n");
        printf("2. Cari Makanan\n");
        printf("3. Tampilkan Daftar Makanan\n");
        printf("4. Hapus Makanan\n");
        printf("x. Keluar\n");
        printf("Pilih menu: ");
        scanf(" %c", &pilihan);
        getchar(); // Membersihkan buffer

        switch (pilihan) {
            case '1':
                clearScreen();
                printf("=== Tambah Makanan ===\n");
                tambahMakanan(makanan, &jumlah);
                printf("\nTekan Enter untuk kembali ke menu...");
                getchar();
                break;

            case '2': {
                clearScreen();
                if (jumlah == 0) {
                    printf("Belum ada makanan yang dimasukkan.\n");
                    printf("\nTekan Enter untuk kembali ke menu...");
                    getchar();
                    break;
                }
                char target[50];
                printf("=== Cari Makanan ===\n");
                printf("\nMasukkan nama makanan yang ingin dicari: ");
                fgets(target, sizeof(target), stdin);
                target[strcspn(target, "\n")] = '\0'; // Menghapus newline

                int hasil = jumpSearch(makanan, jumlah, target);
                if (hasil != -1) {
                    printf("Makanan '%s' ditemukan pada urutan ke-%d dengan rating %d.\n",
                           makanan[hasil].nama, hasil + 1, makanan[hasil].rating);
                } else {
                    printf("Makanan '%s' tidak ditemukan dalam daftar.\n", target);
                }
                printf("\nTekan Enter untuk kembali ke menu...");
                getchar();
                break;
            }

            case '3':
                clearScreen();
                printf("=== Daftar Makanan ===\n");
                bubbleSort(makanan, jumlah);
                tampilkanMakanan(makanan, jumlah);
                printf("\nTekan Enter untuk kembali ke menu...");
                getchar();
                break;

            case '4':
                clearScreen();
                if (jumlah == 0) {
                    printf("Belum ada makanan yang dimasukkan.\n");
                    printf("\nTekan Enter untuk kembali ke menu...");
                    getchar();
                    break;
                }

                int index;
                printf("=== Hapus Makanan ===\n");
                tampilkanMakanan(makanan, jumlah);
                printf("Masukkan nomor makanan yang ingin dihapus: ");
                scanf("%d", &index);
                getchar();

                if (index > 0 && index <= jumlah) {
                    hapusMakanan(makanan, &jumlah, index - 1);
                } else {
                    printf("\nNomor makanan tidak valid.\n");
                }
                printf("\nTekan Enter untuk kembali ke menu...");
                getchar();
                break;

            case 'x':
                printf("\nTerima kasih telah menggunakan program ini!\n");
                break;

            default:
                printf("\nPilihan tidak valid. Silakan pilih lagi.\n");
                break;
        }
    } while (pilihan != 'x');

    return 0;
}
