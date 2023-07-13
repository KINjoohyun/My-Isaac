#include "stdafx.h"
#include "ResourceMgr.h"
#include "rapidcsv.h"

ResourceMgr::~ResourceMgr()
{
	UnLoadAll();
}

void ResourceMgr::Init()
{
	LoadFormCSV("scripts/DefaultResourceList.csv", true); //파일의 경로를 적은 파일의 경로를 적은 파일을 호출하는 함수를 호출하는 함수
}
void ResourceMgr::UnLoadAll()
{
	{
		auto it = mapTexture.begin();
		while (it != mapTexture.end())
		{
			if (std::get<1>(it->second))
			{
				delete std::get<0>(it->second);
				it = mapTexture.erase(it);
			}
			else
			{
				it++;
			}
		}
	}
	{
		auto it = mapFont.begin();
		while (it != mapFont.end())
		{
			if (std::get<1>(it->second))
			{
				delete std::get<0>(it->second);
				it = mapFont.erase(it);
			}
			else
			{
				it++;
			}
		}
	}
	{
		auto it = mapSoundBuffer.begin();
		while (it != mapSoundBuffer.end())
		{
			if (std::get<1>(it->second))
			{
				delete std::get<0>(it->second);
				it = mapSoundBuffer.erase(it);
			}
			else
			{
				it++;
			}
		}
	}
	{
		auto it = mapAnimationClip.begin();
		while (it != mapAnimationClip.end())
		{
			if (std::get<1>(it->second))
			{
				delete std::get<0>(it->second);
				it = mapAnimationClip.erase(it);
			}
			else
			{
				it++;
			}
		}
	}
}

void ResourceMgr::LoadFormCSV(const std::string& path, bool isDefault)
{
	rapidcsv::Document doc(path);
	std::vector<int> types = doc.GetColumn<int>(0); // RESOURCE TYPE
	std::vector<std::string> paths = doc.GetColumn<std::string>(1); // PATH
	for (int i = 0; i < types.size(); i++)
	{
		Load((ResourceTypes)types[i], paths[i], isDefault);
	}
}
void ResourceMgr::Load(ResourceTypes t, const std::string path, bool isDefault)
{
	switch (t)
	{
	case ResourceTypes::Texture:
	{
		auto it = mapTexture.find(path);
		if (mapTexture.end() == it)
		{
			auto texture = new sf::Texture();
			texture->loadFromFile(path);
			mapTexture.insert({ path, { texture, isDefault } });
		}
	}
	break;
	case ResourceTypes::Font:
	{
		auto it = mapFont.find(path);
		if (mapFont.end() == it)
		{
			auto font = new sf::Font();
			font->loadFromFile(path);
			mapFont.insert({ path, { font, isDefault } });
		}
	}
	break;
	case ResourceTypes::SoundBuffer:
	{
		auto it = mapSoundBuffer.find(path);
		if (mapSoundBuffer.end() == it)
		{
			auto sb = new sf::SoundBuffer();
			sb->loadFromFile(path);
			mapSoundBuffer.insert({ path, { sb, isDefault } });
		}
	}
	case ResourceTypes::AnimationClip:
	{
		auto it = mapAnimationClip.find(path);
		if (mapAnimationClip.end() == it)
		{
			auto clip = new AnimationClip();
			if (!clip->LoadFromFile(path))
			{
				std::cerr << "ERROR: AnimationClip LoadFromFile()" << std::endl;
			}
			mapAnimationClip.insert({ path, { clip, isDefault } });
		}
	}
	break;
	}
}
void ResourceMgr::Unload(ResourceTypes t, const std::string id)
{
	switch (t)
	{
	case ResourceTypes::Texture:
	{
		auto it = mapTexture.find(id);
		if (it != mapTexture.end())
		{
			if (!std::get<1>(it->second))
			{
				delete std::get<0>(it->second);
				mapTexture.erase(it);
			}
			else
			{
				std::cerr << "ERROR: ResourceMgr Unload() Texture" << std::endl;
			}
		}
	}
	break;
	case ResourceTypes::Font:
	{
		auto it = mapFont.find(id);
		if (it != mapFont.end())
		{
			if (!std::get<1>(it->second))
			{
				delete std::get<0>(it->second);
				mapFont.erase(it);
			}
			else
			{
				std::cerr << "ERROR: ResourceMgr Unload() Font" << std::endl;
			}
		}
	}
	break;
	case ResourceTypes::SoundBuffer:
	{
		auto it = mapSoundBuffer.find(id);
		if (it != mapSoundBuffer.end())
		{
			if (!std::get<1>(it->second))
			{
				delete std::get<0>(it->second);
				mapSoundBuffer.erase(it);
			}
			else
			{
				std::cerr << "ERROR: ResourceMgr Unload() SoundBuffer" << std::endl;
			}
		}
	}
	case ResourceTypes::AnimationClip:
	{
		auto it = mapAnimationClip.find(id);
		if (it != mapAnimationClip.end())
		{
			if (!std::get<1>(it->second))
			{
				delete std::get<0>(it->second);
				mapAnimationClip.erase(it);
			}
			else
			{
				std::cerr << "ERROR: ResourceMgr Unload() AnimationClip" << std::endl;
			}
		}
	}
	break;
	}

}

sf::Texture* ResourceMgr::GetTexture(const std::string& id)
{
	auto it = mapTexture.find(id);
	if (it != mapTexture.end())
	{
		return std::get<0>(it->second);
	}
	return nullptr;
}
sf::Font* ResourceMgr::GetFont(const std::string& id)
{
	auto it = mapFont.find(id);
	if (it != mapFont.end())
	{
		return std::get<0>(it->second);
	}

	return nullptr;
}
sf::SoundBuffer* ResourceMgr::GetSoundBuffer(const std::string& id)
{
	auto it = mapSoundBuffer.find(id);
	if (it != mapSoundBuffer.end())
	{
		return std::get<0>(it->second);
	}
	return nullptr;
}
AnimationClip* ResourceMgr::GetAnimationClip(const std::string& id)
{
	auto it = mapAnimationClip.find(id);
	if (it != mapAnimationClip.end())
	{
		return std::get<0>(it->second);
	}
	return nullptr;
}