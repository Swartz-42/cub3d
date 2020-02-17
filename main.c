/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlecaill <nlecaill@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/20 17:47:53 by dgascon           #+#    #+#             */
/*   Updated: 2020/02/17 19:49:48 by nlecaill         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3d.h"
#include <Tk/X11/x.h>

int		init_window(t_data *data)
{
	if (!(data->mlx.ptr = mlx_init()))
		return (EXIT_FAILURE);
	if ((data->mlx.win = mlx_new_window(data->mlx.ptr, data->screen.size.x,
			data->screen.size.y, "Dgascon && Nlecaill")) == NULL)
		return (EXIT_FAILURE);
	if (!(data->image.img = mlx_new_image(data->mlx.ptr, data->screen.size.x,
			data->screen.size.y)))
		return (EXIT_FAILURE);
	if (!(data->image.add_image = mlx_get_data_addr(data->image.img,
			&data->image.bpp, &data->image.size_line, &data->image.endian)))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}


//DONE collision avec les sprites
//DONE parsing: ajouter automatiquement les sprites a la liste chaine
//DONE parsing: rendre les valeurs dependante de la BLOCK_SIZE
//DONE gerer le beugue d'affichage des sprites
//DONE regler le probleme de speed quand on court
//DONE rendre la vitesse dependante de la taille de la map
//DONE ralentir la monter et la descente du saut
//DONE gerer le plafond et le avec une block_size differente
//DONE gerer les sprites avec block_size different
//DONE tableau de texture 
//DONE Ajouter une regle makefile debug
//REVIEW Corriger la direction de mouvement
//DONE Si la taille de fenêtre demandée dans la map est plus grande que celle de l’écran, la taille de fenêtre doit être celle de l’écran
//DONE rendre la minimap sans relation avec la blocksize
//DONE changer le sens des texture sud et ouest
//DONE BMP : bitmap
//TODO Key flottante et tournante
//TODO liberer les espaces memoire uttilise par les thread
//TODO rendre case "porte ouverte" transparente 
//TODO les 2 sont des sprites et les 3 sont des portes <=(les 2 sont des portes)
//TODO les * sont des cles (peut etre 1clef 1porte?)
//TODO mettre une condition sur la creation de la fenetre pour --save voir flag data.bmp_save
//TODO remettre les variables en int et ajouter les limites necessaires a la securite 

int main(int ac, char **av)
{
	t_data	data;
	char	*tmp;

	(void)av;
	if (ac <= 1)
		return (ft_msg(TM_ERROR, "Argument is missing !", 1, RED));
	else if (ac > 3)
		return (ft_msg(TM_ERROR, "Too much argument", 1, RED));
	if (ac == 3 && !ft_strcmp("--save", av[2]))
		data.bmp_save = 1;
	else
		data.bmp_save = 0;
	
	data.lst = NULL;
	data.player = (t_player){.fov = M_PI /3, .height_cam = BLOCK_SIZE/2, .speed = MAX_SPEED/2};
	data.screen = (t_screen) {.CF_textured = 1};
	if (parsefile(&data, av[1]))
	{
		destroy(&data);
		return (EXIT_FAILURE);
	}
	// printf("parses_past\n");
	data.raycast = (t_raycast) {.alpha = M_PI / 3,
		.delta_ang = (data.player.fov / data.screen.size.x)};
	data.player.dist_proj_plane = ((float)data.screen.size.x / 2) / tan(data.player.fov / 2);
	data.player.cst = (BLOCK_SIZE * data.player.dist_proj_plane);
	data.player.hdv = data.screen.size.y / 2;
	data.actions = (t_actions) {};
	data.world.door = (t_door) {.locked = 1, .was_lock = 1, .pos.x = 20, .pos.y = 5};
	ft_msg(TM_INFO, tmp = ft_strmjoin("sds", "Number of threads at " ORANGE,
		NB_THREAD, RESET "."), 0, RESET);
	wrfree(tmp); //REVIEW why here?
	mlx_loop_hook(data.mlx.ptr, scan, &data);
	mlx_hook(data.mlx.win, KeyPress, NoEventMask, key_press, &data);
	mlx_hook(data.mlx.win, KeyRelease, NoEventMask, key_release, &data);
	mlx_hook(data.mlx.win, DestroyNotify, NoEventMask, destroy, &data);
	mlx_loop(data.mlx.ptr);
	return (EXIT_SUCCESS);
}