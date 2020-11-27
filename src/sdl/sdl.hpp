#pragma once

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>
#include <stdio.h>

#include <cassert>
#include <optional>
#include <memory>
#include <unordered_map>
#include <string>

#include "../tuvok/tuvok.hpp"

namespace sdl
{
    inline void init_sdl()
    {
        if(SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            throw std::runtime_error(strcat(strdup("SDL could not initialize! SDL_Error: "), SDL_GetError()));
        }

        if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            printf("Warning: Linear texture filtering not enabled!");
        }
    }

    inline void init_opengl()
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    }


    inline void init()
    {
        init_sdl();
        init_opengl();
    }

    inline void quit()
    {
        SDL_Quit();
    }

    inline void printProgramLog( GLuint program )
    {
        //Make sure name is shader
        if( glIsProgram( program ) )
        {
            //Program log length
            int infoLogLength = 0;
            int maxLength = infoLogLength;

            //Get info string length
            glGetProgramiv( program, GL_INFO_LOG_LENGTH, &maxLength );

            //Allocate string
            char* infoLog = new char[ maxLength ];

            //Get info log
            glGetProgramInfoLog( program, maxLength, &infoLogLength, infoLog );
            if( infoLogLength > 0 )
            {
                //Print Log
                printf( "%s\n", infoLog );
            }

            //Deallocate string
            delete[] infoLog;
        }
        else
        {
            printf( "Name %d is not a program\n", program );
        }
    }

    void printShaderLog( GLuint shader )
    {
        //Make sure name is shader
        if( glIsShader( shader ) )
        {
            //Shader log length
            int infoLogLength = 0;
            int maxLength = infoLogLength;

            //Get info string length
            glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &maxLength );

            //Allocate string
            char* infoLog = new char[ maxLength ];

            //Get info log
            glGetShaderInfoLog( shader, maxLength, &infoLogLength, infoLog );
            if( infoLogLength > 0 )
            {
                //Print Log
                printf( "%s\n", infoLog );
            }

            //Deallocate string
            delete[] infoLog;
        }
        else
        {
            printf( "Name %d is not a shader\n", shader );
        }
    }

    class Window
    {
        private:
            SDL_Window* m_window = NULL;
            SDL_GLContext m_context;

        public:
            Window(int w, int h)
            : m_window { SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN) },
              m_context { SDL_GL_CreateContext(m_window) }
            {
                if (m_context == NULL)
                {
                    printf("%s\n", SDL_GetError());
                    throw std::runtime_error("OpenGL context could not be created!");
                    /* throw std::runtime_error(strcat(strdup("OpenGL context could not be created!"), SDL_GetError())); */
                }
                else
                {
                    glewExperimental = GL_TRUE;
                    GLenum glewError = glewInit();
                    if(glewError != GLEW_OK)
                    {
                        printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
                        throw std::runtime_error("Error initializing GLEW");
                    }
                }

                if(SDL_GL_SetSwapInterval(1) < 0)
                {
                    throw std::runtime_error(strcat(strdup("Warning: Unable to set VSync! SDL Error:"), SDL_GetError()));
                }
            }

            void reset_viewport()
            { }

            void clear()
            {
                glClear( GL_COLOR_BUFFER_BIT );
            }

            void update()
            {
                SDL_GL_SwapWindow(m_window);
            }

            std::pair<int, int> get_screen_size()
            {
                SDL_DisplayMode DM;
                SDL_GetCurrentDisplayMode(0, &DM);

                return std::pair<int, int> { DM.w, DM.h };
            }

            void set_resizable(bool v)
            {
                SDL_bool b = v ? SDL_TRUE : SDL_FALSE;
                SDL_SetWindowResizable(m_window, b);
            }

            virtual ~Window()
            {
                if (m_window != NULL)
                { SDL_DestroyWindow(m_window); }
            }
    };

    GLuint VertexArrayID;
    // This will identify our vertex buffer
    GLuint vertexbuffer;
    GLuint programID;

    inline void setup_opengl()
    {
        static const GLfloat g_vertex_buffer_data[] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            0.0f,  1.0f, 0.0f,
        };

        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);

        // Generate 1 buffer, put the resulting identifier in vertexbuffer
        glGenBuffers(1, &vertexbuffer);
        // The following commands will talk about our 'vertexbuffer' buffer
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        // Give our vertices to OpenGL.
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

        programID = tuvok::load_shaders( "shaders/simple_vertex_shader.vertexshader", "shaders/simple_fragment_shader.fragmentshader" );
    }

    inline void render()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
                );
        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
        glDisableVertexAttribArray(0);
    }
}
