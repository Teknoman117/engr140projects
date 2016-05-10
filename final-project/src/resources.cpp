#include <project/resources.hpp>
#include <project/cubemap.hpp>
#include <project/modelgroup.hpp>
#include <json/json.h>

#include <algorithm>
#include <fstream>

Resources::Resources(const std::string& root, const std::string& manifest)
	: modelCache(nullptr)
{
	path = root + "/";

	Json::Value  resources;
	Json::Reader reader;

	std::ifstream file(path + manifest);
	if(!reader.parse(file, resources, true))
	{
        //for some reason it always fails to parse
		std::cerr << "Failed to parse configuration\n"
               	  << reader.getFormattedErrorMessages();
        exit(1);
	}

	LoadTextures(resources);
	LoadShaders(resources);
	LoadPrograms(resources);

	// Load the model cache
	modelCache = new ModelGroup(resources["models"], *this);
}

Resources::~Resources()
{
	delete modelCache;
}

const Shader&  Resources::GetShader(const std::string& name) const
{
	return *(shaders.find(name)->second);
}

const Program& Resources::GetProgram(const std::string& name) const
{
	return *(programs.find(name)->second);
}

const Texture& Resources::GetTexture(const std::string& name) const
{
	return *(textures.find(name)->second);
}

ModelGroup* Resources::GetModelGroup() const
{
	return modelCache;
}


void Resources::LoadShaders(const Json::Value& resources)
{
	// Load shaders
	if(!resources.isMember("shaders"))
	{
		std::cerr << "No shaders specified" << std::endl;
		exit(1);
	}

	const Json::Value& shaders_ = resources["shaders"];
	std::for_each(shaders_.begin(), shaders_.end(), [this] (const Json::Value& shader)
	{
		if(!shader.isMember("name") || !shader.isMember("type") || !shader.isMember("path"))
			return;

		// Get the type of shader
		std::string type = shader["type"].asString();
		GLenum      type_;
		if(type == "vs")
			type_ = GL_VERTEX_SHADER;
		else if(type == "fs")
			type_ = GL_FRAGMENT_SHADER;
		else if(type == "gs")
			type_ = GL_GEOMETRY_SHADER;
		else if(type == "tcs")
			type_ = GL_TESS_CONTROL_SHADER;
		else if(type == "tes")
			type_ = GL_TESS_EVALUATION_SHADER;
		else if(type == "cs")
			type_ = GL_COMPUTE_SHADER;

		shaders[shader["name"].asString()] = std::make_shared<Shader>(path + shader["path"].asString(), type_);
	});
}

void Resources::LoadPrograms(const Json::Value& resources)
{
	// Load programs
	if(!resources.isMember("programs"))
	{
		std::cerr << "No programs specified" << std::endl;
		exit(1);
	}

	const Json::Value& programs_ = resources["programs"];
	std::for_each(programs_.begin(), programs_.end(), [this] (const Json::Value& program)
	{
		if(!program.isMember("name") || !program.isMember("shaders"))
			return;

		const Json::Value&       shaders_ = program["shaders"];
		std::shared_ptr<Program> p        = std::make_shared<Program>();

		std::for_each(shaders_.begin(), shaders_.end(), [this, &p] (const Json::Value& shader)
		{
			auto s = shaders.find(shader.asString());
			if(s == shaders.end())
			{
				std::cerr << "specified shader \"" << shader.asString() << "\" is not loaded" << std::endl;
				exit(1);
			}

			p->Attach(s->second);
		});
		p->Link();

		programs[program["name"].asString()] = p;
	});
}

void Resources::LoadTextures(const Json::Value& resources)
{
	// Load shaders
	if(!resources.isMember("textures"))
		return;

	const Json::Value& textures_ = resources["textures"];
	std::for_each(textures_.begin(), textures_.end(), [this] (const Json::Value& texture)
	{
		if( !texture.isMember("name") || 
			!texture.isMember("type") || 
			!texture.isMember("path") || 
			!texture.isMember("format") || 
			!texture.isMember("wrap"))
		{
			return;
		}

		// Get the type of shader
		std::string format = texture["format"].asString();
		GLenum      format_;
		if(format == "RGB8")
			format_ = GL_RGB8;
		else if(format == "SRGB8_ALPHA8")
			format_ = GL_SRGB8_ALPHA8;

		// Get the wrap mode of shader
		std::string wrap = texture["wrap"].asString();
		GLenum      wrap_;
		if(wrap == "REPEAT")
			wrap_ = GL_REPEAT;
		else if(wrap == "CLAMP_TO_EDGE")
			wrap_ = GL_CLAMP_TO_EDGE;

		// Get the format of shader
		std::string type = texture["type"].asString();
		GLenum      type_;
		if(type == "texture2D")
		{
			textures[texture["name"].asString()] = std::make_shared<Texture>(path + texture["path"].asString(), format_);
		}
		else if(type == "cubemap")
		{
			textures[texture["name"].asString()] = std::shared_ptr<Texture>(new Cubemap(path + texture["path"].asString()));
		}

		textures[texture["name"].asString()]->SetWrapMode(wrap_);
	});
}

const std::string& Resources::GetPath() const
{
	return path;
}

