//Rodrigo Schmidt Becker

#define _USE_MATH_DEFINES
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

double seno(double ang_graus){
    double s;
    double ang_rad;
    ang_rad = ang_graus*M_PI/180;
    s = sin(ang_rad);
    return s;
}


double cosseno(double ang_graus){
    double c;
    double ang_rad;
    ang_rad = ang_graus*M_PI/180;
    c = cos(ang_rad);
    return c;
}

double aSeno(double sen){
    double as;
    double ang_graus;
    as = asin(sen);
    ang_graus = as*180/M_PI;
    return ang_graus;
}

double aCosseno(double sen){
    double ac;
    double ang_graus;
    ac = acos(sen);
    ang_graus = ac*180/M_PI;
    return ang_graus;
}

double leiSen1 (double a1, double a2, double l1){
    double l2;
    l2 = l1 / seno(a1) * seno(a2);
    return l2;
}

double leiSen2 (double a1, double l1, double l2){
    double a2;
    a2 = aSeno(seno(a1) * l2 / l1);
    return a2;
}

double leiCos1 (double l1, double l2, double l3){
    double a1;
    a1 = aCosseno((pow(l2,2) + pow(l3,2) - pow(l1,2)) / (2*l2*l3));
    return a1;
}

double leiCos2 (double l1, double l2, double a3){
    double l3;
    l3 = sqrt(pow(l1,2) + pow(l2,2) - 2 * l1 * l2 * cosseno(a3));
    return l3;
}

bool lll(double l1, double l2, double l3, double *a1, double *a2, double *a3){

    if((l1>0 && l2>0 && l3>0 && l1+l2>=l3 && l1+l3>=l2 && l2+l3>=l1)){
        *a1 = leiCos1(l1, l2, l3);
        *a2 = leiCos1(l2, l3, l1);
        *a3 = leiCos1(l3, l1, l2);
        return true;
    } else {
        return false;
    }
}

bool lal(double l1, double l2, double a3, double *l3, double *a1, double *a2){    

    if(l1>0 && l2>0 && a3>0 && a3<180){
        *l3 = leiCos2(l1, l2, a3);
        *a1 = leiCos1(l1, l2, *l3);
        *a2 = leiCos1(l2, *l3, l1);
        return true;
    } else {
        return false;
    }
}

bool lla(double l1, double l2, double a1, double *a2, double *a3, double *l3){

    if(l1>0 && l2>0 && a1>0 && a1<180){
        *a2 = leiSen2(a1, l1, l2);
        *a3 = 180 - a1 - *a2;
        *l3 = leiCos2(l1, l2, *a3);
        return true;
    } else {
        return false;
    }
}

bool ala(double a1, double a2, double l3, double *a3, double *l2, double *l1){    

    if(l3>0 && a1>0 && a2>0 && a1<180 && a2<180){
        *a3 = 180 - a1 - a2;
        *l2 = leiSen1(*a3, a2, l3);
        *l1 = leiSen1(a2, a1, *l2);
        return true;
    } else {
        return false;
    }
}

bool aal(double a1, double a2, double l1, double *l2, double *a3, double *l3){    

    if(l1>0 && a1>0 && a2>0 && a1<180 && a2<180){
        *l2 = leiSen1(a1, a2, l1);
        *a3 = 180 - a1 - a2;
        *l3 = leiSen1(a1, *a3, l1);
        return true;
    } else {
        return false;
    }
}

double print(double l1, double a1, double l2, double a2, double l3, double a3){
    printf("\nLado 1: %.2lf, Ângulo 1: %.2lf\n"
             "Lado 2: %.2lf, Ângulo 2: %.2lf\n"
             "Lado 3: %.2lf, Ângulo 3: %.2lf\n",
             l1, a1, l2, a2, l3, a3);

    if(l1==l2 && l1==l3 && l2==l3){
        printf("\nEquilátero e ");
        } else if(l1!=l2 && l1!=l3 && l2!=l3){
            printf("\nEscaleno e ");
        } else if((l1==l2)||(l2==l3)||(l1==l3)){
            printf("\nIsósceles e ");
        }

    if(a1<90 && a2<90 && a3<90){
        printf("Acutângulo.\n\n");
        } else if((a1==90)||(a2==90)||(a3==90)){
            printf("Retângulo.\n\n");
        } else if((a1>90)||(a2>90)||(a3>90)){
            printf("Obtusângulo.\n\n");
        }
}

int main (){
    double l1, l2, l3, a1, a2, a3, v1, v2, v3;
    int n;

    printf("Escolha uma opção:\n\n"
           "1. LLL - Lados 1, 2 e 3;\n"
           "2. LAL - Lado 1, Lado 2 e Ângulo 3;\n"
           "3. LLA - Lado 1, Lado 2 e Ângulo 1;\n"
           "4. ALA - Ângulo 1, Ângulo 2 e Lado 3;\n"
           "5. AAL - Ângulo 1, Ângulo 2 e Lado 1.\n\n");
    scanf("%d", &n);
    printf("\nDigite os três dados:\n\n");
    scanf("%lf%lf%lf", &v1, &v2, &v3);
    switch (n){
        case 1:
            l1 = v1;
            l2 = v2;
            l3 = v3;

            if(lll(l1, l2, l3, &a1, &a2, &a3)==true){
                print(l1, a1, l2, a2, l3, a3);
            }
            printf("%s", lll(l1, l2, l3, &a1, &a2, &a3) ? "\nCálculo executado com sucesso.\n" : "\nOs dados fornecidos não constituem um triângulo válido.\n");
            break;
        case 2:
            l1 = v1;
            l2 = v2;
            a3 = v3;

            if(lal(l1, l2, a3, &l3, &a1, &a2)==true){
                print(l1, a1, l2, a2, l3, a3);
            }
            printf("%s", lal(l1, l2, a3, &l3, &a1, &a2) ? "\nCálculo executado com sucesso.\n" : "\nOs dados fornecidos não constituem um triângulo válido.\n");
            break;
        case 3:
            l1 = v1;
            l2 = v2;
            a1 = v3;

            if(lla(l1, l2, a1, &a2, &a3, &l3)==true){
                print(l1, a1, l2, a2, l3, a3);
            }
            printf("%s", lla(l1, l2, a1, &a2, &a3, &l3) ? "Cálculo executado com sucesso." : "Os dados fornecidos não constituem um triângulo válido.");
            break;
        case 4:
            a1 = v1;
            a2 = v2;
            l3 = v3;

            if(ala(a1, a2, l3, &a3, &l2, &l1)==true){
                print(l1, a1, l2, a2, l3, a3);
            }
            printf("%s", ala(a1, a2, l3, &a3, &l2, &l1) ? "Cálculo executado com sucesso." : "Os dados fornecidos não constituem um triângulo válido.");
            break;
        case 5:
            a1 = v1;
            a2 = v2;
            l1 = v3;

            if(aal(a1, a2, l1, &l2, &a3, &l3)==true){
                print(l1, a1, l2, a2, l3, a3);
            }
            printf("%s", aal(a1, a2, l1, &l2, &a3, &l3) ? "Cálculo executado com sucesso." : "Os dados fornecidos não constituem um triângulo válido.");
            break;
    }
    return 0;
}
