#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include "subnet.h"

int main()
{
    unsigned char cip[16] = {0}, csmask[16] = {0}, subnet[16] = {0} ;
    unsigned int ismask = 0, iip = 0 ;
    int iret = 0, option = 0;
    subnet *soperation = NULL;
    char mes[12] = {0};
    soperation = (subnet *) malloc (sizeof(subnet));
    if (soperation == NULL) {
        printf("\nMemory Not Allocated");
        return 0;
    }

    printf("\nEnter the IP address :\t");
    scanf("%s",gip);
    strcpy(cip,gip);
    printf("\nEnter the Subnet Mask :\t");
    scanf("%s",gsmask);
    strcpy(csmask,gsmask);
    iret = get_ip_sm(cip,csmask,&iip,&ismask);
    if (iret < 0) {
        if (iret == -1)
            printf("\nInvalid IP");
        else if (iret == -2)
            printf("\nInvalid Subnet Mask");
        return 0;
    }
    soperation->Display = &message;
    while (1) {
       memset(subnet,0x00,sizeof(subnet));
        memset(mes,0x00,sizeof(mes));
        printf("\n1 Subnet Address");
        printf("\n2 First Host");
        printf("\n3 Last Host");
        printf("\n4 Broadcast");
        printf("\n5 ALL");
        printf("\n*Enter the above one option *\n");
        scanf("%d", &option);
        switch(option) {
            case 1 :
                    soperation->Calc = &sn_calc_naddr;
                    strcpy(mes,"Subnet    ");
                break;
            case 2 :
                    soperation->Calc = &sn_first_host;
                    strcpy(mes,"First Host");
                break;
            case 3 :
                    soperation->Calc = &sn_last_host;
                    strcpy(mes,"Last Host ");
                break;
            case 4 :
                    soperation->Calc = &sn_calc_bcaddr;
                    strcpy(mes,"Broadcast ");
                break;
            case 5 :
                    soperation->Calc = &sn_calc_naddr;
                    strcpy(mes,"Subnet    ");
                    ipcalc(mes,soperation,&iip,&ismask,subnet);
                    soperation->Calc = &sn_first_host;
                    strcpy(mes,"First Host");
                    ipcalc(mes,soperation,&iip,&ismask,subnet);
                    soperation->Calc = &sn_last_host;
                    strcpy(mes,"Last Host ");
                    ipcalc(mes,soperation,&iip,&ismask,subnet);
                    soperation->Calc = &sn_calc_bcaddr;
                    strcpy(mes,"Broadcast ");
                    break;
            default :
                exit(0);
        }
        ipcalc(mes,soperation,&iip,&ismask,subnet);
        printf("\n\n\n");
    }
    return 0;
}
int isvalid(char *ip)
{
    char *ptr = NULL;
    short int num = 0, dot = 0, i = 0;
    for (i = 0; *(ip+i) != '\0'; i++)
        if (*(ip+i) == '.')
            dot++;
    ptr = strtok(ip,".");
    while (ptr != NULL) {
        if (!isdigit(*ptr))
            return 0;
        num = atoi(ptr);
        if (num < 0 || num > 255)
            return 0;
        ptr = strtok(NULL,".");
    }
    if (dot != 3)
        return 0;
    return 1;
}
int get_ip_sm(char *cip, char *csm, unsigned int *iip, unsigned int *ism)
{
    char *ptr = NULL, *p = NULL;
    int temp = 0, i = 3;

    if(!isvalid(cip))
        return -1;
    if(!isvalid(csm))
        return -2;

    memset(cip,0x00,sizeof(cip));
    strcpy(cip,gip);
    p = (char*)iip;
    ptr = strtok(cip,".");
    while (ptr != NULL) {
        temp = atoi(ptr);
        *(p+i) = temp;
        ptr = strtok(NULL,".");
        i--;
    }
    i = 3;
    memset(csm,0x00,sizeof(csm));
    strcpy(csm,gsmask);
    p = (char*)ism;
    ptr = strtok(csm,".");
    while (ptr != NULL) {
        temp = atoi(ptr);
        *(p+i) = temp;
        ptr = strtok(NULL,".");
        i--;
    }
    if (*ism == 0)
        return -2;
    if (*ism & ((~(*ism) & 255) >> 1))
        return -2;
    return 1;
}
int sn_calc_naddr(const unsigned int *ip, const unsigned int *smask, unsigned char *subnet)
{
    char *cip = NULL , *csmask = NULL;
    int i = 0;

    cip = (char *)ip;
    csmask = (char *)smask;
    for (i = 3; i >=0; i--) {
        *(subnet + i) = *(cip + i) & *(csmask + i);
    }
    return 1;
}
int sn_calc_bcaddr(const unsigned int *ip, const unsigned int *smask, unsigned char *subnet)
{
    char *cip = NULL , *csmask = NULL;
    int i = 0;
    unsigned int cnt = 0, temp1 = 0 ;
    unsigned char temp[4] = {0};

    sn_calc_naddr(ip,smask,temp);
    cnt = count(~*smask);
    temp1 = ~(~0 << cnt);
    csmask = (char *) &temp1;
    for (i = 3; i >=0; i--) {
        *(subnet + i) = *(temp + i) | *(csmask + i);
    }
    return 1;
}
int sn_first_host(const unsigned int *ip, const unsigned int *smask, unsigned char *subnet)
{
    char *cip = NULL , *csmask = NULL;
    int i = 0;
    unsigned int cnt = 0, temp1 = 0 ;
    unsigned char temp[4] = {0};

    sn_calc_naddr(ip,smask,temp);
    temp1 =  ~(~0 << 1);
    csmask = (char *) &temp1;

    for (i = 3; i >=0; i--) {
        *(subnet + i) = *(temp + i) | *(csmask + i);
    }
    return 1;
}
int sn_last_host(const unsigned int *ip, const unsigned int *smask, unsigned char *subnet)
{
    char *cip = NULL , *csmask = NULL;
    int i = 0;
    unsigned int cnt = 0, temp1 = 0 ;
    unsigned char temp[4] = {0};

    sn_calc_naddr(ip,smask,temp);
    cnt = count(~*smask);
    temp1 = ~(~0 << cnt - 1);
    temp1 = temp1 << 1;
    csmask = (char *) &temp1;

    for (i = 3; i >=0; i--) {
        *(subnet + i) = *(temp + i) | *(csmask + i);
    }
    return 1;
}
int ipcalc(const char *mes,subnet *fnptr,const unsigned int *ip, const unsigned int *smask, unsigned char *subnet)
{
    fnptr->Calc(ip,smask,subnet);
    fnptr->Display(mes,subnet);
    return 1;
}
void message (const char *mes, unsigned char *subnet)
{
    printf("\n%s : %d.%d.%d.%d",mes,subnet[3],subnet[2],subnet[1],subnet[0]);
}
unsigned int count(unsigned int n)
{
    int i = 0;
    for (; n != 0; n = n>>1)
        if (n&1)
            i++;
    return i;
}
