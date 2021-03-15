typedef int(*pfnipcalc)(const unsigned int *, const unsigned int *, unsigned char *);
typedef void(*pfnmes) (const char *, unsigned char *);
typedef struct _subnet
{
        pfnipcalc Calc;
        pfnmes    Display;
}subnet;
int isvalid(char *ip);
int get_ip_sm(char *cip, char *csm, unsigned int *iip, unsigned int *ism);
int sn_calc_naddr(const unsigned int *ip, const unsigned int *smask, unsigned char *subnet);
int sn_first_host(const unsigned int *ip, const unsigned int *smask, unsigned char *subnet);
int sn_last_host(const unsigned int *ip, const unsigned int *smask, unsigned char *subnet);
int sn_calc_bcaddr(const unsigned int *ip, const unsigned int *smask, unsigned char *subnet);
int ipcalc(const char *mes, subnet *operation, const unsigned int *ip, const unsigned int *smask, unsigned char *subnet);
void message (const char *mes, unsigned char *subnet);
unsigned int count(unsigned int n);
char gip[16] , gsmask[16];
