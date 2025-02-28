#ifndef RESOURCE_HOLDER_H
#define RESOURCE_HOLDER_H

#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>
#include <QString>
#include <QDebug>


template <typename Resource, typename Identifier>
class ResourceHolder
{
    public:
        void						load(Identifier id, const QString &filename);

        Resource&					get(Identifier id);
        const Resource&				get(Identifier id) const;


    public:
        void						insertResource(Identifier id, std::unique_ptr<Resource> resource);


    private:
        std::map<Identifier, std::unique_ptr<Resource>>	mResourceMap;
};

template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::load(Identifier id, const QString& filename)
{
    std::unique_ptr<Resource> resource(new Resource());
    if (!resource->load(filename))
    {
        qDebug() << "ResourceHolder::load - Failed to load " + filename;
    }

    insertResource(id, std::move(resource));
}

template <typename Resource, typename Identifier>
Resource& ResourceHolder<Resource, Identifier>::get(Identifier id)
{
    auto found = mResourceMap.find(id);
    assert(found != mResourceMap.end());

    return *found->second;
}

template <typename Resource, typename Identifier>
const Resource& ResourceHolder<Resource, Identifier>::get(Identifier id) const
{
    auto found = mResourceMap.find(id);
    assert(found != mResourceMap.end());

    return *found->second;
}

template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::insertResource(Identifier id, std::unique_ptr<Resource> resource)
{
    auto inserted = mResourceMap.insert(std::make_pair(id, std::move(resource)));
    assert(inserted.second);
}

#endif // RESOURCE_HOLDER_H
