#include <cstdio>
#include <cstring>
#include <cstdlib>
#define _USE_MATH_DEFINES
#include <cmath>

#include "SceneParser.h"
#include "Camera.h" 
#include "Light.h"
#include "Material.h"

#include "Object3D.h"
#include "Group.h" 
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "Transform.h"
#include "Cylinder.h"
#include "Circle.h"

#define DegreesToRadians(x) ((M_PI * x) / 180.0f)

SceneParser::SceneParser(const char* filename) {

    // initialize some reasonable default values
    group = NULL;
    camera = NULL;
    background_color = Vector3f(0.5,0.5,0.5);
    ambient_light = Vector3f(0,0,0);
    num_lights = 0;
    lights = NULL;
    num_materials = 0;
    materials = NULL;
    current_material = NULL;

    // parse the file
    assert(filename != NULL);
    const char *ext = &filename[strlen(filename)-4];

    if(strcmp(ext,".txt")!=0){
		printf("wrong file name extension\n");
		exit(0);
	}
    file = fopen(filename,"r");

	if (file == NULL){
		printf("cannot open scene file\n");
		exit(0);
	}
    parseFile();
    fclose(file); 
    file = NULL;

    // if no lights are specified, set ambient light to white
    // (do solid color ray casting)
    if (num_lights == 0) {
        printf ("WARNING:    No lights specified\n");
        ambient_light = Vector3f(1,1,1);
    }
}

SceneParser::~SceneParser() {
    if (group != NULL) 
        delete group;
    if (camera != NULL) 
        delete camera;
    int i;
    for (i = 0; i < num_materials; i++) {
        delete materials[i]; }
    delete [] materials;
    for (i = 0; i < num_lights; i++) {
        delete lights[i]; }
    delete [] lights;
}

// ====================================================================
// ====================================================================

void SceneParser::parseFile() {
    //
    // at the top level, the scene can have a camera, 
    // background color and a group of objects
    // (we add lights and other things in future assignments)
    //
    char token[MAX_PARSER_TOKEN_LENGTH];        
    while (getToken(token)) { 
        if (!strcmp(token, "PerspectiveCamera")) {
            parsePerspectiveCamera();
        } else if (!strcmp(token, "OrthographicCamera")) {
            parseOrthographicCamera();
        } else if (!strcmp(token, "Background")) {
            parseBackground();
        } else if (!strcmp(token, "Lights")) {
            parseLights();
        } else if (!strcmp(token, "Materials")) {
            parseMaterials();
        } else if (!strcmp(token, "Group")) {
            group = parseGroup();
        } else if (!strcmp(token, "SkyBox")) {
            skybox = parseSkyBox();
        } else {
            printf ("Unknown token in parseFile: '%s'\n", token);
            exit(0);
        }
    }
}

// ====================================================================
// ====================================================================

void SceneParser::parsePerspectiveCamera() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    // read in the camera parameters
    getToken(token); assert (!strcmp(token, "{"));
    getToken(token); assert (!strcmp(token, "center"));
    Vector3f center = readVector3f();
    getToken(token); assert (!strcmp(token, "direction"));
    Vector3f direction = readVector3f();
    getToken(token); assert (!strcmp(token, "up"));
    Vector3f up = readVector3f();
    getToken(token); assert (!strcmp(token, "angle"));
    float angle_degrees = readFloat();
    float angle_radians = DegreesToRadians(angle_degrees);
    getToken(token); assert (!strcmp(token, "}"));
    camera = new PerspectiveCamera(center,direction,up,angle_radians);
}

void SceneParser::parseOrthographicCamera() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    // read in the camera parameters
    getToken(token); assert (!strcmp(token, "{"));
    getToken(token); assert (!strcmp(token, "center"));
    Vector3f center = readVector3f();
    getToken(token); assert (!strcmp(token, "direction"));
    Vector3f direction = readVector3f();
    getToken(token); assert (!strcmp(token, "up"));
    Vector3f up = readVector3f();
    getToken(token); assert (!strcmp(token, "aspect"));
    float aspect = readFloat();
    getToken(token); assert (!strcmp(token, "scaling"));
    float scaling = readFloat();
    getToken(token); assert (!strcmp(token, "}"));
    camera = new OrthographicCamera(center,direction,up,aspect,scaling);
}

void SceneParser::parseBackground() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    // read in the background color
    getToken(token); assert (!strcmp(token, "{"));    
    while (1) {
        getToken(token); 
        if (!strcmp(token, "}")) { 
            break;    
        } else if (!strcmp(token, "color")) {
            background_color = readVector3f();
        } else if (!strcmp(token, "ambientLight")) {
            ambient_light = readVector3f();
        } else {
            printf ("Unknown token in parseBackground: '%s'\n", token);
            assert(0);
        }
    }
}

// ====================================================================
// ====================================================================

void SceneParser::parseLights() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    getToken(token); assert (!strcmp(token, "{"));
    // read in the number of objects
    getToken(token); assert (!strcmp(token, "numLights"));
    num_lights = readInt();
    lights = new Light*[num_lights];
    // read in the objects
    int count = 0;
    while (num_lights > count) {
        getToken(token); 
        if (!strcmp(token, "DirectionalLight")) {
            lights[count] = parseDirectionalLight();
        } else if(strcmp(token, "PointLight")==0)
		{
			lights[count] = parsePointLight();
		}
		else {
            printf ("Unknown token in parseLight: '%s'\n", token); 
            exit(0);    
        }     
        count++;
    }
    getToken(token); assert (!strcmp(token, "}"));
}


Light* SceneParser::parseDirectionalLight() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    getToken(token); assert (!strcmp(token, "{"));
    getToken(token); assert (!strcmp(token, "direction"));
    Vector3f direction = readVector3f();
    getToken(token); assert (!strcmp(token, "color"));
    Vector3f color = readVector3f();
    getToken(token); assert (!strcmp(token, "}"));
    return new DirectionalLight(direction,color);
}
Light* SceneParser::parsePointLight() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    getToken(token); assert (!strcmp(token, "{"));
    getToken(token); assert (!strcmp(token, "position"));
    Vector3f position = readVector3f();
    getToken(token); assert (!strcmp(token, "color"));
    Vector3f color = readVector3f();
    getToken(token); assert (!strcmp(token, "}"));
    return new PointLight(position,color);
}
// ====================================================================
// ====================================================================

void SceneParser::parseMaterials() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    getToken(token); assert (!strcmp(token, "{"));
    // read in the number of objects
    getToken(token); assert (!strcmp(token, "numMaterials"));
    num_materials = readInt();
    materials = new Material*[num_materials];
    // read in the objects
    int count = 0;
    while (num_materials > count) {
        getToken(token); 
        if (!strcmp(token, "Material") ||
                !strcmp(token, "PhongMaterial")) {
            materials[count] = parseMaterial();
        } else {
            printf ("Unknown token in parseMaterial: '%s'\n", token); 
            exit(0);
        }
        count++;
    }
    getToken(token); assert (!strcmp(token, "}"));
}    


Material* SceneParser::parseMaterial() {
    char token[MAX_PARSER_TOKEN_LENGTH];
	char filename_tex[MAX_PARSER_TOKEN_LENGTH];
	filename_tex[0] = 0;
    char filename_bum[MAX_PARSER_TOKEN_LENGTH];
	filename_bum[0] = 0;
    Vector3f diffuseColor(1,1,1), specularColor(0,0,0);
	float shininess=0;
    getToken(token); assert (!strcmp(token, "{"));
    while (1) {
        getToken(token); 
        if (strcmp(token, "diffuseColor")==0) {
            diffuseColor = readVector3f();
        }
		else if (strcmp(token, "specularColor")==0) {
            specularColor = readVector3f();
        }
		else if (strcmp(token, "shininess")==0) {
            shininess = readFloat();
        }
		else if (strcmp(token, "texture")==0) {
            getToken(filename_tex);
        }
        else if (strcmp(token, "bumps")==0) {
            getToken(filename_bum);
        }
		else {
            assert (!strcmp(token, "}"));
            break;
        }
    }
    Material *answer = new Material(diffuseColor, specularColor, shininess);
	if(filename_tex[0] !=0){
		answer->loadTexture(filename_tex);
	}
    if(filename_bum[0] !=0){
		answer->loadBumps(filename_bum);
	}
    return answer;
}

// ====================================================================
// ====================================================================

Object3D* SceneParser::parseObject(char token[MAX_PARSER_TOKEN_LENGTH]) {
    Object3D *answer = NULL;
    if (!strcmp(token, "Group")) {            
        answer = (Object3D*)parseGroup();
    } else if (!strcmp(token, "Sphere")) {            
        answer = (Object3D*)parseSphere();
    } else if (!strcmp(token, "Plane")) {            
        answer = (Object3D*)parsePlane();
    } else if (!strcmp(token, "Triangle")) {            
        answer = (Object3D*)parseTriangle();
    } else if (!strcmp(token, "TriangleMesh")) {            
        answer = (Object3D*)parseTriangleMesh();
    } else if (!strcmp(token, "Transform")) {            
        answer = (Object3D*)parseTransform();
    } else if (!strcmp(token, "Cylinder")) {
        answer = (Object3D*)parseCylinder();
    } else if (!strcmp(token, "Circle")) {
        answer = (Object3D*)parseCircle();
    } else if (!strcmp(token, "Rectangle")) {
        answer = (Object3D*)parseRectangle();
    } else {
        printf ("Unknown token in parseObject: '%s'\n", token);
        exit(0);
    }
    return answer;
}

// ====================================================================
// ====================================================================

Group* SceneParser::parseGroup() {
    //
    // each group starts with an integer that specifies
    // the number of objects in the group
    //
    // the material index sets the material of all objects which follow,
    // until the next material index (scoping for the materials is very
    // simple, and essentially ignores any tree hierarchy)
    //
    char token[MAX_PARSER_TOKEN_LENGTH];
    getToken(token); assert (!strcmp(token, "{"));

    // read in the number of objects
    getToken(token); assert (!strcmp(token, "numObjects"));
    int num_objects = readInt();

    Group *answer = new Group(num_objects);

    // read in the objects
    int count = 0;
    while (num_objects > count) {
        getToken(token); 
        if (!strcmp(token, "MaterialIndex")) {
            // change the current material
            int index = readInt();
            assert (index >= 0 && index <= getNumMaterials());
            current_material = getMaterial(index);
        } else {
            Object3D *object = parseObject(token);
            assert (object != NULL);
            answer->addObject(count,object);
	    
            count++;
        }
    }
    getToken(token); assert (!strcmp(token, "}"));
    
    // return the group
    return answer;
}

// ====================================================================
// ====================================================================

Sphere* SceneParser::parseSphere() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    getToken(token); assert (!strcmp(token, "{"));
    getToken(token); assert (!strcmp(token, "center"));
    Vector3f center = readVector3f();
    getToken(token); assert (!strcmp(token, "radius"));
    float radius = readFloat();
    getToken(token);
    if (!strcmp(token, "}"))
    {
        assert (current_material != NULL);
        return new Sphere(center,radius,current_material);
    }
    else
    {
        assert (!strcmp(token, "left_edge"));
        Vector3f left_edge = readVector3f();
        getToken(token); assert (!strcmp(token, "top_edge"));
        Vector3f top_edge = readVector3f();
        getToken(token); assert (!strcmp(token, "}"));
        assert (current_material != NULL);
        return new Sphere(center,left_edge,top_edge,radius,current_material);
    }
}


Plane* SceneParser::parsePlane() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    getToken(token); assert (!strcmp(token, "{"));
    getToken(token); assert (!strcmp(token, "normal"));
    Vector3f normal = readVector3f();
    getToken(token); assert (!strcmp(token, "offset"));
    float offset = readFloat();
    getToken(token); assert (!strcmp(token, "}"));
    assert (current_material != NULL);
    return new Plane(normal,offset,current_material);
}


Triangle* SceneParser::parseTriangle() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    getToken(token); assert (!strcmp(token, "{"));
    getToken(token); 
    assert (!strcmp(token, "vertex0"));
    Vector3f v0 = readVector3f();
    getToken(token); 
    assert (!strcmp(token, "vertex1"));
    Vector3f v1 = readVector3f();
    getToken(token); 
    assert (!strcmp(token, "vertex2"));
    Vector3f v2 = readVector3f();
    getToken(token); assert (!strcmp(token, "}"));
    assert (current_material != NULL);
    return new Triangle(v0,v1,v2,current_material);
}

Mesh* SceneParser::parseTriangleMesh() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    char filename[MAX_PARSER_TOKEN_LENGTH];
    // get the filename
    getToken(token); assert (!strcmp(token, "{"));
    getToken(token); assert (!strcmp(token, "obj_file"));
    getToken(filename); 
    getToken(token); assert (!strcmp(token, "}"));
    const char *ext = &filename[strlen(filename)-4];
    assert(!strcmp(ext,".obj"));
    Mesh *answer = new Mesh(filename,current_material);
    
    return answer;
}


Transform* SceneParser::parseTransform() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    Matrix4f matrix = Matrix4f::identity();
    Object3D *object = NULL;
    getToken(token); assert (!strcmp(token, "{"));
    // read in transformations: 
    // apply to the LEFT side of the current matrix (so the first
    // transform in the list is the last applied to the object)
    getToken(token);

    while (1) {
        if (!strcmp(token,"Scale")) {
            Vector3f s = readVector3f();
            matrix = matrix * Matrix4f::scaling( s[0], s[1], s[2] );
        } else if (!strcmp(token,"UniformScale")) {
            float s = readFloat();
            matrix = matrix * Matrix4f::uniformScaling( s );
        } else if (!strcmp(token,"Translate")) {
            matrix = matrix * Matrix4f::translation( readVector3f() );
        } else if (!strcmp(token,"XRotate")) {
            matrix = matrix * Matrix4f::rotateX(DegreesToRadians(readFloat()));
        } else if (!strcmp(token,"YRotate")) {
            matrix = matrix * Matrix4f::rotateY(DegreesToRadians(readFloat()));
        } else if (!strcmp(token,"ZRotate")) {
            matrix = matrix * Matrix4f::rotateZ(DegreesToRadians(readFloat()));
        } else if (!strcmp(token,"Rotate")) {
            getToken(token); assert (!strcmp(token, "{"));
            Vector3f axis = readVector3f();
            float degrees = readFloat();
            float radians = DegreesToRadians(degrees);
            matrix = matrix * Matrix4f::rotation(axis,radians);
            getToken(token); assert (!strcmp(token, "}"));
        } else if (!strcmp(token,"Matrix4f")) {
            Matrix4f matrix2 = Matrix4f::identity();
            getToken(token); assert (!strcmp(token, "{"));
            for (int j = 0; j < 4; j++) {
	            for (int i = 0; i < 4; i++) {
            	    float v = readFloat();
	                matrix2( i, j ) = v; 
            	} 
            }
            getToken(token); assert (!strcmp(token, "}"));
            matrix = matrix2 * matrix;
        } else {
            // otherwise this must be an object,
            // and there are no more transformations
            object = parseObject(token);
            break;
        }
        getToken(token);
    }

    assert(object != NULL);
    getToken(token); assert (!strcmp(token, "}"));
    return new Transform(matrix, object);
}

Cylinder* SceneParser::parseCylinder()
{
    char token[MAX_PARSER_TOKEN_LENGTH];
    getToken(token); assert (!strcmp(token, "{"));
    getToken(token); assert (!strcmp(token, "endpoint"));
    Vector3f endpoint_1 = readVector3f();
    getToken(token); assert (!strcmp(token, "endpoint"));
    Vector3f endpoint_2 = readVector3f();
    getToken(token); assert (!strcmp(token, "radius"));
    float radius = readFloat();
    getToken(token); assert (!strcmp(token, "tex_orin"));
    Vector3f tex_orin = readVector3f();
    getToken(token); assert (!strcmp(token, "}"));
    assert (current_material != NULL);
    return new Cylinder(endpoint_1,endpoint_2,radius, tex_orin, current_material);
}

Circle* SceneParser::parseCircle()
{
    char token[MAX_PARSER_TOKEN_LENGTH];
    getToken(token); assert (!strcmp(token, "{"));
    getToken(token); assert (!strcmp(token, "normal"));
    Vector3f normal = readVector3f();
    getToken(token); assert (!strcmp(token, "center"));
    Vector3f center = readVector3f();
    getToken(token); assert (!strcmp(token, "radius"));
    float radius = readFloat();
    getToken(token); assert (!strcmp(token, "tex_orin"));
    Vector3f tex_orin = readVector3f();
    getToken(token); assert (!strcmp(token, "}"));
    assert (current_material != NULL);
    return new Circle(normal,center,radius,tex_orin,current_material);
}

Rectangle* SceneParser::parseRectangle()
{
    char token[MAX_PARSER_TOKEN_LENGTH];
    getToken(token); assert (!strcmp(token, "{"));
    getToken(token); assert (!strcmp(token, "normal"));
    Vector3f normal = readVector3f();
    getToken(token); assert (!strcmp(token, "center"));
    Vector3f center = readVector3f();
    getToken(token); assert (!strcmp(token, "tex_orin"));
    Vector3f tex_orin = readVector3f();
    getToken(token); assert (!strcmp(token, "width"));
    float width = readFloat();
    getToken(token); assert (!strcmp(token, "height"));
    float height = readFloat();
    getToken(token); assert (!strcmp(token, "}"));
    assert (current_material != NULL);
    return new Rectangle(normal,center,tex_orin,width,height,current_material);
}

SkyBox* SceneParser::parseSkyBox() // TODO: move to better spot
{
    char token[MAX_PARSER_TOKEN_LENGTH];
    getToken(token); assert (!strcmp(token, "{"));
    getToken(token); assert (!strcmp(token, "center"));
    Vector3f center = readVector3f();
    getToken(token); assert (!strcmp(token, "angle"));
    float angle = readFloat();
    float angle_radians = DegreesToRadians(angle);

    Texture* front = new Texture();
    Texture* back = new Texture();
    Texture* right = new Texture();
    Texture* left = new Texture();
    Texture* top = new Texture();
    Texture* bottom = new Texture();
    getToken(token); assert (!strcmp(token, "front"));
    getToken(token); front->load(token);
    getToken(token); assert (!strcmp(token, "back"));
    getToken(token); back->load(token);
    getToken(token); assert (!strcmp(token, "right"));
    getToken(token); right->load(token);
    getToken(token); assert (!strcmp(token, "left"));
    getToken(token); left->load(token);
    getToken(token); assert (!strcmp(token, "top"));
    getToken(token); top->load(token);
    getToken(token); assert (!strcmp(token, "bottom"));
    getToken(token); bottom->load(token);

    getToken(token); assert (!strcmp(token, "}"));
    return new SkyBox(center,angle_radians,front,back,right,left,top,bottom);
}

// ====================================================================
// ====================================================================

int SceneParser::getToken(char token[MAX_PARSER_TOKEN_LENGTH]) {
    // for simplicity, tokens must be separated by whitespace
    assert (file != NULL);
    int success = fscanf(file,"%s ",token);
    if (success == EOF) {
        token[0] = '\0';
        return 0;
    }
    return 1;
}


Vector3f SceneParser::readVector3f() {
    float x,y,z;
    int count = fscanf(file,"%f %f %f",&x,&y,&z);
    if (count != 3) {
        printf ("Error trying to read 3 floats to make a Vector3f\n");
        assert (0);
    }
    return Vector3f(x,y,z);
}


Vector2f SceneParser::readVec2f() {
    float u,v;
    int count = fscanf(file,"%f %f",&u,&v);
    if (count != 2) {
        printf ("Error trying to read 2 floats to make a Vec2f\n");
        assert (0);
    }
    return Vector2f(u,v);
}


float SceneParser::readFloat() {
    float answer;
    int count = fscanf(file,"%f",&answer);
    if (count != 1) {
        printf ("Error trying to read 1 float\n");
        assert (0);
    }
    return answer;
}


int SceneParser::readInt() {
    int answer;
    int count = fscanf(file,"%d",&answer);
    if (count != 1) {
        printf ("Error trying to read 1 int\n");
        assert (0);
    }
    return answer;
}
