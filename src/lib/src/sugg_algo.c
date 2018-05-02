#include "sugg_algo.h"
#include "file_type/bmp.h"
#include "file_type/png.h"
#include <endian.h>

#define ALGO_UNAVAILABLE -1

/** 
 * @brief Teste si l'on peut utiliser l'algorithme LSB pour la dissimulation. 
 * @param infos Structure représentant les informations concernant la dissimulation.
 * @return 0 si l'algorithme est proposé ; 1 sinon 
 * dans la fonction.
 */
int can_use_lsb(info_s* infos){
	// si le fichier hôte est un fichier BMP non compressé
	if(infos->host.type==BMP_UNCOMPRESSED){
		/* 
		Si le nombre de bits utilisés pour coder la couleur de chaque 
		pixel est inférieur ou égal à 8 (soit 256 couleurs), le deuxième 
		en-tête est suivi d’une table des couleurs utilisées dans l’image. 
		LSB non proposé 
		*/
		if(infos->host.file_info.bmp.pixel_length<=8) return 1;
		 
		// calcul du nombre d'octets représentant l'image brute
		uint64_t nb_bits_pic=((infos->host.file_info.bmp.pixel_length)*(infos->host.file_info.bmp.pixel_number))/8; 
		// calcul du nombre de bits modifiables pour l'algorithme LSB
		nb_bits_pic/=4;
		
		// si la taille du fichier a cacher est trop grosse NON
		// attention overflow
		if((infos->hidden_length*8)>nb_bits_pic){
			return 1;
		}
		return 0;
	}
	// si le fichier hôte est un fichier WAV PCM
	else if(infos->host.type==WAV_PCM){
		/* 
		calcul du nombre de bits modifiables pour l'algorithme LSB
		on fait -8 car data_size inclut [signature DATA](4 octets) et [taille du chunk DATA](4 octets)
		([nb doctets data]/[nb de bits par sample])=[nb doctets representant l'audio]
		[nb doctets representant l'audio]*2 -> nb de bits modifiable avec LSB 
		*/
		uint64_t nb_bits_audio=(((infos->host.file_info.wav.data_size)-8)/(infos->host.file_info.wav.chunk_size))*2;

		//attention overflow
		if((infos->hidden_length*8)>nb_bits_audio){
			return 1;
		}
	}
	else return 1;
}

/** 
 * @brief Teste si l'on peut utiliser l'algorithme EOF pour la dissimulation. 
 * @param infos Structure représentant les informations concernant la dissimulation.
 * @return 0 si l'algorithme est proposé ; 1 sinon 
 * dans la fonction.
 */
int can_use_eof(info_s* infos){
	// pour tous les formats proposés par StegX, on propose EOF
	if((infos->host.type==BMP_COMPRESSED)||(infos->host.type==BMP_UNCOMPRESSED)){
		return 0;
	}
	else if(infos->host.type==PNG){
		return 0;
	}
	else if((infos->host.type==WAV_NO_PCM)||(infos->host.type==WAV_PCM)){
		return 0;
	}
	else if(infos->host.type==MP3){
		return 0;
	}
	else if((infos->host.type==AVI_COMPRESSED)||(infos->host.type==AVI_UNCOMPRESSED)){
		return 0;
	}
	else if(infos->host.type==FLV){
		return 0;
	}
	else return 1;
}

/** 
 * @brief Teste si l'on peut utiliser l'algorithme MetaData pour la dissimulation. 
 * @param infos Structure représentant les informations concernant la dissimulation.
 * @return 0 si l'algorithme est proposé ; 1 sinon  
 * dans la fonction.
 */
int can_use_metadata(info_s* infos){
	// pour tous les formats proposés par StegX, on propose MetaData
	if((infos->host.type==BMP_COMPRESSED)||(infos->host.type==BMP_UNCOMPRESSED)){
		// pas toujours pour BMP car cela depend de la taille 
		return 0;
	}
	else if(infos->host.type==PNG){
		return 0;
	}
	else if((infos->host.type==WAV_NO_PCM)||(infos->host.type==WAV_PCM)){
		return 0;
	}
	else if(infos->host.type==MP3){
		return 0;
	}
	else if((infos->host.type==AVI_COMPRESSED)||(infos->host.type==AVI_UNCOMPRESSED)){
		return 0;
	}
	else if(infos->host.type==FLV){
		return 0;
	}
	else return 1;
}

/** 
 * @brief Teste si l'on peut utiliser l'algorithme EOC pour la dissimulation. 
 * @param infos Structure représentant les informations concernant la dissimulation.
 * @return 0 si l'algorithme est proposé ; 1 sinon  
 * dans la fonction.
 */
int can_use_eoc(info_s* infos){
	// pour le format FLV, on propose EOC
	if((infos->host.type==BMP_COMPRESSED)||(infos->host.type==BMP_UNCOMPRESSED)){
		return 1;
	}
	else if(infos->host.type==PNG){
		return 1;
	}
	else if((infos->host.type==WAV_NO_PCM)||(infos->host.type==WAV_PCM)){
		return 1;
	}
	else if(infos->host.type==MP3){
		return 1;
	}
	else if((infos->host.type==AVI_COMPRESSED)||(infos->host.type==AVI_UNCOMPRESSED)){
		return 1;
	}
	else if(infos->host.type==FLV){
		return 0;
	}
	else return 1;
}

/** 
 * @brief Teste si l'on peut utiliser l'algorithme Junk Chunk pour la dissimulation. 
 * @param infos Structure représentant les informations concernant la dissimulation.
 * @return 0 si l'algorithme est proposé ; 1 sinon  
 * dans la fonction.
 */
int can_use_junk_chunk(info_s* infos){
	// pour le format AVI, on propose Junk Chunk
	if((infos->host.type==BMP_COMPRESSED)||(infos->host.type==BMP_UNCOMPRESSED)){
		return 1;
	}
	else if(infos->host.type==PNG){
		return 1;
	}
	else if((infos->host.type==WAV_NO_PCM)||(infos->host.type==WAV_PCM)){
		return 1;
	}
	else if(infos->host.type==MP3){
		return 1;
	}
	else if((infos->host.type==AVI_COMPRESSED)||(infos->host.type==AVI_UNCOMPRESSED)){
		return 0;
	}
	else if(infos->host.type==FLV){
		return 1;
	}
	else return 1;
}

/** 
 * @brief Va remplir le champ host.file_info de infos (particulière en 
 * fonction du fichier hote). 
 * @param infos Structure représentant les informations concernant la dissimulation.
 * @return 0 si tout se passe bien ; 1 si il y a une erreur détectée dans 
 * la fonction. 
 */
int fill_host_info(info_s* infos){
	// si le fichier hôte a un format non reconnu
	if(infos->host.host==NULL) return 1;
	fseek(infos->host.host,0,SEEK_SET);
	int i,read;
	// remplit la structure BMP de infos.host.file_info
	//http://www.mysti2d.net/polynesie2/ETT/C044/31/Steganographie/index.html?Formatbmp.html
	if((infos->host.type==BMP_COMPRESSED)||(infos->host.type==BMP_UNCOMPRESSED)){
		uint32_t begin_pic,length_pic,pixel_width,pixel_height;
		uint32_t read_begin_pic,read_length_pic,read_pixel_width,read_pixel_height;
		uint16_t pixel_length;
		uint16_t read_pixel_length;
		
		// lecture de la taille totale du fichier
		fseek(infos->host.host,BMP_DEF_LENGTH,SEEK_SET);
		read=fread(&read_length_pic,sizeof(uint32_t),1,infos->host.host);
		// convertit read_length_pic de little-endian en l'endian de la machine
		length_pic=le32toh(read_length_pic);
		
		// lecture de la taille du header (qui correspond au debut de data)
		fseek(infos->host.host,BMP_DEF_PIC,SEEK_SET);
		read=fread(&read_begin_pic,sizeof(uint32_t),1,infos->host.host);
		begin_pic=le32toh(read_begin_pic);
		
		infos->host.file_info.bmp.header_size=begin_pic;
		infos->host.file_info.bmp.data_size=length_pic-begin_pic;
		
		// lecture du nombre de bits par pixel
		fseek(infos->host.host,BMP_DEF_PIX_LENGTH,SEEK_SET);
		read=fread(&read_pixel_length,sizeof(uint16_t),1,infos->host.host);
		pixel_length=le32toh(read_pixel_length);
		infos->host.file_info.bmp.pixel_length=pixel_length;
		
		// lecture de la largeur de l'image
		fseek(infos->host.host,BMP_DEF_NB_PIXEL,SEEK_SET);
		read=fread(&read_pixel_width,sizeof(uint32_t),1,infos->host.host);
		pixel_width=le32toh(read_pixel_width);

		// lecture du la hauteur de l'image
		read=fread(&read_pixel_height,sizeof(uint32_t),1,infos->host.host);
		pixel_height=le32toh(read_pixel_height);
		infos->host.file_info.bmp.pixel_number=pixel_width*pixel_height;
		return 0;
	}
	
	// remplit la structure BMP de infos.host.file_info
	// http://www.libpng.org/pub/png/spec/1.2/PNG-Chunks.html
	else if(infos->host.type==PNG){
		// lecture de la taille du chunk IHDR (header)
		fseek(infos->host.host,PNG_DEF_IHDR,SEEK_SET);
		uint32_t read_ihdr_length,ihdr_length;
		read=fread(&read_ihdr_length,sizeof(uint32_t),1,infos->host.host);
		ihdr_length=be32toh(read_ihdr_length); // convertit read_length_pic de big-endian en l'endian de la machine
		infos->host.file_info.png.header_size=PNG_DEF_IHDR+ihdr_length;
		
		uint64_t file_length=0;
		uint8_t byte_read_png;
		int read_iend=0;
		int stop_png=0;
		fseek(infos->host.host,0,SEEK_SET);
		// detection du chunk IEND et lecture de la taille de data (jusqu'a IEND)
		while(stop_png==0){
			read=fread(&byte_read_png,sizeof(uint8_t),1,infos->host.host);
			if(read==0) stop_png=1;
			else{
				if(byte_read_png==PNG_IEND_1) read_iend++;
				else if((byte_read_png==PNG_IEND_2)&&(read_iend==1)){
					read_iend++;
				}
				else if((byte_read_png==PNG_IEND_3)&&(read_iend==2)){
					read_iend++;
				}
				else if((byte_read_png==PNG_IEND_4)&&(read_iend==3)){
					read_iend++;
				}
				else read_iend=0;
				if(read_iend==4){
					file_length=file_length+4;
					stop_png=1;
				}
				file_length++;
			}
		}
		infos->host.file_info.png.data_size=file_length-infos->host.file_info.png.header_size;
		return 0;
	}
	
	// remplit la structure BMP de infos.host.file_info
	// http://soundfile.sapp.org/doc/WaveFormat/
	else if((infos->host.type==WAV_PCM)||(infos->host.type==WAV_NO_PCM)){
		uint8_t byte_read_wav; //l ecture de l'octet a chaque iteration
		uint64_t header_length=0; // longueur du header
		int stop_wav=0; // sort de la boucle quand on a lu les lettres "DATA"
		int read_data=0; // compte les lettres "DATA"
		// lecture de la taille du header (jusqu'a DATA)
		while(stop_wav==0){
			read=fread(&byte_read_wav,sizeof(uint8_t),1,infos->host.host);
			if(read==0) stop_wav=1;
			else{
				if(byte_read_wav==WAV_DATA_1) read_data++;
				else if((byte_read_wav==WAV_DATA_2)&&(read_data==1)){
					read_data++;
				}
				else if((byte_read_wav==WAV_DATA_3)&&(read_data==2)){
					read_data++;
				}
				else if((byte_read_wav==WAV_DATA_4)&&(read_data==3)){
					read_data++;
				}
				else read_data=0;
				if(read_data==4){
					header_length=header_length-4;
					stop_wav=1;
				}
				header_length++;
			}
		}
		infos->host.file_info.wav.header_size=header_length;
		
		// lecture de la taille du chunk DATA
		uint64_t read_data_length;
		uint64_t data_length;
		fseek(infos->host.host,header_length+WAV_DATA_SIZE,SEEK_SET);
		read=fread(&read_data_length,sizeof(uint64_t),1,infos->host.host);
		data_length=le32toh(read_data_length);
		infos->host.file_info.wav.data_size=data_length+WAV_SUBCHUNK_LENGTH;
		
		// lecture du nombre de channels (canaux)
		uint16_t read_bloc_align,bloc_align;
		uint16_t read_nb_channels,nb_channels;
		fseek(infos->host.host,WAV_NUM_CHANNELS,SEEK_SET);
		read=fread(&read_nb_channels,sizeof(uint16_t),1,infos->host.host);
		nb_channels=le32toh(read_nb_channels);
		
		// lecture du nombre de bloc align 
		fseek(infos->host.host,WAV_NB_BLOC_ALIGN,SEEK_SET);
		read=fread(&read_bloc_align,sizeof(uint16_t),1,infos->host.host);
		bloc_align=le32toh(read_bloc_align);

		// nb_bits_par_sample=(nb_bloc_align/nb_channels)*8;
		infos->host.file_info.wav.chunk_size=(bloc_align/nb_channels)*8; 		
		return 0;
	}
	
	// FLV
	// https://www.adobe.com/content/dam/acom/en/devnet/flv/video_file_format_spec_v10.pdf
	else if(infos->host.type==FLV){
		return 0;
	}
	
	// pour les structures AVI et MP3 leurs structures sont 
	else if((infos->host.type==MP3)||(infos->host.type==AVI_COMPRESSED)
		||(infos->host.type==AVI_UNCOMPRESSED)){
			return 0;
	}
	
	else return 1;
}

/** 
 * @brief Va proposer les algorithmes en fonction de la structure infos. 
 * @param infos Structure représentant les informations concernant la dissimulation.
 * @return 0 si tout se passe bien ; 1 si il y a une erreur détectée dans 
 * la fonction. 
 */
int stegx_suggest_algo(info_s* infos){
	if(infos->mode==STEGX_MODE_EXTRACT){
		return 1;
	}
	int fill=fill_host_info(infos);
	if(fill==1){
		//erreur
		return 1;
	} 
	fseek(infos->hidden,0,SEEK_END);
	// lecture de la taille du fichier à cacher
	infos->hidden_length = ftell(infos->hidden);
	// attention pour la taille overflow????!!!!
	
	int i,test;
	/*
	remplissage du tableau stegx_propos_algos pour savoir 
	quels algos sont proposés par l'application en fonction des 
	entrées de l'utilisateur. 
	*/
	stegx_propos_algos=malloc(NB_ALGOS*sizeof(algo_e));
	algo_e tab_algo_possible[NB_ALGOS]={STEGX_ALGO_EOF, STEGX_ALGO_LSB, 
		STEGX_ALGO_METADATA, STEGX_ALGO_EOC, STEGX_ALGO_JUNK_CHUNK};
	for(i=0;i<NB_ALGOS;i++){
		if(i==0) test=can_use_eof(infos);
		else if(i==1) test=can_use_lsb(infos);
		else if(i==2) test=can_use_metadata(infos);
		else if(i==3) test=can_use_eoc(infos);
		else if(i==4) test=can_use_junk_chunk(infos);
		// can_use_XXX renvoie 1 si il ne peut pas et 0 s'il le peut
		if(test==1) stegx_propos_algos[i]=ALGO_UNAVAILABLE;
		else {
			stegx_propos_algos[i]=tab_algo_possible[i];
		}
	}
	return 0;
}

int stegx_choose_algo(info_s* infos,algo_e algo_choosen);
