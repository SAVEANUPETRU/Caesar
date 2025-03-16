#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void read_distribution(const char *filename, double distribution[26]) {
    FILE* file = fopen(filename, "r");
    int i =0;
    char myString[100];
    if (file != NULL) {
        while (fgets(myString, 100, file)) {
            float letter = atof(myString);
            distribution[i] = letter;
            i++;
        }
        fclose(file);
    }
}
void compute_histogram(const char *text, double histogram[26]) {
    int i =0;
    int letter = 0;
    for (int k = 0; k < 26; k++) {
        histogram[i] = 0.0;
    }
    while (text[i] != '\0') {
        int poz = (int)text[i];
        if (poz >96  && poz <123) {
            poz = poz - 97;
            histogram[poz] +=1.0;
            letter++;
        }
        else if(64<poz && poz <91) {
            poz = poz - 65;
            histogram[poz] +=1.0;
            letter++;
        }
        i++;
    }
    for (int j = 0; j < 26; j++) {
        histogram[j] = (histogram[j]/letter)*100.0;
    }

}
double chi_squared_distance(const double hist1[26],const double hist2[26]) {
    double distance = 0.0;
    for (int i = 0; i < 26; i++) {
        distance += (pow(hist1[i] - hist2[i], 2))/hist2[i];
    }
    return distance;
}
double euclidian_distance(const double hist1[26],const double hist2[26]) {
    double distance = 0.0;
    for (int i = 0; i < 26; i++) {
        distance += pow(hist1[i] - hist2[i], 2);
    }
    return pow(distance, 0.5);
}
void encrypt(int shift,const char *text, char encrypted[300]) {

    int i =0;
    while (text[i] != '\0') {
        int poz = (int)text[i];
        if (poz >96  && poz <123) {
            poz = poz +shift;
            if (poz>122)
                poz = poz -26;
            encrypted[i] = (char)poz;
        }
        else if (64<poz && poz <91) {
            poz = poz +shift;
            if (poz>90)
                poz = poz -26;
            encrypted[i] = (char)poz;
        }
        else {
            encrypted[i] = text[i];
        }
        i++;
    }
    encrypted[i]= '\0';
}
double cosine_distance(const double hist1[26],const double hist2[26]) {
    double distance1 = 0.0;
    double distance2 = 0.0;
    double distance3 = 0.0;
    for (int i = 0; i < 26; i++) {
        distance1 += hist1[i]*hist2[i];
        distance2 += hist1[i]*hist1[i];
        distance3 += hist2[i]*hist2[i];
    }
    return 1.0 - (distance1/(pow(distance3, 0.5) * pow(distance2, 0.5)));
}
void break_caesar_cipher(const char* text, int top_shifts[3],double top_distances[3],double distribution[26], double(*distance_function)(const double[],const double[])) {
    top_distances[0] = 10000.0;
    top_distances[1] = 10000.0;
    top_distances[2] = 10000.0;
    top_shifts[0] = 0;
    top_shifts[1] = 0;
    top_shifts[2] = 0;
    double histogram[26];
    double distance =0.0;
    for (int i = 0; i < 26; i++) {
        histogram[i] = 0.0;
    }
    char encrypted[300];
    for (int i = 0; i < 26; i++) {
        for (int k = 0; k < 26; k++) {
            histogram[k] = 0.0;
        }
        encrypt(i,text,encrypted);
        compute_histogram(encrypted,histogram);
        distance = distance_function(histogram,distribution);
        if (distance < top_distances[0]) {
            top_distances[2] = top_distances[1];
            top_distances[1] = top_distances[0];
            top_distances[0] = distance;
            top_shifts[2] = top_shifts[1];
            top_shifts[1] = top_shifts[0];
            top_shifts[0] = i;
        }
        else if (distance < top_distances[1]) {
            top_distances[2] = top_distances[1];
            top_distances[1] = distance;
            top_shifts[2] = top_shifts[1];
            top_shifts[1] = i;

        }
        else if (distance < top_distances[2]) {
            top_distances[2] = distance;
            top_shifts[2] = i;
        }
    }
}
void display_menu() {
    printf("A.Read from keyboard\n");
    printf("B.Read from file\n");
    printf("C.Encrypt with shift\n");
    printf("D.Decrypt with shift\n");
    printf("E.Compute and display histogram\n");
    printf("F.Break caesar cipher\n");
    printf("G.Exit\n");
}
void analysis(const char* filename,const char* filename2, double distribution[26],double(*distance_function)(const double[],const double[])) {
    FILE* file = fopen(filename, "r");
    FILE* fil = fopen(filename2, "w");
    char text[300];
    int top_shifts[3];
    double top_distances[3];
    char encrypted[300];
    char initial[300];
    int score = 0;
    int performance = 0;
    int tests = 0;
    if (file != NULL) {
        while (fgets(initial, 300, file)  && strlen(initial)>1) {

            tests++;
            int shift;
            char number[10];
            fgets(number, 10, file);
            shift = atoi(number);
            encrypt(shift, initial, text);
            break_caesar_cipher(text, top_shifts, top_distances, distribution, distance_function);
            encrypt(top_shifts[0], text, encrypted);
            fprintf(fil, "Text: %s  Shift: %d  Most probable text: %s\n", initial, shift, encrypted);
            if (26-shift == top_shifts[0]) {
                performance++;
                score+= strlen(initial);

            }


        }


    }
    fprintf(fil,"Number of texts decrypted: %d out of %d\n",performance,tests);
    fprintf(fil,"Score made by size of texts: %d\n",score);
    fclose(file);
    fclose(fil);
}
int main(void) {
    int t =1;
    char text[300];
    char encrypted[300];
    int shift;
    int poz = 0;
    double distribution[26];
    read_distribution("distribution.txt",distribution);
    double histogram[26];
    int top_shifts[3];
    double top_distances[3];
    ///analysis("analyse.txt","text_chi.txt",distribution,&chi_squared_distance);
    ///analysis("analyse.txt","text_cosine.txt",distribution,&cosine_distance);
    ///analysis("analyse.txt","text_euclidian.txt",distribution,&euclidian_distance);

    while(t==1) {
        display_menu();
        char choice;
        printf("Enter your choice: ");
        scanf(" %c",&choice);
        getchar();
        switch(choice) {
            case 'A':
                printf("Enter text: ");
                fgets(text, 300, stdin);
                printf("Output : %s",text);
                break;
            case 'B':

                printf("Input a filename");
                char f[300];
                fgets(f, 300, stdin);
                f[strcspn(f, "\n")] = 0;
                FILE* file = fopen(f, "r");
                fgets(text, 300, file);
                printf("Output : %s",text);
                fclose(file);
            break;
            case 'C':
                printf("Input a shift value:");
                scanf("%d",&shift);
                encrypt(shift,text,encrypted);
                poz =0;
                while(encrypted[poz] != '\0') {
                    printf("%c",encrypted[poz]);
                    poz++;
                }
            break;
            case 'D':
                printf("Input a shift value:");
                scanf("%d",&shift);
                getchar();
                printf("Input a text:");
                fgets(text, 300, stdin);
                encrypt(26-shift,text,encrypted);
            poz = 0;
            while(encrypted[poz] != '\0') {
                printf("%c",encrypted[poz]);
                poz++;
            }
            break;
            case 'E':
                compute_histogram(text,histogram);
            for(int i = 0; i < 26; i++) {
                printf("%f\n",histogram[i]);
            }
            break;
            case 'F':
                printf("Input a text:");
            fgets(text, 300, stdin);
            printf("Chi_squared_distance(a)\nCosine(b)\nEuclidian distance(c)\nChoose:");
            char option;
            scanf(" %c",&option);
            getchar();
            printf("The most probable word is:");
            switch(option) {
                case 'a':
                    break_caesar_cipher(text,top_shifts,top_distances,distribution,&chi_squared_distance);
                encrypt(top_shifts[0],text,encrypted);
                poz = 0;
                while(encrypted[poz] != '\0') {
                    printf("%c",encrypted[poz]);
                    poz++;
                }
                break;
                case 'b':
                    break_caesar_cipher(text,top_shifts,top_distances,distribution,&cosine_distance);
                encrypt(top_shifts[0],text,encrypted);
                poz = 0;
                while(encrypted[poz] != '\0') {
                    printf("%c",encrypted[poz]);
                    poz++;
                }
                break;
                case 'c':
                    break_caesar_cipher(text,top_shifts,top_distances,distribution,&euclidian_distance);

                encrypt(top_shifts[0],text,encrypted);
                poz = 0;
                while(encrypted[poz] != '\0') {
                    printf("%c",encrypted[poz]);
                    poz++;
                }
                break;

            }
            break;
            case 'G':
                t = 2;
            break;



        }
    }

    return 0;
}