#include<iostream>
#include<stdio.h>
#include<GL/gl.h>
#include<GL/glut.h>
#include<math.h>

#define PI 3.14159265

using namespace std;

struct point{
    int x;
    int y;
};

struct line{
    point p1;
    point p2;
}L[30];


struct COLOR
{
	float red, green, blue;
};

int size_line = 0;         // no of lines
int x_or = 0;


int choice;          // translation = 1, rotation = 2, scaling = 3
float t_x, t_y, s_x, s_y, r_val;   // translation scaling and rotation parameters
float trans_mat[3][3];

const COLOR WHITE = {0.95, 0.95, 0.95};
const COLOR BLACK = {0.0, 0.0, 0.0};
const COLOR RED   = {0.5, 0.0, 0.0};
const COLOR GREEN = {0.0, 0.6, 0.0};
const COLOR BLUE  = {0.0, 0.0, 1.0};
const COLOR GRAY  = {0.6, 0.6, 0.6};



void drawLine( int x1, int y1, int x2, int y2, COLOR color, int width)
{
	glColor3f(color.red, color.green, color.blue);
	glLineWidth(width);
	glBegin(GL_LINES);
		glVertex2i(x1, y1);
		glVertex2i(x2, y2);
	glEnd();
    glFlush();
}    

void draw()
{
    int i=0;

    for(i=0; i<size_line; i++)
    {
        //draw_line(L[i].p1.x, L[i].p1.y, L[i].p2.x, L[i].p2.y, WHITE, 1.0);
        drawLine(L[i].p1.x,L[i].p1.y,L[i].p2.x,L[i].p2.y,WHITE,1.0);
    }
}

/*void matrix_multplication(float mat[3][3],mat_transform[3][1])
{
    int i=0,j=0,k=0;
    for(i=0;i<3;i++)
    {
        for(j=0;j<3;)
    }
} */

void matrix_transform(float point_m[3][1], int i_val, int p_no)   // p_no is point number 1 or 2
{
	int i=0,j=0,k=0;                             // i_val is the line number in the array line
	float sum = 0.0, mat_c[3][1];
	for(i=0;i<3;i++)
	{
		for(j=0;j<1;j++)
		{
			mat_c[i][j] = 0.0;
			for(k=0;k<3;k++)
			{
				mat_c[i][j] += trans_mat[i][k] * point_m[k][j];
			}
		}
	}
	/* for(i=0;i<3;i++){
		for(j=0;j<1;j++){
			cout << mat_c[i][j] << " ";
		}
		cout << "\n";
	}     */
    if(p_no == 1)  // point 1
    {
        L[i_val].p1.x = mat_c[0][0];
        L[i_val].p1.y = mat_c[1][0];
    }
    if(p_no == 2)              // point 2
    {
        L[i_val].p2.x = mat_c[0][0];
        L[i_val].p2.y = mat_c[1][0];
    }    
}


void convert_point()  // to apply the transformation matrix
{
    int i=0;
    float point_m[3][1];     // point matrix
    point_m[2][0] = 1;         // 0 test
    for(i=0; i<size_line; i++)
    {
        point_m[0][0] = L[i].p1.x;
        point_m[1][0] = L[i].p1.y;
        matrix_transform(point_m, i, 1);
        // ---------------------------------------------//
        point_m[0][0] = L[i].p2.x;
        point_m[1][0] = L[i].p2.y;
        matrix_transform(point_m, i, 2);
    }
    glClear(GL_COLOR_BUFFER_BIT);            // to clear the screens
    draw();
}

void create_matrix(int ch)    // to create transformation matrix
{
   // float trans_mat[3][3], point_mat[3][1];
    float sin_val, cos_val;                            // for calculating sin, cos
    int i=0, j=0;
    for(i=0;i<3;i++)
    {
        for(j=0;j<3;j++)
        {
            if(i == j)        // digonal elements
                trans_mat[i][j] = 1.0;
            else
                trans_mat[i][j] = 0.0;
        }
    }
    // ---------------------------------------------------------------------------------------//
    //---------------------------------------------------------------------------------------//
    if(ch == 1)         // translation
    {
        trans_mat[0][2] = t_x;
        trans_mat[1][2] = t_y;
    }
    else if(ch == 2)      // for rotation
    {
        sin_val = sin(r_val*PI/180);
        cos_val = cos(r_val*PI/180);

        trans_mat[0][0] = cos_val;
        trans_mat[0][1] = -1*sin_val;
        trans_mat[1][0] = sin_val;
        trans_mat[1][1] = cos_val;
    }
    else if(ch == 3)   // scaling
    {
        trans_mat[0][0] = s_x;
        trans_mat[1][1] = s_y;
    }
    convert_point();
}

void show_menu()
{
    cout << " MENU \n";
    cout << "1. Translate \n";
    cout << "2. Rotate  \n";
    cout << "3. Scale \n";
    cin >> choice;
    cout << "\n\n";
    if(choice == 1)      // translate
    {
        cout << "Enter the translation parameters Tx and Ty  ";
        cin >> t_x >> t_y;
        create_matrix(1);                 // for translation
    }
    else if(choice == 2)
    {
        cout << "Enter the angle of rotation  ";
        cin >> r_val;
        create_matrix(2);
    }
    else if(choice == 3)
    {
        cout << "Enter the scaling parameters Sx and Sy  ";
        cin >> s_x >> s_y;
        create_matrix(3);
    }
    else
    {
        cout << "Invalid choice \n";
    }
}


void mouse(int key, int state, int x, int y)
{
    if(state == GLUT_DOWN && key == GLUT_LEFT)
    {
        if(x_or == 0)       // first point
        {
            L[size_line].p1.x = x;
            L[size_line].p1.y = 480 - y;
        }
        else                 // second point
        {
            L[size_line].p2.x = x;
            L[size_line].p2.y = 480 - y;
            size_line++;
            draw();
        }
        x_or = x_or ^ 1;       // to toggle
    }
    if(state == GLUT_DOWN && key == GLUT_RIGHT_BUTTON)
    {
        show_menu();
    }
}

void init()
{
  glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
  glutInitWindowPosition(0,0);
  glutInitWindowSize(640, 480);
  glutCreateWindow("Cohen Sutherland");
 // glColor3f(1.0,1.0,1.0);
  
  
  glColor3f(0.0,0.0,0.0);
  glMatrixMode(GL_PROJECTION);  // test
  gluOrtho2D(0,640,0,480);
  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(0.0,0.0,0.0,0);  
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
	init();
	glutDisplayFunc(draw);
	glutMouseFunc(mouse);
	glutMainLoop();
    return 0;
}