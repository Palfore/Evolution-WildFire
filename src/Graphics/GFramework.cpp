#include "GFramework.h"

#include <time.h> // srand(time(nullptr))

#include "Simulation/Simulation.h" // init
#include "GlutCallBacks.h"
#include "Drawing/Draw.h"
#include "Config.h"
#include "Audio.h"

GFramework::GFramework() : windowSize(INIT_WINDOW_WIDTH, INIT_WINDOW_HEIGHT), drawingState(Drawing::Dimension::NONE),
                    camera(new Camera()), mouse(), userInput(), audio(new Audio(Config::getValue<int>("VOLUME"))),
                    simulation(new Simulation()) {
    /* Init GLUT with title */
    int myargc = 1;
    char *s2 = new char[strlen(WINDOW_TITLE) + 1];
    strcpy(s2, WINDOW_TITLE);
    char * argv2[1] = {s2};
    glutInit(&myargc, argv2);
    delete s2;

    initializeGlut();
    loadColors();
    loadTextures();
    if (Config::getValue<bool>("Fullscreen")) glutFullScreen();
    LOG("Initialized GFramework.");
} std::unique_ptr<GFramework> const GFramework::get(new GFramework()); // Instantiate singleton instance



GFramework::~GFramework() {
    delete simulation;
    delete audio;
    delete camera;
    LOG("Cleaning up GFramework"); // Apparently this is never called now
}

void GFramework::startup() {
    LOG("Initializing Simulation");
    simulation->init();
    LOG("Starting up glutmainloop.");
    glutMainLoop();
}

void GFramework::initializeGlut() {
    srand(time(nullptr));

    /* General Initializations */
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(INIT_WINDOW_X, INIT_WINDOW_Y);
    glutInitWindowSize(INIT_WINDOW_WIDTH, INIT_WINDOW_HEIGHT);
    glutCreateWindow(WINDOW_TITLE);

    /* Lighting */
    GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glShadeModel(GL_SMOOTH);

    GLfloat global_ambient[] = { 0.6f, 0.6f, 0.6f, 0.6f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 0.4 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    /* Texturing */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    /* Blending */
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Register Callbacks */
    glutReshapeFunc(glutCB::changeSize);  // window reshape callback
    glutDisplayFunc(glutCB::renderScene); // (re)display callback
    glutIdleFunc(glutCB::update);         // incremental update
    glutIgnoreKeyRepeat(true);       // ignore key repeat when holding key down
    glutMouseFunc(glutCB::callMouse);   // process mouse button push/release
    glutMotionFunc(glutCB::mouseMove);    // process mouse dragging motion
    glutPassiveMotionFunc(glutCB::passiveMouse);
    //glutKeyboardFunc(processNormalKeys); // process standard key clicks // Better to manually implement
    glutKeyboardFunc(glutCB::keyPressed); // Tell GLUT to use the method "keyPressed" for key presses
    glutKeyboardUpFunc(glutCB::keyUp);    // Tell GLUT to use the method "keyUp" for key up events
    glutSpecialFunc(glutCB::pressSpecialKey);     // process special key pressed
    glutSpecialUpFunc(glutCB::releaseSpecialKey); // process special key release
}

void GFramework::showScene() {
    drawingState = Drawing::Dimension::NONE;
    Drawing::enable3D(); // Corrects viewing for camera movement & warns about drawing 3d over 2d
    glutSwapBuffers();
}

void GFramework::readyDrawing() {
    /* Clear, reset, camera */
    glClearColor(0.00, 0.75, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    auto c = GFramework::get->camera;
    gluLookAt(               c->pos.x,                  c->pos.y,                  c->pos.z,
             c->pos.x + c->dir.x,  c->pos.y + c->dir.y,  c->pos.z + c->dir.z,
                                     0.0,                          0.0,                          1.0);
    GFramework::get->drawingState = Drawing::Dimension::NONE;
    Drawing::enable3D();
}



