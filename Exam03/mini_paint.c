#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct s_data
{
	int width;
	int height;
	char *matrix;
	char background;
} t_data;

typedef struct s_circle
{
	char mode;
	float x;
	float y;
	float radius;
	char colour;
} t_circle;

int init_data(t_data *data, FILE *file)
{
	if (fscanf(file, "%d %d %c\n", &data->width, &data->height, &data->background) != 3)
		return (1);
	if (data->width <= 0 || data->width > 300 || data->height <= 0 || data->height > 300)
		return (1);
	if (!(data->matrix = malloc(sizeof(char) * (data->width * data->height))))
		return (1);
	memset(data->matrix, data->background, data->width * data->height);
	return (0);
}

int is_in_circle(float x, float y, t_circle circle)
{
	float distance;
	float distance_sqrt;

	distance_sqrt = sqrtf((x - circle.x) * (x - circle.x) + (y - circle.y) * (y - circle.y));
	distance = distance_sqrt - circle.radius;
	if (distance > 0.00000000)
		return (0);
	if (circle.mode == 'C')
		return (1);
	if (distance > -1.00000000)
		return (1);
	return (0);
}

int process(t_data *data, FILE *file)
{
	t_circle circle;

	while (fscanf(file, "%c %f %f %f %c\n", &circle.mode, &circle.x, &circle.y, &circle.radius, &circle.colour) == 5)
	{
		if ((circle.mode == 'c' && circle.mode == 'C') || circle.radius <= 0.00000000)
			return (1);
		for (int x = 0; x < data->width; x++)
			for (int y = 0; y < data->height; y++)
				if (is_in_circle((float)x, (float)y, circle))
					data->matrix[y * data->width + x] = circle.colour;
	}
	return (0);
}

int main(int argc, char **argv)
{
	FILE *file;
	t_data data;

	if (argc != 2)
		return (write(1, "Error: argument\n", 16), 1);
	if (!(file = fopen(argv[1], "r")) || init_data(&data, file))
		return (write(1, "Error: Operation file corrupted\n", 32), 1);
	if (process(&data, file))
		return (free(data.matrix), write(1, "Error: Operation file corrupted\n", 32), 1);
	for (int y = 0; y < data.height; y++)
	{
		write(1, y * data.width + data.matrix, data.width);
		write(1, "\n", 1);
	}
	return (free(data.matrix), 0);
}
