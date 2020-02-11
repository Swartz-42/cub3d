/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   main.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nlecaill <nlecaill@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/20 17:47:53 by dgascon      #+#   ##    ##    #+#       */
/*   Updated: 2020/02/11 14:51:32 by nlecaill    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "includes/cub3d.h"
#include <Tk/X11/x.h>

int init_texture(t_data* data)
{
	if (!(data->image.img = mlx_new_image(data->mlx.ptr, data->screen.size.x, data->screen.size.y)))
		return (EXIT_FAILURE);
	if (!(data->image.add_image = mlx_get_data_addr(data->image.img, &data->image.bpp, &data->image.size_line, &data->image.endian)))
		return (EXIT_FAILURE);
	if (!(data->Wtex.img = mlx_xpm_file_to_image(data->mlx.ptr, "assets/images/wood.xpm", &data->Wtex.sizex, &data->Wtex.sizey)))
		return (printf("erreur1"));
	if (!(data->Wtex.add_image = mlx_get_data_addr(data->Wtex.img, &data->Wtex.bpp, &data->Wtex.size_line, &data->Wtex.endian)))
		return (printf("erreur2"));
	if (!(data->Ftex.img = mlx_xpm_file_to_image(data->mlx.ptr, "assets/images/vitrail-3.xpm", &data->Ftex.sizex, &data->Ftex.sizey)))
		return (printf("erreur1"));
	if (!(data->Ftex.add_image = mlx_get_data_addr(data->Ftex.img, &data->Ftex.bpp, &data->Ftex.size_line, &data->Ftex.endian)))
		return (printf("erreur2"));
	if (!(data->Rtex.img = mlx_xpm_file_to_image(data->mlx.ptr, "assets/images/ice.xpm", &data->Rtex.sizex, &data->Rtex.sizey)))
		return (printf("erreur1"));
	if (!(data->Rtex.add_image = mlx_get_data_addr(data->Rtex.img, &data->Rtex.bpp, &data->Rtex.size_line, &data->Rtex.endian)))
		return (printf("erreur2"));
	if (!(data->Vtex.img = mlx_xpm_file_to_image(data->mlx.ptr, "assets/images/viseur.xpm", &data->Vtex.sizex, &data->Vtex.sizey)))
		return (printf("erreur1"));
	if (!(data->Vtex.add_image = mlx_get_data_addr(data->Vtex.img, &data->Vtex.bpp, &data->Vtex.size_line, &data->Vtex.endian)))
		return (printf("erreur2"));
	return (0);
}

int		init_window(t_data *data)
{
	if (!(data->mlx.ptr = mlx_init()))
		return (EXIT_FAILURE);
	if ((data->mlx.win = mlx_new_window(data->mlx.ptr, data->screen.size.x,
						data->screen.size.y, "Dgascon && Nlecaill")) == NULL)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}


//TODO collision avec les sprites
//TODO parsing: ajouter automatiquement les sprites a la liste chaine
//TODO parsing: rendre les valeurs dependante de la BLOCK_SIZE
//DONE gerer le beugue d'affichage des sprites
//TODO regler le probleme de speed quand on court
//TODO rendre la vitesse dependante de la taille de la map
//TODO ralentir la monter et la descente du saut
//DONE gerer le plafond et le avec une block_size differente
//DONE gerer les sprites avec block_size different
//TODO tableau de texture 
//TODO Ajouter une regle makefile debug
//TODO Corriger la direction de mouvement

int main(int ac, char **av)
{
	t_data data;

	(void)av;
	if (ac <= 1)
	{
		ft_printf("Map manquante !");
		return (EXIT_FAILURE);
	} 
	data.lst = NULL;
	data.player = (t_player){.fov = M_PI /3, .height_cam = BLOCK_SIZE/2, .speed = MAX_SPEED/2};
	if (parsefile(&data, av[1]) <= 0)
		return (EXIT_FAILURE);
	data.raycast = (t_raycast) {.alpha = M_PI / 3, .delta_ang = (data.player.fov / data.screen.size.x)};
	// data.image = (t_image) {.bpp = 32,
	// .size_line = data.image.bpp * data.screen.size.x, .endian = 0};
	data.player.dist_proj_plane = ((float)data.screen.size.x / 2) / tan(data.player.fov / 2);
	data.player.CST = (BLOCK_SIZE * data.player.dist_proj_plane);
	data.player.hdv = data.screen.size.y / 2;
	data.actions = (t_actions){};
	data.world.locked = 1;
	data.world.was_lock = 1;
	if (init_texture(&data) != 0)
		return (EXIT_FAILURE);
	mlx_loop_hook(data.mlx.ptr, scan, &data);
	mlx_hook(data.mlx.win, KeyPress, NoEventMask, key_press, &data);
	mlx_hook(data.mlx.win, KeyRelease, NoEventMask, key_release, &data);
	mlx_hook(data.mlx.win, DestroyNotify, NoEventMask, destroy, &data);
	mlx_loop(data.mlx.ptr);
	return (EXIT_SUCCESS);
}