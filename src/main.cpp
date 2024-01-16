#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "shader.h"
#include "Camera.h"
#include "Sphere.h"
#include "Orbit.h"
#include "skyboxTexture.h"
#include "texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processKeyboardInput(GLFWwindow *window);
void mouse_button_callback(GLFWwindow* window, int button, int action,int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

float screenWidth=1600;
float screenHeight=1200;
Camera camera(glm::vec3(2.0f,3.0f,30.0f));



float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间

float lastX = screenWidth/2;
float lastY = screenHeight/2;
bool firstMouse = true;

glm::vec3 lightPos(0.0f, 0.0f, 0.0f);

int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Solar System with skybox", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // 添加相关窗口交互
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window,mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    if(glewInit() != GLEW_OK){
        std::cout << "Failed to initialize GLEW" << std::endl;
        exit(-1);
    }
    glEnable(GL_DEPTH_TEST); // 启用深度测试
    
    Shader lightingShader("shaders/lightingShader.vs", "shaders/lightingShader.fs");
    Shader lightCubeShader("shaders/light_cube.vs", "shaders/light_cube.fs");
    Shader skyboxshader("shaders/skybox.vs","shaders/skybox.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };

    unsigned int VBO,VBO_sky, cubeVAO,skyboxVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VBO_sky);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    glBindVertexArray(cubeVAO);

   // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_sky);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    
    glBindVertexArray(skyboxVAO);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_sky);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    // we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // initial Sun, earth, moon, Mars' color
    const glm::vec3 colorOfSun = glm::vec3(1.0f,0.0f,0.0f);
    const glm::vec3 colorOfEarth = glm::vec3(0.0f,0.0f,1.0f);
    const glm::vec3 colorOfMoon = glm::vec3(0.8f,0.8f,0.8f);
    const glm::vec3 colorOfMars = glm::vec3(0.4f,0.4f,0.4f);
    // initialization
    Sphere sun1 = Sphere(2.5,colorOfSun);
    Sphere sun2 = Sphere(2.5,colorOfSun);
    float sun_revolution_speed = 0.1;
    float sun_sun_angle = glm::radians(0.0f);
    float sun_sun_radius = 10.0f;
    Sphere earth1 = Sphere(1.0,colorOfEarth);
    Sphere earth2 = Sphere(1.0,colorOfEarth);
    float earth_sun_distance = 5;
    float earth_revolution_speed = 1;
    float earth_sun_angle = glm::radians(23.0f);
    Sphere moon1 = Sphere(0.3,colorOfMoon);
    Sphere moon2 = Sphere(0.3,colorOfMoon);
    float moon_earth_distance = 1.5;
    float moon_revolution_speed = 2;
    float moon_earth_angle = glm::radians(10.0f);

    Sphere Mars1 = Sphere(0.7,colorOfMars);
    Sphere Mars2 = Sphere(0.7,colorOfMars);
    float mars_sun_distance = 8;
    float mars_revolution_speed = 0.2;
    float mars_sun_angle = glm::radians(15.0f);

    Orbit sun_orbit = Orbit(sun_sun_radius,glm::vec3(1.0f,1.0f,1.0f));
    Orbit earth1_orbit = Orbit(earth_sun_distance,glm::vec3(1.0f,1.0f,1.0f));
    Orbit earth2_orbit = Orbit(earth_sun_distance,glm::vec3(1.0f,1.0f,1.0f));
    Orbit moon1_orbit = Orbit(moon_earth_distance,glm::vec3(1.0f,1.0f,1.0f));
    Orbit moon2_orbit = Orbit(moon_earth_distance,glm::vec3(1.0f,1.0f,1.0f));
    Orbit Mars1_orbit = Orbit(mars_sun_distance,glm::vec3(1.0f,1.0f,1.0f));
    Orbit Mars2_orbit = Orbit(mars_sun_distance,glm::vec3(1.0f,1.0f,1.0f));

    std::vector<std::string> faces
    {
        "texture/skybox/right.jpg"
        "texture/skybox/left.jpg",
        "texture/skybox/top.jpg",
        "texture/skybox/bottom.jpg",
        "texture/skybox/front.jpg",
        "texture/skybox/back.jpg"
    };
    unsigned int cubemapTexture=loadCubemap(faces); 

    // 导入星球纹理
    unsigned int sunTexture = load_texture("texture/star/Sun.png");
    unsigned int earthTexture = load_texture("texture/star/Earth.png");
    unsigned int moonTexture = load_texture("texture/star/Moon.png");
    unsigned int marsTexture = load_texture("texture/star/Mars.png");
    
    while(!glfwWindowShouldClose(window)){
        
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        processKeyboardInput(window);

        glClearColor(0.1f,0.1f,0.1f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDepthMask(GL_FALSE);
        skyboxshader.use();
        glm::mat4 projection_skybox;
        projection_skybox = glm::perspective(glm::radians(camera.Zoom), screenWidth / screenHeight, 0.1f, 100.0f); 
        int projectionLoc_s = glGetUniformLocation(skyboxshader.ID, "projection");
        glUniformMatrix4fv(projectionLoc_s, 1, GL_FALSE, glm::value_ptr(projection_skybox));

        glm::mat4 view_s = glm::mat4(glm::mat3(camera.GetViewMatrix())); ; 
        int viewLoc_s = glGetUniformLocation(skyboxshader.ID,"view");
        glUniformMatrix4fv(viewLoc_s, 1, GL_FALSE, glm::value_ptr(view_s));
        // ... set view and projection matrix
        glBindVertexArray(skyboxVAO);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthMask(GL_TRUE);

        lightingShader.use();    

        lightingShader.setVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.setVec3("viewPos", camera.Position);

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(camera.Zoom), screenWidth / screenHeight, 0.1f, 100.0f); 
        int projectionLoc = glGetUniformLocation(lightingShader.ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 view = camera.GetViewMatrix(); 
        int viewLoc = glGetUniformLocation(lightingShader.ID,"view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        
        
        glm::mat4 model=glm::mat4(1.0f);
        int modelLoc = glGetUniformLocation(lightingShader.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        // set light uniforms
        lightingShader.setVec3("material.ambient",  glm::vec3(2.0f, 2.0f, 2.0f));
        lightingShader.setVec3("material.diffuse",  glm::vec3(0.5f, 0.5f, 0.5f));
        lightingShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
        lightingShader.setFloat("material.shininess", 64.0f);
        lightingShader.setVec3("light.ambient",  glm::vec3(0.3f, 0.3f, 0.3f));
        lightingShader.setVec3("light.diffuse",  glm::vec3(1.0f, 1.0f, 1.0f)); 
        lightingShader.setVec3("light.specular", glm::vec3(3.0f, 3.0f, 3.0f)); 
        lightingShader.setVec3("light.position", lightPos);
        lightingShader.setVec3("lightColor",  glm::vec3(1.0f,1.0f,1.0f));

        // draw sun1
        lightingShader.use();
        lightingShader.setVec3("objectColor", sun1.Color);
        glm::mat4 model_sun1 = glm::mat4(1.0f);
        model_sun1 = glm::translate(model_sun1, glm::vec3(sun_sun_radius * cos(sun_revolution_speed * currentFrame)*cos(sun_sun_angle),
                                   sun_sun_radius * cos(sun_revolution_speed * currentFrame)*sin(sun_sun_angle),
                                   sun_sun_radius * sin(sun_revolution_speed * currentFrame)));
        sun1.draw(lightingShader,projection,view,model_sun1, sunTexture);
        // draw sun2
        lightingShader.setVec3("objectColor", sun2.Color);
        glm::mat4 model_sun2 = glm::mat4(1.0f);
        model_sun2 = glm::translate(model_sun2, glm::vec3(sun_sun_radius * cos(sun_revolution_speed * currentFrame + PI)*cos(sun_sun_angle),
                                   sun_sun_radius * cos(sun_revolution_speed * currentFrame + PI)*sin(sun_sun_angle),
                                   sun_sun_radius * sin(sun_revolution_speed * currentFrame + PI)));
        sun2.draw(lightingShader,projection,view,model_sun2, sunTexture);
        // draw sun orbit
        glm::mat4 model_sun_orbit = glm::mat4(1.0f);
        model_sun_orbit = glm::rotate(model_sun_orbit,glm::radians(90.0f),glm::vec3(1.0f,0.0f,0.0f));
        model_sun_orbit = glm::rotate(model_sun_orbit,sun_sun_angle,glm::vec3(0.0f,1.0f,0.0f));
        lightingShader.setVec3("objectColor", glm::vec3(1.0f,1.0f,1.0f));
        sun_orbit.Draw(lightingShader,projection,view,model_sun_orbit);

        // draw earth1
        glm::mat4 model_earth1 = model_sun1;
        glm::vec3 earth1_Position = glm::vec3(earth_sun_distance * cos(earth_revolution_speed * currentFrame)*cos(earth_sun_angle),
                                   earth_sun_distance * cos(earth_revolution_speed * currentFrame)*sin(earth_sun_angle),
                                   earth_sun_distance * sin(earth_revolution_speed * currentFrame));
        model_earth1 = glm::translate(model_earth1, earth1_Position);
        lightingShader.setVec3("objectColor", earth1.Color);
        glBindTexture(GL_TEXTURE_2D, earthTexture);
        earth1.draw(lightingShader,projection,view,model_earth1,earthTexture);
        
        // draw earth2
        glm::mat4 model_earth2 = model_sun2;
        glm::vec3 earth2_Position = glm::vec3(earth_sun_distance * cos(earth_revolution_speed * currentFrame + PI)*cos(earth_sun_angle),
                                   earth_sun_distance * cos(earth_revolution_speed * currentFrame + PI)*sin(earth_sun_angle),
                                   earth_sun_distance * sin(earth_revolution_speed * currentFrame + PI));
        model_earth2 = glm::translate(model_earth2, earth2_Position);
        lightingShader.setVec3("objectColor", earth2.Color);
        glBindTexture(GL_TEXTURE_2D, earthTexture);
        earth2.draw(lightingShader,projection,view,model_earth2,earthTexture);

        // draw earth1 orbit
        glm::mat4 model_earth1_orbit = model_sun1;
        model_earth1_orbit = glm::rotate(model_earth1_orbit,glm::radians(90.0f),glm::vec3(1.0f,0.0f,0.0f));
        model_earth1_orbit = glm::rotate(model_earth1_orbit,earth_sun_angle,glm::vec3(0.0f,1.0f,0.0f));
        lightingShader.setVec3("objectColor", earth1_orbit.orbitColor);
        earth1_orbit.Draw(lightingShader,projection,view,model_earth1_orbit);

        // draw earth2 orbit
        glm::mat4 model_earth2_orbit = model_sun2;
        model_earth2_orbit = glm::rotate(model_earth2_orbit,glm::radians(90.0f),glm::vec3(1.0f,0.0f,0.0f));
        model_earth2_orbit = glm::rotate(model_earth2_orbit,earth_sun_angle,glm::vec3(0.0f,1.0f,0.0f));
        lightingShader.setVec3("objectColor", earth2_orbit.orbitColor);
        earth2_orbit.Draw(lightingShader,projection,view,model_earth2_orbit);
        
        // draw moon1
        glm::mat4 model_moon1 = model_earth1;
        glm::vec3 moon1_Position = glm::vec3(moon_earth_distance * cos(moon_revolution_speed * currentFrame)*cos(moon_earth_angle),
                                   moon_earth_distance * cos(moon_revolution_speed * currentFrame)*sin(moon_earth_angle),
                                   moon_earth_distance * sin(moon_revolution_speed * currentFrame));
        model_moon1 = glm::translate(model_moon1, moon1_Position);
        lightingShader.setVec3("objectColor", moon1.Color);
        glBindTexture(GL_TEXTURE_2D, moonTexture);
        moon1.draw(lightingShader,projection,view,model_moon1,moonTexture);

        // draw moon2
        glm::mat4 model_moon2 = model_earth2;
        glm::vec3 moon2_Position = glm::vec3(moon_earth_distance * cos(moon_revolution_speed * currentFrame + PI)*cos(moon_earth_angle),
                                   moon_earth_distance * cos(moon_revolution_speed * currentFrame + PI)*sin(moon_earth_angle),
                                   moon_earth_distance * sin(moon_revolution_speed * currentFrame + PI));
        model_moon2 = glm::translate(model_moon2, moon2_Position);
        lightingShader.setVec3("objectColor", moon2.Color);
        glBindTexture(GL_TEXTURE_2D, moonTexture);
        moon2.draw(lightingShader,projection,view,model_moon2,moonTexture);

        // draw moon1 orbit
        glm::mat4 model_moon1_orbit = model_earth1;
        model_moon1_orbit = glm::rotate(model_moon1_orbit,glm::radians(90.0f),glm::vec3(1.0f,0.0f,0.0f));
        model_moon1_orbit = glm::rotate(model_moon1_orbit,moon_earth_angle,glm::vec3(0.0f,1.0f,0.0f));
        lightingShader.setVec3("objectColor", moon1_orbit.orbitColor);
        moon1_orbit.Draw(lightingShader,projection,view,model_moon1_orbit);

        // draw moon2 orbit
        glm::mat4 model_moon2_orbit = model_earth2;
        model_moon2_orbit = glm::rotate(model_moon2_orbit,glm::radians(90.0f),glm::vec3(1.0f,0.0f,0.0f));
        model_moon2_orbit = glm::rotate(model_moon2_orbit,moon_earth_angle,glm::vec3(0.0f,1.0f,0.0f));
        lightingShader.setVec3("objectColor", moon2_orbit.orbitColor);
        moon2_orbit.Draw(lightingShader,projection,view,model_moon2_orbit);

        // draw Mars1
        glm::mat4 model_Mars1 = model_sun1;
        glm::vec3 Mars1_Position = glm::vec3(mars_sun_distance * cos(mars_revolution_speed * currentFrame)*cos(mars_sun_angle),
                                   mars_sun_distance * cos(mars_revolution_speed * currentFrame)*sin(mars_sun_angle),
                                   mars_sun_distance * sin(mars_revolution_speed * currentFrame));
        model_Mars1 = glm::translate(model_Mars1, Mars1_Position);
        lightingShader.setVec3("objectColor", Mars1.Color);
        glBindTexture(GL_TEXTURE_2D, marsTexture);
        Mars1.draw(lightingShader,projection,view,model_Mars1, marsTexture);

        // draw Mars2
        glm::mat4 model_Mars2 = model_sun2;
        glm::vec3 Mars2_Position = glm::vec3(mars_sun_distance * cos(mars_revolution_speed * currentFrame + PI)*cos(mars_sun_angle),
                                   mars_sun_distance * cos(mars_revolution_speed * currentFrame + PI)*sin(mars_sun_angle),
                                   mars_sun_distance * sin(mars_revolution_speed * currentFrame + PI));
        model_Mars2 = glm::translate(model_Mars2, Mars2_Position);
        lightingShader.setVec3("objectColor", Mars2.Color);
        glBindTexture(GL_TEXTURE_2D, marsTexture);
        Mars2.draw(lightingShader,projection,view,model_Mars2, marsTexture);

        // draw Mars1 orbit
        glm::mat4 model_Mars1_orbit = model_sun1;
        model_Mars1_orbit = glm::rotate(model_Mars1_orbit,glm::radians(90.0f),glm::vec3(1.0f,0.0f,0.0f));
        model_Mars1_orbit = glm::rotate(model_Mars1_orbit,mars_sun_angle,glm::vec3(0.0f,1.0f,0.0f));
        lightingShader.setVec3("objectColor", Mars1_orbit.orbitColor);
        Mars1_orbit.Draw(lightingShader,projection,view,model_Mars1_orbit);

        // draw Mars2 orbit
        glm::mat4 model_Mars2_orbit = model_sun2;
        model_Mars2_orbit = glm::rotate(model_Mars2_orbit,glm::radians(90.0f),glm::vec3(1.0f,0.0f,0.0f));
        model_Mars2_orbit = glm::rotate(model_Mars2_orbit,mars_sun_angle,glm::vec3(0.0f,1.0f,0.0f));
        lightingShader.setVec3("objectColor", Mars2_orbit.orbitColor);
        Mars2_orbit.Draw(lightingShader,projection,view,model_Mars2_orbit);

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
void processKeyboardInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
    float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        camera.ProcessKeyboard(FORWARD,deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action,int mods){
    camera.MouseButton(button,action);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    if(firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    if(camera.MouseDragging){
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;
        camera.ProcessMouseMovement(xoffset,yoffset);
    }
    else{
        lastX = xpos;
        lastY = ypos;
    }
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    camera.ProcessMouseScroll(yoffset);
}

