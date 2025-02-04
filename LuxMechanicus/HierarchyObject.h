#pragma once
#include <vector>
#include <iostream>

class HierarchyObject {
public:
    virtual ~HierarchyObject();
    
    virtual void AddChild(HierarchyObject* child);

    virtual void RemoveChild(HierarchyObject* child);

    virtual void SetParent(HierarchyObject* parent);

    virtual HierarchyObject* GetParent() const;

    const std::vector<HierarchyObject*>& GetChildren() const;

    virtual void ClearChildren();

protected:
    std::vector<HierarchyObject*> mChildren;
    HierarchyObject* mParent;
};