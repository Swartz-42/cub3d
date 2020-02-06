#include "cub3d.h"

int move_x(t_data *data, int value)
{
	int tmp;

	tmp = data->player.pos.x + value;
	if (tmp < (data->world.size.x * BLOCK_SIZE)
	&&	tmp > BLOCK_SIZE
	&&	data->world.map[data->player.pos.y / BLOCK_SIZE][(tmp / BLOCK_SIZE)] != '1')
			data->player.pos.x = tmp;
	return (0);
}
int move_y(t_data *data, int value)
{
	int tmp;

	tmp = data->player.pos.y + value;
	if (tmp < (data->world.size.y * BLOCK_SIZE)
	&&	tmp > BLOCK_SIZE
	&&	data->world.map[tmp / BLOCK_SIZE][data->player.pos.x / BLOCK_SIZE] != '1')
			data->player.pos.y = tmp;
	return (0);
}

void	move(t_data *data, float direction)
{
	move_x(data, cosf(direction) * data->player.speed / 2); 
	move_y(data, sinf(direction) * -data->player.speed / 2);
}