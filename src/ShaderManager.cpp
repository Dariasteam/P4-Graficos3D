#include "ShaderManager.hpp"

bool ShaderManager::create_program(const std::string& program_name,
                                const VertexShader& vertex_shader,
											          const FragmentShader& fragment_shader,
                                const std::vector<std::string>& uniforms_names,
                                const std::vector<std::string>& attributes_names) {

  const unsigned program_id = glCreateProgram();

  unsigned V = vertex_shader.id;
  unsigned F = fragment_shader.id;

  Program* aux_program = new Program;
  aux_program->id = program_id;

  aux_program->vertex = &vertex_shader;
  aux_program->fragment = &fragment_shader;

	glAttachShader(program_id, V);
	glAttachShader(program_id, F);
	glLinkProgram(program_id);

  int linked;

	glGetProgramiv(program_id, GL_LINK_STATUS, &linked);

	if (!linked) {
		//Calculamos una cadena de error
		GLint logLen;
		glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &logLen);
		char* logString = new char[logLen];
		glGetProgramInfoLog(program_id, logLen, NULL, logString);
		std::cerr << "Error linking program: " << logString << std::endl;
		delete[] logString;
		glDeleteProgram(program_id);

    delete aux_program;

		return false;
	}

  // Uniforms IDs
  for (const std::string& name : uniforms_names) {
    int aux = glGetUniformLocation(program_id, name.c_str());
    if (aux < 0) {
      std::cerr << "Error creando el uniform " << name << " " << aux << " para el programa " << program_name << std::endl;
    }
    aux_program->uniforms[name] = aux;
  }

  programs[program_name] = aux_program;
  return true;
}

GLuint ShaderManager::loadShader(const char *fileName, GLenum type) {
  unsigned int fileLen;
  char *source = loadStringFromFile(fileName, fileLen);

  //////////////////////////////////////////////
  // Creación y compilación del Shader
  GLuint shader;
  shader = glCreateShader(type);
  glShaderSource(shader, 1, (const GLchar **)&source, (const GLint *)&fileLen);
  glCompileShader(shader);
  delete[] source;

  GLint compiled;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
  if (!compiled) {
    // Calculamos una cadena de error
    GLint logLen;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
    char *logString = new char[logLen];
    glGetShaderInfoLog(shader, logLen, NULL, logString);
    std::cout << "Error: " << logString << std::endl;
    delete[] logString;
    glDeleteShader(shader);
    return -1;
  }

  return shader;
}

bool ShaderManager::load_fragment_shader (const std::string& path,
                                       const std::string& name) {

  int fshader = loadShader(path.c_str(), GL_FRAGMENT_SHADER_ARB);

  if (fshader < 0) return false;

  FragmentShader* F = new FragmentShader;
  F->id = fshader;
  fragment_shaders[name] = F;

  return true;
}


bool ShaderManager::load_vertex_shader (const std::string& path,
                                     const std::string& name) {

  int vshader = loadShader(path.c_str(), GL_VERTEX_SHADER);

  if (vshader < 0) return false;

  VertexShader* V = new VertexShader;
  V->id = vshader;
  vertex_shaders[name] = V;

  return true;
}

int ShaderManager::bound_program_attributes (Program& program,
                                          const std::map<std::string, unsigned>&
                                          attribute_name_location) {

  // We are using indexes because the shaders are using layout / location
  for (const auto& element : attribute_name_location) {
    if (program.attributes[element.first] != -1)
      glEnableVertexAttribArray(element.second);
  }
}

void ShaderManager::set_mesh_per_program (Program& program,
                                       MeshInstance* mesh) const {

  program.associated_meshes.insert(mesh);
}