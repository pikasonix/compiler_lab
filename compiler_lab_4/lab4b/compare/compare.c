#include <stdio.h>

int main() {
    char out_path[64], exp_path[64];

    for (int i = 1; i <= 4; i++) {
        sprintf(out_path, "../output/example%d", i);
        sprintf(exp_path, "../tests/example%d", i);

        FILE *f1 = fopen(out_path, "rb");
        FILE *f2 = fopen(exp_path, "rb");

        if (!f1 || !f2) {
            printf("Test %d: Error opening files\n", i);
            if (f1) fclose(f1); if (f2) fclose(f2);
            continue;
        }

        int diff = 0, byte_count = 0;
        while (1) {
            int b1 = fgetc(f1);
            int b2 = fgetc(f2);
            
            if (b1 != b2) {
                diff = 1;
                break;
            }
            if (b1 == EOF) break; // Cả hai cùng kết thúc
            byte_count++;
        }

        if (diff) {
            printf("Test %d: Files differ\n", i);
        } else {
            printf("Test %d: Files are identical (%d bytes)\n", i, byte_count);
        }

        fclose(f1);
        fclose(f2);
    }

    return 0;
}
