#include <GL/glut.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#define _USE_MATH_DEFINES
// #define M_PI 3.1415

// already converted
#define WALL_WIDTH 0.01875f
#define DOOR_FRAME_WIDTH 0.00375f

// not converted
#define DOOR_THICKNESS 0.06f
#define DOOR_WIDTH 0.8f

float convert(float x)
{
    return x / 8;
}

void drawFilledRectangle(float x1, float y1, float x2, float y2)
{
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x1, y2);
    glVertex2f(x2, y2);
    glVertex2f(x2, y1);
    glEnd();
}
void drawRectangle(float x1, float y1, float x2, float y2)
{
    glBegin(GL_LINE_LOOP);
    glVertex2f(x1, y1);
    glVertex2f(x1, y2);
    glVertex2f(x2, y2);
    glVertex2f(x2, y1);
    glEnd();
}

void drawVerticalWall(float x, float y, float length)
{
    x = convert(x);
    y = convert(y);
    length = convert(length);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x, y + length);
    glVertex2f(x + WALL_WIDTH, y + length);
    glVertex2f(x + WALL_WIDTH, y);
    glEnd();
}

void drawWindRose(float x, float y)
{
    float radius = convert(0.5);
    float angle = M_PI / 4;
    float angle_increment = M_PI / 2;
    float x1, y1, x2, y2;
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 4; i++)
    {
        x1 = x + cos(angle) * radius;
        y1 = y + sin(angle) * radius;
        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex2f(x1, y1);

        x1 = x + cos(angle + angle_increment / 2) * radius * 0.3;
        y1 = y + sin(angle + angle_increment / 2) * radius * 0.3;
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex2f(x1, y1);
        // x1 = x + cos(angle + M_PI) * radius * 0.9;
        // y1 = y + sin(angle + M_PI) * radius * 0.9;
        // glColor3f(0.1f, 0.1f, 0.1f);
        // glVertex2f(x1, y1);
        angle += angle_increment;
    }
    glEnd();
    angle = 0;
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 4; i++)
    {
        x1 = x + cos(angle) * radius;
        y1 = y + sin(angle) * radius;
        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex2f(x1, y1);

        x1 = x + cos(angle + angle_increment / 2) * radius * 0.3;
        y1 = y + sin(angle + angle_increment / 2) * radius * 0.3;
        glVertex2f(x1, y1);
        // x1 = x + cos(angle + M_PI) * radius * 0.9;
        // y1 = y + sin(angle + M_PI) * radius * 0.9;
        // glColor3f(0.1f, 0.1f, 0.1f);
        // glVertex2f(x1, y1);
        angle += angle_increment;
    }
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(x, y + radius);
    glVertex2f(x, y - radius);
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(x + radius, y);
    glVertex2f(x - radius, y);
    glEnd();

    float fontsize = 0.0001;
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(x - .016, y + .08);
    glPushMatrix();
    glScalef(fontsize, fontsize, 1);

    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'N');
    glPopMatrix();
}

void drawCircle(float x, float y, float r, int direction)
{
    glColor3f(1, 0, 0);
    GLfloat angle;
    GLfloat circle_points = 100.0f;
    glBegin(GL_LINE_STRIP);

    for (int i = 0; i < circle_points; i++)
    {
        angle = 2 * M_PI * i / circle_points;
        if (angle > M_PI / 2)
        {
            break;
        }
        if (direction)
            glVertex2f(x + cos(angle) * r, y - sin(angle) * r);
        else
            glVertex2f(x + cos(angle) * r, y + sin(angle) * r);
    }
    glEnd();
}

void draw_horizontal_door(float x1, float y1, float x2, float y2, int direction)
{
    float door_width = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    // remove wall
    glColor3f(0.0f, 0.0f, 0.0f);
    drawFilledRectangle(x1 - DOOR_FRAME_WIDTH, y1 + .01, x2 + DOOR_FRAME_WIDTH, y2 - .01);

    // Draw the arch outline
    glColor3f(1.0f, 0.0f, 0.0f);
    if (direction)
        drawCircle(x1, y1, door_width, 0);
    else
        drawCircle(x1, y1 - WALL_WIDTH, door_width, 1);

    // Draw the outer frame of the door
    glColor3f(1.0f, 1.0f, 0.0f);
    drawRectangle(x1, y1, x1 - (DOOR_THICKNESS / 8), y1 - WALL_WIDTH);
    drawRectangle(x2, y2, x2 + (DOOR_THICKNESS / 8), y2 + WALL_WIDTH);

    // Draw the door
    if (direction)
        drawRectangle(x1, y1, x1 + (DOOR_THICKNESS / 8), y1 + door_width);
    else
        drawRectangle(x1, y1 - WALL_WIDTH, x1 + (DOOR_THICKNESS / 8), y1 - door_width - WALL_WIDTH);
}

void drawWindow(float x1, float y1, float x2, float y2)
{
    // float window_width = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    // remove wall
    glColor3f(1.0f, 1.0f, 0.0f);

    if (x1 > x2)
    {
        float temp = x1;
        x1 = x2;
        x2 = temp;
    }
    float gap = 0.008;
    for (float i = x1; i < x2; i += gap * 2)
    {
        // de cima
        glBegin(GL_LINES);
        glVertex2f(i, y1 - convert(0.06));
        if (i + gap > x2)
            glVertex2f(x2, y1 - convert(0.06));
        else
            glVertex2f(i + gap, y1 - convert(0.06));
        glEnd();
        // de baixo
        glBegin(GL_LINES);
        glVertex2f(i, y2 + convert(0.06));
        if (i + gap > x2)
            glVertex2f(x2, y2 + convert(0.06));
        else
            glVertex2f(i + gap, y2 + convert(0.06));
        glEnd();
    }

    glColor3f(1.0f, 0.0f, 0.0f);

    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(x1, y2);
    glVertex2f(x2, y2);
    glEnd();
}

void drawTable(float x, float y)
{
    float length = convert(1);
    float width = convert(2);
    glColor3f(1.0f, 1.0f, 0.0f);
    drawRectangle(x, y, x + width, y + length);
}

void drawWord(float x, float y, char *string)
{
    int len = strlen(string);
    x = x - len * 0.012;
    glColor3f(1.0f, 1.0f, 0.0f);
    float fontsize = 0.0001;
    glRasterPos2f(x, y);
    glPushMatrix();
    glScalef(fontsize, fontsize, 1);
    for (char *c = string; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        // glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
    glPopMatrix();
}

void drawToilet(float x, float y)
{
    // draw a semielipse
    glColor3f(1.0f, 1.0f, 0.0f);
    float theta = 0;
    float radius = convert(0.35);
    float x1 = x + radius;
    float y1 = y;
    float aux;
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 181; i++)
    {
        theta = i * 3.142 / 180;
        if (i == 20)
        {
            glVertex2f(x1 + radius * cos(-theta) * 0.9, y1 + radius * sin(theta) * 3);
        }
        glVertex2f(x1 + radius * cos(theta) * 0.9, y1 + radius * sin(theta) * 3);
    }
    glEnd();

    y1 += convert(0.7);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; i++)
    {
        theta = i * 3.142 / 180;
        glVertex2f(x1 + radius * cos(theta) * 0.4, y1 + radius * sin(theta) * .7);
    }
    glEnd();

    y1 -= convert(0.35);
    glBegin(GL_LINES);
    glVertex2f(x1 - convert(.30), y1);
    glVertex2f(x1 + convert(.31), y1);

    glEnd();
}

void drawBola(float x, float y)
{
    glColor3f(1.0f, 1.0f, 0.0f);
    float theta = 0;
    float radius = convert(0.35);
    float x1 = x + radius;
    float y1 = y;
    float aux;
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; i++)
    {
        theta = i * 3.142 / 180;
        glVertex2f(x1 + radius * cos(theta), y1 + radius * sin(theta));
    }
    glEnd();
}

void drawRoom(float x1, float y1, float x2, float y2, int direction, int hasWindow, int hasDoor, char *name, float area, int isOpen)
/*
    direction = 0 os comodos de baixo
                1 os comodos de cima
*/
{
    // drawBola(convert(x1), convert(y1));
    int isServico = strcmp(name, "SERVICO") == 0;
    int isToilet = strcmp(name, "BANHEIRO") == 0;
    if (strcmp(name, "GINASTICA") == 0)
    {
        drawBola(convert((x1 + x2) / 2), convert((y1 + y2) / 2 - 1));
        float limit = 3.5;
        if (x1 - x2 > limit)
        {
            area = (x2 - x1) * (y2 - y1);
            x2 = x1 - limit;
            float height = area / (x2 - x1);
            y1 = y2 - height;
            printf("x1: %.1f, x2: %.1f, y1: %.1f, y2: %.1f, height: %.1f, area: %.1f\n", x1, x2, y1, y2, height, area);
            area = 0;
        }
    }
    if (strcmp(name, "SALA DE JANTAR") == 0)
        drawTable(convert((x1 + x2) / 2 - .5), convert((y1 + y2) / 2 - .5));

    x1 = convert(x1);
    y1 = convert(y1);
    x2 = convert(x2);
    y2 = convert(y2);
    glColor3f(0.9f, 0, 1.0f);

    glBegin(GL_LINE_LOOP);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();

    float wall = WALL_WIDTH;

    if (area == 0)
        area = (x2 - x1) * (y2 - y1) * 64;

    char string[100];
    sprintf(string, "%.2fm2", area);

    glColor3f(0.9f, 0, 1.0f);

    glBegin(GL_LINE_LOOP);
    glVertex2f(x1 + wall, y1 + wall);
    glVertex2f(x2 - wall, y1 + wall);
    glVertex2f(x2 - wall, y2 - wall);
    glVertex2f(x1 + wall, y2 - wall);
    glEnd();

    float avg = x1 + (x2 - x1) / 2;

    if (direction)
    {
        if (isServico)
        {
            glColor3f(0.0f, 0.0f, 0.0f);
            drawFilledRectangle(x1 - .001, y1, x1 + WALL_WIDTH + .001, y1 - convert(1));
            glColor3f(1.0f, 0.0f, 0.9f);
            glBegin(GL_LINES);
            glVertex2f(x1, y1);
            glVertex2f(x1 + WALL_WIDTH, y1);
            glEnd();
            glBegin(GL_LINES);
            glVertex2f(x1, y1 - convert(1));
            glVertex2f(x1 + WALL_WIDTH, y1 - convert(1));
            glEnd();
        }
        if (hasDoor && !isServico)
            draw_horizontal_door(x2 + .05, y2, x2 + convert(DOOR_WIDTH) + .05, y2 - WALL_WIDTH, direction);
        if (hasWindow)
            drawWindow(x1 + 0.3 * (x2 - x1), y1 + WALL_WIDTH, x2 - 0.3 * (x2 - x1), y1);
    }
    else
    {
        if (hasDoor && !isServico)
            draw_horizontal_door(x2 + .05, y1 + WALL_WIDTH, x2 + convert(DOOR_WIDTH) + .05, y1, direction);
        if (hasWindow)
            drawWindow(x1 + 0.3 * (x2 - x1), y2, x2 - 0.3 * (x2 - x1), y2 - WALL_WIDTH);
        if (isToilet)
            drawToilet(x2 + .08, y2 + .01);
    }
    if (isOpen)
    {
        float open = 0.1;
        glColor3f(0.0f, 0.0f, 0.0f);
        drawFilledRectangle(
            x1 + open * (x2 - x1),
            y2 + .01,
            x2 - open * (x2 - x1),
            y2 - WALL_WIDTH - .01);
        glColor3f(1.0f, 0.0f, 0.9f);
        glBegin(GL_LINES);
        glVertex2f(x1 + open * (x2 - x1), y2);
        glVertex2f(x1 + open * (x2 - x1), y2 - WALL_WIDTH);
        glEnd();
        glBegin(GL_LINES);
        glVertex2f(x2 - open * (x2 - x1), y2 - WALL_WIDTH);
        glVertex2f(x2 - open * (x2 - x1), y2);
        glEnd();
    }
    drawWord((x1 + x2) / 2, (y1 + y2) / 2, name);
    drawWord((x1 + x2) / 2, (y1 + y2) / 2 - 0.05, string);
}

void drawStairs(float x1, float y1)
{
    float x2 = x1 + 3.14;
    float y2 = y1 + 3.14;

    glColor3f(1.0f, 1.0f, 0.0f);

    // make a circle
    glBegin(GL_LINE_LOOP);
    float theta, x, y;
    for (float i = 0; i < 100; i++)
    {
        theta = 2.0f * 3.1415926f *  i / 100.0; // get the current angle

        x = convert((x2 - x1) / 2) * cosf(theta + 3.1415926f); // calculate the x component
        y = convert((y2 - y1) / 2) * sinf(theta + 3.1415926f); // calculate the y component

        glVertex2f(x + x1, y + y1); // output vertex

        // glVertex2f(x + x2, y + y2); //output vertex
    }
    glEnd();

    glColor3f(1.0f, 1.0f, 0.0f);
    // make the lines cutting the circle
    for (float i = 0; i < 12; i++)
    {
        glBegin(GL_LINES);
        theta = 2.0f * 3.1415926f * i / 12.0; // get the current angle

        x = convert((x2 - x1) / 2) * cosf(theta); // calculate the x component
        y = convert((y2 - y1) / 2) * sinf(theta); // calculate the y component

        glVertex2f(x + x1, y + y1); // output vertex

        x = convert((x2 - x1) / 2) * cosf(theta + 3.1415926f); // calculate the x component
        y = convert((y2 - y1) / 2) * sinf(theta + 3.1415926f); // calculate the y component

        glVertex2f(x + x1, y + y1); // output vertex
        glEnd();
    }

    // make a solid circle in the middle
    glColor3f(0.0f, 0, 0.0f);
    glBegin(GL_POLYGON);
    for (float i = 0; i < 100; i++)
    {
        theta = 2.0f * 3.1415926f * i / 100.0; // get the current angle

        x = 0.2 * convert((x2 - x1) / 2) * cosf(theta); // calculate the x component
        y = 0.2 * convert((y2 - y1) / 2) * sinf(theta); // calculate the y component

        glVertex2f(x + x1, y + y1); // output vertex
    }
    glEnd();
    // outline the circle in the middle
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        theta = 2.0f * 3.1415926f * i / 100.0; // get the current angle

        x = 0.2 * convert((x2 - x1) / 2) * cosf(theta); // calculate the x component
        y = 0.2 * convert((y2 - y1) / 2) * sinf(theta); // calculate the y component

        glVertex2f(x + x1, y + y1); // output vertex
    }
    glEnd();
}

void drawFloor(int rooms, float width, float height)
{
    char names[5][20];
    strcpy(names[0], "SALA DE JANTAR");
    strcpy(names[1], "COZINHA");
    strcpy(names[2], "SERVICO");
    strcpy(names[3], "BANHEIRO");
    strcpy(names[4], "GINASTICA");

    float areas[5] = {19.0, 14.92, 7.0, 8.0, 10.0};
    int hasDoor[5] = {0, 0, 0, 1, 1};
    int isOpen[5] = {1, 1, 0, 0, 0};

    float sum_areas = 0;
    for (int i = 0; i < 5; i++)
    {
        //obtemos as somas das areas para saber a area da circulacao
        sum_areas += areas[i];
    }

    char name[20];
    strcpy(name, "CIRCULACAO");
    drawRoom(
        -width / 2,
        -height / 2,
        width / 2,
        height / 2,
        0,
        0,
        0,
        name,
        width * height - sum_areas,
        0);

    float room_width;
    float room_height;

    float x = width / 2 + .15;
    float y = height / 2;

    float corridor_width = (width / 2) / (rooms + 3);

    int direction = 1;
    int corner;

    float limit = 2;
    for (int i = 0; i < 5; i++)
    {
        corner = 0;//flag se estou no canto
        room_height = height / 3; //altura recomendada dos comodos
        room_width = areas[i] / room_height;

        //.15, .3 e .45 sao compensacoes para as paredes

        if (x - room_width <= -width / 2 + 2) // tenho 2m de distancia apenas da parede
        {
            if (x - room_width >= -width / 2) // estiver dentro
            {
                room_width = x - .45 - (-width / 2);
                room_height = areas[i] / room_width;
            }
            else // estou fora
            {
                if (x <= -width / 2 + 2)
                {
                    x = width / 2 + .15;
                    y = -height / 2 + room_height + .3;
                    direction = 0;
                }
                else
                {
                    room_width = x - (-width / 2) - .45;
                    x -= room_width + .15;
                    room_height = areas[i] / room_width;
                    corner = 1;
                }
            }
        }
        if (!corner && direction && x < 3 - width / 2)
        {
            room_width = x - .45 - (-width / 2);
            room_height = areas[i] / room_width;
            x = -width / 2 + .3;
            corner = 1;
        }
        if (room_width < 1.5)
        {
            room_width = 1.5;
            room_height = areas[i] / room_width;
            x -= room_width + .15;
            if (direction)
                drawRoom( // cima
                    x + room_width - .15,
                    height / 2 - .15,
                    x - .15,
                    height / 2 - room_height - .15,
                    direction,
                    1,
                    hasDoor[i],
                    names[i],
                    0,
                    isOpen[i]);
            else
                drawRoom( // baixo
                    x + room_width - .15,
                    -height / 2 + .15 + room_height,
                    x - .15,
                    -height / 2 + .15,
                    direction,
                    1,
                    hasDoor[i],
                    names[i],
                    0,
                    isOpen[i]);
            continue;
        }
        if (!corner)
            x -= room_width + .15;
        drawRoom(
            x + room_width - .15,
            y - .15,
            x - .15,
            y - room_height - .15,
            direction,
            1,
            hasDoor[i],
            names[i], 0, isOpen[i]);
    }
    draw_horizontal_door(
        convert(-width / 2 + 4.1),
        convert(-height / 2) + WALL_WIDTH,
        convert(-width / 2 + 5.3),
        convert(-height / 2), 1);

    drawStairs(convert(-width / 2 + 1.87), convert(-height / 2 + 1.87));
}

void display()
{
    // 2 andares e 1 sotao
    // 3 quartos, 2 banheiros e 1 closet
    // 1 sala de estar, 1 sala de jantar, 1 cozinha e 1 lavanderia 1 banheiro

    // float area = 175;
    // float width = sqrt(area);
    // float height;
    // for (int i = 0;; i++)
    // {
    //     if (i * width >= area)
    //         break;
    //     height = i;
    // }

    float width = 13; // no minimo 12
    float height = 11;

    // while (1)
    // {
    //     printf("Insira a largura da casa: ");
    //     scanf("%f", &width);
    //     if (width >= 10 && width <= 15)
    //         break;
    //     printf("Largura invalida, tente novamente.\n");
    // }
    // while (1)
    // {
    //     printf("Insira a profundidade da casa: ");
    //     scanf("%f", &height);
    //     if (height >= 10 && height <= 15)
    //         break;
    //     printf("Largura invalida, tente novamente.\n");
    // }
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.9f, 0, 1.0f);

    char direction = 'N';
    int rotation;

    switch (direction)
    {
    case 'N':
        rotation = 0;
        break;
    case 'E':
        rotation = 90;
        break;
    case 'S':
        rotation = 180;
        break;
    case 'W':
        rotation = 270;
        break;
    }

    // draw the outer walls of the house
    glPushMatrix();
    glRotatef(rotation, 0, 0, 1);
    drawFloor(6, width, height);
    glPopMatrix();

    drawWindRose(
        0.9,
        -0.9);

    glFlush();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(900, 900);
    glutInitWindowPosition(-100, -100);

    glutCreateWindow("Blueprint Generator");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}