//
//  GameViewController.m
//  OpenGL_ES_Lab
//
//  Created by ZhaoZhihui on 16/6/30.
//  Copyright © 2016年 ZhaoZhihui. All rights reserved.
//

#import "GameViewController.h"
#import <OpenGLES/ES3/glext.h>

//Interface of C++ code for native obj-c to invoke
#include "esUtil.h"

extern void esMain( ESContext *esContext );

@interface GameViewController () {
    
    ESContext _esContext;
    
}

@property (strong, nonatomic) EAGLContext *context;
@property (strong, nonatomic) GLKBaseEffect *effect;

- (void)setupGL;
- (void)tearDownGL;

@end

@implementation GameViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    
    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    
    [self setupGL];
}

- (void)dealloc
{
    [self tearDownGL];
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    
    if ([self isViewLoaded] && ([[self view] window] == nil)) {
        self.view = nil;
        
        [self tearDownGL];
        
        if ([EAGLContext currentContext] == self.context) {
            [EAGLContext setCurrentContext:nil];
        }
        self.context = nil;
    }
    
    // Dispose of any resources that can be recreated.
}

- (BOOL)prefersStatusBarHidden {
    return YES;
}

- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];
    
    memset( &_esContext, 0, sizeof( _esContext ) );
    esMain( &_esContext );
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];
    
    if (_esContext.shutdownFunc){
        _esContext.shutdownFunc(&_esContext);
    }
}

#pragma mark - GLKView and GLKViewController delegate methods

- (void)update
{
    if ( _esContext.updateFunc )
    {
        _esContext.updateFunc( &_esContext, self.timeSinceLastUpdate );
    }
    /*
     float aspect = fabs(self.view.bounds.size.width / self.view.bounds.size.height);
     GLKMatrix4 projectionMatrix = GLKMatrix4MakePerspective(GLKMathDegreesToRadians(65.0f), aspect, 0.1f, 100.0f);
     
     self.effect.transform.projectionMatrix = projectionMatrix;
     
     GLKMatrix4 baseModelViewMatrix = GLKMatrix4MakeTranslation(0.0f, 0.0f, -4.0f);
     baseModelViewMatrix = GLKMatrix4Rotate(baseModelViewMatrix, _rotation, 0.0f, 1.0f, 0.0f);
     
     // Compute the model view matrix for the object rendered with GLKit
     GLKMatrix4 modelViewMatrix = GLKMatrix4MakeTranslation(0.0f, 0.0f, -1.5f);
     modelViewMatrix = GLKMatrix4Rotate(modelViewMatrix, _rotation, 1.0f, 1.0f, 1.0f);
     modelViewMatrix = GLKMatrix4Multiply(baseModelViewMatrix, modelViewMatrix);
     
     self.effect.transform.modelviewMatrix = modelViewMatrix;
     
     // Compute the model view matrix for the object rendered with ES2
     modelViewMatrix = GLKMatrix4MakeTranslation(0.0f, 0.0f, 1.5f);
     modelViewMatrix = GLKMatrix4Rotate(modelViewMatrix, _rotation, 1.0f, 1.0f, 1.0f);
     modelViewMatrix = GLKMatrix4Multiply(baseModelViewMatrix, modelViewMatrix);
     
     _normalMatrix = GLKMatrix3InvertAndTranspose(GLKMatrix4GetMatrix3(modelViewMatrix), NULL);
     
     _modelViewProjectionMatrix = GLKMatrix4Multiply(projectionMatrix, modelViewMatrix);
     
     _rotation += self.timeSinceLastUpdate * 0.5f;
     */
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    _esContext.width = (GLint)view.drawableWidth;
    _esContext.height = (GLint)view.drawableHeight;
    
    if ( _esContext.drawFunc )
    {
        _esContext.drawFunc( &_esContext );
    }
    /*
     glClearColor(0.65f, 0.65f, 0.65f, 1.0f);
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     
     glBindVertexArrayOES(_vertexArray);
     
     // Render the object with GLKit
     [self.effect prepareToDraw];
     
     glDrawArrays(GL_TRIANGLES, 0, 36);
     
     // Render the object again with ES2
     glUseProgram(_program);
     
     glUniformMatrix4fv(uniforms[UNIFORM_MODELVIEWPROJECTION_MATRIX], 1, 0, _modelViewProjectionMatrix.m);
     glUniformMatrix3fv(uniforms[UNIFORM_NORMAL_MATRIX], 1, 0, _normalMatrix.m);
     
     glDrawArrays(GL_TRIANGLES, 0, 36);
     */
}

@end
