// anim.cpp version 5.0 -- Template code for drawing an articulated figure.  CS 174A.
#include "../CS174a template/Utilities.h"
#include "../CS174a template/Shapes.h"

std::stack<mat4> mvstack;

int g_width = 650, g_height = 550 ;
float zoom = 1 ;

int animate = 0, recording = 0, basis_to_display = -1;
double TIME = 0;

const unsigned X = 0, Y = 1, Z = 2;


//Original Value:  eye( 0, 0, 15, 1), ref( 0, 0, 0, 1 ), up( 0, 1, 0, 0 );
vec4 eye( -40, 0, 0, 1), ref( 0, 0, 0, 1 ), up( 0, 1, 0, 0 );// The eye point and look-at point.

mat4	orientation, model_view;
ShapeData cubeData, sphereData, coneData, cylData,hexData, triPrismData;
// Structs that hold the Vertex Array Object index and number of vertices of each shape.
GLuint	texture_cube, texture_earth,
        texture_SpiderSkin,
        texture_wallMoutainScenery,
        texture_humanFace,
        texture_spiderNet,
        texture_solidRock,
        texture_caveWall,
        texture_whiteCloth;


GLint   uModelView, uProjection, uView,
		uAmbient, uDiffuse, uSpecular, uLightPos, uShininess,
		uTex, uEnableTex;

// Some Parameters that I define:
double displayStartTime;
double displayEndTime;



void init()
{
#ifdef EMSCRIPTEN
    GLuint program = LoadShaders( "vshader.glsl", "fshader.glsl" );								// Load shaders and use the resulting shader program
    TgaImage coolImage ("challenge.tga");    
    TgaImage earthImage("earth.tga");
    TgaImage spiderSkinImage("spiderSkin3.tga");
    TgaImage wallMoutainScenery ("Top_Of_The_Moutain.tga");
    TgaImage humanFaceImage("face.tga");
    TgaImage spiderNet("spidernet.tga");
    TgaImage solidRockImage("solid_rock.tga");
    TgaImage caveWallImage("Wall_Texture.tga");
    TgaImage whiteClothImage("whiteCloth.tga");
    
#else
	GLuint program = LoadShaders( "../my code/vshader.glsl", "../my code/fshader.glsl" );		// Load shaders and use the resulting shader program
    TgaImage coolImage ("../my code/challenge.tga");    
    TgaImage earthImage("../my code/earth.tga");
    TgaImage spiderSkinImage("../my code/spiderSkin3.tga");
    TgaImage wallMoutainScenery("../my code/Top_Of_The_Moutain.tga");
    TgaImage humanFaceImage("../my code/face.tga");
    TgaImage spiderNet("../my code/spidernet.tga");
    TgaImage solidRockImage("../my code/solid_rock.tga");
    TgaImage caveWallImage("../my code/Wall_Texture.tga");
    TgaImage whiteClothImage("../my code/whiteCloth.tga");
    
#endif
    glUseProgram(program);

	generateCube(program, &cubeData);		// Generate vertex arrays for geometric shapes
    generateSphere(program, &sphereData);
    generateCone(program, &coneData);
    generateCylinder(program, &cylData);
    generateHexagon(program, &hexData);
    generateTriPrism(program, &triPrismData);
    

    uModelView  = glGetUniformLocation( program, "ModelView"  );
    uProjection = glGetUniformLocation( program, "Projection" );
    uView		= glGetUniformLocation( program, "View"       );
    uAmbient	= glGetUniformLocation( program, "AmbientProduct"  );
    uDiffuse	= glGetUniformLocation( program, "DiffuseProduct"  );
    uSpecular	= glGetUniformLocation( program, "SpecularProduct" );
    uLightPos	= glGetUniformLocation( program, "LightPosition"   );
    uShininess	= glGetUniformLocation( program, "Shininess"       );
    uTex		= glGetUniformLocation( program, "Tex"             );
    uEnableTex	= glGetUniformLocation( program, "EnableTex"       );

    glUniform4f( uAmbient,    0.2,  0.2,  0.2, 1 );
    glUniform4f( uDiffuse,    0.6,  0.6,  0.6, 1 );
    glUniform4f( uSpecular,   0.2,  0.2,  0.2, 1 );
    glUniform4f( uLightPos,  15.0, 15.0, 30.0, 0 );
    glUniform1f( uShininess, 100);

    glEnable(GL_DEPTH_TEST);
    
    //generate Texture of cool_Image;
    glGenTextures( 1, &texture_cube );
    glBindTexture( GL_TEXTURE_2D, texture_cube );
    
    glTexImage2D(GL_TEXTURE_2D, 0, 4, coolImage.width, coolImage.height, 0,
                 (coolImage.byteCount == 3) ? GL_BGR : GL_BGRA,
                 GL_UNSIGNED_BYTE, coolImage.data );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    
    //generate Texture of earth_Image
    glGenTextures( 1, &texture_earth );
    glBindTexture( GL_TEXTURE_2D, texture_earth );
    
    glTexImage2D(GL_TEXTURE_2D, 0, 4, earthImage.width, earthImage.height, 0,
                 (earthImage.byteCount == 3) ? GL_BGR : GL_BGRA,
                 GL_UNSIGNED_BYTE, earthImage.data );
    
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    
    //generate Texture of my own spider Skin
    glGenTextures( 1, &texture_SpiderSkin );
    glBindTexture( GL_TEXTURE_2D, texture_SpiderSkin );
    
    glTexImage2D(GL_TEXTURE_2D, 0, 4, spiderSkinImage.width, spiderSkinImage.height, 0,
                 (spiderSkinImage.byteCount == 3) ? GL_BGR : GL_BGRA,
                 GL_UNSIGNED_BYTE, spiderSkinImage.data );
    
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    
    
    
    //generate Texture of my own "wall "
    glGenTextures( 1, &texture_wallMoutainScenery );
    glBindTexture( GL_TEXTURE_2D, texture_wallMoutainScenery );
    
    glTexImage2D(GL_TEXTURE_2D, 0, 4, wallMoutainScenery.width, wallMoutainScenery.height, 0,
                 (wallMoutainScenery.byteCount == 3) ? GL_BGR : GL_BGRA,
                 GL_UNSIGNED_BYTE, wallMoutainScenery.data );
    
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    
    
    //generate Texture of my own HUMAN FACE:
    glGenTextures( 1, &texture_humanFace );
    glBindTexture( GL_TEXTURE_2D, texture_humanFace );
    
    glTexImage2D(GL_TEXTURE_2D, 0, 4, humanFaceImage.width, humanFaceImage.height, 0,
                 (humanFaceImage.byteCount == 3) ? GL_BGR : GL_BGRA,
                 GL_UNSIGNED_BYTE, humanFaceImage.data );
    
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    
    //generate Texture of my own SPIDERNET:
    glGenTextures( 1, &texture_spiderNet );
    glBindTexture( GL_TEXTURE_2D, texture_spiderNet );
    
    glTexImage2D(GL_TEXTURE_2D, 0, 4, spiderNet.width, spiderNet.height, 0,
                 (spiderNet.byteCount == 3) ? GL_BGR : GL_BGRA,
                 GL_UNSIGNED_BYTE, spiderNet.data );
    
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    
    
    //generate Texture of my own Solid Rock
    glGenTextures( 1, &texture_solidRock );
    glBindTexture( GL_TEXTURE_2D, texture_solidRock );
    
    glTexImage2D(GL_TEXTURE_2D, 0, 4, solidRockImage.width, solidRockImage.height, 0,
                 (solidRockImage.byteCount == 3) ? GL_BGR : GL_BGRA,
                 GL_UNSIGNED_BYTE, solidRockImage.data );
    
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    
    
    //generate Texture of my own Solid Rock
    glGenTextures( 1, &texture_caveWall );
    glBindTexture( GL_TEXTURE_2D, texture_caveWall );
    
    glTexImage2D(GL_TEXTURE_2D, 0, 4, caveWallImage.width, caveWallImage.height, 0,
                 (caveWallImage.byteCount == 3) ? GL_BGR : GL_BGRA,
                 GL_UNSIGNED_BYTE, caveWallImage.data );
    
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    
    
    //generate Texture of my own White Cloth
    glGenTextures( 1, &texture_whiteCloth );
    glBindTexture( GL_TEXTURE_2D, texture_whiteCloth );
    
    glTexImage2D(GL_TEXTURE_2D, 0, 4, whiteClothImage.width, whiteClothImage.height, 0,
                 (whiteClothImage.byteCount == 3) ? GL_BGR : GL_BGRA,
                 GL_UNSIGNED_BYTE, whiteClothImage.data );
    
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    
    glUniform1i( uTex, 0);	// Set texture sampler variable to texture unit 0
	
	glEnable(GL_DEPTH_TEST);
}

struct color{ color( float r, float g, float b) : r(r), g(g), b(b) {} float r, g, b;};
std::stack<color> colors;
void set_color(float r, float g, float b)
{
	colors.push(color(r, g, b));

	float ambient  = 0.2, diffuse  = 0.6, specular = 0.2;
    glUniform4f(uAmbient,  ambient*r,  ambient*g,  ambient*b,  1 );
    glUniform4f(uDiffuse,  diffuse*r,  diffuse*g,  diffuse*b,  1 );
    glUniform4f(uSpecular, specular*r, specular*g, specular*b, 1 );
}

int mouseButton = -1, prevZoomCoord = 0 ;
vec2 anchor;
void myPassiveMotionCallBack(int x, int y) {	anchor = vec2( 2. * x / g_width - 1, -2. * y / g_height + 1 ); }

void myMouseCallBack(int button, int state, int x, int y)	// start or end mouse interaction
{
    mouseButton = button;
   
    if( button == GLUT_LEFT_BUTTON && state == GLUT_UP )
        mouseButton = -1 ;
    if( button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN )
        prevZoomCoord = -2. * y / g_height + 1;

    glutPostRedisplay() ;
}

void myMotionCallBack(int x, int y)
{
	vec2 arcball_coords( 2. * x / g_width - 1, -2. * y / g_height + 1 );
	 
    if( mouseButton == GLUT_LEFT_BUTTON )
    {
	   orientation = RotateX( -10 * (arcball_coords.y - anchor.y) ) * orientation;
	   orientation = RotateY(  10 * (arcball_coords.x - anchor.x) ) * orientation;
    }
	
	if( mouseButton == GLUT_RIGHT_BUTTON )
		zoom *= 1 + .1 * (arcball_coords.y - anchor.y);
    glutPostRedisplay() ;
}


void idleCallBack(void)
{
    if( !animate ) return;
	double prev_time = TIME;
    TIME = TM.GetElapsedTime() ;
	if( prev_time == 0 ) TM.Reset();
    
    
    glutPostRedisplay() ;
}

void drawRenderedCylinder(GLuint texture)	//render a solid cylinder oriented along the Z axis; bases are of radius 1, placed at Z = 0, and at Z = 1.
{
    glBindTexture( GL_TEXTURE_2D, texture );
    glUniform1i( uEnableTex, 1 );
    
    
    glUniformMatrix4fv( uModelView, 1, GL_FALSE, transpose(model_view) );
    glBindVertexArray( cylData.vao );
    glDrawArrays( GL_TRIANGLES, 0, cylData.numVertices );
    
    
    glUniform1i( uEnableTex, 0 );
}

void drawCylinder()	//render a solid cylinder oriented along the Z axis; bases are of radius 1, placed at Z = 0, and at Z = 1.
{
    glUniformMatrix4fv( uModelView, 1, GL_FALSE, transpose(model_view) );
    glBindVertexArray( cylData.vao );
    glDrawArrays( GL_TRIANGLES, 0, cylData.numVertices );
}

void drawCone()	//render a solid cone oriented along the Z axis; bases are of radius 1, placed at Z = 0, and at Z = 1.
{
    glUniformMatrix4fv( uModelView, 1, GL_FALSE, transpose(model_view) );
    glBindVertexArray( coneData.vao );
    glDrawArrays( GL_TRIANGLES, 0, coneData.numVertices );
}


// for the cube and the sphere function, I write two functions for each to seperate the
// rendered and unrendered functions.
void drawCube()
{
    glUniformMatrix4fv( uModelView, 1, GL_FALSE, transpose(model_view) );
    glBindVertexArray( cubeData.vao );
    glDrawArrays( GL_TRIANGLES, 0, cubeData.numVertices );
}
void drawRenderedCube(GLuint texture)
// draw a cube with dimensions 1,1,1 centered around the origin.
{
	glBindTexture( GL_TEXTURE_2D, texture );
    glUniform1i( uEnableTex, 1 );
    glUniformMatrix4fv( uModelView, 1, GL_FALSE, transpose(model_view) );
    glBindVertexArray( cubeData.vao );
    glDrawArrays( GL_TRIANGLES, 0, cubeData.numVertices );
    glUniform1i( uEnableTex, 0 );
}
void drawSphere()
{
    glUniformMatrix4fv( uModelView, 1, GL_FALSE, transpose(model_view) );
    glBindVertexArray( sphereData.vao );
    glDrawArrays( GL_TRIANGLES, 0, sphereData.numVertices );
}

void drawRenderedSphere(GLuint texture)
// draw a sphere with radius 1 centered around the origin.
{
	glBindTexture( GL_TEXTURE_2D, texture);
    glUniform1i( uEnableTex, 1);
    glUniformMatrix4fv( uModelView, 1, GL_FALSE, transpose(model_view) );
    glBindVertexArray( sphereData.vao );
    glDrawArrays( GL_TRIANGLES, 0, sphereData.numVertices );
    glUniform1i( uEnableTex, 0 );
}

void drawHex()	//
{
    glUniformMatrix4fv( uModelView, 1, GL_FALSE, transpose(model_view) );
    glBindVertexArray( hexData.vao );
    glDrawArrays( GL_TRIANGLES, 0, hexData.numVertices );
}
void drawRenderedHex(GLuint texture){
    
    glBindTexture( GL_TEXTURE_2D, texture);
    glUniform1i( uEnableTex, 1);
    glUniformMatrix4fv( uModelView, 1, GL_FALSE, transpose(model_view) );
    glBindVertexArray( hexData.vao );
    glDrawArrays( GL_TRIANGLES, 0, hexData.numVertices );
    glUniform1i( uEnableTex, 0 );
    
}


void drawRenderedTriPrism(GLuint texture)	//
{
    glBindTexture( GL_TEXTURE_2D, texture);
    glUniform1i( uEnableTex, 1);
    glUniformMatrix4fv( uModelView, 1, GL_FALSE, transpose(model_view) );
    glBindVertexArray( triPrismData.vao );
    glDrawArrays( GL_TRIANGLES, 0, triPrismData.numVertices );
    glUniform1i( uEnableTex, 0 );
}

void drawTriPrism()	//
{
    glUniformMatrix4fv( uModelView, 1, GL_FALSE, transpose(model_view) );
    glBindVertexArray( triPrismData.vao );
    glDrawArrays( GL_TRIANGLES, 0, triPrismData.numVertices );
}


int basis_id = 0;
void drawOneAxis()
{
	mat4 origin = model_view;
	model_view *= Translate	( 0, 0, 4 );
	model_view *= Scale(.25) * Scale( 1, 1, -1 );
	drawCone();
	model_view = origin;
	model_view *= Translate	( 1,  1, .5 );
	model_view *= Scale		( .1, .1, 1 );
	drawCube();
	model_view = origin;
	model_view *= Translate	( 1, 0, .5 );
	model_view *= Scale		( .1, .1, 1 );
	drawCube();
	model_view = origin;
	model_view *= Translate	( 0,  1, .5 );
	model_view *= Scale		( .1, .1, 1 );
	drawCube();
	model_view = origin;
	model_view *= Translate	( 0,  0, 2 );
	model_view *= Scale(.1) * Scale(   1, 1, 20);
    drawCylinder();	
	model_view = origin;
}

void drawAxes(int selected)
{
	if( basis_to_display != selected ) 
		return;
	mat4 given_basis = model_view;
	model_view *= Scale		(.25);
	drawSphere();
	model_view = given_basis;
	set_color( 0, 0, 1 );
	drawOneAxis();
	model_view *= RotateX	(-90);
	model_view *= Scale		(1, -1, 1);
	set_color( 1, 1, 1);
	drawOneAxis();
	model_view = given_basis;
	model_view *= RotateY	(90);
	model_view *= Scale		(-1, 1, 1);
	set_color( 1, 0, 0 );
	drawOneAxis();
	model_view = given_basis;
	
	colors.pop();
	colors.pop();
	colors.pop();
	set_color( colors.top().r, colors.top().g, colors.top().b );
}



//Actually I rarely change these three parameters
GLfloat groundDepth = -5 , groundSizeX = 200, groundSizeZ =200;
void drawGround(GLfloat depth){
	mvstack.push(model_view);
    set_color( .8, .8, .8 );
    model_view *= Translate	(0, depth, 0);
    model_view *= Scale		(groundSizeX, 1, groundSizeZ);
    drawRenderedCube(texture_spiderNet);
	model_view = mvstack.top(); mvstack.pop();
}
void drawWall  (GLfloat positionX, GLfloat positionY, GLfloat positionZ,
                GLfloat scaleX, GLfloat scaleY, GLfloat scaleZ,
                bool right,
                GLuint texture){
    
    mvstack.push(model_view);
    
    //set_color(1, 1, 1);
    model_view *= Translate(positionX, positionY + groundDepth, positionZ);
    
    if (right) {
        model_view *= RotateY(90);
    }
    model_view *= Scale(scaleX, scaleY, scaleZ);
    
    drawRenderedCube(texture);
    
    
    model_view = mvstack.top();mvstack.pop();
}
//The position of the Cave is very much depend on the size of the ground.
void drawCaveAndValley()
{
    mvstack.push(model_view);
    mvstack.push(model_view);
    model_view *= Translate(50, 15+groundDepth, 20);
    model_view *= RotateX(90);
    model_view *= Scale(10, 10, 30);
    set_color(0.54509, 0.54509, 0.54509);
    drawRenderedHex(texture_caveWall);
    model_view = mvstack.top();mvstack.pop();
    
    mvstack.push(model_view);
    model_view *= Translate(50, 15+groundDepth, -20);
    model_view *= RotateX(90);
    model_view *= Scale(10, 10, 30);
    drawRenderedHex(texture_caveWall);
    model_view = mvstack.top();mvstack.pop();
    
    //the top
    mvstack.push(model_view);
    model_view *= Translate(75, 30+groundDepth+10, 0);
    //model_view *= RotateX(90);
    model_view *= Scale(50, 20, 50);
    //set_color(0, 0.545098, 0.545098);
    drawRenderedCube(texture_caveWall);
    model_view = mvstack.top();mvstack.pop();
    
    
    //the side Wall
    mvstack.push(model_view);
    model_view *= Translate(75, groundDepth+15, 20);
    model_view *= Scale(50, 30, 10);
    drawRenderedCube(texture_caveWall);
    model_view = mvstack.top();mvstack.pop();

    //another Side Wall
    mvstack.push(model_view);
    model_view *= Translate(75, groundDepth+15, -20);
    model_view *= Scale(50, 30, 10);
    drawRenderedCube(texture_caveWall);
    model_view = mvstack.top();mvstack.pop();
    
    //
    //now It is the valley:
    //
    mvstack.push(model_view);
    model_view *= Translate(0, groundDepth, -100);
    model_view *= RotateZ(90);
    model_view *= Scale(50, 200, 70);
    
    set_color(0.6, 0.6, 0.6);
    drawRenderedTriPrism(texture_solidRock);
    model_view = mvstack.top();mvstack.pop();
    //
    mvstack.push(model_view);
    model_view *= Translate(0, groundDepth, 100);
    model_view *= RotateZ(90);
    model_view *= RotateX(180);
    model_view *= Scale(50, 200, 70);
    drawRenderedTriPrism(texture_solidRock);
    model_view = mvstack.top();mvstack.pop();
    
    
    
    
    
    model_view = mvstack.top();mvstack.pop();

}
void drawShapes()
{
	mvstack.push(model_view);

    model_view *= Translate	( 0, 3, 0 );									drawAxes(basis_id++);
    model_view *= Scale		( 3, 3, 3 );									drawAxes(basis_id++);
    set_color( .8, .0, .8 );
    drawCube();

    model_view *= Scale		( 1/3.0f, 1/3.0f, 1/3.0f );						drawAxes(basis_id++);
    model_view *= Translate	( 0, 3, 0 );									drawAxes(basis_id++);
    set_color( 0, 1, 0 );
    drawCone();

    model_view *= Translate	( 0, -9, 0 );									drawAxes(basis_id++);
    set_color( 1, 1, 0 );
    drawCylinder();

	model_view = mvstack.top(); mvstack.pop();								drawAxes(basis_id++);
	
    model_view *= Scale		( 1/3.0f, 1/3.0f, 1/3.0f );						drawAxes(basis_id++);

	drawGround(-5);
}

void drawPlanets()
{
    set_color( .8, .0, .0 );	//model sun
    mvstack.push(model_view);
    model_view *= Scale(3);													drawAxes(basis_id++);
    drawSphere();
    model_view = mvstack.top(); mvstack.pop();								drawAxes(basis_id++);
    
    set_color( .0, .0, .8 );	//model earth
    model_view *= RotateY	( 10*TIME );									drawAxes(basis_id++);
    model_view *= Translate	( 15, 5*sin( 30*DegreesToRadians*TIME ), 0 );	drawAxes(basis_id++);
    mvstack.push(model_view);
    model_view *= RotateY( 300*TIME );										drawAxes(basis_id++);
    drawCube();
    model_view = mvstack.top(); mvstack.pop();								drawAxes(basis_id++);
    
    set_color( .8, .0, .8 );	//model moon
    model_view *= RotateY	( 30*TIME );									drawAxes(basis_id++);
    model_view *= Translate	( 2, 0, 0);										drawAxes(basis_id++);
    model_view *= Scale(0.2);												drawAxes(basis_id++);
    drawCylinder();
	
}

void drawMidterm()
{
	mvstack.push(model_view);
	mvstack.push(model_view);
	model_view *= Translate	( -1, 0, 0 );									drawAxes(basis_id++);
	model_view *= Scale		( 2, 1, 1 );									drawAxes(basis_id++);
	drawCube();
	model_view = mvstack.top(); mvstack.pop();								drawAxes(basis_id++);
	
	model_view *= Scale		( 2, 1, 1 );									drawAxes(basis_id++);
	model_view *= Translate	( 1, 0, 0 );									drawAxes(basis_id++);
	drawCube();

	
	model_view *= Translate	( 0, 2, 0 );									drawAxes(basis_id++);
	model_view *= RotateZ	( 90 + 360 * TIME );							drawAxes(basis_id++);
	drawCube();
	model_view = mvstack.top(); mvstack.pop();								drawAxes(basis_id++);
}

    //the spider's eight legs are seperated into four sets.
    //the 2nd set counteract the first set.
    //the 3rd set mimics the first set.
    //the 4th set mimics the 2nd set.
    //reference: https://www.youtube.com/watch?v=GtHzpX0FCFY
void drawSpiderLegFront(GLfloat positionX,
                        GLfloat positionY,
                        GLfloat positionZ,
                        int left,
                        
                        double seg1,
                        double seg2,
                        double seg3,
                        double spiderWalkSpeed,
                        
                        GLfloat angleYA,
                        GLfloat angleYB,
                        GLfloat angleX,
                        bool animate)
//left must be -1 OR 1, otherwise this function will return nothing
{
    //
    if (left != -1 && left != 1) return;
    
    mvstack.push(model_view);
    
    if (left == 1) {

        model_view *= RotateY(
                          (-1*angleYA
                           + angleYB * (animate?sin(spiderWalkSpeed)*sin(spiderWalkSpeed):1)
                           )
                          );//
        model_view *= RotateX( -1*angleX - 5 * (animate?sin(spiderWalkSpeed)*sin(spiderWalkSpeed):1));
        //this rotateX controls the legs' rising angle
    }
    else
    {
        model_view *= RotateY(
                              (angleYA
                               - angleYB * (animate?(1 - sin(spiderWalkSpeed)*sin(spiderWalkSpeed)):1)
                               )
                              );//
        model_view *= RotateX( angleX + 5*(animate?(1 - sin(spiderWalkSpeed)*sin(spiderWalkSpeed)):1));
    }
    
    //0, -1, 3
    model_view *= Translate(positionX, positionY, positionZ);
    mvstack.push(model_view);
    
    model_view *= Translate(0, 0, seg1*left);
    
    model_view *= Scale(0.2, 0.2, seg1);
    drawCylinder();
    
    model_view = mvstack.top();mvstack.pop();
    
    
    mvstack.push(model_view);
    
    model_view *= Translate(0, 0, 2*seg1*left);
    model_view *= Scale(0.2);
    drawSphere();
    model_view = mvstack.top();mvstack.pop();
    

        mvstack.push(model_view);
    
        model_view *= Translate(0, 0, 2*seg1*left);///
        // Rotate from 90 to 60;
        if (left == 1) {
            model_view *= RotateX(
                                  (120 - 20 *(animate?sin(spiderWalkSpeed)*sin(spiderWalkSpeed):1)
                               )
                              );//
        }
        else
        {
            model_view *= RotateZ(180);
            model_view *= RotateX(
                                  (120 - 20 *(animate?
                                              (1-sin(spiderWalkSpeed)*sin(spiderWalkSpeed)):1)
                                   )+180
                                  );//
            
        }
            mvstack.push(model_view);
            model_view *= Translate(0, 0, seg2);
            model_view *= Scale(0.2,0.2,seg2);
    
            drawCylinder();
            model_view = mvstack.top();mvstack.pop();
    
            mvstack.push(model_view);
            model_view *= Translate(0,0,2*seg2);
            model_view *= Scale(0.2);
            drawSphere();
            model_view = mvstack.top();mvstack.pop();
    
               mvstack.push(model_view);
               model_view *= Translate(0, 0, 2*seg2);
               // rotate from 60 to 20
               if(left == 1){
                    model_view *= RotateX(
                                          (60 - 40 *(animate?sin(spiderWalkSpeed)*sin(spiderWalkSpeed):1)
                                      )
                                     );//
               }
               else{
                   model_view *= RotateX(
                                         (60 - 40*(animate?(1-sin(spiderWalkSpeed)*sin(spiderWalkSpeed)):1)
                                          )
                                         );//
               }
    
               mvstack.push(model_view);
               model_view *= Translate(0, 0, seg3);
               model_view *= Scale(0.2, 0.2, seg3);
               drawCylinder();
               model_view = mvstack.top();mvstack.pop();
    
                     mvstack.push(model_view);
                     model_view *= Translate(0,0,2*seg3);
                     model_view *= Scale(0.2);
                     drawSphere();
                     model_view = mvstack.top();mvstack.pop();
    
                     mvstack.push(model_view);
                     model_view *= RotateX(90);
    
                     model_view *= Translate(0,2*seg3,0);
    
                     if(left == 1){
                       model_view *= RotateX(
                                             (40 *(animate?sin(spiderWalkSpeed)*sin(spiderWalkSpeed):1)
                                            )
                                           );
                     }
                     else{
                         model_view *= RotateX(
                                               (40 *(animate?
                                                     (1-sin(spiderWalkSpeed)*sin(spiderWalkSpeed)):1)
                                                )
                                               );
                     }
                     model_view *= Translate(0,0,-0.6);
                     model_view *= Scale(0.2, 0.2, 0.6);//
                     drawCylinder();// the "claw"
        // Question: how to make the "claws" parallel to the ground ALL THE TIME?
    
                     model_view = mvstack.top();mvstack.pop();
    
               model_view = mvstack.top();mvstack.pop();
    
    
        model_view = mvstack.top();mvstack.pop();
    model_view = mvstack.top();mvstack.pop();
}
void drawSpiderLegBack( GLfloat positionX,
                        GLfloat positionY,
                        GLfloat positionZ,
                        int left,
                       
                        double seg1,
                        double seg2,
                        double seg3,
                        double spiderWalkSpeed,
                       
                        GLfloat angleYA,
                        GLfloat angleYB,
                        GLfloat angleX,
                       bool animate)
//left must be -1 OR 1, otherwise this function will return nothing
{
    //
    if (left != -1 && left != 1) return;

    //Subject to change the parameter:
    /*double spiderWalkSpeed = TIME*5;
    
    double seg1 = 3;
    double seg2 = 2;
    double seg3 = 2;*/
    
    
    //front left leg
    // rotate from -40 to -20
    mvstack.push(model_view);
    
    if (left == 1) {
        model_view *= RotateY(
                              (-1*angleYA
                               + angleYB * (animate?(1-sin(spiderWalkSpeed)*sin(spiderWalkSpeed)):1)
                               )
                              );//
        model_view *= RotateX(-1*angleX - 5*(animate?(1-sin(spiderWalkSpeed)*sin(spiderWalkSpeed)):1) );//this rotateX controls the legs' rising angle
    }
    else
    {
        model_view *= RotateY(
                              (angleYA +
                               (-1)* (animate?angleYB* sin(spiderWalkSpeed)*sin(spiderWalkSpeed):1)
                               )
                              );//
        model_view *= RotateX(angleX
                              + 5*(animate?sin(spiderWalkSpeed)*sin(spiderWalkSpeed):1));
    }
    //0, -1, 3
    model_view *= Translate(positionX, positionY, positionZ);

    mvstack.push(model_view);
    
    model_view *= Translate(0, 0, seg1*left);
    
    model_view *= Scale(0.2, 0.2, seg1);
    drawCylinder();
    
    model_view = mvstack.top();mvstack.pop();
    
    
    mvstack.push(model_view);
    
    model_view *= Translate(0, 0, 2*seg1*left);
    model_view *= Scale(0.2);
    drawSphere();
    model_view = mvstack.top();mvstack.pop();
    
    
    mvstack.push(model_view);
    
    model_view *= Translate(0, 0, 2*seg1*left);
    // Rotate from 90 to 60;
    if (left == 1) {
        model_view *= RotateX(
                              (120 - 20 *(animate?
                                          (1-sin(spiderWalkSpeed)*sin(spiderWalkSpeed)):1)
                               )
                              );//
    }
    else
    {
        model_view *= RotateZ(180);
        model_view *= RotateX(
                              (120 - 20 *(animate?
                                          sin(spiderWalkSpeed)*sin(spiderWalkSpeed):1)
                               )+180
                              );//
        
    }
    mvstack.push(model_view);
    model_view *= Translate(0, 0, seg2);
    model_view *= Scale(0.2,0.2,seg2);
    
    drawCylinder();
    model_view = mvstack.top();mvstack.pop();
    
    mvstack.push(model_view);
    model_view *= Translate(0,0,2*seg2);
    model_view *= Scale(0.2);
    drawSphere();
    model_view = mvstack.top();mvstack.pop();
    
    mvstack.push(model_view);
    model_view *= Translate(0, 0, 2*seg3);
    // rotate from 60 to 20
    if(left == 1){
        model_view *= RotateX(
                              (60 - 30*(animate?
                                        (1-sin(spiderWalkSpeed)*sin(spiderWalkSpeed)):1)
                               )
                              );//
    }
    else{
        model_view *= RotateX(
                              (60 - 30*(animate?sin(spiderWalkSpeed)*sin(spiderWalkSpeed):1)
                               )
                              );//
    }
    
    mvstack.push(model_view);
    model_view *= Translate(0, 0, seg3);
    model_view *= Scale(0.2, 0.2, seg3);
    drawCylinder();
    model_view = mvstack.top();mvstack.pop();
    
    mvstack.push(model_view);
    model_view *= Translate(0,0,2*seg3);
    model_view *= Scale(0.2);
    drawSphere();
    model_view = mvstack.top();mvstack.pop();
    
    mvstack.push(model_view);
    model_view *= RotateX(90);
    
    model_view *= Translate(0,2*seg3,0);
    
    if(left == 1){
        model_view *= RotateX(
                              (40 *(animate?
                                    (1-sin(spiderWalkSpeed)*sin(spiderWalkSpeed)):1)
                               )
                              );
    }
    else{
        model_view *= RotateX(
                              (40 *(animate?
                                    sin(spiderWalkSpeed)*sin(spiderWalkSpeed):1)
                               )
                              );
    }
    model_view *= Translate(0,0,-0.6);
    model_view *= Scale(0.2, 0.2, 0.6);//
    drawCylinder();// the "claw"
    // Question: how to make the "claws" parallel to the ground ALL THE TIME?
    
    model_view = mvstack.top();mvstack.pop();
    
    model_view = mvstack.top();mvstack.pop();
    
    
    model_view = mvstack.top();mvstack.pop();
    model_view = mvstack.top();mvstack.pop();
}
void drawSpider(bool animate)
{
    //abdomen
    mvstack.push(model_view);
    model_view *= Scale(2, 1.5, 1.5);
    //set_color(0.2, 0.2, 0.5);
    drawRenderedSphere(texture_SpiderSkin);
    model_view = mvstack.top();mvstack.pop();
    
    //body
    mvstack.push(model_view);
    model_view *= RotateZ(15*sin(TIME)*sin(TIME));
    model_view *= Translate(5, 0, 0);
    model_view *= Scale(4,3,3);
    drawRenderedSphere(texture_SpiderSkin);
    model_view = mvstack.top();mvstack.pop();
    
    //head
    mvstack.push(model_view);
    model_view *= Translate(-2, 0, 0);
    model_view *= Scale(1,1,1);
    set_color(0.2, 0.1, 0.3);
    drawSphere();
    model_view = mvstack.top();mvstack.pop();
    
    set_color(0.8, 0.1, 0.1);
    mvstack.push(model_view);
    model_view *= Translate(-3, 0, 0);
    model_view *= Scale(0.2);
    model_view *= RotateY(90);
    set_color(0.8, 0.1, 0.1);
    drawHex();
    model_view = mvstack.top();mvstack.pop();

    
    mvstack.push(model_view);
    model_view *= Translate(-3, 0, 0.3);
    model_view *= Scale(0.2);
    model_view *= RotateY(90);
    drawHex();
    model_view = mvstack.top();mvstack.pop();
    
    mvstack.push(model_view);
    model_view *= Translate(-3, 0, -0.3);
    model_view *= Scale(0.2);
    model_view *= RotateY(90);
    set_color(0.8, 0.1, 0.1);
    drawHex();
    model_view = mvstack.top();mvstack.pop();
    
    mvstack.push(model_view);
    model_view *= Translate(-2.8, 0.5, 0.2);
    model_view *= Scale(0.2);
    model_view *= RotateY(90);
    set_color(0.8, 0.1, 0.1);
    drawHex();
    model_view = mvstack.top();mvstack.pop();
    
    
    mvstack.push(model_view);
    model_view *= Translate(-2.8, 0.5, -0.2);
    model_view *= Scale(0.2);
    model_view *= RotateY(90);
    set_color(0.8, 0.1, 0.1);
    drawHex();
    model_view = mvstack.top();mvstack.pop();
    
    
    /*void drawSpiderLegFront(
     GLfloat positionX,
     GLfloat positionY,
     GLfloat positionZ,
     int left,
     
     double seg1,
     double seg2,
     double seg3,
     double spiderWalkSpeed,
     
     GLfloat angleYA,
     GLfloat angleYB,
     GLfloat angleX
     )*/
    set_color(0.1, 0.1, 0.2);
    drawSpiderLegFront(0, -1, 0, 1, 2, 1.5, 1.5, TIME*5, 60, 10, 60,animate);
    drawSpiderLegFront(0, -1, 0, -1, 2, 1.5, 1.5, TIME*5, 60, 10, 60,animate);
    
    drawSpiderLegBack(0, -1, 0, 1, 3, 2, 2, TIME*5, 30, 10, 50,animate);
    drawSpiderLegBack(0, -1, 0, -1, 3, 2, 2, TIME*5, 30, 10, 50,animate);
    
    drawSpiderLegFront(0, -1, 0, 1, 3, 2, 2, TIME*5, 10, 10, 55,animate);
    drawSpiderLegFront(0, -1, 0, -1, 3, 2, 2, TIME*5, 10, 10, 55,animate);
    
    drawSpiderLegBack(0, -1, 0, 1, 2.5, 2, 2, TIME*5, -45, -20, 55,animate);
    drawSpiderLegBack(0, -1, 0, -1, 2.5, 2, 2, TIME*5, -45, -20, 55,animate);
    
}


//this DRAWHUMAN function take three position parameters.
void drawHuman(bool animate)
{
    GLfloat animateSpeed = 10*TIME;
    
    mvstack.push(model_view);
    model_view *= RotateY(-90);
    //head
    mvstack.push(model_view);
    model_view *= Translate(0 , 3 , 0);
    set_color(1, 0.921569 ,0.803922);
    drawCube();
    mvstack.push(model_view);
    model_view *= Translate(0, -0.3, 0.5);
    model_view *= Scale(0.2, 0.1, 0.1);
    set_color(1, 0.498039 ,0.313725);
    drawCube();// the  mouth
    model_view = mvstack.top();mvstack.pop();
    
    model_view *= Translate(-0.2, 0.3, 0.5);
    model_view *=Scale(0.2, 0.1,0.1);
    set_color(0.647059, 0.164706, 0.1647);
    drawCube();
    model_view *= Translate(2.0, 0, 0);
    drawCube();
    model_view = mvstack.top();mvstack.pop();
    
    //the hair
    mvstack.push(model_view);
    model_view *= Translate(0 , 3.65, 0);
    model_view *= Scale(1, 0.3, 1);
    set_color(0, 0, 0);
    drawCube();
    model_view = mvstack.top();mvstack.pop();
    
    //the neck
    mvstack.push(model_view);
    model_view *= Translate(0 , 2.5 , 0);
    model_view *= Scale(0.6, 2, 0.6);
    set_color(0.870588, 0.721569, 0.529412);
    drawCube();
    model_view = mvstack.top();mvstack.pop();
    
    
    //body
    mvstack.push(model_view);
    model_view *= Translate(0, 1, 0);
    model_view *= Scale(1.5, 2, 1);
    set_color(0.870588, 0.721569, 0.529412);
    drawCube();
    model_view = mvstack.top();mvstack.pop();
    
    //one leg
    mvstack.push(model_view);
    model_view *= Translate(-0.5, 0, 0);
    model_view *= RotateX(animate ? 30* sin(animateSpeed):0);
    model_view *= Translate(0, -1, 0);
    model_view *= Scale(0.5, 2, 0.5);
    set_color(0.545098, 0.270588, 0.0745098);
    drawCube();
    model_view = mvstack.top();mvstack.pop();
    
    //the other leg
    mvstack.push(model_view);
    model_view *= Translate(0.5, 0, 0);
    model_view *= RotateX(animate ? -30* sin(animateSpeed):0);
    model_view *= Translate(0, -1, 0);
    model_view *= Scale(0.5, 2, 0.5);
    drawCube();
    model_view = mvstack.top();mvstack.pop();
    
    //arm
    mvstack.push(model_view);
    model_view *= Translate(0.75, 2, 0);
    model_view *= RotateZ(30);
    model_view *= RotateX(animate ? 30* sin(animateSpeed):0);
    model_view *= Translate(0, -1, 0);
    model_view *= Scale(0.5, 2, 0.5);
    set_color(0.870588, 0.721569, 0.529412);
    drawCube();
    model_view = mvstack.top();mvstack.pop();
    
    //the other arm
    mvstack.push(model_view);
    model_view *= Translate(-0.75, 2, 0);
    model_view *= RotateZ(-30);
    model_view *= RotateX(animate ? -30* sin(animateSpeed):0);
    model_view *= Translate(0, -1, 0);
    model_view *= Scale(0.5, 2, 0.5);
    set_color(0.870588, 0.721569, 0.529412);
    drawCube();
    model_view = mvstack.top();mvstack.pop();
    
    model_view = mvstack.top();mvstack.pop();
}
void drawTheFallingHuman(double startTime, double endTime)
{
    double duration = endTime - startTime;
    mvstack.push(model_view);
    model_view *= Translate(0, (TIME > startTime && TIME < endTime) ? -2 : 0, 0);
    model_view *= RotateZ((TIME > startTime && TIME < endTime) ? 90/duration*(TIME-startTime) : 0);
    model_view *= Translate(0, (TIME > startTime && TIME < endTime) ? 2 : 0, 0);
    
    if (TIME >= endTime) {
        //model_view *=Translate(0, 4, 0);
        model_view *= RotateZ(90);
    }
    if ((TIME > startTime && TIME < endTime)) {
        drawHuman(false);
    }
    
    model_view = mvstack.top();mvstack.pop();
}
void drawTheRollingHuman(double startTime, double endTime){//12-15
    double duration = endTime - startTime;
    mvstack.push(model_view);
    model_view *= RotateZ(90);
    mvstack.push(model_view);
    model_view *= RotateY((TIME > startTime && TIME < endTime)? 3600/duration*(TIME-startTime):0);
    if ((TIME > startTime && TIME < endTime)) {
        drawHuman(false);
    }
    model_view = mvstack.top();mvstack.pop();
    model_view = mvstack.top();mvstack.pop();
}
void drawTheWhiteEllipsoid(){
    //mvstack.push(model_view);
    //model_view *= Scale( (TIME > 12 && TIME < 15)? 15/3*(TIME-12):0,
    //                    (TIME > 12 && TIME < 15)? 1/3*(TIME-12):0,
    //                    (TIME > 12 && TIME < 15)? 1/3*(TIME-12):0);
    //if (TIME >=15 ) {
    model_view *= Translate(0, -2, 0);
    model_view *= Scale(2, 5, 2);
    //}
    set_color(1, 1, 1);
    drawRenderedSphere(texture_whiteCloth);
    
    //model_view = mvstack.top();mvstack.pop();
}

void rotateSeceneY(double startTime, double duration, double angle, vec4 startPoint){
    
    double rotationYBeginTime = startTime;
    double timeToRotateY = duration;
    double rotationYSceneTime = TIME - rotationYBeginTime;
    if( rotationYSceneTime > 0 && rotationYSceneTime < timeToRotateY ){
        eye = RotateY(angle/timeToRotateY * rotationYSceneTime) * startPoint;
    }
    
}




/*Diplay Function*/
vec4 unRotatedPoint = eye;

double PREV_TIME = 0;
int frameNumber = 0;
double average = 0;

void display(void)
{
    if (TIME != PREV_TIME)
    {
        double newMeasurement = 1.0/(TIME - PREV_TIME);
    
        const float decay_period = 10;
        const float a = 2.0 / ( decay_period - 1.0);
        average = a * newMeasurement+ (1 - a) * average;
    
        if (frameNumber%10 == 0)
           std::cout<< "FrameRate: "<< average << std::endl;
    
        PREV_TIME = TIME;
    }
    frameNumber++;
    
   
	basis_id = 0;
    glClearColor( .1, .1, .2, 1 );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	set_color( .6, .6, .6 );
	
    
    //Rotating the scene
    //we can do the rotation Y first
    float rotationYBeginTime = 1;
    float timeToRotateY = 5;
    float rotationYSceneTime = TIME - rotationYBeginTime;
    if( rotationYSceneTime > 0 && rotationYSceneTime < timeToRotateY ){
       eye = RotateY(360/timeToRotateY * rotationYSceneTime) * unRotatedPoint;
    }
    
    if( 0 <= TIME && TIME < rotationYBeginTime ){
        
        eye = RotateZ(-40)*vec4(-100,10,0,1);
        //eye = vec4(0,30,0,1);
       unRotatedPoint = eye;
       //ref = Translate(0,10,0)*vec4(0,0,0,1);
    }
    
    
    //then do the rotation Z
    float rotationZBeginTime = 7;
    float timeToRotateZ = 2;
    float rotationZSceneTime = TIME - rotationZBeginTime;
    if (rotationZSceneTime > 0 && rotationZSceneTime < timeToRotateZ) {
        eye = RotateZ(-30/timeToRotateZ * rotationZSceneTime)*unRotatedPoint;
    //this Z-Direction rotation will have a view point sudden change if the angle
    //passes the normal vector
    }
    
    
    //I'm trying to do the lookat translation:
    float translationBeginTime = 9;
    float timeToTranslation = 2;
    float translationSceneTime = TIME - translationBeginTime;
    
    
    vec4 newScenePoint = vec4 (-20,0,30,1);
    if (translationSceneTime > 0 && translationSceneTime < timeToTranslation) {
    
        eye = Translate(-20/timeToTranslation * translationSceneTime, 0, 0)*newScenePoint;
        ref = Translate(-20/timeToTranslation * translationSceneTime, 0, 0)* vec4(0,0,0,1);
        
    }

    
    //then I do another rotation around Y axis
    if (TIME >= 11 && TIME <= 21){
       
        rotateSeceneY(11, 10, -225, vec4(45,20,45,1));
    }
    
	model_view = LookAt( eye, ref, up );
    //orientation *= RotateY(TIME);
	model_view *= orientation;
    model_view *= Scale(zoom);
    
    //the movement of the spider
    mvstack.push(model_view);
    model_view *= Translate(80, -1, 0);
    model_view *= Translate(TIME < 10? TIME*(-10): -100,0,0);
    
    //spider turning around: time
    mvstack.push(model_view);                  //***********
    model_view *= RotateY( (TIME > 17 && TIME < 19) ? 180/2*(TIME-17) : 0);
    if (TIME>=19) {
        model_view *= RotateY(180);
    }
    
    //spider going home
    if (TIME >19){
        model_view *= Translate(TIME < 29? (TIME-19)*(-10): -100,0,0);
        
    }

    
    //spider legs stop moving duration: 10 - 12;//consistant with human falling time.
    drawSpider( (TIME>12 || TIME < 10) ? true:false);
    model_view = mvstack.top();mvstack.pop();  //***********
    

    model_view = mvstack.top();mvstack.pop();
    
    //draw the human//
    mvstack.push(model_view);
    model_view *= Translate(20,0,0);
    model_view *= Translate(TIME < 10? TIME*(-4.4): -44, -2,0);
    
    //the falling of the human
    drawTheFallingHuman(10, 12);
    
    mvstack.push(model_view);
    model_view *= Translate(-2, -2, 0);
    //the rolling of the human
    drawTheRollingHuman(12, 15);
    model_view = mvstack.top();mvstack.pop();

    if (TIME< 10) {
        drawHuman(TIME< 10 ? true : false);
    }
    if (TIME >= 15) {
        mvstack.push(model_view);
        model_view *= Translate(-4, -2, 0);
        model_view *= RotateZ(90);
        if (TIME > 15 && TIME < 19) {
            drawTheWhiteEllipsoid();
        }
        if (TIME >=19 ) {
            model_view *= Translate(0,TIME < 29? (TIME-19)*(-10): -100,0);
            drawTheWhiteEllipsoid();
        }
        //drawTheWhiteEllipsoid();
        model_view = mvstack.top();mvstack.pop();
    }
    
   
    
    
    
    
    model_view = mvstack.top();mvstack.pop();

    
    //draw Walls and Valley, this part remains static
    groundSizeX = 200;
    groundSizeZ = 200;
    drawGround(groundDepth);
    drawWall(-100, 40, 0, 1, 80, 200, false, texture_wallMoutainScenery);
    drawWall(100, 40, 0, 1, 80, 200, false, texture_wallMoutainScenery);
    drawWall(0, 40, -100, 1, 80, 200, true, texture_wallMoutainScenery);
    drawWall(0, 40, 100, 1, 80, 200, true, texture_wallMoutainScenery);
    
    drawCaveAndValley();
    
    glutSwapBuffers();
    
}


void myReshape(int w, int h)	// Handles window sizing and resizing.
{    
    mat4 projection = Perspective( 50, (float)w/h, 1, 1000 );
    glUniformMatrix4fv( uProjection, 1, GL_FALSE, transpose(projection) );
	glViewport(0, 0, g_width = w, g_height = h);	
}		

void instructions() {	 std::cout <<	"Press:"									<< '\n' <<
										"  r to restore the original view."			<< '\n' <<
										"  0 to restore the original state."		<< '\n' <<
										"  a to toggle the animation."				<< '\n' <<
										"  b to show the next basis's axes."		<< '\n' <<
										"  B to show the previous basis's axes."	<< '\n' <<
										"  q to quit."								<< '\n';	}

void myKey(unsigned char key, int x, int y)
{
    switch (key) {
        case 'q':   case 27:				// 27 = esc key
            exit(0); 
		case 'b':
			std::cout << "Basis: " << ++basis_to_display << '\n';
			break;
		case 'B':
			std::cout << "Basis: " << --basis_to_display << '\n';
			break;
        case 'a':							// toggle animation           		
            if(animate) std::cout << "Elapsed time " << TIME << '\n';
            animate = 1 - animate ; 
            break ;
		case '0':							// Add code to reset your object here.
			TIME = 0;	TM.Reset() ;											
        case 'r':
			orientation = mat4();			
            break ;
    }
    glutPostRedisplay() ;
}

int main() 
{
	char title[] = "Title";
	int argcount = 1;	 char* title_ptr = title;
	glutInit(&argcount,		 &title_ptr);
	glutInitWindowPosition (230, 70);
	glutInitWindowSize     (g_width, g_height);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow(title);
	#if !defined(__APPLE__) && !defined(EMSCRIPTEN)
		glewExperimental = GL_TRUE;
		glewInit();
	#endif
    std::cout << "GL version " << glGetString(GL_VERSION) << '\n';
	instructions();
	init();


	glutDisplayFunc(display);
    
    glutIdleFunc(idleCallBack) ;
    glutReshapeFunc (myReshape);
    glutKeyboardFunc( myKey );
    glutMouseFunc(myMouseCallBack) ;
    glutMotionFunc(myMotionCallBack) ;
    glutPassiveMotionFunc(myPassiveMotionCallBack) ;

	glutMainLoop();
}