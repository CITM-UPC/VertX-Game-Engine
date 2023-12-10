#pragma once

#include <list>

template <class T>
class Tree {

private:
	T* _parent_ptr{ nullptr };
	std::list<T> _children;

	void updateChildrenParent() {
		for (Tree& child : _children) {
			child._parent_ptr = static_cast<T*>(this);
			child.updateChildrenParent();
		}
	}

public:
	T* parent() { return _parent_ptr; }
	const T* parent() const { return _parent_ptr; }
	const std::list<T>& children() const { return _children; }
	std::list<T>& children() { return _children; }

	template <typename... Args>
	T& addChild(Args... args) {
		_children.emplace_back(args...);
		updateChildrenParent();
		return _children.back();
	}

	auto removeChikd(std::list<T>::iterator itr) {
		return _children.erase(itr);
	}

	Tree() = default;
	Tree(Tree&& t) noexcept : _children(std::move(t._children)) {
		if (t._parent_ptr) t._parent_ptr->removeChild((T&&)t);
	}
	virtual ~Tree() = default;

private:
	Tree(const Tree&) = delete;
	Tree& operator=(const Tree&) = delete;
};
