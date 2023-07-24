#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXLENGTH 100
#define SIZE 3

unsigned char header[2][54];
unsigned char imgin[2][3][512][512];
unsigned char imgout[2][3][512][512];

double dtemp[2][3][512][512];
int itemp[2][3][512][512];
double filter[15][15];
int padded_itemp[512 + 100][512 + 100];

void convert(int n);

void get_data(int index);
void rgb_to_ybr(int index);
void processing();
void ybr_to_rgb(int index);
void put_data(int index);

void gen_gauss_fil();
void mirror_padding(int index);
void apply_gauss_fil(int index);
void matching();
void binarization();

int value(int a, int b, int index);
int width[2], height[2], bite[2];

double rgb_con_ybr[3][3] = {
    {0.2990, 0.5870, 0.1140},
    {-0.1687, -0.3313, 0.5000},
    {0.5000, -0.4187, -0.0813}};

double ybr_con_rgb[3][3] = {
    {1.0000, 0.0000, 1.4020},
    {1.0000, -0.3441, -0.7141},
    {1.0000, 1.7720, 0.0000}};

int main()
{
    get_data(0);
    // get_data(1);

    rgb_to_ybr(0);
    // rgb_to_ybr(1);

    processing();

    ybr_to_rgb(0);
    // ybr_to_rgb(1);

    put_data(0);
    // put_data(1);

    return 0;
}

void get_data(int index)
{
    FILE *fp;

    char filename[MAXLENGTH];
    int i, c, x, y, j;
    int filesize, offset, bite_pixel;

    if (index == 0)
    {
        printf("<注目画像(入力)>\n");
        printf("ファイル名を入力して下さい:");
        scanf("%s", filename);
    }
    else
    {
        printf("<参照画像(入力)>\n");
        printf("ファイル名を入力して下さい:");
        scanf("%s", filename);
    }

    fp = fopen(filename, "rb");

    if (fp == NULL)
    {
        printf("ファイルをオーブンできません.\n");
        exit(1);
    }

    printf("ファイルをオーブンしました.\n");

    for (i = 0; i < 54; i++)
    {
        c = fgetc(fp);
        header[index][i] = (unsigned char)c;
    }

    filesize = value(5, 2, index);

    offset = value(13, 10, index),

    width[index] = value(21, 18, index);

    height[index] = value(25, 22, index);

    bite_pixel = value(29, 28, index);

    bite[index] = (offset + width[index] * height[index] * (bite_pixel / 8)) % 4;

    for (y = height[index] - 1; y >= 0; y--)
    {
        for (x = 0; x < width[index]; x++)
        {
            for (i = 2; i >= 0; i--)
            {
                c = fgetc(fp);
                imgin[index][i][x][y] = (unsigned char)c;
            }
        }
    }
    fclose(fp);

    // printf("--- width : %d\n---height : %d\n", width[index], height[index]);
    printf("\n%s をクローズしました.\n", filename);
}

void convert(int n)
{
    printf("%02x", n);
}

int value(int a, int b, int index)
{
    int i;
    int value;
    value = header[index][a];
    for (i = a - 1; i >= b; i--)
    {
        value <<= 8;
        value += header[index][i];
    }
    return value;
}

void processing()
{
    binarization();
}

void binarization()
{
    mirror_padding(0);
    int x, y, i, j, block, h_block, sum, offset, ave;
    offset = 0;
    block = 25; // <= 51
    h_block = block / 2;

    for (y = 0; y < height[0]; y++)
    {
        for (x = 0; x < width[0]; x++)
        {
            imgout[0][0][x][y] = imgin[0][0][x][y];
            imgout[0][1][x][y] = imgin[0][1][x][y];
            imgout[0][2][x][y] = imgin[0][2][x][y];
        }
    }

    for (y = 50; y < height[0] + 50; y++)
    {
        for (x = 50; x < width[0] + 50; x++)
        {
            sum = 0;
            for (i = -h_block; i < h_block; i++)
            {
                for (j = -h_block; j < h_block; j++)
                {
                    sum += padded_itemp[x + j][y + i];
                }
            }

            if (imgin[0][0][x - 50][y - 50] < (sum / (block * block) - offset))
            {
                imgout[0][0][x - 50][y - 50] = 0;
            }
            else
            {
                imgout[0][0][x - 50][y - 50] = 255;
            }
        }
    }
}

void matching()
{
    int i, j, x, y;
    int x_start, y_start;
    int index;
    int diff_abs_sum[width[1] - 15][height[1] - 15];
    int min;
    int min_index[2]; // 0 = width, 1 = height

    printf("<パラメータ入力>\n対象ブロックの左上端画素の位置を入力して下さい\n");
    printf("--- 水平: ");
    scanf("%d", &x_start);
    printf("--- 垂直: ");
    scanf("%d", &y_start);

    printf("マッチングを開始します\n");

    for (index = 0; index < 2; index++)
    {
        for (y = 0; y < height[index]; y++)
        {
            for (x = 0; x < width[index]; x++)
            {
                for (i = 0; i < 3; i++)
                {
                    imgout[index][i][x][y] = imgin[index][i][x][y];
                }
            }
        }
    }

    for (y = y_start; y < y_start + 16; y++)
    {
        for (x = x_start; x < x_start + 16; x++)
        {
            if ((x == x_start || x == x_start + 15) || (y == y_start || y == y_start + 15))
            {
                imgout[0][1][x][y] = 128;
                imgout[0][2][x][y] = 255;
            }
        }
    }

    for (y = 0; y < height[1] - 15; y++)
    {
        for (x = 0; x < width[1] - 15; x++)
        {
            diff_abs_sum[x][y] = 0;

            for (i = 0; i < 16; i++)
            {
                for (j = 0; j < 16; j++)
                {
                    diff_abs_sum[x][y] += abs(imgin[1][0][x + j][y + i] - imgin[0][0][x_start + j][y_start + i]);
                }
            }
        }
    }

    min = diff_abs_sum[0][0];
    min_index[0] = 0;
    min_index[1] = 0;
    for (y = 0; y < height[1] - 15; y++)
    {
        for (x = 0; x < width[1] - 15; x++)
        {
            if (diff_abs_sum[x][y] < min)
            {
                min = diff_abs_sum[x][y];
                min_index[0] = x;
                min_index[1] = y;
            }
        }
    }

    printf("<マッチング結果>\nマッチング位置: (%d, %d)\nマッチング誤差: %d\n", min_index[0], min_index[1], diff_abs_sum[min_index[0]][min_index[1]]);

    for (y = min_index[1]; y < min_index[1] + 16; y++)
    {
        for (x = min_index[0]; x < min_index[0] + 16; x++)
        {
            if ((x == min_index[0] || x == min_index[0] + 15) | (y == min_index[1] || y == min_index[1] + 15))
            {
                imgout[1][1][x][y] = 128;
                imgout[1][2][x][y] = 255;
            }
        }
    }
    FILE *fp;

    fp = fopen("test.txt", "w");

    if (fp == NULL)
    {
        printf("ファイルをオーブンできません.\n");
        exit(1);
    }
    printf("ファイルをオープンしました.\n");

    for (y = 0; y < height[1] - 15; y++)
    {
        for (x = 0; x < width[1] - 15; x++)
        {
            fprintf(fp, "%d,", diff_abs_sum[x][y]);
        }
        fprintf(fp, "\n");
    }
    printf("ファイルに書き出しました.\n");
    fclose(fp);
    printf("ファイルをクローズしました.\n");
}

void gen_gauss_fil()
{

    int x, fx, y, fy;
    int fw = 15, fh = 15;
    int w = fw / 2, h = fh / 2;
    double par = 1.7;
    double sum = 0.0;

    for (fy = 0; fy < fh; fy++)
    {
        for (fx = 0; fx < fw; fx++)
        {
            filter[fx][fy] = exp(-(pow(fx - w, 2) + pow(fy - h, 2)) / (2 * pow(par, 2))) / 2 * M_PI * pow(par, 2);
            sum += filter[fx][fy];
        }
    }

    for (fy = 0; fy < fh; fy++)
    {
        for (fx = 0; fx < fw; fx++)
        {
            filter[fx][fy] /= sum;
        }
    }
    printf("Generating\n");
}

void mirror_padding(int index)
{
    int x, y, i;

    for (y = 0; y < height[index]; y++)
    {
        for (x = 0; x < width[index]; x++)
        {
            padded_itemp[x + 50][y + 50] = imgin[index][0][x][y];
        }
    }

    for (y = 50; y < height[index] + 50; y++)
    {
        for (i = 0; i < 50; i++)
        {
            padded_itemp[i][y] = padded_itemp[2 * 50 - i][y];                                   // left side
            padded_itemp[width[index] + 2 * 50 - i - 1][y] = padded_itemp[width[index] + i][y]; // right side
        }
    }

    for (x = 0; x < width[index] + 2 * 50; x++)
    {
        for (i = 0; i < 50; i++)
        {
            padded_itemp[x][i] = padded_itemp[x][2 * 50 - i];                                     // top side
            padded_itemp[x][height[index] + 2 * 50 - i - 1] = padded_itemp[x][height[index] + i]; // bottom side
        }
    }
    printf("padding\n");
}

void apply_gauss_fil(int index)
{
    int x, y, fx, fy;
    double sum;

    gen_gauss_fil();
    mirror_padding(index);

    // Applying the gaussian filter.
    for (y = 7; y < height[index] + 7; y++)
    {
        for (x = 7; x < width[index] + 7; x++)
        {
            sum = 0.0;
            for (fy = -7; fy <= 7; fy++)
            {
                for (fx = -7; fx <= 7; fx++)
                {
                    sum += padded_itemp[x + fx][y + fy] * filter[fx + 7][fy + 7];
                }
            }

            // Clipping the output to be in the range of 0-255.
            if (sum > 255.0)
            {
                sum = 255.0;
            }
            else if (sum < 0.0)
            {
                sum = 0.0;
            }

            imgout[index][0][x - 7][y - 7] = (unsigned char)sum + 0.5;
            imgout[index][1][x - 7][y - 7] = 128;
            imgout[index][2][x - 7][y - 7] = 128;
        }
    }
    printf("apply\n");
}

void put_data(int index)
{
    FILE *fp;
    char filename[MAXLENGTH];
    int i, x, y;

    if (index == 0)
    {
        printf("<注目画像(出力)>\n");
        printf("ファイル名を入力して下さい:");
        scanf("%s", filename);
    }
    else
    {
        printf("<参照画像(出力)>\n");
        printf("ファイル名を入力して下さい:");
        scanf("%s", filename);
    }

    fp = fopen(filename, "wb");

    printf("%sをオープンしました.\n", filename);

    for (i = 0; i < 54; i++)
    {
        fputc(header[index][i], fp);
    }

    for (y = height[index] - 1; y >= 0; y--)
    {
        for (x = 0; x < width[index]; x++)
        {
            for (i = 2; i >= 0; i--)
            {
                fputc(imgout[index][i][x][y], fp);
            }
        }
    }

    for (i = 0; i < bite[index]; i++)
    {
        fputc('\0', fp);
    }

    fclose(fp);

    printf("%sをクローズしました.\n", filename);
}

void rgb_to_ybr(int index)
{
    int i, x, y, j;

    for (y = 0; y < height[index]; y++)
    {
        for (x = 0; x < width[index]; x++)
        {
            for (i = 0; i < 3; i++)
            {
                dtemp[index][i][x][y] = 0.0;
                for (j = 0; j < 3; j++)
                {
                    dtemp[index][i][x][y] += rgb_con_ybr[i][j] * (double)imgin[index][j][x][y];
                }
            }
        }
    }

    for (y = 0; y < height[index]; y++)
    {
        for (x = 0; x < width[index]; x++)
        {
            for (i = 0; i < 3; i++)
            {

                if (dtemp[index][i][x][y] > 0.0)
                {
                    itemp[index][i][x][y] = (int)(dtemp[index][i][x][y] + 0.5);
                }
                else
                {
                    itemp[index][i][x][y] = (int)(dtemp[index][i][x][y] - 0.5);
                }

                if (i != 0)
                {
                    itemp[index][i][x][y] += 128;
                }

                if (itemp[index][i][x][y] > 255)
                {
                    itemp[index][i][x][y] = 255;
                }
                else if (itemp[index][i][x][y] < 0)
                {
                    itemp[index][i][x][y] = 0;
                }

                imgin[index][i][x][y] = (unsigned char)itemp[index][i][x][y];
            }
        }
    }
}

void ybr_to_rgb(int index)
{
    int i, x, y, j;

    for (y = 0; y < height[index]; y++)
    {
        for (x = 0; x < width[index]; x++)
        {
            for (i = 0; i < 3; i++)
            {
                dtemp[index][i][x][y] = 0.0;
                for (j = 0; j < 3; j++)
                    if (j == 0)
                        dtemp[index][i][x][y] += ybr_con_rgb[i][j] * (double)imgout[index][j][x][y];
                    else
                        dtemp[index][i][x][y] += ybr_con_rgb[i][j] * (double)(imgout[index][j][x][y] - 128);
            }
        }
    }

    for (y = 0; y < height[index]; y++)
    {
        for (x = 0; x < width[index]; x++)
        {
            for (i = 0; i < 3; i++)
            {

                if (dtemp[index][i][x][y] > 0.0)
                {
                    itemp[index][i][x][y] = (int)(dtemp[index][i][x][y] + 0.5);
                }
                else
                {
                    itemp[index][i][x][y] = (int)(dtemp[index][i][x][y] - 0.5);
                }

                if (itemp[index][i][x][y] > 255)
                {
                    itemp[index][i][x][y] = 255;
                }
                else if (itemp[index][i][x][y] < 0)
                {
                    itemp[index][i][x][y] = 0;
                }

                imgout[index][i][x][y] = (unsigned char)itemp[index][i][x][y];
            }
        }
    }
}
