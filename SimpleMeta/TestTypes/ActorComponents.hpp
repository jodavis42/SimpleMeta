#pragma once

#include "Math.hpp"
#include <string>

struct ActorComponent
{
    bool operator==(const ActorComponent& rhs) const {
        return true;
    }

    virtual BoundType* VirtualGetBoundType() const {
        return StaticTypeId<ActorComponent>::GetBoundType();
    }

    static void Bind(MetaLibrary& library, BoundType& boundType) {
    }
};

struct BreedableComponent : public ActorComponent {
    bool operator==(const BreedableComponent& rhs) const
    {
        return mMateType == rhs.mMateType;
    }

    virtual BoundType* VirtualGetBoundType() const
    {
        return StaticTypeId<BreedableComponent>::GetBoundType();
    }

    static void Bind(MetaLibrary& library, BoundType& boundType)
    {
        BindBase(library, boundType, ActorComponent);
        BindField(library, boundType, BreedableComponent, mMateType);
    }

    std::string mMateType;
};

struct FamilyComponent : public ActorComponent {
    bool operator==(const FamilyComponent& rhs) const
    {
        return mFamilyTypes == rhs.mFamilyTypes;
    }

    virtual BoundType* VirtualGetBoundType() const {
        return StaticTypeId<FamilyComponent>::GetBoundType();
    }

    static void Bind(MetaLibrary& library, BoundType& boundType) {
        BindBase(library, boundType, ActorComponent);
        BindFieldAs(library, boundType, FamilyComponent, mFamilyTypes, "family");
    }

    std::vector<std::string> mFamilyTypes;
};

struct AttackDamageComponent : public ActorComponent {
    bool operator==(const AttackDamageComponent& rhs) const
    {
        return mValue == rhs.mValue;
    }

    virtual BoundType* VirtualGetBoundType() const {
        return StaticTypeId<FamilyComponent>::GetBoundType();
    }

    static void Bind(MetaLibrary& library, BoundType& boundType) {
        BindBase(library, boundType, ActorComponent);
        BindFieldAs(library, boundType, AttackDamageComponent, mValue, "value");
    }

    float mValue;
};

struct CollisionBoxComponent : public ActorComponent {
    bool operator==(const CollisionBoxComponent& rhs) const
    {
        return mWidth == rhs.mWidth && mHeight == rhs.mHeight;
    }

    virtual BoundType* VirtualGetBoundType() const {
        return StaticTypeId<CollisionBoxComponent>::GetBoundType();
    }

    static void Bind(MetaLibrary& library, BoundType& boundType) {
        BindBase(library, boundType, ActorComponent);
        BindFieldAs(library, boundType, CollisionBoxComponent, mWidth, "width");
        BindFieldAs(library, boundType, CollisionBoxComponent, mHeight, "height");
    }

    float mWidth, mHeight;
};

struct VariantComponent : public ActorComponent {
    bool operator==(const VariantComponent& rhs) const
    {
        return mValue == rhs.mValue;
    }

    virtual BoundType* VirtualGetBoundType() const {
        return StaticTypeId<VariantComponent>::GetBoundType();
    }

    static void Bind(MetaLibrary& library, BoundType& boundType) {
        BindBase(library, boundType, ActorComponent);
        BindFieldAs(library, boundType, VariantComponent, mValue, "value");
    }

    int mValue;
};