
#include "koch_common.h"
#include "koch_fonctions.h"
#include "koch_ihm.h"

#ifdef PSP2
#include <psp2/ctrl.h>
#include <psp2/touch.h>
#include <psp2/display.h>
#include <psp2/gxm.h>
#include <psp2/types.h>
#include <psp2/moduleinfo.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/dirent.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/kernel/sysmem.h>
#include <vita2d.h>

PSP2_MODULE_INFO(0, 0, "koch");

#define PRESSED(b, a, m) (((b & m) == 0) && ((a & m) == m))

#endif


/* Programme "Flocon de Koch" */
int main(int argc, char *argv[])
{
	struct parameters parameters;
	struct list *koch = NULL;
	uint32_t *picture = NULL;
	char *outfile = NULL;
	bool screenshots = false;

	srand(time(NULL));

#ifdef PSP2
	vita2d_init();
	vita2d_set_clear_color(RGBA8(0, 0, 0, 0xFF));

	vita2d_texture *tex = vita2d_create_empty_texture(960, 544);
	uint32_t *tex_data = vita2d_texture_get_datap(tex);

	SceCtrlData pad, lastbuttons;
	memset(&pad, 0, sizeof(pad));
#endif


	/* Initialisations */
	init_parameters(&parameters, argc, argv);
	show_parameters(parameters);
	init_koch(&koch, parameters.image_size, parameters.segment_length);
	outfile = malloc(3 + strlen(parameters.outfile) + 1);


	int32_t i = 0, step = 1;
	bool done = false;

	/* Génération de chaque flocon */
	while (!done)
	{
#ifdef PSP2
		lastbuttons = pad;

		sceCtrlPeekBufferPositive(0, &pad, 1);
		if (pad.buttons & PSP2_CTRL_TRIANGLE) break;


		if (PRESSED(lastbuttons.buttons, pad.buttons, PSP2_CTRL_LTRIGGER))
			parameters.fg_color = RGBA8(rand()%255, rand()%255, rand()%255, 255);

		//if (PRESSED(lastbuttons.buttons, pad.buttons, PSP2_CTRL_RTRIGGER))
		//	screenshots ^= 1;

		vita2d_start_drawing();
		vita2d_clear_screen();
#endif
		sprintf(outfile, "%02d_%s", i, parameters.outfile);

		if (step > 0)
			generer_koch(koch, i);
		else {

			for (uint32_t j = 0; j < i; j++)
				generer_koch(koch, j);
		}
		//show_koch_list(koch);

		/* Ne générer l'image que si c'est la dernière ou qu'on les génère toutes */
		if (parameters.all_images || i == parameters.nb_iterations)
		{
			init_picture(&picture, parameters.image_size, parameters.bg_color);

			render_image_bresenham(picture, koch, parameters.image_size, parameters.fg_color);

			#ifndef PSP2

			const uint32_t LEN = 960 * 544;

			uint32_t *data = calloc(LEN, sizeof(uint32_t));

			for (uint32_t i = 0; i < LEN; i++)
				data[i] = 0;


			uint32_t k = 0;

			for (uint32_t i = 0; i < 544; i++)
				for (uint32_t j = 0; j < 960; j++) {

					if (208 <= j && j < 752 && k < 544*544)
						data[i * 960 + j] = picture[k++];

					else
						data[i * 960 + j] = 0;
				}

			create_image(data, 960, 544, outfile);
			//create_image(picture, parameters.image_size, parameters.image_size, outfile);
			#else

			uint32_t k = 0;
			for (uint32_t i = 0; i < 544; i++)
				for (uint32_t j = 0; j < 960; j++) {

					if (208 <= j && j < 752 && k < 544*544)
						tex_data[i * 960 + j] = picture[k++];

					else
						tex_data[i * 960 + j] = 0;
				}

			if (screenshots)
				create_image(tex_data, 960, 544, outfile);

			#endif
		}


#ifdef PSP2
		vita2d_draw_texture(tex, 0, 0);
		vita2d_end_drawing();
		vita2d_swap_buffers();
		sceKernelDelayThread(100000);
#endif



		i = i + step;


		if (i == parameters.nb_iterations)
#ifdef PSP2
			step = -1;
#else
			done = true;
#endif

		else if (i == 0) {
			step = 1;
			free_koch(koch);
			init_koch(&koch, parameters.image_size, parameters.segment_length);
		}

		if (step < 0) {
			free_koch(koch);
			init_koch(&koch, parameters.image_size, parameters.segment_length);
		}

		if (parameters.nb_iterations == 1)
			done = true;
	}

	/* Libérations mémoire */
	SAFE_FREE(outfile);
	SAFE_FREE(picture);
	SAFE_FREE(parameters.outfile);


#ifdef PSP2
	vita2d_fini();
	vita2d_free_texture(tex);


	sceKernelExitProcess(0);
#endif

	return EXIT_SUCCESS;
}
