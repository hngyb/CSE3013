#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int WIDTH;
int HEIGHT;
int *horizontalWall;
int *verticalWall;
int *maze;
int ID = 1;

FILE *fp;

void draw_wall()
{
    int i;
    for (i = 0; i < WIDTH; i++)
    {
        fprintf(fp, "+-");
        printf("+-");
    }
    fprintf(fp, "+\n");
    printf("+\n");
}

void draw_verticalWall()
{
    int i;
    fprintf(fp, "|");
    printf("|");

    for (i = 0; i < WIDTH - 1; i++)
    {
        fprintf(fp, " ");
        printf(" ");

        if (verticalWall[i])
        {
            fprintf(fp, "|");
            printf("|");
        }
        else
        {
            fprintf(fp, " ");
            printf(" ");
        }
    }
    fprintf(fp, " |\n");
    printf(" |\n");
}

void draw_horizontalWall()
{
    int i;

    fprintf(fp, "+");
    printf("+");
    for (i = 0; i < WIDTH; i++)
    {
        if (horizontalWall[i])
        {
            fprintf(fp, "-+");
            printf("-+");
        }
        else
        {
            fprintf(fp, " +");
            printf(" +");
        }
    }
    fprintf(fp, "\n");
    printf("\n");
}

void vertical_join()
{
    int i;
    int join_flag = 0;
    int prev_mazeID = maze[0];

    for (i = 0; i < WIDTH; i++)
    {
        horizontalWall[i] = rand() % 2;
        if (!horizontalWall[i])
        {
            join_flag = 1;
        }

        if ((i < WIDTH - 1) && (prev_mazeID != maze[i + 1]))
        {
            if (!join_flag)
            {
                horizontalWall[i] = 0;
            }
            else
            {
                join_flag = 0;
            }
            prev_mazeID = maze[i + 1];
        }

        if ((i == WIDTH - 1) && (!join_flag))
        {
            horizontalWall[i] = 0;
        }

        if (horizontalWall[i])
        {
            maze[i] = ID;
            ID++;
        }
    }
}

void horizontal_join()
{
    int i, j;
    int prev_mazeID;

    for (i = 0; i < WIDTH - 1; i++)
    {
        if (maze[i] != maze[i + 1])
        {
            verticalWall[i] = rand() % 2;
            if (!verticalWall[i])
            {
                prev_mazeID = maze[i + 1];
                maze[i + 1] = maze[i];
                for (j = 0; j < WIDTH; j++)
                {
                    if (maze[j] == prev_mazeID)
                    {
                        maze[j] = maze[i];
                    }
                }
            }
        }
        else
        {
            verticalWall[i] = 1;
        }
    }
}

void make_firstLine()
{
    int i, j;
    int prevWall = -1;

    for (i = 0; i < WIDTH - 1; i++)
    {
        verticalWall[i] = rand() % 2;
        if (verticalWall[i])
        {
            for (j = prevWall + 1; j <= i; j++)
            {
                maze[j] = ID;
            }
            prevWall = i;
            ID++;
        }
    }
    for (i = prevWall + 1; i < WIDTH; i++)
    {
        maze[i] = ID;
    }
    ID++;
}

void make_middleLines()
{
    int i;
    for (i = 1; i < HEIGHT - 1; i++)
    {
        vertical_join();
        draw_horizontalWall();
        horizontal_join();
        draw_verticalWall();
    }
}

void make_lastLine()
{
    int i, j;
    int prev_mazeID;

    vertical_join();
    draw_horizontalWall();

    for (i = 0; i < WIDTH - 1; i++)
    {
        if (maze[i] != maze[i + 1])
        {
            verticalWall[i] = 0;
            prev_mazeID = maze[i + 1];
            maze[i + 1] = maze[i];
            for (j = 0; j < WIDTH; j++)
            {
                if (maze[j] == prev_mazeID)
                {
                    maze[j] = maze[i];
                }
            }
        }
        else
        {
            verticalWall[i] = 1;
        }
    }

    draw_verticalWall();
}

int main()
{
    fp = fopen("maze.maz", "wt");
    printf("Width: ");
    scanf("%d", &WIDTH);
    printf("Height: ");
    scanf("%d", &HEIGHT);

    horizontalWall = (int *)malloc(sizeof(int) * WIDTH);
    verticalWall = (int *)malloc(sizeof(int) * (WIDTH - 1));
    maze = (int *)calloc(WIDTH, sizeof(int));

    draw_wall();
    srand(time(NULL));
    make_firstLine();
    draw_verticalWall();

    make_middleLines();

    make_lastLine();
    draw_wall();

    fclose(fp);

    return 0;
}