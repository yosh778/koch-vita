
#include "koch_fonctions.h"

/* Initialisation de la liste chainee koch correspondant au triangle
   de Koch initial */
void init_koch(struct list **koch, uint32_t size, uint32_t segment_length)
{
	struct list *pt_a, *pt_b, *pt_c;
	const uint32_t BORDER = BORDER_CALC(segment_length);

	SAFE_FREE(*koch);

	pt_a = malloc(sizeof(struct list));
	assert(pt_a);
	*koch = pt_a;

	/* Création du triangle équilatéral initial */
	pt_a->x = BORDER;
	pt_a->y = size - (BORDER + segment_length/5);
	pt_a->next = malloc(sizeof(struct list));

	pt_b = pt_a->next;
	pt_b->x = BORDER + segment_length;
	pt_b->y = size - (BORDER + segment_length/5);
	pt_b->next = malloc(sizeof(struct list));

	pt_c = pt_b->next;
	pt_c->x = (pt_a->x + pt_b->x) / 2;
	pt_c->y = size - ( BORDER + segment_length/5 + sin( DEGREE_TO_RADIAN(60) ) * (pt_b->x - pt_a->x) );
	pt_c->next = NULL;
}

/* Initialisation de l'image avec la couleur de fond definie dans les
   parametres */
void init_picture(uint32_t **picture, uint32_t size, uint32_t bg_color)
{
	const uint32_t LEN = size * size;

	SAFE_FREE(*picture);

	uint32_t *data = calloc(LEN, sizeof(uint32_t));
	*picture = data;
	assert(data);

	for (uint32_t i = 0; i < LEN; i++)
		(*picture)[i] = bg_color;
}

/* Calcul de la fractale de Koch apres un nombre d'iterations donne ;
   generation de la liste chainee koch correspondante */
void generer_koch(struct list *koch, uint32_t nb_iterations)
{
	struct list *pt_a, *pt_e, *next;
	struct list *pt_b, *pt_c, *pt_d;

	if (nb_iterations > 0)
	{
		const double COS_60 = cos( DEGREE_TO_RADIAN(60) );
		const double SIN_60 = sin( DEGREE_TO_RADIAN(60) );

		next = koch;
		while ((pt_a = next))
		{
			next = pt_a->next;
			pt_e = next;

			/* Fermeture de la figure */
			if (pt_e == NULL)
				pt_e = koch;

			/* Insertion des points de la nouvelle itération */
			pt_b = malloc(sizeof(struct list));
			pt_c = malloc(sizeof(struct list));
			pt_d = malloc(sizeof(struct list));

			pt_b->x = (int32_t)pt_a->x + ((int32_t)pt_e->x - (int32_t)pt_a->x)/3;
			pt_b->y = (int32_t)pt_a->y + ((int32_t)pt_e->y - (int32_t)pt_a->y)/3;

			pt_d->x = (int32_t)pt_a->x + (2 * ((int32_t)pt_e->x - (int32_t)pt_a->x)) / 3;
			pt_d->y = (int32_t)pt_a->y + (2 * ((int32_t)pt_e->y - (int32_t)pt_a->y)) / 3;

			pt_c->x = (pt_b->x + pt_d->x) * COS_60 - ((int32_t)pt_d->y - (int32_t)pt_b->y) * SIN_60;
			pt_c->y = (pt_b->y + pt_d->y) * COS_60 + ((int32_t)pt_d->x - (int32_t)pt_b->x) * SIN_60;

			pt_a->next = pt_b;
			pt_b->next = pt_c;
			pt_c->next = pt_d;
			pt_d->next = next;
		}
	}
}

/*  Tracé de ligne entre 2 points de coordonnées
    (x0,y0) et (x1,y1) */
void bresenham(uint32_t x0, uint32_t x1, uint32_t y0, uint32_t y1,
				uint32_t color, uint32_t *picture, uint32_t size)
{
	bool draw = true;
	int32_t dx, dy, sx, sy, err, e2;

	dx = abs(x1-x0);
	dy = abs(y1-y0);

	if (x0 < x1)
		sx = 1;
	else
		sx = -1;

	if (y0 < y1)
		sy = 1;
	else
		sy = -1;

	err = dx - dy;

	do {
		/* On dessine le pixel s'il est valide */
		if ((x0 < size) && (y0 < size))
			picture[y0 * size + x0] = color;
		/* Si on sort de l'image, finir */
		else
			draw = false;

		if ((x0 == x1) && (y0 == y1))
			draw = false;
		else {
			e2 = 2 * err;

			if (e2 > -dy)
			{
				err = err - dy;
				x0 = x0 + sx;
			}

			if (e2 < dx)
			{
				err = err + dx;
				y0 = y0 + sy;
			}
		}
	} while (draw);
}

/* Rendu image via algorithme breseham - version generalisee
   simplifiee */
void render_image_bresenham(uint32_t *picture, struct list *koch,
			    uint32_t size, uint32_t fg_color)
{
	struct list *pt, *next;
	pt = koch;

	/* Rendu de tous les segments */
	while (pt) {
		next = pt->next;

		/* Fermeture de la figure */
		if (next == NULL)
			next = koch;

		bresenham(pt->x, next->x, pt->y, next->y, fg_color, picture, size);
		pt = pt->next;
	}
}

/* Liberation de la memoire allouee a la liste chainee */
void free_koch(struct list *koch)
{
	struct list *next;

	while (koch) {
		next = koch->next;
		free(koch);
		koch = next;
	}
}
