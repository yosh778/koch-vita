
#include "koch_ihm.h"

/* Initialisation des differents parametres de la fractale a partir
   des arguments du programme ou a defaut en mode interactif */
void init_parameters(struct parameters *parameters, int argc, char *argv[])
{
	if (parameters)
	{
		const uint8_t NB_ARGS = 6;
		char input[NB_ARGS][MAX_ARG_SIZE], *temp = NULL;
		const uint32_t COUNT = argc-1;
		uint32_t size;
		bool error;


		// /* Récupération des arguments fournis en ligne de commande */
		// for (uint32_t i = 0; i < COUNT; ++i)
		// {
		// 	strncpy(input[i], argv[i+1], MAX_ARG_SIZE);
		// 	input[i][MAX_ARG_SIZE-1] = '\0';
		// }

		// /* Requete des arguments manquants */
		// for (uint32_t i = COUNT; i < NB_ARGS; ++i)
		// {
		// 	do {
		// 		error = false;

		// 		switch(i) {
		// 		case 0:
		// 			printf("Taille des segments ? ");
		// 			break;
		// 		case 1:
		// 			printf("Nombre d'iterations ? ");
		// 			break;
		// 		case 2:
		// 			printf("Couleur de trace ? (format 0xRRVVBB) ");
		// 			break;
		// 		case 3:
		// 			printf("Couleur de fond ? (format 0xRRVVBB) ");
		// 			break;
		// 		case 4:
		// 			printf("Nom de sortie ? ");
		// 			break;
		// 		case 5:
		// 			printf("Generer toutes les images intermediaires ? (o/n) ");
		// 			break;
		// 		}

		// 		temp = input[i];
		// 		fgets(temp, MAX_ARG_SIZE, stdin);

		// 		/* Enlève le caractère de retour chariot */
		// 		size = strlen(temp);
		// 		if ( size > 0 && temp[size-1] == '\n' )
		// 			temp[size-1] = '\0';

		// 		/* Gestion erreurs */
		// 		if (strlen(temp) < 1) {
		// 			error = true;
		// 			printf("Saisie invalide.\n\n");
		// 		}
		// 	} while (error);
		// }

		// if (COUNT < NB_ARGS)
		// 	putchar('\n');

		/* Enregistrement des valeurs */
		parameters->segment_length = 365;
		parameters->image_size = 544;
		parameters->nb_iterations = 7;
		parameters->fg_color = 0xFFFFFF00;
		parameters->fg_color = parameters->fg_color ? parameters->fg_color : 0xFFFFFFFF;
		parameters->bg_color = 0;

		temp = "out.ppm";
		parameters->outfile = malloc(strlen(temp) + 1);
		strcpy(parameters->outfile, temp);

		temp = input[5];
		parameters->all_images = true;
		/*parameters->all_images = !strcasecmp(temp, "all") ||
									!strcasecmp(temp, "1") ||
									!strcasecmp(temp, "y") ||
									!strcasecmp(temp, "o") ||
									!strcasecmp(temp, "yes") ||
									!strcasecmp(temp, "oui") ||
									!strcasecmp(temp, "ok");*/
	}
}

/* Affichage de controle sous forme texte de la liste des coordonnees
   des points du flocon de Koch */
void show_koch_list(struct list *koch)
{
	printf( "Liste des coordonnees :\n" );

	while (koch) {
		printf("x = %d, y = %d\n", koch->x, koch->y);
		koch = koch->next;
	}

	putchar('\n');
}

/* Affichage de controle des differents parametres de la fractale */
void show_parameters(struct parameters parameters)
{
	printf(
			"Taille des segments : %d pixels\n"
			"Taille de l'image : %d pixels\n"
			"Nombre d'iterations : %d\n"
			"Couleur de trace : 0x%06X\n"
			"Couleur de fond : 0x%06X\n",
			parameters.segment_length,
			parameters.image_size,
			parameters.nb_iterations,
			parameters.fg_color,
			parameters.bg_color
			);

	if (parameters.all_images)
		printf("Generer toutes les images intermediaires\n");
	else
		printf("Ne generer que l'image finale\n");

	printf("Nom de sortie : %s\n\n", parameters.outfile);
}

/* Creation de l'image ppm dans un fichier */
void create_image(uint32_t *picture, int32_t size_x, int32_t size_y,
		  char *filename)
{
	FILE *file = fopen(filename, "wb");

	if (file != NULL) {
		const uint32_t LEN = size_x * size_y;
		uint32_t pixel;
		char buffer[3];

		fprintf(
				file,
				"P6\n"
				"%d %d\n"
				"255\n",
				size_x,
				size_y
				);

		for (uint32_t i = 0; i < LEN; i++) {
			pixel = picture[i];

			/* Rouge */
			buffer[0] = pixel >> 16;

			/* Vert */
			buffer[1] = pixel >> 8;

			/* Bleu */
			buffer[2] = pixel;

			fwrite(buffer, 3, 1, file);
		}

		fclose(file);
	} else {
		TRACE(2, "Erreur lors de la generation PPM");
	}
}
