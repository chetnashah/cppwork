#include <iostream>
class IRenderer
{
public:
    virtual void render() = 0;      // = 0 means pure virtual, will prevent IRenderer to be instantiated
    virtual void update() = 0;      // = 0 means pure virtual, will prevent IRenderer to be instantiated
    virtual ~IRenderer() = default; // good practice for cleanup of implementers
};

class OpenGLRenderer : public IRenderer
{
public:
    void render() override
    {
        std::cout << "OpenGLRenderer::render()" << std::endl;
    }
    void update() override
    {
        std::cout << "OpenGLRenderer::update()" << std::endl;
    }
};

class VulkanRenderer : public IRenderer
{
public:
    void render() override
    {
        std::cout << "VulkanRenderer::render()" << std::endl;
    }
    void update() override
    {
        std::cout << "VulkanRenderer::update()" << std::endl;
    }
};

int main()
{
    IRenderer *renderer = new OpenGLRenderer();
    renderer->render();
    renderer->update();
    delete renderer;

    renderer = new VulkanRenderer();
    renderer->render();
    renderer->update();
    delete renderer;

    return 0;
}