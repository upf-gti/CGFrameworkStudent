#include "shader.h"
#include "utils.h"
#include <iostream>

std::map<std::string,Shader*> Shader::s_Shaders;
Shader* Shader::current = NULL;

Shader::Shader()
{
	compiled = false;
}

Shader::~Shader()
{
	Release();
}

void Shader::SetFilenames(const std::string& vsf, const std::string& psf)
{
	vs_filename = vsf;
	ps_filename = psf;
}

bool Shader::Load(const std::string& vsf, const std::string& psf, const char* macros)
{
	assert(	compiled == false );
	assert (glGetError() == GL_NO_ERROR);

	std::string name = std::string(vsf) + "," + std::string(psf);
	std::string vs_fullpath = absResPath(vsf);
	std::string ps_fullpath = absResPath(psf);

	vs_filename = vsf;
	ps_filename = psf;

	printf("Vertex shader:\n%s\n", vsf.c_str());
	printf("Fragment shader:\n%s\n", psf.c_str());
	std::string vsm,psm;
	if (!ReadFile(vs_fullpath,vsm) || !ReadFile(ps_fullpath,psm))
		return false;

	if (macros)
	{
		vsm = macros + vsm;
		psm = macros + psm;
		this->macros = macros;
	}

	// printf("Vertex shader from memory:\n%s\n", vsm.c_str());
	// printf("Fragment shader from memory:\n%s\n", psm.c_str());

	if (!CompileFromMemory(vsm,psm))
		return false;

	assert (glGetError() == GL_NO_ERROR);

	return true;
}

Shader* Shader::Get(const char* vsf, const char* psf, const char* macros)
{
	std::string name;

	if (psf)
		name = std::string(vsf) + "," + std::string(psf ? psf : "") + (macros ? macros : "");
	else
		name = vsf;
	std::map<std::string, Shader*>::iterator it = s_Shaders.find(name);
	if (it != s_Shaders.end())
		return it->second;

	if (!psf)
		return NULL;

	Shader* sh = new Shader();
	if (!sh->Load(vsf, psf, macros))
		return NULL;
	s_Shaders[name] = sh;
	return sh;
}

void Shader::SetMacros(const char* macros)
{
	this->macros = macros;
	this->Recompile();
}

void Shader::ReloadSingleShader(const char* filename)
{
	for (std::map<std::string, Shader*>::iterator it = s_Shaders.begin(); it != s_Shaders.end(); it++) {

		Shader* shader = it->second;

		if (shader->vs_filename.find(filename) > 0 ||
			shader->ps_filename.find(filename) > 0) {
			if (shader->Recompile()) {
				std::string s_filename = filename;
				size_t index = s_filename.find("shaders/");
				s_filename = s_filename.substr(index);
				std::cout << ">>>>> " << s_filename << " recompiled" << std::endl;
			}
		}
	}
}

void Shader::ReloadAll()
{
	for( std::map<std::string,Shader*>::iterator it = s_Shaders.begin(); it!=s_Shaders.end();it++)
		it->second->Recompile();
	std::cout << "Shaders recompiled" << std::endl;
}

bool Shader::Compile()
{
	assert(!compiled && "Shader already compiled" );
    return Load(vs_filename, ps_filename, macros.size() ? macros.c_str() : NULL);
}

bool Shader::Recompile()
{
	if (!vs_filename.size() || !ps_filename.size()) // Shaders compiled from memory cannot be recompiled
		return false;
	Release(); // Remove old shader
    return Load(vs_filename, ps_filename, macros.size() ? macros.c_str() : NULL);
}

bool Shader::ReadFile(const std::string& filename, std::string& content)
{
	content.clear();

	size_t count=0;

	FILE *fp = fopen(filename.c_str(),"rb");
	if (fp == NULL) 
	{
		printf("Shader::readFile: file not found %s\n",filename.c_str());
		return false;
	}

	fseek(fp, 0, SEEK_END);
	count = ftell(fp);
	rewind(fp);

	content.resize(count);
	if (count > 0) 
	{
		count = fread(&content[0], sizeof(char), count, fp);
	}
	fclose(fp);

	return true;
}

std::string Shader::GetInfoLog() const
{
	return info_log;
}

bool Shader::HasInfoLog() const
{
	return info_log.size() > 0; 
}

// ******************************************

bool Shader::CompileFromMemory(const std::string& vsm, const std::string& psm)
{
	
	if (glCreateProgramObjectARB == 0)
	{
		std::cout << "Error: your graphics cards dont support shaders. Sorry." << std::endl;
		exit(0);
	}

	program = glCreateProgramObjectARB();
	assert (glGetError() == GL_NO_ERROR);

	if (!CreateVertexShaderObject(vsm))
	{
		printf("Vertex shader compilation failed\n");
		return false;
	}

	if (!CreateFragmentShaderObject(psm))
	{
		printf("Fragment shader compilation failed\n");
		return false;
	}

	glLinkProgramARB(program);
	assert (glGetError() == GL_NO_ERROR);

	GLint linked=0;
#ifdef __APPLE__
	glGetProgramiv( program, GL_LINK_STATUS, &linked );
#else
	glGetObjectParameterivARB(program, GL_OBJECT_LINK_STATUS_ARB, &linked);
#endif
	assert(glGetError() == GL_NO_ERROR);

	if (!linked)
	{
		SaveInfoLog(program);
		Release();
		return false;
	}

#ifdef _DEBUG
	Validate();
#endif

	compiled = true;

	return true;
}

bool Shader::Validate()
{
	glValidateProgramARB(program);
	assert ( glGetError() == GL_NO_ERROR );

	GLint validated = 0;
#ifdef __APPLE__
	glGetProgramiv( program, GL_LINK_STATUS, &validated );
#else	
	glGetObjectParameterivARB(program,GL_OBJECT_VALIDATE_STATUS_ARB,&validated);
#endif
	assert(glGetError() == GL_NO_ERROR);
	
	if (!validated)
	{
		printf("Shader validation failed\n");
		SaveInfoLog(program);
		return false;
	}

	return true;
}

bool Shader::CreateVertexShaderObject(const std::string& shader)
{
	return CreateShaderObject(GL_VERTEX_SHADER_ARB,vs,shader);
}

bool Shader::CreateFragmentShaderObject(const std::string& shader)
{
	return CreateShaderObject(GL_FRAGMENT_SHADER_ARB,fs,shader);
}

bool Shader::CreateShaderObject(unsigned int type, GLuint& handle, const std::string& shader)
{
	handle = glCreateShaderObjectARB(type);
	assert (glGetError() == GL_NO_ERROR);

	const char* ptr = shader.c_str();
	glShaderSourceARB(handle, 1, &ptr, NULL);
	assert ( glGetError() == GL_NO_ERROR);
	
	glCompileShaderARB(handle);
	assert( glGetError() == GL_NO_ERROR);

	GLint compile=0;
#ifdef __APPLE__
	glGetShaderiv( handle, GL_COMPILE_STATUS, &compile );
#else
	glGetObjectParameterivARB(handle,GL_OBJECT_COMPILE_STATUS_ARB,&compile);
#endif
	assert( glGetError() == GL_NO_ERROR);

	//we want to see the compile log if we are in debug (to check warnings)
	if (!compile)
	{
		SaveInfoLog(handle);
		printf("Shader code:%s\n", shader.c_str());
		return false;
	}

	glAttachObjectARB(program,handle);
	assert( glGetError() == GL_NO_ERROR);

	return true;
}


void Shader::Release()
{
	if (vs)
	{
		glDeleteObjectARB(vs);
		assert(glGetError() == GL_NO_ERROR);
		vs = 0;
	}

	if (fs)
	{
		glDeleteObjectARB(fs);
		assert(glGetError() == GL_NO_ERROR);
		fs = 0;
	}

	if (program)
	{
		glDeleteObjectARB(program);
		assert(glGetError() == GL_NO_ERROR);
		program = 0;
	}

	locations.clear();

	compiled = false;
}


void Shader::Enable()
{
	/*if (current == this)
		return;

	current = this;*/

	glUseProgramObjectARB(program);
	assert (glGetError() == GL_NO_ERROR);

	last_slot = 0;
}


void Shader::Disable()
{
	current = NULL;

	glUseProgramObjectARB(0);
	glActiveTexture(GL_TEXTURE0);
	assert (glGetError() == GL_NO_ERROR);
}

void Shader::DisableShaders()
{
	glUseProgramObjectARB(0);
	assert (glGetError() == GL_NO_ERROR);
}

void Shader::SaveInfoLog(GLuint obj)
{
	int len = 0;
#ifdef __APPLE__	
	glGetShaderiv( obj, GL_INFO_LOG_LENGTH, &len );
#else
	glGetObjectParameterivARB(obj, GL_OBJECT_INFO_LOG_LENGTH_ARB, &len);
#endif
	assert(glGetError() == GL_NO_ERROR);

	if (len > 0)
	{
		char* ptr = new char[len+1];
		GLsizei written=0;
		glGetInfoLogARB(obj, len, &written, ptr);
		ptr[written-1]='\0';
		assert(glGetError() == GL_NO_ERROR);
		log.append(ptr);
		delete[] ptr;

		printf("LOG **********************************************\n%s\n",log.c_str());
	}
}

GLint Shader::GetLocation(const char* varname, loctable* table)
{
	if(varname == 0 || table == 0)
		return 0;

	GLint loc = 0;
	loctable* locs = table;

	loctable::iterator cur = locs->find(varname);
	
	if(cur == locs->end()) //not found in the locations table
	{
		loc = (GLint)glGetUniformLocationARB(program, varname);
		if (loc == -1)
		{
			return -1;
		}

		//insert the new value
		locs->insert(loctable::value_type(varname,loc));
	}
	else //found in the table
	{
		loc = (*cur).second;
	}
	return loc;
}

int Shader::GetAttribLocation(const char* varname)
{
	GLint loc = glGetAttribLocationARB(program, varname);
	if (loc == -1)
	{
		return loc;
	}
	assert(glGetError() == GL_NO_ERROR);

	return loc;
}

int Shader::GetUniformLocation(const char* varname)
{
	int loc = GetLocation(varname, &locations);
	if (loc == -1)
	{
		return loc;
	}
	assert(glGetError() == GL_NO_ERROR);
	return loc;
}

void Shader::SetTexture(const char* varname, Texture* tex)
{
	glActiveTexture(GL_TEXTURE0 + last_slot);
	glBindTexture(GL_TEXTURE_2D, tex->texture_id);
	SetUniform1(varname, last_slot);
	last_slot++;
	glActiveTexture(GL_TEXTURE0 + last_slot);
}

void Shader::SetTexture(const char* varname, unsigned int tex)
{
	glActiveTexture(GL_TEXTURE0 + last_slot);
	glBindTexture(GL_TEXTURE_2D,tex);
	SetUniform1(varname,last_slot);
	last_slot++;
	glActiveTexture(GL_TEXTURE0 + last_slot);
}

void Shader::SetUniform1(const char* varname, int input1)
{
	GLint loc = GetLocation(varname, &locations);
	CHECK_SHADER_VAR(loc,varname);
	glUniform1iARB(loc, input1);
	assert (glGetError() == GL_NO_ERROR);
}

void Shader::SetUniform2(const char* varname, int input1, int input2)
{
	GLint loc = GetLocation(varname, &locations);
	CHECK_SHADER_VAR(loc,varname);
	glUniform2iARB(loc, input1, input2);
	assert (glGetError() == GL_NO_ERROR);
}

void Shader::SetUniform3(const char* varname, int input1, int input2, int input3)
{
	GLint loc = GetLocation(varname, &locations);
	CHECK_SHADER_VAR(loc,varname);
	glUniform3iARB(loc, input1, input2, input3);
	assert (glGetError() == GL_NO_ERROR);
}

void Shader::SetUniform4(const char* varname, const int input1, const int input2, const int input3, const int input4)
{
	GLint loc = GetLocation(varname, &locations);
	CHECK_SHADER_VAR(loc,varname);
	glUniform4iARB(loc, input1, input2, input3, input4);
	assert (glGetError() == GL_NO_ERROR);
}

void Shader::SetUniform1Array(const char* varname, const int* input, const int count)
{
	GLint loc = GetLocation(varname, &locations);
	CHECK_SHADER_VAR(loc,varname);
	glUniform1ivARB(loc,count,input);
	assert (glGetError() == GL_NO_ERROR);
}

void Shader::SetUniform2Array(const char* varname, const int* input, const int count)
{
	GLint loc = GetLocation(varname, &locations);
	CHECK_SHADER_VAR(loc,varname);
	glUniform2ivARB(loc,count,input);
	assert (glGetError() == GL_NO_ERROR);
}

void Shader::SetUniform3Array(const char* varname, const int* input, const int count)
{
	GLint loc = GetLocation(varname, &locations);
	CHECK_SHADER_VAR(loc,varname);
	glUniform3ivARB(loc,count,input);
	assert (glGetError() == GL_NO_ERROR);
}

void Shader::SetUniform4Array(const char* varname, const int* input, const int count)
{
	GLint loc = GetLocation(varname, &locations);
	CHECK_SHADER_VAR(loc,varname);
	glUniform4ivARB(loc,count,input);
	assert (glGetError() == GL_NO_ERROR);
}

void Shader::SetUniform1(const char* varname, const float input1)
{
	GLint loc = GetLocation(varname, &locations);
	CHECK_SHADER_VAR(loc,varname);
	glUniform1fARB(loc, input1);
	assert (glGetError() == GL_NO_ERROR);
}

void Shader::SetUniform2(const char* varname, const float input1, const float input2)
{
	GLint loc = GetLocation(varname, &locations);
	CHECK_SHADER_VAR(loc,varname);
	glUniform2fARB(loc, input1, input2);
	assert (glGetError() == GL_NO_ERROR);
}

void Shader::SetUniform3(const char* varname, const float input1, const float input2, const float input3)
{
	GLint loc = GetLocation(varname, &locations);
	CHECK_SHADER_VAR(loc,varname);
	glUniform3fARB(loc, input1, input2, input3);
	assert (glGetError() == GL_NO_ERROR);
}

void Shader::SetUniform4(const char* varname, const float input1, const float input2, const float input3, const float input4)
{
	GLint loc = GetLocation(varname, &locations);
	CHECK_SHADER_VAR(loc,varname);
	glUniform4fARB(loc, input1, input2, input3, input4);
	assert (glGetError() == GL_NO_ERROR);
}

void Shader::SetUniform1Array(const char* varname, const float* input, const int count)
{
	GLint loc = GetLocation(varname, &locations);
	CHECK_SHADER_VAR(loc,varname);
	glUniform1fvARB(loc,count,input);
	assert (glGetError() == GL_NO_ERROR);
}

void Shader::SetUniform2Array(const char* varname, const float* input, const int count)
{
	GLint loc = GetLocation(varname, &locations);
	CHECK_SHADER_VAR(loc,varname);
	glUniform2fvARB(loc,count,input);
	assert (glGetError() == GL_NO_ERROR);
}

void Shader::SetUniform3Array(const char* varname, const float* input, const int count)
{
	GLint loc = GetLocation(varname, &locations);
	CHECK_SHADER_VAR(loc,varname);
	glUniform3fvARB(loc,count,input);
	assert (glGetError() == GL_NO_ERROR);
}

void Shader::SetUniform4Array(const char* varname, const float* input, const int count)
{
	GLint loc = GetLocation(varname, &locations);
	CHECK_SHADER_VAR(loc,varname);
	glUniform4fvARB(loc,count,input);
	assert (glGetError() == GL_NO_ERROR);
}

void Shader::SetMatrix44(const char* varname, const float* m)
{
	GLint loc = GetLocation(varname, &locations);
	CHECK_SHADER_VAR(loc,varname);
	glUniformMatrix4fvARB(loc, 1, GL_FALSE, m);
	assert (glGetError() == GL_NO_ERROR);
}

void Shader::SetMatrix44(const char* varname, const Matrix44 &m)
{
	GLint loc = GetLocation(varname, &locations);
	CHECK_SHADER_VAR(loc,varname);
	glUniformMatrix4fvARB(loc, 1, GL_FALSE, m.m);
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		std::cout << err << std::endl;
	}
		
	assert (glGetError() == GL_NO_ERROR);
}
