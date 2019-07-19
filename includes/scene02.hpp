#ifndef SCENE02_HPP
#define SCENE02_HPP

#include <vector>
#include "base_scene.hpp"
#include "util.hpp"
#include "import_mesh.hpp"
#include "texture2d.hpp"
#include "scene_update_args.hpp"

class CScene02 : public CBaseScene
{
public:
    CScene02();
    ~CScene02();

    void Init(CSceneInitArgs &args);
    void SetGlStates();
    void Render(CSceneUpdateArgs &args);
    void Update(CSceneUpdateArgs &args);
    void Release();
};
#endif