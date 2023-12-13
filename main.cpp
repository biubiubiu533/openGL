#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION

#include "camera_class.h"
#include "mesh.h"
#include "model.h"
#include <glm/gtx/string_cast.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int load_texture(const char *path);
unsigned int load_cube_map(vector<std::string> texture_faces);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
    Shader skyShader("/Users/a533/CLionProjects/LearnOpenGL/vs/skybox.vs", "/Users/a533/CLionProjects/LearnOpenGL/fs/skybox.fs");
    Shader lightShader("/Users/a533/CLionProjects/LearnOpenGL/vs/vshader.vs", "/Users/a533/CLionProjects/LearnOpenGL/fs/light_fshader.fs");
    Shader cubeShader("/Users/a533/CLionProjects/LearnOpenGL/vs/vshader.vs", "/Users/a533/CLionProjects/LearnOpenGL/fs/fshader.fs");
    Shader screenShader("/Users/a533/CLionProjects/LearnOpenGL/vs/framebuffer.vs", "/Users/a533/CLionProjects/LearnOpenGL/fs/framebuffer.fs");

    // load model
    // ------------------------------------
    Model ourModel("/Users/a533/CLionProjects/LearnOpenGL/nanosuit_reflection/nanosuit.obj");

    vector<glm::vec3> vegetation;
    vegetation.push_back(glm::vec3(-1.5f,  0.0f, -0.48f));
    vegetation.push_back(glm::vec3( 1.5f,  0.0f,  0.51f));
    vegetation.push_back(glm::vec3( 0.0f,  0.0f,  0.7f));
    vegetation.push_back(glm::vec3(-0.3f,  0.0f, -2.3f));
    vegetation.push_back(glm::vec3( 0.5f,  0.0f, -0.6f));

    // screen quad VAO
    float quad[] = {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 1.0f
    };
    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), &quad, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindVertexArray(0);

    //skybox cube VAO
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
    unsigned int skyVAO, skyVBO;
    glGenVertexArrays(1, &skyVAO);
    glGenBuffers(1, &skyVBO);

    glBindVertexArray(skyVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // load textures
    // -------------
    unsigned int cubeTexture  = load_texture("/Users/a533/CLionProjects/LearnOpenGL/image/container2.png");
    unsigned int floorTexture = load_texture("/Users/a533/CLionProjects/LearnOpenGL/image/container.jpg");
    unsigned int grassTexture  = load_texture("/Users/a533/CLionProjects/LearnOpenGL/image/window.png");

    vector<std::string> faces{
        "/Users/a533/CLionProjects/LearnOpenGL/skybox/right.jpg",
        "/Users/a533/CLionProjects/LearnOpenGL/skybox/left.jpg",
        "/Users/a533/CLionProjects/LearnOpenGL/skybox/bottom.jpg",
        "/Users/a533/CLionProjects/LearnOpenGL/skybox/top.jpg",
        "/Users/a533/CLionProjects/LearnOpenGL/skybox/front.jpg",
        "/Users/a533/CLionProjects/LearnOpenGL/skybox/back.jpg"
    };
    unsigned int cubemapTexture = load_cube_map(faces);

    //1.创建并绑定帧缓冲
    // -------------
    unsigned int fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    //2.创建纹理附件-颜色
    unsigned int texColorBuffer;
    glGenTextures(1, &texColorBuffer);
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0); //解除绑定
    //附加纹理到帧缓冲上
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

    //3.创建渲染缓冲附件-模版深度
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    //创建深度和模版渲染缓冲对象
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);//上述分配内存后即可解绑
    //附加到帧缓冲中
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    //4.检查帧缓冲是否完整
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // bind texture sampler
    // -----------
    skyShader.use();
    skyShader.setInt("skybox", 0);

    cubeShader.use();
    cubeShader.setFloat("material.shininess", 32.0f);

    //directional light
    cubeShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    cubeShader.setVec3("dirLight.ambientStrength", 0.05f, 0.05f, 0.05f);
    cubeShader.setVec3("dirLight.diffuseStrength", 0.4f, 0.4f, 0.4f);
    cubeShader.setVec3("dirLight.specularStrength", 0.5f, 0.5f, 0.5f);

    //point lights
    glm::vec3 pointLightPositions[] = {
            glm::vec3( 0.7f,  0.2f,  2.0f),
            glm::vec3( 2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f,  2.0f, -12.0f),
            glm::vec3( 0.0f,  0.0f, -3.0f)
    };
    for (int i = 0; i < 4; i++){
        std::string uniformName = "pointLights[" + std::to_string(i) + "].";
        cubeShader.setVec3((uniformName+"position").c_str(), pointLightPositions[i]);
        cubeShader.setVec3((uniformName+"ambientStrength").c_str(), 0.05f, 0.05f, 0.05f);
        cubeShader.setVec3((uniformName+"diffuseStrength").c_str(), 0.8f, 0.8f, 0.8f);
        cubeShader.setVec3((uniformName+"specularStrength").c_str(), 1.0f, 1.0f, 1.0f);
        cubeShader.setFloat((uniformName+"constant").c_str(), 1.0f);
        cubeShader.setFloat((uniformName+"linear").c_str(), 0.09f);
        cubeShader.setFloat((uniformName+"quadratic").c_str(), 0.032f);
    }
    cubeShader.setInt("material.skybox", 4);

    // render loop
    // -----------
    while(!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render  第一阶段：激活自定义帧缓冲
        // ------
        //glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // configure global opengl state
        // -----------------------------
        //glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);

        cubeShader.use();
        cubeShader.setVec3("viewPos", camera.Position);

        //spot light,需要实时更新的变量写在循环内
        cubeShader.setVec3("spotLight.position",  camera.Position);
        cubeShader.setVec3("spotLight.direction", camera.Front);
        cubeShader.setFloat("spotLight.cutoff",   glm::cos(glm::radians(10.0f)));
        cubeShader.setFloat("spotLight.cut",   glm::cos(glm::radians(15.0f)));

        cubeShader.setVec3("spotLight.ambientStrength",  0.2f, 0.2f, 0.2f);
        cubeShader.setVec3("spotLight.diffuseStrength",  0.5f, 0.5f, 0.5f); // 将光照调暗了一些以搭配场景
        cubeShader.setVec3("spotLight.specularStrength", 1.0f, 1.0f, 1.0f);

        cubeShader.setFloat("spotLight.constant",  1.0f);
        cubeShader.setFloat("spotLight.linear",    0.09f);
        cubeShader.setFloat("spotLight.quadratic", 0.032f);

        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        cubeShader.setMat4("projection", projection);
        cubeShader.setMat4("view", view);

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        cubeShader.setMat4("model", model);
        glm::mat4 nmodel = transpose(inverse(model));
        cubeShader.setMat4("nmodel", nmodel);
        ourModel.Draw(cubeShader);

        // render skybox
        glDepthFunc(GL_LEQUAL);  //depth=1.0
        skyShader.use();
        glBindVertexArray(skyVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        skyShader.setMat4("view", view);
        skyShader.setMat4("projection", projection);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthFunc(GL_LESS);

        // render transparent
        // -----------------------------
/*        glDepthMask(GL_FALSE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        cubeShader.use();

        glBindVertexArray(transparentVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, grassTexture);

        // sort obj from near to far
        std::map<float, glm::vec3> sorted;
        for (int i = 0; i < vegetation.size(); i++){
            float distance = glm::length(camera.Position - vegetation[i]);
            sorted[distance] = vegetation[i];
        }

        for(std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); it++){
            model = glm::mat4(1.0f);
            model = glm::translate(model, it->second);
            cubeShader.setMat4("model", model);
            nmodel = transpose(inverse(model));
            cubeShader.setMat4("nmodel", nmodel);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        glDepthMask(GL_TRUE);*/


        //第二阶段：使用默认的帧缓冲
/*        glBindFramebuffer(GL_FRAMEBUFFER, 0); // 返回默认
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        screenShader.use();
        screenShader.setInt("texture1", 0);

        glBindVertexArray(quadVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texColorBuffer);

        glDisable(GL_DEPTH_TEST);
        glDrawArrays(GL_TRIANGLES, 0, 6);*/

        glBindVertexArray(0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------


    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

//texture
unsigned int load_texture(const char *path) {
    unsigned int texture;
    glGenTextures(1, &texture);

    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load(path, &width, &height,
                                    &nrChannels, 0);
    if (data) {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //GL_CLAMP_TO_EDGE
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    return texture;
}

unsigned int load_cube_map(vector<std::string> texture_faces ) {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    int width, height, nrChannels;
    unsigned char *data;
     for (int i = 0; i < texture_faces.size(); i++) {
         data = stbi_load(texture_faces[i].c_str(), &width, &height, &nrChannels, 0);
         if (data) {
             glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                          data);

         } else {
             std::cout << "Failed to load cubemap at path:" << texture_faces[i] << std::endl;
         }
         stbi_image_free(data);
     }

     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texture;
}

