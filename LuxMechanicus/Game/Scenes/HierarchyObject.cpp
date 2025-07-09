#include "HierarchyObject.h"

HierarchyObject::~HierarchyObject() {
    if (mParent) {
        mParent->RemoveChild(this);
    }

    ClearChildren();
}

void HierarchyObject::AddChild(HierarchyObject* child) {
    if (child) {
        child->SetParent(this);
        mChildren.push_back(child);
    }
}

void HierarchyObject::RemoveChild(HierarchyObject* child) {
    if (!child) {
        std::cout << "Could not find a child to remove" << std::endl;
        return;
    }
    
    auto iterator = std::find(mChildren.begin(), mChildren.end(), child);
    if (iterator != mChildren.end()) {
        mChildren.erase(iterator);
        child->mParent = nullptr;  
    }
}

void HierarchyObject::SetParent(HierarchyObject* parent) {
    mParent = parent;
}

void HierarchyObject::ClearChildren() {
    mChildren.clear();
}

const std::vector<HierarchyObject*>& HierarchyObject::GetChildren() const {
    return mChildren;
}

HierarchyObject* HierarchyObject::GetParent() const {
    return mParent;
}