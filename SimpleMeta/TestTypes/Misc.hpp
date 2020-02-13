#pragma once

#include "Bindings.hpp"



struct NameId
{
  NameId()
  {
  }
  NameId(int id, const std::string& name) : mId(id), mName(name)
  {
  }

  bool operator==(const NameId& rhs) const
  {
    return mId == rhs.mId && mName == rhs.mName;
  }
  static void Bind(MetaLibrary& library, BoundType& boundType)
  {
    BindField(library, boundType, NameId, mId);
    BindField(library, boundType, NameId, mName);
  }

  int mId;
  std::string mName;
};

struct NameIdList
{
  bool operator==(const NameIdList& rhs) const
  {
    return mIds == rhs.mIds;
  }
  static void Bind(MetaLibrary& library, BoundType& boundType)
  {
    BindField(library, boundType, NameIdList, mIds);
  }

  std::vector<NameId> mIds;
};

struct NumberList
{
  bool operator==(const NumberList& rhs) const
  {
    return mData == rhs.mData;
  }
  static void Bind(MetaLibrary& library, BoundType& boundType)
  {
    BindField(library, boundType, NumberList, mData);
  }

  std::vector<int> mData;
};

struct Dictionary
{
  bool operator==(const Dictionary& rhs) const
  {
    return mData == rhs.mData;
  }
  static void Bind(MetaLibrary& library, BoundType& boundType)
  {
    BindField(library, boundType, Dictionary, mData);
  }
  
  void Add(const std::string& key, const std::string& value)
  {
    mData.insert(std::make_pair(key, value));
  }

  std::unordered_map<std::string, std::string> mData;
};

struct MyStruct
{
  int mData;
  float mFloat;
  char mChar1;
  int value = 5;
  char mChar2;
  double mDouble1;
  std::string mString1;

  bool operator==(const MyStruct& rhs) const
  {
    return mData == rhs.mData &&
      mFloat == rhs.mFloat &&
      mChar1 == rhs.mChar1 &&
      value == rhs.value &&
      mChar2 == rhs.mChar2 &&
      mDouble1 == rhs.mDouble1 &&
      mString1 == rhs.mString1;
  }
};

inline void BindMyStruct(MetaLibrary& library, BoundType& boundType)
{
  BindField(library, boundType, MyStruct, mData);
  BindField(library, boundType, MyStruct, mFloat);
  BindField(library, boundType, MyStruct, mChar1);
  BindField(library, boundType, MyStruct, mChar2);
  BindField(library, boundType, MyStruct, mDouble1);
  BindField(library, boundType, MyStruct, mString1);
}

struct AnimationKeyFrame {
    std::string lerpMode;
    std::vector<float> postTransform;

    static void Bind(MetaLibrary& library, BoundType& boundType)
    {
        BindFieldAs(library, boundType, AnimationKeyFrame, lerpMode, "lerp_mode");
        BindFieldAs(library, boundType, AnimationKeyFrame, postTransform, "post");
    }
};

struct AnimationBone {
    std::unordered_map<std::string, AnimationKeyFrame> positions;
    std::unordered_map<std::string, AnimationKeyFrame> rotations;

    static void Bind(MetaLibrary& library, BoundType& boundType)
    {
        BindFieldAs(library, boundType, AnimationBone, positions, "position");
        BindFieldAs(library, boundType, AnimationBone, rotations, "rotation");
    }
};

struct AnimationEffect {
    std::string effectName;
    bool enabled;

    static void Bind(MetaLibrary& library, BoundType& boundType)
    {
        BindFieldAs(library, boundType, AnimationEffect, effectName, "effect_name");
        BindFieldAs(library, boundType, AnimationEffect, enabled, "enabled");
    }
};

struct Animation {
    bool looping = false;
    std::string animTimeUpdate;
    float blendWeight = 1.0f;
    bool overridePreviousAnimation = false;
    std::unordered_map<std::string, AnimationBone> bones;
    std::unordered_map<std::string, std::vector<AnimationEffect> > effects;

    static void Bind(MetaLibrary& library, BoundType& boundType) {
        BindFieldAs(library, boundType, Animation, looping, "looping");
        BindFieldAs(library, boundType, Animation, animTimeUpdate, "anim_time_update");
        BindFieldAs(library, boundType, Animation, blendWeight, "blend_weight");
        BindFieldAs(library, boundType, Animation, overridePreviousAnimation, "override_previous_animation");
        BindFieldAs(library, boundType, Animation, bones, "bones");
        BindFieldAs(library, boundType, Animation, effects, "sound_effects");
    }
};

struct AnimationDictionary {
    std::string formatVersion;
    std::unordered_map<std::string, Animation> animations;

    static void Bind(MetaLibrary& library, BoundType& boundType)
    {
        BindFieldAs(library, boundType, AnimationDictionary, formatVersion, "format_version");
        BindFieldAs(library, boundType, AnimationDictionary, animations, "animations");
    }

    bool operator==(const AnimationDictionary& rhs) const {
        return true;
    }
};