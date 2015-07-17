
#ifndef _KOCH_FONCTIONS_H_
#define _KOCH_FONCTIONS_H_

#include "koch_common.h"

/* Liste chainee pour stocker les points de la fractale de Koch */
struct list {
	uint32_t x;
	uint32_t y;
	struct list *next;
};

/* Parametres du programme de calcul du flocon de Koch */
struct parameters {
	/* longueur en pixels d'un segment du triangle initial */
	uint32_t segment_length;

	/* taille en pixels d'un cote du carre englobant l'image finale */
	uint32_t image_size;

	/* nombre d'iterations du calcul la fractale de Koch */
	uint32_t nb_iterations; 

	/* couleur du trace de la fractale */
	uint32_t fg_color;

	/* couleur du fond */
	uint32_t bg_color;

	/* true s'il faut generer toutes les images intermediaires du
	   calcul de la fractale de Koch */
	bool all_images;

	/* nom du fichier image resultat */
	char *outfile;
};

/* Initialisation de la liste chainee koch correspondant au triangle
   de Koch initial */
void init_koch(struct list **koch, uint32_t size, uint32_t segment_length);

/* Initialisation de l'image avec la couleur de fond definie dans les
   parametres */
void init_picture(uint32_t **picture, uint32_t size, uint32_t bg_color);

/* Calcul de la fractale de Koch apres un nombre d'iterations donne ;
   generation de la liste chainee koch correspondante */
void generer_koch(struct list *koch, uint32_t nb_iterations);

/* Rendu image via algorithme breseham - version generalisee
   simplifiee */
void render_image_bresenham(uint32_t *picture, struct list *koch,
			    uint32_t size, uint32_t fg_color);

/* Liberation de la memoire allouee a la liste chainee */
void free_koch(struct list *koch);

#endif
