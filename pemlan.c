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
    int terminalWidth = 80;  // Asumsi lebar terminal
    int textLength = strlen(text);
    int padding = (terminalWidth - textLength) / 2;

	int i;
    for (i = 0; i < padding; i++) {
        printf(" ");
    }
    printf("%s\n", text);
}

// Fungsi untuk memproses string (trim dan lowercase)
void processString(char str[]) {
    int start = 0, end = strlen(str) - 1;

    while (str[start] == ' ') start++;
    while (end >= start && str[end] == ' ') end--;

    int i;
    for (i = 0; start <= end; i++, start++) {
        str[i] = tolower(str[start]);
    }
    str[i] = '\0';
}

// Fungsi untuk menampilkan daftar makanan
void tampilkanMakanan(Makanan makanan[], int jumlah) {
    if (jumlah == 0) {
        printf("\nBelum ada makanan yang dimasukkan.\n");
        return;
    }

    int lebarNama = 30;
    int i; // Deklarasi di luar loop
    printf("\nDaftar Makanan:\n");
    printf("---------------------------------------------------\n");
    printf("| %-4s | %-*s | %-6s |\n", "No", lebarNama, "Nama Makanan", "Rating");
    printf("---------------------------------------------------\n");

    for (i = 0; i < jumlah; i++) {
        printf("| %-4d | %-*s | %-6d |\n", i + 1, lebarNama, makanan[i].nama, makanan[i].rating);
    }

    printf("---------------------------------------------------\n");
}

// Fungsi untuk mengurutkan makanan
void bubbleSort(Makanan makanan[], int jumlah) {
    int i, j; // Deklarasi di luar loop
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

// Fungsi untuk mencari makanan
int jumpSearch(Makanan makanan[], int jumlah, char target[]) {
    processString(target);
    int step = (int)sqrt(jumlah); // Pastikan step adalah integer
    int prev = 0, i;
    char tempNama[50];

    // Loop untuk melompati blok
    while (prev < jumlah && strcmp(makanan[(step < jumlah ? step : jumlah) - 1].nama, target) < 0) {
        prev = step;
        step += (int)sqrt(jumlah); // Update langkah berikutnya
        if (prev >= jumlah) return -1;
    }

    // Linear search di blok yang relevan
    for (i = prev; i < (step < jumlah ? step : jumlah); i++) {
        strcpy(tempNama, makanan[i].nama);
        processString(tempNama);

        if (strcmp(tempNama, target) == 0) return i;
    }

    return -1; // Tidak ditemukan
}


// Fungsi untuk menambah makanan
void tambahMakanan(Makanan makanan[], int *jumlah) {
    if (*jumlah >= MAX_MAKANAN) {
        printf("\nDaftar makanan sudah penuh!\n");
        return;
    }

    printf("\nMasukkan nama makanan: ");
    fgets(makanan[*jumlah].nama, sizeof(makanan[*jumlah].nama), stdin);
    makanan[*jumlah].nama[strcspn(makanan[*jumlah].nama, "\n")] = '\0';
    processString(makanan[*jumlah].nama);

    if (strlen(makanan[*jumlah].nama) == 0) {
        printf("Nama makanan tidak boleh kosong.\n");
        return;
    }

    do {
        printf("Masukkan rating makanan (1-5): ");
        scanf("%d", &makanan[*jumlah].rating);
        getchar();
        if (makanan[*jumlah].rating < 1 || makanan[*jumlah].rating > 5) {
            printf("Rating tidak valid. Masukkan angka 1-5.\n");
        }
    } while (makanan[*jumlah].rating < 1 || makanan[*jumlah].rating > 5);

    (*jumlah)++;
    printf("\nMakanan berhasil ditambahkan!\n");
}

// Fungsi untuk menghapus makanan
void hapusMakanan(Makanan makanan[], int *jumlah, int index) {
    if (index < 0 || index >= *jumlah) {
        printf("Nomor makanan tidak valid.\n");
        return;
    }

    int i; // Deklarasi di luar loop
    for (i = index; i < *jumlah - 1; i++) {
        makanan[i] = makanan[i + 1];
    }

    (*jumlah)--;
    printf("\nMakanan berhasil dihapus!\n");
}

// Fungsi utama
int main() {
    Makanan makanan[MAX_MAKANAN];
    int jumlah = 0;
    char pilihan;
    int index;

    clearScreen();
    printCentered("=== Selamat Datang di Program Pengurutan Makanan ===");
    printf("\nTekan Enter untuk melanjutkan...");
    getchar();

    do {
        clearScreen();
        printf("=== Menu ===\n");
        printf("1. Tambahkan Makanan\n");
        printf("2. Cari Makanan\n");
        printf("3. Tampilkan Daftar Makanan\n");
        printf("4. Hapus Makanan\n");
        printf("5. Keluar\n");
        printf("Pilih menu: ");
        scanf(" %c", &pilihan);
        getchar();

        switch (pilihan) {
            case '1':
                tambahMakanan(makanan, &jumlah);
                break;
            case '2': {
                if (jumlah == 0) {
                    printf("Belum ada makanan yang dimasukkan.\n");
                    break;
                }
                char target[50];
                printf("Masukkan nama makanan: ");
                fgets(target, sizeof(target), stdin);
                target[strcspn(target, "\n")] = '\0';
                int hasil = jumpSearch(makanan, jumlah, target);
                if (hasil != -1) {
                    printf("Makanan '%s' ditemukan di urutan ke-%d dengan rating %d.\n",
                           makanan[hasil].nama, hasil + 1, makanan[hasil].rating);
                } else {
                    printf("Makanan '%s' tidak ditemukan.\n");
                }
                break;
            }
            case '3':
                bubbleSort(makanan, jumlah);
                tampilkanMakanan(makanan, jumlah);
                break;
            case '4':
                tampilkanMakanan(makanan, jumlah);
                printf("Masukkan nomor makanan yang ingin dihapus: ");
                scanf("%d", &index);
                getchar();
                hapusMakanan(makanan, &jumlah, index - 1);
                break;
            case '5':
                printf("Terima kasih telah menggunakan program ini.\n");
                break;
            default:
                printf("Pilihan tidak valid. Coba lagi.\n");
                break;
        }
        printf("\nTekan Enter untuk melanjutkan...");
        getchar();
    } while (pilihan != '5');

    return 0;
}
