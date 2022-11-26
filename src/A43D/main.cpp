#include <A4Engine/Matrix4.hpp>
#include <A4Engine/SDLpp.hpp>
#include <A4Engine/SDLppWindow.hpp>
#include <A4Engine/Vector3.hpp>
#include <A4Engine/Vector4.hpp>
#include <GL/glew.h>
#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/std.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Vertex_Pos3Color4
{
	float x, y, z;
	float r, g, b, a;
};

enum class VertexLayout
{
	Pos3Color4
};

GLuint CreateBuffer(GLenum bufferTarget, const void* data, std::size_t size);
GLuint CreateVertexArray(VertexLayout layout, GLuint vertexBuffer, GLuint indexBuffer = 0);
GLuint CreateProgram(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);
GLuint CreateShader(GLenum shaderType, const std::filesystem::path& path);
void UpdateShaderValue(GLuint program, const char* name, const Matrix4f& matrix);
void UpdateShaderValue(GLuint program, const char* name, const Vector3f& matrix);
void UpdateShaderValue(GLuint program, const char* name, const Vector4f& matrix);

int main()
{
	SDLpp sdl;
	SDLppWindow window("OpenGL Window", 1280, 720, SDL_WINDOW_OPENGL);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GLContext context = SDL_GL_CreateContext(window.GetHandle());

	glewInit();

	std::vector<Vertex_Pos3Color4> vertices = {
		{
			{
				-0.75f, -0.75f, 0.f,
				1.f, 0.f, 0.f, 1.f
			},
			{
				0.f, 0.75f, 0.f,
				0.f, 1.f, 0.f, 1.f
			},
			{
				0.75f, -0.75f, 0.f,
				0.f, 0.f, 1.f, 1.f
			},
		}
	};

	GLuint vertexBuffer = CreateBuffer(GL_ARRAY_BUFFER, vertices.data(), vertices.size() * sizeof(Vertex_Pos3Color4));
	GLuint vertexArray = CreateVertexArray(VertexLayout::Pos3Color4, vertexBuffer);
	GLuint program;
	try
	{
		program = CreateProgram("assets/shaders/basic.vert", "assets/shaders/basic.frag");
	}
	catch (const std::exception& e)
	{
		fmt::print(stderr, fg(fmt::color::red), "{}\n", e.what());
		return EXIT_FAILURE;
	}

	Uint64 lastUpdate = SDL_GetPerformanceCounter();
	float angle = 0.f;

	bool isOpen = true;
	while (isOpen)
	{
		Uint64 now = SDL_GetPerformanceCounter();
		float deltaTime = (float)(now - lastUpdate) / SDL_GetPerformanceFrequency();
		lastUpdate = now;

		SDL_Event event;
		while (sdl.PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				isOpen = false;
			else if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_F5)
				{
					try
					{
						program = CreateProgram("assets/shaders/basic.vert", "assets/shaders/basic.frag");
					}
					catch (const std::exception& e)
					{
						fmt::print(stderr, fg(fmt::color::red), "{}\n", e.what());
					}
				}
			}
		}

		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		//angle += 45.f * deltaTime;

		UpdateShaderValue(program, "transform", Matrix4f::Identity());

		glUseProgram(program);
		glBindVertexArray(vertexArray);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		SDL_GL_SwapWindow(window.GetHandle());
	}

	SDL_GL_DeleteContext(context);
}

GLuint CreateBuffer(GLenum bufferTarget, const void* data, std::size_t size)
{
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(bufferTarget, buffer);
	glBufferData(bufferTarget, size, data, GL_STATIC_DRAW);

	return buffer;
}

GLuint CreateVertexArray(VertexLayout layout, GLuint vertexBuffer, GLuint indexBuffer)
{
	GLuint vao;
	glGenVertexArrays(1, &vao);

	// Un VAO s'utilise en deux temps, pour commencer on le configure en le bindant une première fois
	// puis en appelant les fonctions gl(Enable)VertexAttrib* avant de le débinder
	// Lors de l'affichage, il suffira alors de binder le VAO pour réactiver les attributs
	glBindVertexArray(vao);
	
	// Les buffers activés vont être pris en compte par les vertex attrib et enregistrés dans le VAO
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	if (indexBuffer != 0)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	// En fonction du layout demandé, on spécifie les attributs
	switch (layout)
	{
		case VertexLayout::Pos3Color4:
		{
			// Le stride est l'espace (en octets) entre deux attributs de même type
			GLsizei stride = sizeof(Vertex_Pos3Color4);
			const std::uint8_t* ptr = nullptr; //< on utilise un pointeur pour symboliser un offset (à cause d'une vieille API)

			// On active et spécifie l'attribut 0 (position)
			// - de taille 3 et de type GL_FLOAT
			// - pas normalisé
			// - d'un écart entre deux valeurs de la taille de la structure entière
			// - positionné à l'offset 0 par rapport au début du buffer
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, ptr + 0);

			// On active et spécifie l'attribut 1 (couleur)
			// - de taille 4 et de type GL_FLOAT
			// - pas normalisé
			// - d'un écart entre deux valeurs de la taille de la structure entière
			// - positionné à l'offset 3 * sizeof(float) par rapport au début du buffer
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, ptr + 3 * sizeof(float));
		}
	}

	// On désactive le VAO pour notifier qu'on a terminé de le configurer
	glBindVertexArray(0);

	return vao;
}

GLuint CreateProgram(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath)
{
	// Création des deux étages de shaders
	GLuint fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fragmentPath);
	GLuint vertexShader = CreateShader(GL_VERTEX_SHADER, vertexPath);

	// Création du programme
	GLuint program = glCreateProgram();

	// Constitution du programme final (= l'ensemble des shaders)
	glAttachShader(program, fragmentShader);
	glAttachShader(program, vertexShader);

	// Link (mise en relation) des shaders
	glLinkProgram(program);

	// Récupération du status et gestion des erreurs
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status != GL_TRUE)
	{
		// La compilation a échouée, on récupère le log d'erreur
		GLint logSize;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);

		std::string log(logSize, '\0');
		if (logSize > 0)
			glGetProgramInfoLog(program, logSize, nullptr, log.data());
		else
			log = "<no log>";

		// Affichage de l'erreur
		throw std::runtime_error(fmt::format("failed to link program: {}\n", log));
	}

	// "Suppression" des shaders (ceux-ci ne seront réellement supprimés que lorsque le program sera supprimé)
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	return program;
}

GLuint CreateShader(GLenum shaderType, const std::filesystem::path& path)
{
	// Ouverture d'un fichier en lecture
	std::ifstream inputFile(path);
	if (!inputFile.is_open())
		throw std::runtime_error(fmt::format("failed to open shader file {}\n", path));

	// Lecture du fichier
	std::string str((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());

	// Création de l'objet shader
	GLuint shader = glCreateShader(shaderType);

	// On paramètre le code source du shader
	const char* source = str.data();
	GLint sourceLength = static_cast<GLint>(str.size());

	glShaderSource(shader, 1, &source, &sourceLength);

	// Compilation du shader
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		// La compilation a échouée, on récupère le log d'erreur
		GLint logSize;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);

		std::string log(logSize, '\0');
		if (logSize > 0)
			glGetShaderInfoLog(shader, logSize, nullptr, log.data());
		else
			log = "<no log>";

		// Affichage de l'erreur
		throw std::runtime_error(fmt::format("failed to compile shader file {}: {}\n", path, log));
	}

	return shader;
}

void UpdateShaderValue(GLuint program, const char* name, const Matrix4f& matrix)
{
	GLint location = glGetUniformLocation(program, name);
	if (location == -1)
		return;

	glUniformMatrix4fv(location, 1, GL_TRUE, &matrix(0, 0));
}
