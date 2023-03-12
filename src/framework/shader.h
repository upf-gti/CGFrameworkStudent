/*
	This allows to use compile and use shaders when rendering. Used for advanced lighting.
*/

#pragma once

#include "framework.h"
#include "texture.h"
#include <string>
#include <map>
#include <cstring>

#ifdef _DEBUG
	#define CHECK_SHADER_VAR(a,b) if (a == -1) return
	//#define CHECK_SHADER_VAR(a,b) if (a == -1) { std::cout << "Shader error: Var not found in shader: " << b << std::endl; return; } 
#else
	#define CHECK_SHADER_VAR(a,b) if (a == -1) return
#endif

class Shader
{
	int last_slot;

public:
	static Shader* current;

	Shader();
	virtual ~Shader();

	virtual void SetFilenames(const std::string& vsf, const std::string& psf); // Set but not compile
	virtual bool Compile();
	virtual bool Recompile();

	virtual bool Load(const std::string& vsf, const std::string& psf, const char* macros = NULL);

	// Internal functions
	virtual bool CompileFromMemory(const std::string& vsm, const std::string& psm);
	virtual void Release();
	virtual void Enable();
	virtual void Disable();

	static void DisableShaders();

	// Uniform exist
	virtual bool IsVar(const char* varname) { return (GetUniformLocation(varname) != -1); }

	// Upload
	virtual void SetFloat(const char* varname, const float& input) { SetUniform1(varname, input); }
	virtual void SetVector2(const char* varname, const Vector2& input) { SetUniform2(varname, input.x, input.y); }
	virtual void SetVector3(const char* varname, const Vector3& input) { SetUniform3(varname, input.x, input.y, input.z); }
	virtual void SetMatrix44(const char* varname, const float* m);
	virtual void SetMatrix44(const char* varname, const Matrix44 &m);

	virtual void SetUniform1Array(const char* varname, const float* input, const int count) ;
	virtual void SetUniform2Array(const char* varname, const float* input, const int count) ;
	virtual void SetUniform3Array(const char* varname, const float* input, const int count) ;
	virtual void SetUniform4Array(const char* varname, const float* input, const int count) ;

	virtual void SetUniform1Array(const char* varname, const int* input, const int count) ;
	virtual void SetUniform2Array(const char* varname, const int* input, const int count) ;
	virtual void SetUniform3Array(const char* varname, const int* input, const int count) ;
	virtual void SetUniform4Array(const char* varname, const int* input, const int count) ;

	virtual void SetUniform1(const char* varname, const int input1) ;
	virtual void SetUniform2(const char* varname, const int input1, const int input2) ;
	virtual void SetUniform3(const char* varname, const int input1, const int input2, const int input3) ;
	virtual void SetUniform3(const char* varname, const Vector3& input) { SetUniform3(varname, input.x, input.y, input.z); }
	virtual void SetUniform4(const char* varname, const int input1, const int input2, const int input3, const int input4) ;

	virtual void SetUniform1(const char* varname, const float input) ;
	virtual void SetUniform2(const char* varname, const float input1, const float input2) ;
	virtual void SetUniform3(const char* varname, const float input1, const float input2, const float input3) ;
	virtual void SetUniform4(const char* varname, const float input1, const float input2, const float input3, const float input4) ;

	virtual void SetTexture(const char* varname, Texture* tex);
	virtual void SetTexture(const char* varname, const unsigned int tex) ;

	virtual int GetAttribLocation(const char* varname);
	virtual int GetUniformLocation(const char* varname);

	std::string GetInfoLog() const;
	bool HasInfoLog() const;
	void SetMacros(const char* macros);

	static Shader* Get(const char* vsf, const char* psf, const char* macros = NULL);
	static void ReloadSingleShader(const char* filename);
	static void ReloadAll();
	static std::map<std::string,Shader*> s_Shaders;

protected:

	bool compiled;
	bool ReadFile(const std::string& filename, std::string& content);

	std::string info_log;
	std::string vs_filename;
	std::string ps_filename;
	std::string macros;

	bool CreateVertexShaderObject(const std::string& shader);
	bool CreateFragmentShaderObject(const std::string& shader);
	bool CreateShaderObject(unsigned int type, GLuint& handle, const std::string& shader);
	void SaveInfoLog(GLuint obj);

	bool Validate();

	GLuint vs;
	GLuint fs;
	GLuint program;
	std::string log;

// This is a hack to speed up shader usage (save info locally)
private: 

	struct ltstr
	{
		bool operator()(const char* s1, const char* s2) const
		{
			return std::strcmp(s1, s2) < 0;
		}
	};	
	typedef std::map<const char*, int, ltstr> loctable;

public:
	GLint GetLocation(const char* varname, loctable* table);
	loctable locations;	
};
