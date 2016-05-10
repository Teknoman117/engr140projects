#ifndef __RESOURCES_HPP__
#define __RESOURCES_HPP__

#include <project/common.hpp>
#include <project/program.hpp>
#include <project/texture.hpp>
#include <json/json.h>

class ModelGroup;

class Resources
{
	std::string path;

	ModelGroup *modelCache;

	std::map<std::string, std::shared_ptr<Shader>>  shaders;
    std::map<std::string, std::shared_ptr<Program>> programs;
    std::map<std::string, std::shared_ptr<Texture>> textures;

    void LoadShaders(const Json::Value&);
    void LoadPrograms(const Json::Value&);
    void LoadTextures(const Json::Value&);

public:
	Resources(const std::string& root, const std::string& manifest);
	virtual ~Resources();

	const Shader&  GetShader(const std::string& name) const;
	const Program& GetProgram(const std::string& name) const;
	const Texture& GetTexture(const std::string& name) const;
	ModelGroup *GetModelGroup() const;

	const std::string& GetPath() const;
};

#endif
