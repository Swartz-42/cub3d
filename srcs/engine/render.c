/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlecaill <nlecaill@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/20 17:58:25 by dgascon           #+#    #+#             */
/*   Updated: 2020/02/17 11:44:30 by nlecaill         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int		select_wall_color(t_data *data, float prptn, int wall_row, int dir)
{
	t_coord	ratio;
	float	proportion;

	proportion = (float)data->w_tex[dir].size.x / BLOCK_SIZE;
	ratio.y = (int)(prptn * wall_row) % data->w_tex[dir].size.y;
	if (data->raycast.face_detect == 'V')
	{
		ratio.x = (int)(data->raycast.inter.y * proportion)
			% data->w_tex[dir].size.x;
	}
	else
	{
		ratio.x = (int)(data->raycast.inter.x * proportion)
			% data->w_tex[dir].size.x;
	}
	if (dir >= 2 && dir <= 3)
			ratio.x = data->w_tex[dir].size.x - ratio.x;
	return (*(int*)(data->w_tex[dir].add_image
	+ (data->w_tex[dir].size_line * ratio.y) + (ratio.x * sizeof(int))));
}

int		select_sprite_color(t_f_coord offset, int wall_row, t_image sprite, t_f_coord lim)
{
	t_coord ratio;

	ratio.y = (int)(((float)sprite.size.y / lim.y)
			* wall_row) % sprite.size.y;
	ratio.x = (int)(((float)sprite.size.x / lim.x)
			* (lim.x / 2 - offset.x )) % sprite.size.x;
	return (*(int*)(sprite.add_image
	+ (sprite.size_line * ratio.y) + (ratio.x * sizeof(int))));
}


void	print_sprite(t_data *data)
{
	int				row;
	t_f_coord		lim;
	t_f_coord		offset_mid_object;
	unsigned int	val;
	float			angle_raycast_mid_obj;
	int				cmp;
	t_lsprite		*list;

	cmp = 0;
	list = data->lst;
	while (list != NULL)
	{
		if (list->visible[data->th_num] == 1)
		{
			angle_raycast_mid_obj = data->raycast.alpha - ((M_PI_2 - atanf((float)(list->pos.y - data->player.pos.y) / (list->pos.x - data->player.pos.x ))) + M_PI_2);
			offset_mid_object.x = tanf(angle_raycast_mid_obj) * list->dist;
			offset_mid_object.x *= (data->player.dist_proj_plane / list->dist);
			lim.x = BLOCK_SIZE / 2 * (data->player.dist_proj_plane / list->dist); // taille .x a l'ecran
			if (offset_mid_object.x < lim.x / 2 && (list->dist * cosf(data->raycast.beta)) < data->raycast.dist
			&& offset_mid_object.x > -(lim.x / 2))
			{
				lim.y = BLOCK_SIZE / 2 * (data->player.dist_proj_plane/list->dist); //taille en y de la texture a l'ecran
				row = data->player.hdv + ((data->player.height_cam - BLOCK_SIZE / 2) / list->dist) * data->player.dist_proj_plane; //- (data->player.hdv/2 - (float)data->player.hdv/2 / ((float)BLOCK_SIZE  / data->player.height_cam));//POSITION DE DEPART
				while (cmp < lim.y && row < data->screen.size.y)
				{
					val = select_sprite_color(offset_mid_object, cmp, list->texture, lim);
					if (val != 0xff000000 && row > 0)
						*(int*)(data->image.add_image + (row * data->image.size_line) + data->raycast.column * sizeof(int)) = val;
					row++;
					cmp++;
				}
			}
			cmp = 0;
			list->visible[data->th_num] = 0;		
		}
		list = list->next;
	}
}

int		fill_column(t_data *data, int direction)
{
	int		height_proj_plane;
	int		row;
	int		wall_row = 0;
	char	*add_opp;
	int		gnagna;
	int		h_max;
	
/*	if (data->raycast.end == 0 && data->raycast.column == data->screen.size.x/2)
		printf("render by thread\n");
	else if (data->raycast.column == data->screen.size.x/2)
	{
		printf("rendre by main\n");
	}
		*/
	add_opp = data->image.add_image + (data->raycast.column * sizeof(int));
	height_proj_plane = floorf(data->player.cst / data->raycast.dist); //REVIEW Optimisation
	gnagna = (float)height_proj_plane / ((float)BLOCK_SIZE / data->player.height_cam); //hauteur sur ratio de la hauteur de la camera 
	row = data->player.hdv - (height_proj_plane - gnagna);

	if (row < 0)
	{
		wall_row = 0 - row;
		row = 0;
	}
	h_max = data->player.hdv + gnagna;
	if (h_max > data->screen.size.y)
	{
		h_max = data->screen.size.y;
	}
	float racourcis = (float)data->w_tex[direction].size.y / height_proj_plane;
	if (!data->screen.CF_textured)
		fill_background(data);
	while (row < h_max)
	{
		*(int*)(add_opp + (row * data->image.size_line)) = select_wall_color(data, racourcis, wall_row, direction);
		row++;
		wall_row++;
	}
	//TODO faire un recap de toute les variable (surtout les alpha beta gamma)
	if (data->screen.CF_textured)
		pt_floor_ceil(data, row, gnagna, height_proj_plane, h_max);
	print_sprite(data);
	return (0);
}
