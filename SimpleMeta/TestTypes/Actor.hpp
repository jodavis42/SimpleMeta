#pragma once

#include "Bindings.hpp"
#include <string>
#include <vector>
#include <map>

struct ActorComponent;

struct ActorDescription {
    std::string mId;
    std::string mRuntimeType;
    bool mIsSummonable;
    bool mIsSpawnable;
    bool mIsExperimental;

    bool operator==(const ActorDescription& rhs) const = default;

    static void Bind(MetaLibrary& library, BoundType& boundType) {
        BindFieldAs(library, boundType, ActorDescription, mId, "identifier");
        BindFieldAs(library, boundType, ActorDescription, mRuntimeType, "runtime_identifier");
        BindFieldAs(library, boundType, ActorDescription, mIsSummonable, "is_summonable");
        BindFieldAs(library, boundType, ActorDescription, mIsSpawnable, "is_spawnable");
        BindFieldAs(library, boundType, ActorDescription, mIsExperimental, "is_experimental");
    }
};


struct ActorDefinition {
    ActorDefinition() = default;

    bool operator==(const ActorDefinition& rhs) const {
        return mDescription == rhs.mDescription && mComponentGroups == rhs.mComponentGroups && mComponents == rhs.mComponents;
    }

    static void Bind(MetaLibrary& library, BoundType& boundType) {
        BindFieldAs(library, boundType, ActorDefinition, mDescription, "description");
        BindFieldAs(library, boundType, ActorDefinition, mComponentGroups, "component_groups");
        BindFieldAs(library, boundType, ActorDefinition, mComponents, "components");
    }

    ActorDescription mDescription;
    std::unordered_map<std::string, std::vector<ActorComponent*> > mComponentGroups;
    std::vector<ActorComponent*> mComponents;
};

struct ActorDefinitionAsset {
    ActorDefinitionAsset() = default;

    bool operator==(const ActorDefinitionAsset& rhs) const {
        return formatVersion == rhs.formatVersion && definition == rhs.definition;
    }

    static void Bind(MetaLibrary& library, BoundType& boundType) {
        BindFieldAs(library, boundType, ActorDefinitionAsset, formatVersion, "format_version");
        BindFieldAs(library, boundType, ActorDefinitionAsset, definition, "minecraft:entity");
    }

    std::string formatVersion;
    ActorDefinition definition;
};