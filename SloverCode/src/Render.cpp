#include "Render.h"
#include "ngl/ShaderManager.h"
#include "ngl/VBOPrimitives.h"

Render::Render()
{
}
void Render::initialiseRender(EnvironmentAndRigidBody* _environment)
{

    m_bbox = new ngl::BBox(_environment->getBBox()->center(),_environment->getBBox()->width(),_environment->getBBox()->height(),_environment->getBBox()->depth());
    ngl::VBOPrimitives *prim =  ngl::VBOPrimitives::instance();
    prim->createVBOSphere("particle",1,32);

    ngl::ShaderManager *shader=ngl::ShaderManager::instance();

    shader->createShaderProgram("3DShading");

    shader->attachShader("3DShadingVertex",ngl::VERTEX);
    shader->attachShader("3DShadingFragment",ngl::FRAGMENT);
    shader->loadShaderSource("3DShadingVertex","shaders/Basic.vs");
    shader->loadShaderSource("3DShadingFragment","shaders/Basic.fs");

    shader->compileShader("3DShadingVertex");
    shader->compileShader("3DShadingFragment");
    shader->attachShaderToProgram("3DShading","3DShadingVertex");
    shader->attachShaderToProgram("3DShading","3DShadingFragment");
    shader->bindAttribute( "3DShading", 0, "inVertex");
    shader->bindAttribute( "3DShading", 1, "perVColor");
    shader->linkProgramObject("3DShading");
    shader->use("3DShading");

    shader->createShaderProgram("ConstantShading");

    shader->attachShader("ConstantShadingVertex",ngl::VERTEX);
    shader->attachShader("ConstantShadingFragment",ngl::FRAGMENT);
    shader->loadShaderSource("ConstantShadingVertex","shaders/Colour.vs");
    shader->loadShaderSource("ConstantShadingFragment","shaders/Colour.fs");

    shader->compileShader("ConstantShadingVertex");
    shader->compileShader("ConstantShadingFragment");
    shader->attachShaderToProgram("ConstantShading","ConstantShadingVertex");
    shader->attachShaderToProgram("ConstantShading","ConstantShadingFragment");
    shader->bindAttribute( "ConstantShading", 0, "inVertex");
    shader->bindAttribute( "ConstantShading", 1, "perVColor");
    shader->linkProgramObject("ConstantShading");
    shader->use("ConstantShading");
}

void Render::draw(EnvironmentAndRigidBody *_environment, SPHSolver *_solver,ngl::TransformStack _transformStack, ngl::Camera* _cam)
{
//    std::cout << "Enters the Render" << std::endl;
    std::string _shadername;
    std::vector<FluidParticle> _fluidParticleList;
    std::vector<Particle> _rbdSphereList;
    ngl::VBOPrimitives* prim = ngl::VBOPrimitives::instance();

    ngl::ShaderManager *shader=ngl::ShaderManager::instance();

    /*--- Drawing the Boundary Box -----------*/
    (*shader)["ConstantShading"]->use();
    shader->setShaderParam4f("ConstantShading", "inColor", _environment->getBoundaryColour().m_r, _environment->getBoundaryColour().m_g, _environment->getBoundaryColour().m_b ,1);
    shader->setShaderParamFromMatrix("ConstantShading","ViewMatrix",_cam->getModelView());
    _transformStack.loadGlobalAndCurrentMatrixToShader("ConstantShading","ModelMatrix");
            m_bbox->draw();


    _fluidParticleList = _solver->getFluidParticleList();
    _rbdSphereList = _environment->getRBDSphereList();

    /*---Checking if the user wants to visualize the fluid particles------*/
    if (_solver->getDrawFluidParticles())
    {
        if (_solver->getVisualizationType() == "3DShading")
        {
            (*shader)["3DShading"]->use();
            _shadername = "3DShading";
            shader->setShaderParamFromMatrix(_shadername,"ViewMatrix",_cam->getModelView());
        }
        if (_solver->getVisualizationType() == "ConstantShading")
        {
             (*shader)["ConstantShading"]->use();
            _shadername = "ConstantShading";
            shader->setShaderParamFromMatrix(_shadername,"ViewMatrix",_cam->getModelView());
        }

        /*--- Drawing the fluid particles--------*/
        _transformStack.pushTransform();
        {
            for (int i = 0;i<_fluidParticleList.size();++i)
            {
                ngl::Vector _position = _fluidParticleList[i].getCurrentPosition();
                ngl::Real _scale = _fluidParticleList[i].getDrawRadius();
                ngl::Colour _colour = _fluidParticleList[i].getParticleColour();
                _transformStack.getCurrentTransform().setPosition(_position);
                _transformStack.getCurrentTransform().setScale(_scale,_scale,_scale);
                shader->setShaderParam4f(_shadername, "inColor", _colour.m_r,_colour.m_g,_colour.m_b,1.0);
                shader->setShaderParamFromMatrix(_shadername,"ModelMatrix", _transformStack.getCurrAndGlobal().getMatrix());
                prim->draw("particle");
            }

        }
        _transformStack.popTransform();
    }
    /*---Checking are obstacles enabled or not------*/
    if (_environment->getObstacleEnabled())
    {

    (*shader)["3DShading"]->use();
    shader->setShaderParamFromMatrix("3DShading","ViewMatrix",_cam->getModelView());

        /*--- Drawing the RBD spheres------*/
        _transformStack.pushTransform();
        {
            for (int i = 0;i<_rbdSphereList.size();++i)
            {
                ngl::Vector _position = _rbdSphereList[i].getCurrentPosition();
                ngl::Real _scale = _rbdSphereList[i].getDrawRadius();
                ngl::Colour _colour = _rbdSphereList[i].getParticleColour();
                _transformStack.getCurrentTransform().setPosition(_position);
                _transformStack.getCurrentTransform().setScale(_scale,_scale,_scale);
                shader->setShaderParam4f("3DShading", "inColor", _colour.m_r,_colour.m_g,_colour.m_b,1.0);
                shader->setShaderParamFromMatrix("3DShading","ModelMatrix", _transformStack.getCurrAndGlobal().getMatrix());
                prim->draw("particle");
            }
        }
        _transformStack.popTransform();
    }
}

void Render::updateProjection(ngl::Camera *_cam)
{
    ngl::ShaderManager *shader=ngl::ShaderManager::instance();
    (*shader)["3DShading"]->use();
          shader->setShaderParamFromMatrix("3DShading","projectionMatrix",_cam->getProjection());
    (*shader)["ConstantShading"]->use();
          shader->setShaderParamFromMatrix("ConstantShading","projectionMatrix",_cam->getProjection());
}
