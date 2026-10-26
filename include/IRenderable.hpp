#ifndef I_RENDERABLE_HPP
#define I_RENDERABLE_HPP

class IRenderable {
public:
    virtual ~IRenderable() = default;
    virtual void render() const = 0; // Metodă pur virtuală
};

#endif