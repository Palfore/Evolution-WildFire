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
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

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

    glEnable(GL_DEPTH_TEST);
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


Camera::Camera() : translationSpeed(0), rotationSpeed(0), pos(0,0,0), mov(0,0,0), dir(0,0,0), ang(0,0,0), del(0,0,0) {
    reset();
}

void Camera::reset() {
    translationSpeed = DEFAULT_T_SPEED;
    rotationSpeed    = DEFAULT_R_SPEED;
    pos = Vec(0, -40, DEFAULT_HEIGHT);
    mov = Vec(0, 0, 0);
    dir = Vec(0, 1, 0);
    ang = Vec(0,0,0);
    del = Vec(0,0,0);
}

void Camera::cinematicCamera(const Vec& lookat, const double distance) {
        static int step = 0;

        double SPEED = 10.0;
        double speed = SPEED * 1 / 2000.0;
        static double arr[3] = {0.5, 1.0, 1.5};

        static double theta = 0.0; // 0 - 2pi
        static double phi = 0.0;
        static double r = distance;

        static double thetaDot = 1.0 / 1000.0;
        static double phiDot = 1.0 / 1000.0;
        static double rDot = 1.0 / 1000.0;

        /* Change Speeds Randomly */
        if (randf(100) < (0.1)) {
            int shift = rand() % 3;
            arr[0] = arr[(shift + 0) % 3];
            arr[1] = arr[(shift + 1) % 3];
            arr[2] = arr[(shift + 2) % 3];
        }

        theta += 3*thetaDot * arr[0];
        phi += 3*phiDot * arr[1];
        r += 3*rDot * arr[2];

        if (phi > 3.14159/2.0) {
            phiDot = -fabs(phiDot);
            phi = 3.14159/2.0 - 0.001;
        } else if (phi < 0.1745) {
            phiDot = fabs(phiDot);
            phi = 0.1745 + 0.0001;
        }
        if (theta > 2*3.14159) {
            thetaDot = -fabs(thetaDot);
            theta = 2*3.14159 - 0.001;
        } else if (theta < 0) {
            thetaDot = fabs(thetaDot);
            theta = 0.001;
        }

        /* Move and Look at New Position */
        this->pos.x = lookat.x + r*cos(theta)*sin(phi);
        this->pos.y = lookat.y + r*sin(theta)*sin(phi);
        this->pos.z = lookat.z + r*cos(phi) + 50;

        this->dir.x = lookat.x - this->pos.x + 50 * sinf(step * speed);
        this->dir.y = lookat.y - this->pos.y + 50 * sinf(step * speed);
        this->dir.z = lookat.z - this->pos.z + 50 * sinf(step * speed) + 20;

        step = (step + 1) % 1'000'000;
}