#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> /* define inteiros de tamanho específico */
#define ubyte unsigned char

#pragma pack(push, 1) /* diz pro compilador nÃ£o alterar alinhamento 
                        ou tamanho da struct */
typedef  unsigned char RGBelement; // Cada eleemento RGB é representdao por 8 bits; um pixex tem 3 bytes (24 bits)

/*struct pixel{uint8_t r,g,b;};*/
struct bmpHeader {
    uint16_t type;
    uint32_t size;
    uint16_t reserved1,
            reserved2;
    uint32_t offset,
            header_size;
    int32_t width,
        height;    
    uint16_t planes, 
            bits;
    uint32_t compression,
                imagesize;
    int32_t xresolution,
        yresolution;
    uint32_t ncolours,
                importantcolours;
    uint32_t redbitmask,greenbitmask,bluebitmask,alphabitmask;
    uint32_t ColorSpaceType;
    uint32_t ColorSpaceEndPoints[9];
    uint32_t Gamma_Red,Gamma_Green,Gamma_Blue,intent,ICCProfileData,ICCProfileSize,Reserved;
};

#pragma pack(pop) /* restaura comportamento do compilador */

void printbin(char texto[], unsigned int valor)
{
     int i;
     printf("%s",texto);
     for (i=31;i>=0;i--) printf("%u",valor >> i & 1); 
     printf("\n");
 }

int main() {
    FILE *imagem;
    FILE *nova;
    int erro;
    RGBelement  valorReal;
    
    imagem = fopen("GiocondaHighResolution.bmp", "rb");
    nova = fopen("GiocondaHighResolutionSepia.bmp", "wb");
    
    struct bmpHeader bH;
  
    if (imagem == NULL)
        perror("Erro ao abrir a imagem");
    
    fread(&bH, sizeof(struct bmpHeader), 1, imagem);
    fwrite(&bH, sizeof(struct bmpHeader), 1, nova);
    
    RGBelement **matriz;
    
    matriz = (RGBelement **) malloc(sizeof(RGBelement *) * bH.height); 
    
    int i, j;
    
    for (i = 0; i < bH.height; i++) {
        matriz[i] = (RGBelement *) malloc(sizeof(RGBelement) * bH.width * 3); // Multplica por 3 porque cada pixel tem 3 bytes
    }
    
        printf("Aqui 0\n");
    fflush(stdout);
    /* leitura */
    int sepiaDepth = 20;
    for (i = 0; i < bH.height; i++) {
        for (j = 0; j < bH.width*3; j++) {
            fread(&matriz[i][j], sizeof(RGBelement), 1, imagem);//printf("%d ", matriz[i][j]);
            if (j>3 && j % 3 == 0){
            	int r = matriz[i][j];
            	int g = matriz[i][j - 1];
            	int b = matriz[i][j - 2];
	            int gry = (r + g + b) / 3;
            	r = g = b = gry;
            	r = r + (sepiaDepth * 2);
            	g = g + sepiaDepth;

            	if (r > 255) 
                	r = 255;
            	if (g > 255) 
                	g = 255;
	            if (b > 255) 
                	b = 255;
	            // Darken blue color to increase sepia effect
            	//b -= sepiaIntensity;

            	// normalize if out of bounds
            	if (b < 0) {
                	b = 0;
            	}
            	if (b > 255) {
                	b = 255;
            	}

            	matriz[i][j] = r;
            	matriz[i][j - 1] = g;
            	matriz[i][j - 2] = b;
        	}
    
		}
    }
    
    printf("Aqui 1\n");
    fflush(stdout);
    
    /* escrita */
    for (i = 0; i < bH.height; i++) {
        for (j = 0; j < bH.width*3; j++) {
            fwrite(&matriz[i][j], sizeof(RGBelement), 1, nova);
        }
        
    }
    
    unsigned char a;
    while (fread(&a,1,1,imagem))
       fwrite(&a,1,1,nova);
    
    for (i = 0; i < bH.height; i++) {
        free(matriz[i]);
    }
    free(matriz);
    
    
    printf("Image size = %d x %d\n", bH.width, bH.height);
    printf("Tamanho do arquivo = %u\n", bH.size);
    printf("Number of colour planes is %hu\n", bH.planes);
    printf("Bits per pixel is %hu\n", bH.bits);
    printf("Compression type is %u\n", bH.compression);
    printf("Number of colours is %u\n", bH.ncolours);
    printf("Number of required colours is %u\n", bH.importantcolours);
    printf("X resolution: %d - Y resolution: %d (ps: pixels per meter)\n", bH.xresolution, bH.yresolution);
    printf("Header size is %u\n", bH.header_size);
    printbin("Red channel bitmask ", bH.redbitmask);
    printbin("Blue channel bitmask ", bH.bluebitmask);
    printbin("Green channel bitmask ", bH.greenbitmask);
    printbin("Alpha channel bitmask ", bH.alphabitmask);
    printf("Tamanho do bmp header %u\n",sizeof(struct bmpHeader));
    
    fclose(imagem);
    fclose(nova);
    system("pause");    
    return 0;
}
