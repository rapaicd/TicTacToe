#include "stdio.h"
#include "stdlib.h"
#include "string.h"

/*
int board[25] = {
	:,:,:,:,:,
	:,O,-,X,:,
	:,X,-,-,:,
	:,-,-,-,:,
	:,:,:,:,:,

	 0, 1, 2, 3, 4,
	 5, 6, 7, 8, 9,   // 1 2 3    5 5 5
	10,11,12,13,14,   // 4 5 6    7 7 7
	15,16,17,18,19,   // 7 8 9    9 9 9
	20,21,22,23,24
}
*/

enum { OKS, IKS, GRANICA, PRAZNO };

const int Pravci[4] = { 1, 5, 4, 6 };

const int PretvoriUFormatTable[9] = {
	6, 7, 8,
	11,12,13,
	16,17,18
};

const int USredini = 4;
const int Uglovi[4] = { 0, 2, 6, 8 };

int odigrano = 0;
int potezi = 0;
int brojPolja = 0;

int VratiBrojZaPravac(int pocetak, const int pravac, const int *tabla, const int strana) {
	int pronadjeno = 0;
	while(tabla[pocetak] != GRANICA) {
		if(tabla[pocetak] != strana) {
			break;
		}
		pronadjeno++;
		pocetak += pravac;
	}
	return pronadjeno;
}

int PronadjiTriUNizu(const int *tabla, const int indeksTable, const int strana) {

	int indeksPravca = 0;
	int pravac = 0;
	int brojacZaTri = 1;

	for(indeksPravca = 0; indeksPravca < 4; ++indeksPravca) {
		pravac = Pravci[indeksPravca];
		brojacZaTri += VratiBrojZaPravac(indeksTable + pravac, pravac, tabla, strana);
		brojacZaTri += VratiBrojZaPravac(indeksTable + pravac * -1, pravac * -1, tabla, strana);
		if(brojacZaTri == 3) {
			break;
		}
		brojacZaTri = 1;
	}
	return brojacZaTri;
}

int PronadjiTriUNizuCelaTabla(const int *tabla, const int strana) {
	int pronadjenaTri = 0;
	int indeks;
	for(indeks = 0; indeks < 9; ++indeks) {
		if(tabla[PretvoriUFormatTable[indeks]] == strana) {
			if(PronadjiTriUNizu(tabla, PretvoriUFormatTable[indeks], strana) == 3) {
				pronadjenaTri = 1;
				break;
			}
		}
	}
	return pronadjenaTri;
}

int ProceniPobedu(const int *tabla, const int strana) {

	if(PronadjiTriUNizuCelaTabla(tabla, strana) != 0)
		return 1;
	if(PronadjiTriUNizuCelaTabla(tabla, strana ^ 1) != 0)
		return -1;

	return 0;
}

int MinMax(int *tabla, int strana) {

	int listaPoteza[9];
	int brojPoteza = 0;
	int najboljiSkor = -2;
	int skor = -2;
	int najboljiPotez = -1;
	int potez;
	int indeks;

	if(odigrano > brojPolja) brojPolja = odigrano;
	potezi++;

	if(odigrano > 0) {
		skor = ProceniPobedu(tabla, strana);
		if(skor != 0) {
			return skor;
		}
	}

	// Ispuni listu poteza
	for(indeks = 0; indeks < 9; ++indeks) {
		if( tabla[PretvoriUFormatTable[indeks]] == PRAZNO) {
			listaPoteza[brojPoteza++] = PretvoriUFormatTable[indeks];
		}
	}

	// Prodji kroz sve poteze
	for(indeks = 0; indeks < brojPoteza; ++indeks) {
		potez = listaPoteza[indeks];
		tabla[potez] = strana;

		odigrano++;
		skor = -MinMax(tabla, strana^1);
		if(skor > najboljiSkor) {
			najboljiSkor = skor;
			najboljiPotez = potez;
		}
		tabla[potez] = PRAZNO;
		odigrano--;
	}

	if(brojPoteza==0) {
		najboljiSkor = PronadjiTriUNizuCelaTabla(tabla, strana);
	}

	if(odigrano!=0)
		return najboljiSkor;
	else
		return najboljiPotez;
}

void NapraviTablu(int *tabla) {
	int indeks = 0;

	for(indeks = 0; indeks < 25; ++indeks) {
		tabla[indeks] = GRANICA;
	}

	for(indeks = 0; indeks < 9; ++indeks) {
		tabla[PretvoriUFormatTable[indeks]] = PRAZNO;
	}
}

void NacrtajTablu(const int *tabla) {
	int indeks = 0;
	char dozvoljeniZnakovi[] = "OX|-";

	printf("\n\nTabla:\n\n");
	for(indeks = 0; indeks < 9; ++indeks) {
		if(indeks!=0 && indeks%3==0) {
			printf("\n\n");
		}
		printf("%4c",dozvoljeniZnakovi[tabla[PretvoriUFormatTable[indeks]]]);
	}
	printf("\n");
}

int ImaSlobodnih(const int *tabla) {
	int indeks = 0;

	for(indeks = 0; indeks < 9; ++indeks) {
		if( tabla[PretvoriUFormatTable[indeks]] == PRAZNO) return 1;
	}
	return 0;
}

void NapraviPotez(int *tabla, const int zadnjiPotez, const strana) {
	tabla[zadnjiPotez] = strana;
}

int PredvidiNajboljiSledeci(const int *tabla) {

	int korisnikovPotez = PretvoriUFormatTable[USredini];
	if(tabla[korisnikovPotez] == PRAZNO) {
		return korisnikovPotez;
	}

	int indeks = 0;
	korisnikovPotez = -1;

	for(indeks = 0; indeks < 4; indeks++) {
		korisnikovPotez = PretvoriUFormatTable[Uglovi[indeks]];
		if(tabla[korisnikovPotez] == PRAZNO) {
			break;
		}
		korisnikovPotez = -1;
	}

	return korisnikovPotez;
}

int PredvidiPobednickiPotez(int *tabla, const int strana) {

	int korisnikovPotez = -1;
	int pronadjiPobedu = 0;
	int indeks = 0;

	for(indeks = 0; indeks < 9; ++indeks) {
		if( tabla[PretvoriUFormatTable[indeks]] == PRAZNO) {
			korisnikovPotez = PretvoriUFormatTable[indeks];
			tabla[korisnikovPotez] = strana;

			if(PronadjiTriUNizu(tabla, korisnikovPotez, strana) == 3) {
				pronadjiPobedu = 1;
			}
			tabla[korisnikovPotez] = PRAZNO;
			if(pronadjiPobedu == 1) {
				break;
			}
			korisnikovPotez = -1;
		};
	}
	return korisnikovPotez;
}

int pretvoriNazad(int najbolji){
    int indeks;
    if(najbolji == 6 || najbolji == 7 || najbolji == 8){
        return najbolji - 5;
    } else if (najbolji == 11 || najbolji == 12 || najbolji == 13){
        return najbolji - 7;
    } else {
        return najbolji - 9;
    }
}

int PotezKompjutera(int *tabla, const int strana) {

	odigrano = 0;
	potezi = 0;
	brojPolja = 0;
	int najbolji = MinMax(tabla, strana);
	int najboljiPotez = pretvoriNazad(najbolji);
	printf("\nZavrsena pretraga dostupnih kombinacija poteza: %d \nBroj preostalih polja: %d \nNajbolji potez: %d \n", potezi, brojPolja, najboljiPotez);
	return najbolji;

}

int PotezKorisnika(const int *tabla) {

	char unosKorisnika[4];
	int potezValidan = 0;
	int potez = -1;

	while (potezValidan == 0) {
        fflush(stdin);
		printf("Unesite polje od 1 do 9:\n");
		fgets(unosKorisnika, 3, stdin);
		fflush(stdin);

		//if(strlen(unosKorisnika) != 2) {
		//	printf("Nedovoljna duzina unosa (strlen())\n");
		//	continue;
		//}

		if( sscanf(unosKorisnika, "%d", &potez) != 1) {
			potez = -1;
			//printf("Pogresan sscanf()\n");
			continue;
		}

		if( potez < 1 || potez > 9) {
			potez = -1;
			printf("Los broj polja (range)\n");
			continue;
		}

		potez--; // Nulto indeksiranje

		if( tabla[PretvoriUFormatTable[potez]]!=PRAZNO) {
			potez=-1;
			printf("Polje nije dostupno\n");
			continue;
		}
		potezValidan = 1;
	}
	printf("Odigran potez...%d\n",(potez+1));
	return PretvoriUFormatTable[potez];
}

void Pokreni() {

    char izbor;
    int brojac = 0;
    do {
        if(brojac%2==0){
            int krajIgre = 0;
            int naPotezu = OKS;
            int poslednjiNapravljenPotez = 0;
            int tabla[25];

            NapraviTablu(&tabla[0]);
            NacrtajTablu(&tabla[0]);

            while(!krajIgre) {
                if(naPotezu==IKS) {
                    poslednjiNapravljenPotez = PotezKompjutera(&tabla[0], IKS);
                    NapraviPotez(&tabla[0],poslednjiNapravljenPotez,naPotezu);
                    naPotezu=OKS;
                    NacrtajTablu(&tabla[0]);
                } else {
                    poslednjiNapravljenPotez = PotezKorisnika(&tabla[0]);
                    NapraviPotez(&tabla[0],poslednjiNapravljenPotez,naPotezu);
                    naPotezu=IKS;
                    NacrtajTablu(&tabla[0]);
                }

                // Ako ima tri u nizu kraj igre
                if( PronadjiTriUNizu(tabla, poslednjiNapravljenPotez, naPotezu ^ 1) == 3) {
                    printf("\nKraj igre!\n");
                    krajIgre = 1;
                    if(naPotezu==OKS) {
                        printf("IKS je pobedio!\n");
                    } else {
                        printf("OKS je pobedio!\n");
                    }
                }

                // Ako ne postoji jos poteza, nereseno je
                if(!ImaSlobodnih(tabla)) {
                    printf("\nKraj igre!\n");
                    krajIgre = 1;
                    printf("Nereseno je!\n");
                }
            }
                brojac++;
                printf("\nDa li zelite da igrate ponovo? (d/n) ");
                scanf(" %c", &izbor);
        }
        else{
            int krajIgre = 0;
            int naPotezu = IKS;
            int poslednjiNapravljenPotez = 0;
            int tabla[25];

            NapraviTablu(&tabla[0]);
            NacrtajTablu(&tabla[0]);

            while(!krajIgre) {
                if(naPotezu==OKS) {
                    poslednjiNapravljenPotez = PotezKompjutera(&tabla[0], OKS);
                    NapraviPotez(&tabla[0],poslednjiNapravljenPotez,naPotezu);
                    naPotezu=IKS;
                    NacrtajTablu(&tabla[0]);
                } else {
                    poslednjiNapravljenPotez = PotezKorisnika(&tabla[0]);
                    NapraviPotez(&tabla[0],poslednjiNapravljenPotez,naPotezu);
                    naPotezu=OKS;
                    NacrtajTablu(&tabla[0]);
                }

                // Ako ima tri u nizu kraj igre
                if( PronadjiTriUNizu(tabla, poslednjiNapravljenPotez, naPotezu ^ 1) == 3) {
                    printf("\nKraj igre!\n");
                    krajIgre = 1;
                    if(naPotezu==IKS) {
                        printf("OKS je pobedio!\n");
                    } else {
                        printf("IKS je pobedio!\n");
                    }
                }

                // Ako ne postoji jos poteza, nereseno je
                if(!ImaSlobodnih(tabla)) {
                    printf("\nKraj igre!\n");
                    krajIgre = 1;
                    printf("Nereseno je!\n");
                }
            }
                brojac++;
                printf("\nDa li zelite da igrate ponovo? (d/n) ");
                scanf(" %c", &izbor);
            }
        } while(izbor == 'D' || izbor == 'd');

}

int main() {

	srand(time(0)); // srand(time(NULL));
	Pokreni();

	return 0;
}
