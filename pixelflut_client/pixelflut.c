#include <arpa/inet.h>
#include <inttypes.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/ip.h>
typedef struct _BitmapFileHeader {
	uint16_t bfType;
	uint32_t bfSize;
	uint32_t bfReserved1;
	uint32_t bfOffBits;
} __attribute__((packed)) BitmapFileHeader;

typedef struct _BitmapInfoHeader {
	uint32_t biSize;
	int32_t  biWidth;
	int32_t  biHeight;
	uint16_t biPlanes;
	uint16_t biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	int32_t  biXPelsPerMeter;
	int32_t  biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;
} __attribute__((packed)) BitmapInfoHeader;

typedef struct _Bitmap {
	BitmapFileHeader fileHeader;
	BitmapInfoHeader infoHeader;
	unsigned char	 content[];
} __attribute__((packed)) Bitmap;

typedef struct _Color {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} Color;

static void pixelflut(Bitmap *image);

int main(int argc, char **argv) {
	Bitmap *image;

	if(argc < 2) {
		printf("Missing filename\n");
		return 1;
	}

	struct stat st;
	if(stat(argv[1], &st) < 0) {
		printf("Failed to stat %s: %s\n", argv[1], strerror(errno));
		return 1;
	}

	image = (Bitmap*) malloc(st.st_size);
	if(image == NULL) {
		printf("Failed to allocate memory: %s\n", strerror(errno));
		return 1;
	}

	FILE *bmp = fopen(argv[1], "r");
	if(bmp == NULL) {
		printf("Failed to open %s: %s\n", argv[1], strerror(errno));
		return 1;
	}

	size_t read = 0;
	while(read < st.st_size) {
		size_t result = fread(((char*) image) + read, 1, st.st_size - read, bmp);
		if(result <= 0) {
			printf("Failed to read from %s: %s\n", argv[1], strerror(errno));
			return 1;
		}

		read += result;
	}
	fclose(bmp);

	if(image->infoHeader.biHeight < 0) {
		printf("Only bottom-up BMPs are supported\n");
		return 1;
	}

	if(image->infoHeader.biCompression != 0) {
		printf("Only uncompressed BMPs are supported\n");
		return 1;
	}

	printf("Loaded BMP with resolution %ix%i pixels\n", image->infoHeader.biWidth, image->infoHeader.biHeight);

	pixelflut(image);
	free(image);

	return 0;
}

static Color get_color(Bitmap *image, uint32_t x, uint32_t y) {
	Color c;

	y = image->infoHeader.biHeight - y - 1;

	int lineLength = image->infoHeader.biWidth * 3;
	if(lineLength % 4 != 0) {
		lineLength += 4 - lineLength % 4;
	}

	c.red = image->content[y * lineLength + x * 3 + 2];
	c.green = image->content[y * lineLength + x * 3 + 1];
	c.blue = image->content[y * lineLength + x * 3];

	return c;
}

#define HOST "127.0.0.1"
#define PORT 1337  

static void pixelflut(Bitmap *image) {
	int i = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in sai;
	sai.sin_family = AF_INET;
	sai.sin_port = htons(PORT);
	inet_pton(AF_INET,"172.17.64.131", &(sai.sin_addr));

	int o = connect(i,(struct sockaddr*) &sai,sizeof(sai));
	

	while (1==1){
		for (size_t x = 0; x <image->infoHeader.biWidth; x++) {
			for (size_t y = 0; y < image->infoHeader.biHeight; y++){

				Color color = get_color(image,x,y);
				char message[100];
				sprintf(message,"PX %li %li %02X%02X%02X\n",x,y,color.red,color.green,color.blue);
				write(i,message,strlen(message));
			}
	
		}
	
	}
	
	// TODO Socket erstellen - socket()
	// Tipp: Wir nutzen IPv4 mit TCP (TCP ist ein verbindungsorientiertes Protokoll, aka Streamprotokoll)

	// TODO Socket verbinden - htons(), inet_pton() und connect()
	// Tipp: connect() müsst ihr ein struct sockaddr_in* übergeben (man 7 ip)

	// TODO Über Pixel der Bitmap iterieren und versenden (und das in einer Endlosschleife)
	//	* Breite und Höhe liegen in der BitmapInfoHeader-Struktur
	//	* Pixel mit get_color() (oben definiert) holen
	// 	* Befehl, um Pixel zu zeichnen (also das, was ihr an den Server schicken müsst):
	//		PX <x> <y> <RRGGBB>\n
	//		\n ist ein Zeilenumbruch (ASCII-Zeichen 0x10)
	//		ohne < und >
	//		Farbwerte in (zweistelligem) Hexadezimal
	//	* Befehl am besten per sprintf() in String schreiben
	//	* write() zum Versenden
	
	// TODO Bei Langeweile:
	//	* sprintf() ist relativ langsam - wie geht's schneller?
	//	* Mehr als einen Befehl pro write() übertragen
	//	* Die Grafik an einer anderen Position als oben links zeichnen
	//	* Vielleicht eine kleine Animation?
}
