//
//  Scene.hpp
//  OpenGL_ES_Lab
//
//  Created by ZhaoZhihui on 16/8/2.
//  Copyright © 2016年 ZhaoZhihui. All rights reserved.
//

#ifndef Scene_hpp
#define Scene_hpp

#include <stdio.h>
#include <string>

using namespace std;

class Scene{
public:
    static Scene* CreateScene(const string& sceneName);
    
    void enter();
    
    void update(float delta);
    
    void render();
    
    void exit();
    
protected:
    
    
};


#endif /* Scene_hpp */
