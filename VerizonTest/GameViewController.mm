//
//  GameViewController.m
//  VerizonTest
//
//  Created by James Folk on 6/21/16.
//  Copyright © 2016 NJLIGames Ltd. All rights reserved.
//

#import "GameViewController.h"
#import <OpenGLES/ES2/glext.h>
#include "Cubenado.hpp"
#include "Node.h"
#include "Geometry.h"

#include <pthread.h>

#include "glm/glm.hpp"

struct thread_data{
    float timeSinceLastUpdate;
    float aspect;
};

@interface GameViewController   ()
{
    struct thread_data threadData;
}
@property (strong, nonatomic) EAGLContext *context;

@property (weak, nonatomic) IBOutlet UILabel *lblNumberOfCubes;
@property (weak, nonatomic) IBOutlet UISlider *sldNumberOfCubes;
@property (weak, nonatomic) IBOutlet UILabel *lblRandomnessPercent;
@property (weak, nonatomic) IBOutlet UISlider *sldRandomnessPercent;
@property (weak, nonatomic) IBOutlet UITextField *tf;
@property (strong,nonatomic) NSMutableArray *theData;
- (void)setupGL;
- (void)tearDownGL;

@end

@implementation GameViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

    self.preferredFramesPerSecond = 30;
    
    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat16;
    view.contentScaleFactor = [UIScreen mainScreen].scale;
    
    njli::Cubenado::createInstance();
    
    NSMutableString *path = [[NSMutableString alloc] initWithString:[[NSBundle mainBundle] resourcePath]];
    [path appendString: [[NSString alloc] initWithCString:"/assets/"
                                                 encoding:NSASCIIStringEncoding]];
    njli::Cubenado::setBundlePath([path UTF8String]);
    
    [self setupGL];
    
    NSString *s = [NSString stringWithFormat:@"Randomness: %.0f%%", njli::Cubenado::getInstance()->getRandomness()*100];
    [_lblRandomnessPercent setText:s];
    [_sldRandomnessPercent setMinimumValue:0];
    [_sldRandomnessPercent setMaximumValue:1];
    [_sldRandomnessPercent setValue:njli::Cubenado::getInstance()->getRandomness()];
    
    s = [NSString stringWithFormat:@"Number of Cubes: %lu", njli::Cubenado::getInstance()->numberOfCubes()];
    [_lblNumberOfCubes setText:s];
    [_sldNumberOfCubes setMinimumValue:10];
    [_sldNumberOfCubes setMaximumValue:njli::Cubenado::getInstance()->numberOfCubes()];
    [_sldNumberOfCubes setValue:10000];
    
    UIPickerView *picker = [[UIPickerView alloc] init];
    picker.dataSource = self;
    picker.delegate = self;
    self.tf.inputView = picker;
    [self.tf becomeFirstResponder];
    [self.tf resignFirstResponder];
    
    
    std::vector<std::string> names = njli::Cubenado::getInstance()->getShaderNames();
    
    self.theData = [[NSMutableArray alloc] initWithCapacity:names.size()];
    
    NSUInteger idx = 0;
    for (std::vector<std::string>::iterator i = names.begin(); i != names.end(); i++)
    {
        [self.theData insertObject:[NSString stringWithUTF8String:(*i).c_str()] atIndex:idx];
        idx++;
    }
//    self.theData = @[@"one",@"two",@"three",@"four"];
}

-(NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component {
    return self.theData.count;
}

-(NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView {
    return  1;
}

-(NSString *)pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component {
    return self.theData[row];
}

-(void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component {
    self.tf.text = self.theData[row];
    
    std::string s([self.theData[row] UTF8String]);
    njli::Cubenado::getInstance()->setShader(s);
    
    [self.tf resignFirstResponder];
}







- (void)dealloc
{    
    [self tearDownGL];
    
    njli::Cubenado::destroyInstance();
    
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
    
    GLKView *view = (GLKView *)self.view;
    njli::Cubenado::getInstance()->create(0, 0, view.frame.size.width * view.contentScaleFactor, view.frame.size.height * view.contentScaleFactor);
    
    if(njli::Cubenado::getInstance()->getCamera()->getNodeOwner())
    {
        glm::mat4x4 cameraT(1.0f);
        njli::Cubenado::getInstance()->getCamera()->getNodeOwner()->setTransform(cameraT);
    }
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];
    
    njli::Cubenado::getInstance()->destroy();
}

#pragma mark - GLKView and GLKViewController delegate methods

void *_update(void *threadarg)
{
    struct thread_data *my_data;
    my_data = (struct thread_data *) threadarg;
    
    njli::Cubenado::getInstance()->update(my_data->timeSinceLastUpdate);
    
    pthread_exit(NULL);
}

- (void)update
{
    njli::Cubenado::getInstance()->update(self.timeSinceLastUpdate);
    
//    threadData.timeSinceLastUpdate = self.timeSinceLastUpdate;
//    
//    pthread_t threads;
//    int rc;
//    rc = pthread_create(&threads, NULL, _update, (void *)&threadData);
//    if (rc){
//        printf("ERROR; return code from pthread_create() is %d\n", rc);
//        exit(-1);
//    }
//    
//    if(pthread_join(threads, NULL))
//    {
//        
//        fprintf(stderr, "Error joining thread\n");
//        exit(-1);
//    }
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    njli::Cubenado::getInstance()->render();
}

-(void)viewDidLayoutSubviews
{
    [super viewDidLayoutSubviews];
    
    GLKView *view = (GLKView *)self.view;
    
    njli::Cubenado::getInstance()->getCamera()->setAspectRatio(fabs(self.view.bounds.size.width / self.view.bounds.size.height));
    
    njli::Cubenado::getInstance()->resize(0, 0, view.frame.size.width * view.contentScaleFactor, view.frame.size.height * view.contentScaleFactor);
}

- (IBAction)numberOfCubesChanged:(id)sender
{
    UISlider *slider = sender;
    [slider value];
    
    int numberOfCubes = (int)[slider value];
    njli::Cubenado::getInstance()->setNumberOfCubes(numberOfCubes);
    
    NSString *s = [NSString stringWithFormat:@"Number of Cubes: %d", numberOfCubes];
    [_lblNumberOfCubes setText:s];
}

- (IBAction)randomnessPercentChanged:(id)sender
{
    UISlider *slider = sender;
    [slider value];
    
    float randomness = [slider value];
    njli::Cubenado::getInstance()->setRandomness(randomness/100.0f);
    NSString *s = [NSString stringWithFormat:@"Randomness: %.0f%%", njli::Cubenado::getInstance()->getRandomness()*100];
//    NSLog(s);
    [_lblRandomnessPercent setText:s];
}

@end
