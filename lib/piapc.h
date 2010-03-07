/*
 * piapc.h
 * by Laurent Navet <Laurent_Navet@yahoo.com>
 * Released under GNU/LGPL license
 */

#define byte      unsigned char

/* définition des registres du PIA 8255  */

#define PORT_A    0x310     // Si Carte paramétrée en Adresses basses 
                            // ( Avec Strap en A ) Sinon PORT_A = 0x314
#define PORT_B   (PORT_A+1)
#define PORT_C   (PORT_B+1)
#define REG_CTRL (PORT_C+1) // Registre de Controle du PIA 8255


/* définition des différentes configuration du PIA 8255 */
typedef enum CONFIGS
{
 MODE_1 = 0x8B,      // PORT_A en Sortie, PORT_B et PORT_C en Entrées
 MODE_2 = 0x89,      // PORT_A et PORT_B en Sorties, PORT_C en Entrée
 MODE_3 = 0x82       // PORT_A et PORT_C en Sorties, PORT_B en Entrée 
}_CONFIGS;

typedef enum IO_MODES
{
  INPUT = 0,
  OUTPUT
}_IO_MODES;

typedef struct PIAPort
{
  int adresse; 
  enum IO_MODES mode;
}_PIAPort;

typedef struct PIACtrl
{
  struct PIAPort Port_A;
  struct PIAPort Port_B;
  struct PIAPort Port_C;
}_PIACtrl;

int init_pia  (struct PIACtrl * retpia,unsigned char config);
int write_pia (int adresse, byte valeur);
int read_pia  (int adresse, byte * valeur);


